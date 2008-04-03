/*
 * Copyright (C) 2005-2008 Atsushi Konno All rights reserved.
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <unistd.h>
#include <string.h>

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_log.h"
#include "ap_config.h"
#include "apr_strings.h"
#include "util_filter.h"
#include "apr_buckets.h"
#include "apr_lib.h"
#include "apr_tables.h"
#include "apr_dso.h"
#include "apr_general.h"
#include "apr_pools.h"

#include "mod_chxj.h"
#include "chxj_encoding.h"
#include "qs_ignore_sp.h"
#include "qs_log.h"
#include "qs_malloc.h"
#include "qs_parse_attr.h"
#include "qs_parse_file.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "chxj_load_device_data.h"
#include "chxj_load_emoji_data.h"
#include "chxj_specified_device.h"
#include "chxj_tag_util.h"
#include "chxj_xhtml_mobile_1_0.h"
#include "chxj_hdml.h"
#include "chxj_chtml10.h"
#include "chxj_chtml20.h"
#include "chxj_chtml30.h"
#include "chxj_jhtml.h"
#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#if defined(USE_MEMCACHE_COOKIE)
#  include "chxj_memcache.h"
#endif

#include "chxj_img_conv_format.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_apply_convrule.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_str_util.h"
#include "chxj_emoji.h"
#include "chxj_jreserved_tag.h"


#define CHXJ_VERSION_PREFIX PACKAGE_NAME "/"
#define CHXJ_VERSION        PACKAGE_VERSION

converter_t convert_routine[] = {
  {
    /* CHXJ_SPEC_UNKNOWN          */
    .converter = NULL,
    .encoder  = NULL,
  },
  {
    /* CHXJ_SPEC_Chtml_1_0        */
    .converter = chxj_convert_chtml10,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_2_0        */
    .converter = chxj_convert_chtml20,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_3_0        */
    .converter = chxj_convert_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_4_0        */
    .converter = chxj_convert_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_5_0        */
    .converter = chxj_convert_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_6_0        */
    .converter = chxj_convert_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Chtml_7_0        */
    .converter = chxj_convert_chtml30,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_XHtml_Mobile_1_0 */
    .converter = chxj_convert_xhtml_mobile_1_0,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Hdml             */
    .converter = chxj_convert_hdml,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_Jhtml            */
    .converter = chxj_convert_jhtml,
    .encoder  = chxj_encoding,
  },
  {
    /* CHXJ_SPEC_HTML             */
    .converter = NULL,
    .encoder  = NULL,
  },
};

static int chxj_convert_input_header(request_rec *r,chxjconvrule_entry *entryp, device_table *spec);

/**
 * Only when User-Agent is specified, the User-Agent header is camouflaged. 
 *
 * @param r   [i]
 */
static apr_status_t 
chxj_headers_fixup(request_rec *r)
{
  mod_chxj_config     *dconf; 
  chxjconvrule_entry  *entryp;
  char                *user_agent;
  device_table        *spec;
  char                *content_length;
  char                *content_type;

  DBG(r, "start chxj_headers_fixup()");
  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  user_agent     = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  spec = chxj_specified_device(r, user_agent);

  switch(GET_HTML_SPEC_TYPE(spec)) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
    entryp = chxj_apply_convrule(r, dconf->convrules);
    if (! entryp) {
      DBG(r, "end chxj_headers_fixup() no pattern");
      return DECLINED;
    }
  
    apr_table_setn(r->headers_in, 
                   CHXJ_HTTP_USER_AGENT, 
                   user_agent);
  
    if (entryp->user_agent)
      apr_table_setn(r->headers_in, 
                     HTTP_USER_AGENT, 
                     entryp->user_agent);

    chxj_convert_input_header(r,entryp,spec);
    content_type = (char *)apr_table_get(r->headers_in, "Content-Type");
    if (r->method_number != M_POST
        || content_type == NULL
        || ! STRNCASEEQ('a','A', FORM_CONTENT_TYPE, content_type, strlen(FORM_CONTENT_TYPE))) {
      DBG(r, "NOT POST METHOD:[%d] content_type:[%s]", r->method_number, content_type);
      break;
    }
    content_length = (char*)apr_table_get(r->headers_in, HTTP_CONTENT_LENGTH);
    DBG(r, "content-Type:[%s]", content_type);
    DBG(r, "content-Length:[%s]", content_length);
    if (content_length && strlen(content_length) != 0) {
      if (STRNCASEEQ('A','a', FORM_CONTENT_TYPE, content_type, strlen(FORM_CONTENT_TYPE))) {
        long cl = (long)((double)chxj_atoi(content_length) * 3. / 2.);
        if (cl <= CHXJ_CONTENT_LENGTH_MAX) {
          if (cl > 0) {
            apr_table_setn(r->headers_in, CHXJ_HTTP_ORIG_CONTENT_LENGTH, content_length);
            apr_table_setn(r->headers_in, "Content-Length", apr_psprintf(r->pool, "%ld", cl));
          }
        }
        else {
          ERR(r, "too large Content-Length data[%s]", content_length);
        }
      }
    }
    break;
  
  default:
    DBG(r, "user-agent:[%s] is PC?", user_agent);
    break;

  }



  DBG(r, "end chxj_headers_fixup()");

  return DECLINED;
}


/**
 * It converts it from CHTML into XXML corresponding to each model. 
 *
 * @param r   [i]
 * @param src [i]   It is former HTML character string. 
 * @param len [i/o] It is length of former HTML character string. 
 */
static char *
chxj_convert_html(request_rec *r, const char** src, apr_size_t* len, device_table *spec, const char *ua)
{
  char                *user_agent;
  char                *dst;
  cookie_t            *cookie;
  mod_chxj_config     *dconf; 
  chxjconvrule_entry  *entryp;

  DBG(r,"start chxj_convert_html()");
  dst = apr_pstrdup(r->pool, (char*)*src);

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);


  entryp = chxj_apply_convrule(r, dconf->convrules);

  if (!entryp || !(entryp->action & CONVRULE_ENGINE_ON_BIT))
    return (char *)*src;


  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  if (entryp->user_agent)
    user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
  else
    user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);

  DBG(r,"User-Agent:[%s]", user_agent);
  DBG(r, "start chxj_convert_html()");
  DBG(r,"content type is %s", r->content_type);


  if (  !STRNCASEEQ('t','T',"text/html", r->content_type, sizeof("text/html") - 1)
      &&  !STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml") - 1)) {
    DBG(r,"content type is %s", r->content_type);
    DBG(r,"end chxj_convert_html()");
    return (char*)*src;
  }

  if (ua && user_agent && strcasecmp(user_agent, ua) != 0) {
    /* again */
    spec = chxj_specified_device(r, user_agent);
  }

  /*
   * save cookie.
   */
  cookie = NULL;
  if (entryp->action & CONVRULE_COOKIE_ON_BIT) {
    switch(GET_HTML_SPEC_TYPE(spec)) {
    case CHXJ_SPEC_Chtml_1_0:
    case CHXJ_SPEC_Chtml_2_0:
    case CHXJ_SPEC_Chtml_3_0:
    case CHXJ_SPEC_Chtml_4_0:
    case CHXJ_SPEC_Chtml_5_0:
    case CHXJ_SPEC_Chtml_6_0:
    case CHXJ_SPEC_Chtml_7_0:
    case CHXJ_SPEC_Jhtml:
      cookie = chxj_save_cookie(r);
      break;
    default:
      break;
    }
  }

  if (!r->header_only) {
    char *tmp = NULL;
    if (STRNCASEEQ('t','T',"text/",r->content_type, sizeof("text/")-1) || STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)) {
      /* convert Emoji code to META Emoji */
      tmp = chxj_emoji_to_meta_emoji(r,
                                     entryp->encoding,
                                     (const char *)*src,
                                     (apr_size_t*)len);
      /* convert charactor code */
      if (convert_routine[GET_HTML_SPEC_TYPE(spec)].encoder) {
        tmp = convert_routine[GET_HTML_SPEC_TYPE(spec)].encoder(r, 
                                                                tmp, 
                                                                (apr_size_t*)len);
      }
    }
    else {
      tmp = (char *)*src;
    }

    if (tmp) {
      /* for PC */
      dst = tmp;
    }

    /* Now, ``src'' is CP932 */
    if (convert_routine[GET_HTML_SPEC_TYPE(spec)].converter) {
      dst = convert_routine[GET_HTML_SPEC_TYPE(spec)].converter(r,
                                                                spec, 
                                                                tmp, 
                                                                *len, 
                                                                len, 
                                                                entryp, 
                                                                cookie);
      if (STRNCASEEQ('t','T',"text/",r->content_type, sizeof("text/")-1) || STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)) {
        /* convert SJIS to device charactor code */
        if (! IS_SJIS_STRING(GET_SPEC_CHARSET(spec))) {
          dst = chxj_encoding_by_spec(r, 
                                      spec,
                                      dst,
                                      len);
        }
  
        /* convert META Emoji to Emoji code */
        dst = chxj_meta_emoji_to_emoji(r,
                                       spec,
                                       entryp,
                                       GET_EMOJI_INDEX(dconf),
                                       dst);
        *len = strlen(dst);
      }
    }
  }

  if (*len == 0) {
    dst = apr_psprintf(r->pool, "\n");
    *len = 1;
  }
  dst[*len] = 0;

  DBG(r, "end chxj_convert_html()");

  return dst;
}


/**
 * POSTDATA and GETDATA convertor.
 */
static char *
chxj_convert_emoji_param_list(request_rec *r, device_table *spec, chxjconvrule_entry *entryp, char *result, const char *name, const char *value,
  char *(*func_emoji_to_meta_emoji)(request_rec *, const char *, const char *, apr_size_t *))
{
  char *new_val, *new_nam;
  apr_size_t new_nam_len, new_val_len;
  if (strlen(result) != 0)
    result = apr_pstrcat(r->pool, result, "&", NULL);
  new_nam = chxj_url_decode(r, apr_pstrdup(r->pool, name));
  new_val = chxj_url_decode(r, apr_pstrdup(r->pool, (value) ? value : ""));
  new_nam_len = strlen(new_nam);
  new_val_len = strlen(new_val);
  new_nam = func_emoji_to_meta_emoji(r,GET_SPEC_CHARSET(spec), new_nam, &new_nam_len);
  new_val = func_emoji_to_meta_emoji(r,GET_SPEC_CHARSET(spec), new_val, &new_val_len);
  new_nam = chxj_convert_encoding(r, GET_SPEC_CHARSET(spec), entryp->encoding, new_nam, &new_nam_len);
  new_val = chxj_convert_encoding(r, GET_SPEC_CHARSET(spec), entryp->encoding, new_val, &new_val_len);
  new_nam = chxj_postdata_meta_emoji_to_emoji(r,entryp->encoding, new_nam, spec);
  new_val = chxj_postdata_meta_emoji_to_emoji(r,entryp->encoding, new_val, spec);
  new_nam = chxj_url_encode(r,new_nam);
  new_val = chxj_url_encode(r,new_val);
  return apr_pstrcat(r->pool, result, new_nam, "=", new_val, NULL);
}


/**
 * It converts it from HEADER.
 *
 * @param r   [i]
 */
static int
chxj_convert_input_header(request_rec *r,chxjconvrule_entry *entryp, device_table *spec) 
{

  char       *buff;
  char       *buff_pre;
  apr_size_t urilen;
  char       *result;
  char       *pair;
  char       *name;
  char       *value;
  char       *pstate;
  char       *vstate;
  cookie_t   *cookie;
  int        no_update_flag = 0;
  char     *(*func_emoji_to_meta_emoji)(request_rec *, const char *, const char *, apr_size_t *);

  DBG(r, "start chxj_convert_input_header()");

  if (! r->args) {
    DBG(r, "r->args=[null]");
    DBG(r, "end   chxj_convert_input_header()");
    return 0;
  }
  urilen = strlen(r->args);

  result = qs_alloc_zero_byte_string(r);

  buff_pre = apr_pstrdup(r->pool, r->args);

  for (;;) {
    char *pair_sv;

    pair = apr_strtok(buff_pre, "&", &pstate);
    if (pair == NULL)
      break;

    buff_pre = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0) {
      DBG(r, "found cookie no update parameter");
      no_update_flag++;
    }
  }

  buff = apr_pstrdup(r->pool, r->args);
  DBG(r, "r->args=[%s]", buff);

  /* use postdata convert functions */
  switch(GET_HTML_SPEC_TYPE(spec)) {
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
    func_emoji_to_meta_emoji = chxj_postdata_ezweb_emoji_to_meta_emoji;
    break;
  case CHXJ_SPEC_Jhtml:
    func_emoji_to_meta_emoji = chxj_postdata_softbank_emoji_to_meta_emoji;
    break;
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  default:
    func_emoji_to_meta_emoji = chxj_postdata_imode_emoji_to_meta_emoji;
    break;
  }

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) {
    char* pair_sv;

    pair = apr_strtok(buff, "&", &pstate);
    if (pair == NULL)
      break;

    buff = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    name = chxj_safe_to_jreserved_tag(r, name);

    if (strncasecmp(name, "_chxj", 5) != 0) {
      if (strlen(result) != 0) 
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (strcasecmp(entryp->encoding, "NONE") != 0 && value && strlen(value)) {
        result = chxj_convert_emoji_param_list(r, spec, entryp, result, name, value, func_emoji_to_meta_emoji);
        DBG(r, "result:[%s]", result);
      }
      else {
        if (strcmp(name, pair_sv) != 0)
          result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
        else
          result = apr_pstrcat(r->pool, result, name, NULL);
      }
    }
    else
    if (strncasecmp(name, "_chxj_c_", 8) == 0 
    ||  strncasecmp(name, "_chxj_r_", 8) == 0
    ||  strncasecmp(name, "_chxj_s_", 8) == 0) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);

      result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
    }
    else
    if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0) {
      DBG(r, "found cookie parameter[%s]", value);
      DBG(r, "call start chxj_load_cookie()");
      cookie = chxj_load_cookie(r, value);
      DBG(r, "call end   chxj_load_cookie()");
      if (! no_update_flag && cookie) {
        chxj_update_cookie(r, cookie);
      }
    }
  }
  r->args = result;

  DBG(r, "GETDATA:[%s]", r->args);
  DBG(r, "GETDATA result r->args=[%s]", r->args);
  DBG(r, "end   chxj_convert_input_header()");
  return 0;
}


/**
 * It converts it from POSTDATA .
 *
 * @param r   [i]
 * @param src [i]   It is POSTDATA character string.
 * @param len [i/o] It is length of former HTML character string.
 */
static char *
chxj_input_convert(
  request_rec         *r, 
  const char          **src, 
  apr_size_t          *len, 
  chxjconvrule_entry  *entryp,
  device_table        *spec)
{
  char     *pair;
  char     *name;
  char     *value;
  char     *pstate;
  char     *vstate;
  char     *s;
  char     *result;
  cookie_t *cookie;
  char     *buff_pre;
  int      no_update_flag = 0;
  int      cenc;         /* client side encoding */
  char     *(*func_emoji_to_meta_emoji)(request_rec *, const char *, const char *, apr_size_t *);

  /* judge client encoding */
  if (IS_SJIS_STRING(GET_SPEC_CHARSET(spec))) {
    cenc = SJIS;
  }
  else if (IS_EUCJP_STRING(GET_SPEC_CHARSET(spec))) {
    cenc = EUCJP;
  }
  else if (IS_UTF8_STRING(GET_SPEC_CHARSET(spec))) {
    cenc = UTF8;
  }
  switch(GET_HTML_SPEC_TYPE(spec)) {
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
    func_emoji_to_meta_emoji = chxj_postdata_ezweb_emoji_to_meta_emoji;
    break;
  case CHXJ_SPEC_Jhtml:
    func_emoji_to_meta_emoji = chxj_postdata_softbank_emoji_to_meta_emoji;
    break;
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  default:
    func_emoji_to_meta_emoji = chxj_postdata_imode_emoji_to_meta_emoji;
    break;
  }

  /* 
   * Now, the character-code of the client has been understood !!
   */
  s        = apr_pstrdup(r->pool, *src);
  buff_pre = apr_pstrdup(r->pool, *src);

  result = qs_alloc_zero_byte_string(r);
  
  DBG(r, "============== POST ===============");
  DBG(r, "BEFORE input convert source = [%s]", s);
  DBG(r, "============== POST ===============");
  for (;;) {
    char *pair_sv;

    pair = apr_strtok(buff_pre, "&", &pstate);
    if (pair == NULL)
      break;

    buff_pre = NULL;

    pair_sv = apr_pstrdup(r->pool, pair);

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strcasecmp(name, CHXJ_COOKIE_NOUPDATE_PARAM) == 0) {
      DBG(r, "found cookie no update parameter");
      no_update_flag++;
    }
  }

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) {
    pair = apr_strtok(s, "&", &pstate);
    if (pair == NULL)
      break;
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    name  = chxj_safe_to_jreserved_tag(r, name);

    if (strncasecmp(name, "_chxj", 5) != 0) {
      result = chxj_convert_emoji_param_list(r, spec, entryp, result, name, value, func_emoji_to_meta_emoji);
      DBG(r, "result:[%s]", result);
    }
    else if (strncasecmp(name, "_chxj_c_", 8) == 0 
        ||  strncasecmp(name, "_chxj_r_", 8) == 0
        ||  strncasecmp(name, "_chxj_s_", 8) == 0) {
      if (value == NULL)
        continue;

      if (strlen(value) == 0)
        continue;

      if (strlen(result) != 0)
        result = apr_pstrcat(r->pool, result, "&", NULL);

      if (strcasecmp(entryp->encoding, "NONE") != 0 && value && strlen(value)) {
        apr_size_t dlen;
        char*      dvalue;

        dlen   = strlen(value);
        value = chxj_url_decode(r, value);
        dvalue = chxj_rencoding(r, value, &dlen);
        dvalue = chxj_url_encode(r,dvalue);
        result = apr_pstrcat(r->pool, result, &name[8], "=", dvalue, NULL);

      }
      else {
        result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
      }
    }
    else if (strcasecmp(name, CHXJ_COOKIE_PARAM) == 0) {
      DBG(r, "found cookie parameter[%s]", value);
      DBG(r, "call start chxj_load_cookie()");
      cookie = chxj_load_cookie(r, value);
      DBG(r, "call end   chxj_load_cookie()");
      if (! no_update_flag && cookie) {
        chxj_update_cookie(r, cookie);
      }
    }
  }
  *len = strlen(result);

  DBG(r, "AFTER input convert result = [%s]", result);

  return result;
}


/**
 * The received data is returned to the filter.
 *
 * @param f    [i/o] It is a filter. 
 * @param data [i]   It is data returned to the filter. 
 * @param len  [i]   It is length of the data returned to the filter. 
 */
static apr_status_t 
pass_data_to_filter(ap_filter_t *f, const char *data, 
                                        apr_size_t len)
{
  request_rec         *r = f->r;
  conn_rec            *c = r->connection;
  apr_status_t        rv;
  apr_bucket_brigade  *bb;
  apr_bucket          *b;

  DBG(r, "start pass_data_to_filter()");

  bb = apr_brigade_create(r->pool, c->bucket_alloc);
  b  = apr_bucket_transient_create(data, len, c->bucket_alloc);

  APR_BRIGADE_INSERT_TAIL(bb, b);
  b = apr_bucket_eos_create(f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(bb, b);

  rv = ap_pass_brigade(f->next, bb);
  if (rv != APR_SUCCESS) {
    DBG(r, "ap_pass_brigade()");
    return rv;
  }

  DBG(r, "end pass_data_to_filter()");

  return rv;
}


/**
 * It is the main loop of the output filter. 
 *
 * @param f   [i/o] It is a filter.
 * @param bb  [i]   
 */
static apr_status_t 
chxj_output_filter(ap_filter_t *f, apr_bucket_brigade *bb)
{
  request_rec         *r;
  apr_status_t        rv;
  apr_bucket          *b;
  const char          *data;
  char                *user_agent = NULL;
  char                *contentLength;
  apr_size_t          len;
  mod_chxj_ctx        *ctx       = (mod_chxj_ctx *)f->ctx;
  cookie_t            *cookie;
  char                *location_header;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp = NULL;
  device_table        *spec   = NULL;



  DBG(f->r, "start of chxj_output_filter()");

  r  = f->r;
  rv = APR_SUCCESS;

  if (!f->ctx) {
    if ((f->r->proto_num >= 1001) 
    &&  !f->r->main 
    &&  !f->r->prev) 
      f->r->chunked = 1;
  }
  if (r->content_type) {
    if (! STRNCASEEQ('t','T',"text/html",r->content_type, sizeof("text/html")-1)
    &&  ! STRNCASEEQ('t','T',"text/xml", r->content_type, sizeof("text/xml")-1)
    &&  ! STRNCASEEQ('a','A',"application/xhtml+xml", r->content_type, sizeof("application/xhtml+xml")-1)
    &&  ! (STRNCASEEQ('i','I',"image/",  r->content_type, sizeof("image/") -1)
          && ( STRCASEEQ('j','J',"jpeg",            &r->content_type[6])         /* JPEG */
            || STRCASEEQ('j','J',"jp2",             &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('j','J',"jpeg2000",        &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('j','J',"jpeg2000-image",  &r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('x','X',"x-jpeg2000-image",&r->content_type[6])         /* JPEG2000 */
            || STRCASEEQ('p','P',"png",             &r->content_type[6])         /* PNG */
            || STRCASEEQ('x','X',"x-png",           &r->content_type[6])         /* PNG */
            || STRCASEEQ('g','G',"gif",             &r->content_type[6])))) {     /* GIF */
      
      DBG(r, "not convert content-type:[%s]", r->content_type);
      ap_pass_brigade(f->next, bb);
      return APR_SUCCESS;
    }
  }
  else {
    DBG(r, "not convert content-type:[(null)]");
    ap_pass_brigade(f->next, bb);
    return APR_SUCCESS;
  }

  dconf      = ap_get_module_config(r->per_dir_config, &chxj_module);
  user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  if (ctx && ctx->entryp) entryp = ctx->entryp;
  else                    entryp = chxj_apply_convrule(r, dconf->convrules);
  if (ctx && ctx->spec)   spec   = ctx->spec;
  else                    spec   = chxj_specified_device(r, user_agent);

  for (b = APR_BRIGADE_FIRST(bb);
       b != APR_BRIGADE_SENTINEL(bb); 
       b = APR_BUCKET_NEXT(b)) {

    if (APR_BUCKET_IS_EOS(b)) {

      DBG(r, "eos");
      /*----------------------------------------------------------------------*/
      /* End Of File                                                          */
      /*----------------------------------------------------------------------*/
      if (f->ctx) {

        ctx = (mod_chxj_ctx*)f->ctx;

        DBG(r, "content_type=[%s]", r->content_type);

        if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN
            && r->content_type
            && (STRNCASEEQ('t','T',"text/html", r->content_type,sizeof("text/html")-1) 
            ||  STRNCASEEQ('a','A',"application/xhtml+xml",r->content_type,sizeof("application/xhtml+xml")-1))) {
          if (ctx->len) {
            char* tmp;

            tmp = apr_palloc(r->pool, ctx->len + 1);

            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);

#if 1
            DBG(r, "input data=[%s] len=[%d]", tmp, ctx->len);
#endif

            ctx->buffer = chxj_convert_html(r, 
                                        (const char**)&tmp, 
                                        (apr_size_t*)&ctx->len,
                                        spec,
                                        user_agent);

#if 1
            DBG(r, "output data=[%.*s]", ctx->len,ctx->buffer);
#endif
          }
          else {
            ctx->buffer = apr_psprintf(r->pool, "\n");
            ctx->len += 1;
            ctx->buffer = chxj_convert_html(r, 
                                        (const char**)&ctx->buffer, 
                                        (apr_size_t*)&ctx->len,
                                        spec,
                                        user_agent);

          }
        }
        if (r->content_type && STRNCASEEQ('t','T',"text/xml",r->content_type, sizeof("text/xml")-1)) {
          DBG(r, "text/XML");

          Doc       doc;
          Node      *root;
          Node      *child;
          qr_code_t qrcode;
          int       sts;
      
          memset(&doc,    0, sizeof(Doc));
          memset(&qrcode, 0, sizeof(qr_code_t));
          doc.r = r;
          doc.parse_mode  = PARSE_MODE_CHTML;
          qrcode.doc      = &doc;
          qrcode.r        = r;
      
          qs_init_malloc(&doc);
      
          root = qs_parse_string(&doc, ctx->buffer, ctx->len);

          sts = 0;
          for (child = qs_get_child_node(&doc,root);
               child ;
               child = qs_get_next_node(&doc,child)) {
        
            char *name;
        
            name = qs_get_node_name(&doc,child);
        
            if (strcasecmp("qrcode",name) == 0) {
              sts++;
              break;
            }
          }
          qs_all_free(&doc,QX_LOGMARK);
          if (sts) {
            r->handler = apr_psprintf(r->pool, "chxj-qrcode");
            chxj_qrcode_node_to_qrcode(&qrcode, root);
            sts = chxj_qrcode_create_image_data(&qrcode, &ctx->buffer, &ctx->len);
            if (sts != OK) {
              ERR(r, "qrcode create failed.");
              return sts;
            }
            r->content_type = apr_psprintf(r->pool, "image/jpg");
          }
        }

        if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN
            && r->content_type
            && STRNCASEEQ('i','I',"image/",r->content_type,sizeof("image/")-1)
            && ( STRCASEEQ('j','J',"jpeg",            &r->content_type[6])         /* JPEG */
              || STRCASEEQ('j','J',"jp2",             &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('j','J',"jpeg2000",        &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('j','J',"jpeg2000-image",  &r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('x','X',"x-jpeg2000-image",&r->content_type[6])         /* JPEG2000 */
              || STRCASEEQ('p','P',"png",             &r->content_type[6])         /* PNG */
              || STRCASEEQ('x','X',"x-png",           &r->content_type[6])         /* PNG */
              || STRCASEEQ('g','G',"gif",             &r->content_type[6]))) {     /* GIF */
          if (ctx->len) {
            char *tmp;

            tmp = apr_palloc(r->pool, ctx->len + 1);

            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);

#if 1
            DBG(r, "input data=[%s]", tmp);
#endif


            ctx->buffer = 
              chxj_convert_image(r, 
                                 (const char**)&tmp,
                                 (apr_size_t*)&ctx->len);

            if (ctx->buffer == NULL)
              ctx->buffer = tmp;

#if 1
            DBG(r, "output data=[%.*s]", ctx->len,ctx->buffer);
#endif
          }
        }

        contentLength = apr_psprintf(r->pool, "%d", (int)ctx->len);
        apr_table_setn(r->headers_out, "Content-Length", contentLength);
        
        if (ctx->len > 0) {
          rv = pass_data_to_filter(f, 
                                   (const char*)ctx->buffer, 
                                   (apr_size_t)ctx->len);
        }
        f->ctx = NULL;

        return rv;
      }
      else {
        DBG(r, " SAVE COOKIE[%x]", entryp->action);

        /*
         * save cookie.
         */
        if (entryp->action & CONVRULE_COOKIE_ON_BIT) {
          DBG(r, "entryp->action == COOKIE_ON_BIT");
          char          *user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
          device_table  *spec = chxj_specified_device(r, user_agent);

          switch(GET_HTML_SPEC_TYPE(spec)) {
          case CHXJ_SPEC_Chtml_1_0:
          case CHXJ_SPEC_Chtml_2_0:
          case CHXJ_SPEC_Chtml_3_0:
          case CHXJ_SPEC_Chtml_4_0:
          case CHXJ_SPEC_Chtml_5_0:
          case CHXJ_SPEC_Chtml_6_0:
          case CHXJ_SPEC_Chtml_7_0:
          case CHXJ_SPEC_Jhtml:

            cookie = chxj_save_cookie(r);
  
            /*
             * Location Header Check to add cookie parameter.
             */
            location_header = (char *)apr_table_get(r->headers_out, "Location");
            if (location_header) {
              DBG(r, "Location Header=[%s]", location_header);
              location_header = chxj_add_cookie_parameter(r,
                                                          location_header,
                                                          cookie);
              apr_table_setn(r->headers_out, "Location", location_header);
              DBG(r, "Location Header=[%s]", location_header);
            }
            break;

          default:
            break;
          }
        }

        apr_table_setn(r->headers_out, "Content-Length", "0");
        rv = pass_data_to_filter(f, (const char*)"", (apr_size_t)0);

        return rv;
      }
    }
    else
    if (apr_bucket_read(b, &data, &len, APR_BLOCK_READ) == APR_SUCCESS) {
      DBG(r, "read data[%.*s]",(int)len, data);

      if (f->ctx == NULL) {
        /*--------------------------------------------------------------------*/
        /* Start                                                              */
        /*--------------------------------------------------------------------*/
        DBG(r, "new context");
        ctx = (mod_chxj_ctx*)apr_palloc(r->pool, sizeof(mod_chxj_ctx));
        if (len > 0) {
          ctx->buffer = apr_palloc(r->pool, len);
          memcpy(ctx->buffer, data, len);
        }
        else {
          ctx->buffer = apr_palloc(r->pool, 1);
          ctx->buffer = '\0';
        }
        ctx->len = len;
        ctx->entryp = entryp;
        ctx->spec   = spec;
        f->ctx = (void*)ctx;
      }
      else {
        /*--------------------------------------------------------------------*/
        /* append data                                                        */
        /*--------------------------------------------------------------------*/
        char  *tmp;
        DBG(r, "append data start");
        ctx = (mod_chxj_ctx*)f->ctx;

        if (len > 0) {
          tmp = apr_palloc(r->pool, ctx->len);
          memcpy(tmp, ctx->buffer, ctx->len);

          ctx->buffer = apr_palloc(r->pool, ctx->len + len);

          memcpy(ctx->buffer, tmp, ctx->len);
          memcpy(&ctx->buffer[ctx->len], data, len);

          ctx->len += len;
        }
        DBG(r, "append data end");
      }
    }
  }
  apr_brigade_destroy(bb);

  DBG(r, "end of output filter");

  return APR_SUCCESS;
}


/**
 * It is the main loop of the input filter. 
 *
 * @param f         [i/o] It is a filter.
 * @param bb        [i]   brigade
 * @param mode      [i]   mode
 * @param block     [i]   block
 * @param readbytes [i]   readbyte
 */
static apr_status_t 
chxj_input_filter(ap_filter_t         *f, 
                 apr_bucket_brigade   *bb,
                 ap_input_mode_t      mode, 
                 apr_read_type_e      block,
                 apr_off_t            readbytes)
{
  request_rec         *r;
  apr_status_t        rv;
  conn_rec            *c;
  apr_bucket          *b;
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for output                                        */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade* ibb;            
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for input                                         */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade  *obb;
  apr_size_t          len;
  apr_bucket          *eos;
  const char          *data;
  char                *data_bucket;
  char                *data_brigade;
  char                *content_type;
  char                *content_length;
  char                *orig_content_length;
  device_table        *spec;
  char                *user_agent;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;
  mod_chxj_ctx        *ctx = f->ctx;

  r = f->r;
  c = r->connection;

  DBG(r, "start of chxj_input_filter()");

  data_brigade = qs_alloc_zero_byte_string(r);

  DBG(r, "remaining:[%qd]",   r->remaining);
  DBG(r, "read_length:[%qd]", r->read_length);
  DBG(r, "eos_sent:[%d]", r->eos_sent);
  DBG(r, "clength:[%qd]", r->clength);

  ibb = apr_brigade_create(r->pool, c->bucket_alloc);
  obb = apr_brigade_create(r->pool, c->bucket_alloc);

  content_type    = (char *)apr_table_get(r->headers_in, "Content-Type");
  content_length  = (char *)apr_table_get(r->headers_in, "Content-Length");
  orig_content_length  = (char *)apr_table_get(r->headers_in, CHXJ_HTTP_ORIG_CONTENT_LENGTH);
  if (mode != AP_MODE_READBYTES 
      || r->method_number != M_POST
      || content_type == NULL
      || ! STRNCASEEQ('a','A', FORM_CONTENT_TYPE, content_type, strlen(FORM_CONTENT_TYPE))
      || content_length == NULL
      || strlen(content_length) == 0
      || strcmp("0", content_length) == 0) {
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }
  DBG(r, "CONTENT-LENGTH:[%s]", content_length);
  DBG(r, "CONTENT-TYPE:[%s]", content_type);
  DBG(r, "READBYTES:[%qd]", readbytes);
  if (! ctx) {
    f->ctx = ctx = apr_palloc(r->pool, sizeof(*ctx));
    memset(ctx, 0, sizeof(*ctx));
    ctx->len = 0;
    if (orig_content_length && strlen(orig_content_length) && ! chxj_chk_numeric(orig_content_length)) {
      ctx->len = (apr_size_t)chxj_atoi(orig_content_length);
    }
    else if (content_length && strlen(content_length) && ! chxj_chk_numeric(content_length)) {
      ctx->len = (apr_size_t)chxj_atoi(content_length);
    }
  }
  if (ctx->len == 0) {
    DBG(r, "ctx->len is 0");
    eos = apr_bucket_eos_create(f->c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, eos);
    return APR_SUCCESS;
  }

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  if (ctx->entryp) entryp = ctx->entryp;
  else             ctx->entryp = entryp = chxj_apply_convrule(r, dconf->convrules);
  if (!entryp || !(entryp->action & CONVRULE_ENGINE_ON_BIT)) {
    DBG(r,"EngineOff");
    ctx->len = 0;

    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }

  user_agent = (char*)apr_table_get(r->headers_in, "User-Agent");
  if (ctx->spec) spec = ctx->spec;
  else           ctx->spec = spec = chxj_specified_device(r, user_agent);

  switch(GET_HTML_SPEC_TYPE(spec)) {
  case CHXJ_SPEC_Chtml_1_0:
  case CHXJ_SPEC_Chtml_2_0:
  case CHXJ_SPEC_Chtml_3_0:
  case CHXJ_SPEC_Chtml_4_0:
  case CHXJ_SPEC_Chtml_5_0:
  case CHXJ_SPEC_Chtml_6_0:
  case CHXJ_SPEC_Chtml_7_0:
  case CHXJ_SPEC_XHtml_Mobile_1_0:
  case CHXJ_SPEC_Hdml:
  case CHXJ_SPEC_Jhtml:
    break;

  default:
    DBG(r, "chxj_specified_device() != MOBILE");
    ctx->len = 0;
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }


  DBG(r, "start ap_get_brigade() ctx->len:[%d]", ctx->len);
  rv = ap_get_brigade(f->next, ibb, mode, block, readbytes);
  if (rv != APR_SUCCESS) {
    DBG(r, "ap_get_brigade() failed");
    return rv;
  }
  DBG(r, "end ap_get_brigade()");

  for (b =  APR_BRIGADE_FIRST(ibb); 
       b != APR_BRIGADE_SENTINEL(ibb);
       b =  APR_BUCKET_NEXT(b)) {

    rv = apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
    if (rv != APR_SUCCESS) {
      DBG(r, "apr_bucket_read() failed");
      return rv;
    }

    if (data != NULL) {
      data_bucket = apr_palloc(r->pool, len+1);
      memset((void*)data_bucket, 0, len+1);
      memcpy(data_bucket, data, len);
      ctx->len -= len;
      DBG(r, "(in)POSTDATA:[%.*s]LEN:[%d] method_number:[%d] content_length:[%s]", len, data_bucket, len, r->method_number, content_length);
  
      data_brigade = apr_pstrcat(r->pool, data_brigade, data_bucket, NULL);
    }

    if (APR_BUCKET_IS_EOS(b)) {
      break;
    }
  }
  apr_brigade_cleanup(ibb);


  len = strlen(data_brigade);
  if (len == 0) {
    DBG(r,"data_brigade length is 0");
    DBG(r,"end of chxj_input_filter()");
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }

  data_brigade = chxj_input_convert(
    r, 
    (const char**)&data_brigade, 
    (apr_size_t*)&len,
    entryp,
    spec);

  if (len > 0) {
    DBG(r, "(in:convert)POSTDATA:[%.*s]:LEN:[%d]", len, data_brigade, len);
    if (STRCASEEQ('P','p', "post", r->method)) {
      char *new_area;
      if (STRNCASEEQ('a','A', FORM_CONTENT_TYPE, content_type, strlen(FORM_CONTENT_TYPE))) {
        if (content_length && strlen(content_length) != 0) {
          apr_size_t cl = (apr_size_t)chxj_atoi(content_length);
          new_area = apr_palloc(r->pool, cl + 1); 
          memset(new_area, 0, cl+1);
          if (len < cl) {
            apr_size_t pp;
            memcpy(new_area, data_brigade, len);
            for (pp=0; pp<cl-len; pp++) {
              new_area[len + pp] = '&';
            }
            data_brigade = new_area;
            len = cl;
          }
        }
      }
    }
    if (ctx->len > 0) {
      b = apr_bucket_pool_create(data_brigade, len, r->pool, c->bucket_alloc);
      APR_BRIGADE_INSERT_TAIL(bb, b);
    }
    if (ctx->len <= 0) {
      ctx->len = 0;
      b = apr_bucket_pool_create(data_brigade, len, r->pool, c->bucket_alloc);
      APR_BRIGADE_INSERT_TAIL(bb, b);
      eos = apr_bucket_eos_create(f->c->bucket_alloc);
      APR_BRIGADE_INSERT_TAIL(bb, eos);
    }
  }
  else {
    ctx->len = 0;
    eos = apr_bucket_eos_create(f->c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, eos);
  }

  DBG(r, "end of chxj_input_filter()");

  return APR_SUCCESS;
}


static mod_chxj_global_config *
chxj_global_config_create(apr_pool_t *pool, server_rec *s)
{
  mod_chxj_global_config *conf;

  SDBG(s, "start chxj_global_config_create()");

  /*--------------------------------------------------------------------------*/
  /* allocate an own subpool which survives server restarts                   */
  /*--------------------------------------------------------------------------*/
  conf = (mod_chxj_global_config*)apr_palloc(pool, 
                  sizeof(mod_chxj_global_config));

#if 0
  conf->cookie_db_lock = NULL;
#endif

  SDBG(s, "end   chxj_global_config_create()");

  return conf;
}


/**
 * initialize chxj module
 */
static int 
chxj_init_module(apr_pool_t *p, 
                  apr_pool_t *UNUSED(plog), 
                  apr_pool_t *UNUSED(ptemp), 
                  server_rec *s)
{
  void *user_data;

  SDBG(s, "start chxj_init_module()");

  apr_pool_userdata_get(&user_data, CHXJ_MOD_CONFIG_KEY, s->process->pool);
  if (user_data == NULL) {
    /*
     * dummy user_data set.
     */
    apr_pool_userdata_set(
      (const void *)(1), 
      CHXJ_MOD_CONFIG_KEY, 
      apr_pool_cleanup_null, 
      s->process->pool);
    SDBG(s, "end  chxj_init_module()");
    return OK;
  }

  ap_add_version_component(p, CHXJ_VERSION_PREFIX CHXJ_VERSION);

  SDBG(s, "end  chxj_init_module()");

  return OK;
}


static void 
chxj_child_init(apr_pool_t *UNUSED(p), server_rec *s)
{
  SDBG(s, "start chxj_child_init()");
  SDBG(s, "end   chxj_child_init()");
}


/**
 * A set structure of each server is generated. 
 * 
 * @param p
 * @param s
 */
static void *
chxj_config_server_create(apr_pool_t *p, server_rec *s)
{
  return chxj_global_config_create(p,s);
}


static int
chxj_translate_name(request_rec *r)
{
  return chxj_trans_name(r);
}


static void 
chxj_insert_filter(request_rec *r)
{
  char                *user_agent;
  device_table        *spec;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;

  DBG(r, "start chxj_insert_filter()");

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  user_agent = (char *)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  spec = chxj_specified_device(r, user_agent);
  entryp = NULL;

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (!entryp) {
    DBG(r, "end chxj_insert_filter()");
    return;
  }

  if (entryp->action & CONVRULE_ENGINE_ON_BIT) {
    DBG(r, "INSERT FILTER method:[%s]", r->method);
    ap_add_input_filter("chxj_input_filter",   NULL, r, r->connection);
    ap_add_output_filter("chxj_output_filter", NULL, r, r->connection);
  }

  DBG(r, "end   chxj_insert_filter()");
}


/**
 * The hook is registered.
 *
 * @param p
 */
static void 
chxj_register_hooks(apr_pool_t *UNUSED(p))
{
  ap_hook_post_config(chxj_init_module,
                      NULL,
                      NULL,
                      APR_HOOK_REALLY_FIRST);
  ap_hook_child_init(chxj_child_init, 
                     NULL, 
                     NULL, 
                     APR_HOOK_REALLY_FIRST);
  ap_register_output_filter (
                      "chxj_output_filter", 
                      chxj_output_filter, 
                      NULL, 
                      AP_FTYPE_RESOURCE);
  ap_register_input_filter(
                      "chxj_input_filter", 
                      chxj_input_filter, 
                      NULL, 
                      AP_FTYPE_RESOURCE);
  ap_hook_insert_filter(chxj_insert_filter, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_img_conv_format_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_qr_code_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_translate_name(chxj_translate_name, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_fixups(chxj_headers_fixup, NULL, NULL, APR_HOOK_LAST);
}


/**
 * A set structure according to the directory is generated. 
 *
 * @param p
 * @param arg
 */
static void *
chxj_create_per_dir_config(apr_pool_t *p, char *arg) 
{
  mod_chxj_config *conf;

  conf = apr_pcalloc(p, sizeof(mod_chxj_config));
  conf->device_data_file = NULL;
  conf->devices          = NULL;
  conf->emoji_data_file  = NULL;
  conf->emoji            = NULL;
  conf->emoji_tail       = NULL;
  conf->image            = CHXJ_IMG_OFF;
  conf->image_cache_dir  = apr_psprintf(p, "%s",DEFAULT_IMAGE_CACHE_DIR);
  conf->image_cache_limit = 0;
  conf->server_side_encoding = NULL;
  conf->cookie_db_dir    = NULL;
  conf->cookie_timeout   = 0;
  conf->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  conf->cookie_lazy_mode  = 0;
#if defined(USE_MYSQL_COOKIE)
  memset((void*)&conf->mysql, 0, sizeof(mysql_t));
  conf->mysql.port       = MYSQL_PORT;
  conf->mysql.host       = NULL;
#endif
#if defined(USE_MEMCACHE_COOKIE)
  memset((void*)&conf->memcache, 0, sizeof(memcache_t));
  conf->memcache.host    = NULL;
  conf->memcache.port    = 0;
#endif

  if (arg == NULL) {
    conf->dir                  = NULL;
  }
  else {
    conf->dir                  = apr_pcalloc(p, strlen(arg)+1);
    memset(conf->dir, 0, strlen(arg)+1);
    strcpy(conf->dir, arg);
  }
  conf->convrules   = apr_array_make(p, 2, sizeof(chxjconvrule_entry));

  /* Default is copyleft */
  conf->image_copyright = NULL; 

  return conf;
}


/*
 *  Merge per-directory CHXJ configurations
 */
static void *
chxj_merge_per_dir_config(apr_pool_t *p, void *basev, void *addv)
{
  mod_chxj_config *base;
  mod_chxj_config *add;
  mod_chxj_config *mrg;

  base = (mod_chxj_config *)basev;
  add  = (mod_chxj_config *)addv;
  mrg  = (mod_chxj_config *)apr_palloc(p, sizeof(mod_chxj_config));

  mrg->device_data_file = NULL;
  mrg->devices          = NULL;
  mrg->emoji_data_file  = NULL;
  mrg->image            = CHXJ_IMG_OFF;
  mrg->image_cache_dir  = NULL;
  mrg->image_copyright  = NULL;
  mrg->image_cache_limit  = 0;
  mrg->emoji               = NULL;
  mrg->emoji_tail          = NULL;
  mrg->ezweb2imode         = NULL;
  mrg->ezweb2imode_tail    = NULL;
  mrg->softbank2imode      = NULL;
  mrg->softbank2imode_tail = NULL;

  mrg->dir = apr_pstrdup(p, add->dir);

  if (! add->device_data_file) {
    mrg->devices = base->devices;
    mrg->device_data_file = apr_pstrdup(p, base->device_data_file);
  }
  else {
    mrg->devices = add->devices;
    mrg->device_data_file = apr_pstrdup(p, add->device_data_file);
  }

  if (! add->emoji_data_file) {
    mrg->emoji               = base->emoji;
    mrg->emoji_tail          = base->emoji_tail;
    mrg->emoji_data_file     = apr_pstrdup(p, base->emoji_data_file);
    mrg->ezweb2imode         = base->ezweb2imode;
    mrg->ezweb2imode_tail    = base->ezweb2imode_tail;
    mrg->softbank2imode      = base->softbank2imode;
    mrg->softbank2imode_tail = base->softbank2imode_tail;
    chxj_emoji_init(mrg);
  }
  else {
    mrg->emoji               = add->emoji;
    mrg->emoji_tail          = add->emoji_tail;
    mrg->emoji_data_file     = apr_pstrdup(p, add->emoji_data_file);
    mrg->ezweb2imode         = add->ezweb2imode;
    mrg->ezweb2imode_tail    = add->ezweb2imode_tail;
    mrg->softbank2imode      = add->softbank2imode;
    mrg->softbank2imode_tail = add->softbank2imode_tail;
    chxj_emoji_init(mrg);
  }

  if (add->image == CHXJ_IMG_OFF) 
    mrg->image = base->image;
  else 
    mrg->image = add->image;


  if (strcasecmp(add->image_cache_dir ,DEFAULT_IMAGE_CACHE_DIR)==0) 
    mrg->image_cache_dir = apr_pstrdup(p, base->image_cache_dir);
  else 
    mrg->image_cache_dir = apr_pstrdup(p, add->image_cache_dir);

   if (add->image_cache_limit) {
     mrg->image_cache_limit = add->image_cache_limit;
   }
   else {
     mrg->image_cache_limit = base->image_cache_limit;
   }

  if (add->image_copyright) 
    mrg->image_copyright = apr_pstrdup(p, add->image_copyright);
  else
    mrg->image_copyright = apr_pstrdup(p, base->image_copyright);

  if (add->server_side_encoding) {
    mrg->server_side_encoding = apr_pstrdup(p, add->server_side_encoding);
  }
  else 
  if (base->server_side_encoding) {
    mrg->server_side_encoding = apr_pstrdup(p, base->server_side_encoding);
  }
  else {
    mrg->server_side_encoding = apr_pstrdup(p, DEFAULT_SERVER_SIDE_ENCODING);
  }

  mrg->convrules    = apr_array_append(p, add->convrules, base->convrules);

  if (add->cookie_db_dir) {
    mrg->cookie_db_dir = apr_pstrdup(p, add->cookie_db_dir);
  }
  else
  if (base->cookie_db_dir) {
    mrg->cookie_db_dir = apr_pstrdup(p, base->cookie_db_dir);
  }
  else {
    mrg->cookie_db_dir = NULL;
  }

  if (add->cookie_timeout) {
    mrg->cookie_timeout   = add->cookie_timeout;
  }
  else
  if (base->cookie_db_dir) {
    mrg->cookie_timeout   = base->cookie_timeout;
  }
  else {
    mrg->cookie_timeout   = 0;
  }

#if defined(USE_MYSQL_COOKIE)
  if (add->mysql.host) {
    mrg->mysql.host = apr_pstrdup(p, add->mysql.host);
  }
  else if (base->mysql.host) {
    mrg->mysql.host = apr_pstrdup(p, base->mysql.host);
  }
  else {
    mrg->mysql.host = NULL;
  }
  if (add->mysql.port) {
    mrg->mysql.port = add->mysql.port;
  }
  else if (base->mysql.port) {
    mrg->mysql.port = base->mysql.port;
  }
  else {
    mrg->mysql.port = 0;
  }

  if (add->mysql.database) {
    mrg->mysql.database = apr_pstrdup(p, add->mysql.database);
  }
  else if (base->mysql.database) {
    mrg->mysql.database = apr_pstrdup(p, base->mysql.database);
  }
  else {
    mrg->mysql.database = NULL;
  }

  if (add->mysql.username) {
    mrg->mysql.username = apr_pstrdup(p, add->mysql.username);
  }
  else if (base->mysql.username) {
    mrg->mysql.username = apr_pstrdup(p, base->mysql.username);
  }
  else {
    mrg->mysql.username = NULL;
  }

  if (add->mysql.password) {
    mrg->mysql.password = apr_pstrdup(p, add->mysql.password);
  }
  else if (base->mysql.password) {
    mrg->mysql.password = apr_pstrdup(p, base->mysql.password);
  }
  else {
    mrg->mysql.password = NULL;
  }

  if (add->mysql.tablename) {
    mrg->mysql.tablename = apr_pstrdup(p, add->mysql.tablename);
  }
  else if (base->mysql.tablename) {
    mrg->mysql.tablename = apr_pstrdup(p, base->mysql.tablename);
  }
  else {
    mrg->mysql.tablename = NULL;
  }

  if (add->mysql.socket_path) {
    mrg->mysql.socket_path = apr_pstrdup(p, add->mysql.socket_path);
  }
  else if (base->mysql.socket_path) {
    mrg->mysql.socket_path = apr_pstrdup(p, base->mysql.socket_path);
  }
  else {
    mrg->mysql.socket_path = NULL;
  }

  if (add->mysql.charset) {
    mrg->mysql.charset = apr_pstrdup(p, add->mysql.charset);
  }
  else if (base->mysql.charset) {
    mrg->mysql.charset = apr_pstrdup(p, base->mysql.charset);
  }
  else {
    mrg->mysql.charset = NULL;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (add->memcache.host) {
    mrg->memcache.host = apr_pstrdup(p, add->memcache.host);
  }
  else if (base->memcache.host) {
    mrg->memcache.host = apr_pstrdup(p, base->memcache.host);
  }
  else {
    mrg->memcache.host = NULL;
  }
  if (add->memcache.port) {
    mrg->memcache.port = add->memcache.port;
  }
  else if (base->memcache.port) {
    mrg->memcache.port = base->memcache.port;
  }
  else {
    mrg->memcache.port = 0;
  }
#endif
  if (add->cookie_store_type) {
    mrg->cookie_store_type = add->cookie_store_type;
  }
  else if (base->cookie_store_type) {
    mrg->cookie_store_type = base->cookie_store_type;
  }
  else {
    mrg->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  }
  if (add->cookie_lazy_mode) {
    mrg->cookie_lazy_mode = add->cookie_lazy_mode;
  }
  else if (base->cookie_lazy_mode) {
    mrg->cookie_lazy_mode = base->cookie_lazy_mode;
  }
  else {
    mrg->cookie_lazy_mode = 0;
  }
  return mrg;
}


static int
chxj_command_parse_take5(
  const char *arg, 
  char       **prm1, 
  char       **prm2, 
  char       **prm3, 
  char       **prm4, 
  char       **prm5)
{
  int isquoted;
  char *strp;

  strp = (char *)arg;

  for (;*strp == ' '||*strp == '\t'; strp++) ;

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }

  *prm1 = strp;

  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm2 = strp;
    *prm3 = strp;
    *prm4 = strp;
    *prm5 = strp;
    return 1;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++) ;

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }

  *prm2 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm3 = strp;
    *prm4 = strp;
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm3 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm4 = strp;
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm4 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }

  if (! *strp) {
    *prm5 = strp;
    return 0;
  }

  *strp++ = '\0';

  for (;*strp == ' '||*strp == '\t'; strp++);

  isquoted = 0; 
  if (*strp == '"') { 
    isquoted = 1;
    strp++;
  }
  *prm5 = strp;
  for (; *strp != '\0'; strp++) {
    if ((isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (*strp == '\\' && (*(strp+1) == ' ' || *(strp+1) == '\t'))) {
      strp++;
      continue;
    }

    if ((!isquoted && (*strp == ' ' || *strp == '\t'))
    ||  (isquoted  && *strp == '"'))
      break;
  }
  *strp = '\0';

  return 0;
}


/**
 * The device definition file is loaded. 
 *
 * @param arg     [i]   The name of the device definition file is specified.
 * @param mconfig [i/o] The pointer to a set structure is specified. 
 * @param parms   [i]   
 */
static const char * 
cmd_load_device_data(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "mod_chxj: device data filename too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->device_data_file = apr_pstrdup(parms->pool, arg);

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc*)&doc, (const char*)arg);
  chxj_load_device_data(&doc,parms->pool, conf);
  qs_all_free(&doc, QX_LOGMARK);

  return NULL;
}


/**
 * Device definition information is loaded. 
 *
 * @param parms 
 * @param arg     [i]   The name of the device definition file is specified. 
 * @param mconfig [i/o] The pointer to a set structure is specified. 
 * @return 
 */
static const char *
cmd_load_emoji_data(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config  *conf;
  char             *rtn;
  Doc              doc;

  doc.r = NULL;


  if (strlen(arg) > 256) 
    return "mod_chxj: emoji data filename too long.";

  conf = (mod_chxj_config*)mconfig;
  conf->emoji_data_file = apr_pstrdup(parms->pool, arg);
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  qs_parse_file((Doc*)&doc, (const char*)arg);
  rtn = chxj_load_emoji_data(&doc,parms->pool, conf);
  qs_all_free(&doc, QX_LOGMARK);
  return rtn;
}


static const char *
cmd_set_image_engine(cmd_parms *UNUSED(parms), void *mconfig, const char *arg) 
{
  mod_chxj_config *conf;
  Doc             doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "image uri is too long.";

  conf = (mod_chxj_config *)mconfig;
  if (strcasecmp("ON", arg) == 0)
    conf->image = CHXJ_IMG_ON;
  else
    conf->image = CHXJ_IMG_OFF;

  return NULL;
}


static const char *
cmd_set_image_cache_dir(cmd_parms *parms, void *mconfig, const char *arg)
{
  mod_chxj_config  *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "cache dir name is too long.";

  conf = (mod_chxj_config *)mconfig;
  conf->image_cache_dir = apr_pstrdup(parms->pool, arg);

  return NULL;
}


static const char *
cmd_set_image_cache_limit(cmd_parms *parms, void *mconfig, const char* arg)
{
  mod_chxj_config* conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > IMAGE_CACHE_LIMIT_FMT_LEN)
    return "cache size is too long.";

  conf = (mod_chxj_config*)mconfig;
  errno = 0;
  /*
   * I use strtol function because strtoul is not portable function.
   */
  conf->image_cache_limit = (unsigned long)strtol(arg, NULL, 10);
  switch (errno) {
  case EINVAL:
    return apr_psprintf(parms->pool, "ChxjImageCacheLimit invalid value [%s] errno:[%d]", arg, errno);
  case ERANGE:
    return apr_psprintf(parms->pool, "ChxjImageCacheLimit Out of range [%s] errno:[%d]", arg, errno);
  default:
    break;
  }
  return NULL;
}


static const char*
cmd_set_image_copyright(cmd_parms *parms, void *mconfig, const char *arg) 
{
  mod_chxj_config  *conf;
  Doc              doc;

  doc.r = NULL;

  if (strlen(arg) > 256) 
    return "Copyright Flag is too long.";

  conf = (mod_chxj_config *)mconfig;
  conf->image_copyright = apr_pstrdup(parms->pool, arg);

  return NULL;
}


static const char *
cmd_convert_rule(cmd_parms *cmd, void *mconfig, const char *arg)
{
  int                 mode;
  ap_regex_t          *regexp;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *newrule;
  char                *prm1;
  char                *prm2;
  char                *prm3;
  char                *prm4;
  char                *prm5;
  char                *pstate;
  char                *action;
  char                *pp;

  dconf = (mod_chxj_config *)mconfig;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjConvertRule: is too long.";

  dconf = (mod_chxj_config *)mconfig;
  if (dconf->convrules == NULL)
    dconf->convrules   = apr_array_make(cmd->pool, 
                                        2, 
                                        sizeof(chxjconvrule_entry));

  newrule = apr_array_push(dconf->convrules);

  newrule->flags  = 0;
  newrule->action = 0;

  if (chxj_command_parse_take5(arg, &prm1, &prm2, &prm3, &prm4, &prm5))
    return "ChxjConvertRule: bad argument line";

  newrule->pattern = apr_pstrdup(cmd->pool, prm1);

  /* Parse action */
  for (;;) {
    if ((action = apr_strtok(prm2, ",", &pstate)) == NULL)
      break;
    prm2 = NULL;
    switch(*action) {
    case 'e':
    case 'E':
      if (strcasecmp(CONVRULE_ENGINE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_ENGINE_ON_BIT;
      }
      else
      if (strcasecmp(CONVRULE_ENGINE_OFF_CMD, action) == 0) {
        newrule->action |= CONVRULE_ENGINE_OFF_BIT;
      }
      break;

    case 'C':
    case 'c':
      if (strcasecmp(CONVRULE_COOKIE_ON_CMD, action) == 0) {
        newrule->action |= CONVRULE_COOKIE_ON_BIT;
      }
      break;
    default:
      break;
    }
  }
  
  pp = prm1;
  if (*pp == '!') {
    newrule->flags |= CONVRULE_FLAG_NOTMATCH;
    pp++;
  }

  mode = AP_REG_EXTENDED;
  if ((regexp = ap_pregcomp((apr_pool_t *)cmd->pool, (const char *)pp, mode)) == NULL)
    return "RewriteRule: cannot compile regular expression ";

  newrule->regexp = regexp;
  if (*prm3)
    newrule->encoding = apr_pstrdup(cmd->pool, prm3);
  else
    newrule->encoding = apr_pstrdup(cmd->pool, "none");

  newrule->pc_flag = CONVRULE_PC_FLAG_OFF_BIT;
  if (*prm4)
    if (strcasecmp(CONVRULE_PC_FLAG_ON_CMD, prm4) == 0)
      newrule->pc_flag = CONVRULE_PC_FLAG_ON_BIT;

  newrule->user_agent = NULL;
  if (*prm5)
    newrule->user_agent = apr_pstrdup(cmd->pool, prm5);
    
  return NULL;
}


static const char *
cmd_set_cookie_dir(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieDir is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->cookie_db_dir = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_timeout(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieTimeout is too long.";

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieTimeout is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->cookie_timeout = chxj_atoi(arg);

  return NULL;
}


#if defined(USE_MYSQL_COOKIE)
static const char *
cmd_set_cookie_mysql_database(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlDatabase is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.database = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_username(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlUsername is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.username = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_password(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlPassword is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.password = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_table_name(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlTableName is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.tablename = apr_pstrdup(cmd->pool, arg);

  return NULL;
}

static const char *
cmd_set_cookie_mysql_port(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlPort is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieMysqlPort is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.port = chxj_atoi(arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_host(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlHost is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.host = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_socket_path(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 4096) 
    return "mod_chxj: ChxjCookieMysqlSocketPath is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.socket_path = apr_pstrdup(cmd->pool, arg);

  return NULL;
}


static const char *
cmd_set_cookie_mysql_charset(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMysqlCharset is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->mysql.charset = apr_pstrdup(cmd->pool, arg);

  return NULL;
}
#endif
#if defined(USE_MEMCACHE_COOKIE)
static const char *
cmd_set_cookie_memcache_port(
  cmd_parms   *UNUSED(cmd), 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMemcachePort is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (chxj_chk_numeric(arg) != 0)
    return "mod_chxj: ChxjCookieMemcachePort is not numeric.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->memcache.port = (apr_port_t)chxj_atoi(arg);

  return NULL;
}


static const char *
cmd_set_cookie_memcache_host(
  cmd_parms   *cmd, 
  void        *mconfig, 
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255) 
    return "mod_chxj: ChxjCookieMemcacheHost is too long.";

  dconf = (mod_chxj_config *)mconfig;

  dconf->memcache.host = apr_pstrdup(cmd->pool, arg);

  return NULL;
}
#endif
static const char *
cmd_set_cookie_lazy_mode(
  cmd_parms   *UNUSED(cmd),
  void        *mconfig,
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255)
    return "mod_chxj: ChxjCookieLazyMode is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (strcasecmp("TRUE",arg) == 0) {
    dconf->cookie_lazy_mode = COOKIE_LAZY_ON;
  }
  else {
    dconf->cookie_lazy_mode = COOKIE_LAZY_OFF;
  }

  return NULL;
}

static const char *
cmd_set_cookie_store_type(
  cmd_parms   *UNUSED(cmd),
  void        *mconfig,
  const char  *arg)
{
  mod_chxj_config  *dconf;

  if (strlen(arg) > 255)
    return "mod_chxj: ChxjCookieStoreType is too long.";

  dconf = (mod_chxj_config *)mconfig;

  if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_DBM, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_DBM;
  }
  else if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_MYSQL, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_MYSQL;
  }
  else if (strcasecmp(CHXJ_COOKIE_STORE_TYPE_MEMCACHE, arg) == 0) {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_MEMCACHE;
  }
  else {
    dconf->cookie_store_type = COOKIE_STORE_TYPE_NONE;
  }

  return NULL;
}


static const command_rec cmds[] = {
  AP_INIT_TAKE1(
    "ChxjLoadDeviceData",
    cmd_load_device_data,
    NULL,
    OR_ALL,
    "Load Device Data"),
  AP_INIT_TAKE1(
    "ChxjLoadEmojiData",
    cmd_load_emoji_data,
    NULL,
    OR_ALL,
    "Load Emoji Data"),
  AP_INIT_TAKE1(
    "ChxjImageEngine",
    cmd_set_image_engine,
    NULL,
    OR_ALL,
    "Convert Target URI"),
  AP_INIT_TAKE1(
    "ChxjImageCacheDir",
    cmd_set_image_cache_dir,
    NULL,
    OR_ALL,
    "Image Cache Directory"),
  AP_INIT_TAKE1(
    "ChxjImageCacheLimit",
    cmd_set_image_cache_limit,
    NULL,
    OR_ALL,
    "Image Cache Limit"),
  AP_INIT_TAKE1(
    "ChxjImageCopyright",
    cmd_set_image_copyright,
    NULL,
    OR_ALL,
    "Copyright Flag"),
  AP_INIT_RAW_ARGS(
    "ChxjConvertRule",
    cmd_convert_rule,
    NULL, 
    OR_FILEINFO,
    "an URL-applied regexp-pattern and a substitution URL"),
  AP_INIT_TAKE1(
    "ChxjCookieDir",
    cmd_set_cookie_dir,
    NULL,
    OR_ALL,
    "save cookie.db directory."),
  AP_INIT_TAKE1(
    "ChxjCookieTimeout",
    cmd_set_cookie_timeout,
    NULL,
    OR_ALL,
    "The compulsion time-out time of the cookie is specified. "),
  AP_INIT_TAKE1(
    "ChxjCookieStoreType",
    cmd_set_cookie_store_type,
    NULL,
    OR_ALL,
    "It specifies preserving of the cookie ahead. (DBM/MYSQL/MEMCACHE)"),
  AP_INIT_TAKE1(
    "ChxjCookieLazyMode",
    cmd_set_cookie_lazy_mode,
    NULL,
    OR_ALL,
    "OneTimeID is negligently done. (TRUE/FALSE)"),
#if defined(USE_MYSQL_COOKIE)
  AP_INIT_TAKE1(
    "ChxjCookieMysqlHost",
    cmd_set_cookie_mysql_host,
    NULL,
    OR_ALL,
    "The MySQL database host used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlPort",
    cmd_set_cookie_mysql_port,
    NULL,
    OR_ALL,
    "The MySQL database port used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlDatabase",
    cmd_set_cookie_mysql_database,
    NULL,
    OR_ALL,
    "The MySQL database name used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlUsername",
    cmd_set_cookie_mysql_username,
    NULL,
    OR_ALL,
    "The MySQL username used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlPassword",
    cmd_set_cookie_mysql_password,
    NULL,
    OR_ALL,
    "The MySQL password used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlTableName",
    cmd_set_cookie_mysql_table_name,
    NULL,
    OR_ALL,
    "The MySQL table name used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlSocketPath",
    cmd_set_cookie_mysql_socket_path,
    NULL,
    OR_ALL,
    "The MySQL socket path used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMysqlCharset",
    cmd_set_cookie_mysql_charset,
    NULL,
    OR_ALL,
    "The MySQL charset used by saving Cookie"),
#endif
#if defined(USE_MEMCACHE_COOKIE)
  AP_INIT_TAKE1(
    "ChxjCookieMemcacheHost",
    cmd_set_cookie_memcache_host,
    NULL,
    OR_ALL,
    "The Memcached host used by saving Cookie"),
  AP_INIT_TAKE1(
    "ChxjCookieMemcachePort",
    cmd_set_cookie_memcache_port,
    NULL,
    OR_ALL,
    "The Memcached port used by saving Cookie"),
#endif
  {NULL}
};


/*----------------------------------------------------------------------------*/
/* Dispatch list for API hooks                                                */
/*----------------------------------------------------------------------------*/
module AP_MODULE_DECLARE_DATA chxj_module = {
  STANDARD20_MODULE_STUFF, 
  chxj_create_per_dir_config,          /* create per-dir    config structures */
  chxj_merge_per_dir_config,           /* merge  per-dir    config structures */
  chxj_config_server_create,           /* create per-server config structures */
  NULL,                                /* merge  per-server config structures */
  cmds,                                /* table of config file commands       */
  chxj_register_hooks                  /* register hooks                      */
};
/*
 * vim:ts=2 et
 */
