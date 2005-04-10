/*
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 * Copyright (C) 2005 Atsushi Konno All rights reserved.
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
#include "ap_regex.h"

#include "mod_chxj.h"
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

#include "chxj_img_conv_format.h"
#include "chxj_qr_code.h"


#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif

#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif

#ifdef PACKAGE_STRING
#undef PACKAGE_STRING
#endif

#ifdef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif

#include "config.h"

#define CHXJ_VERSION_PREFIX PACKAGE_NAME "/"
#define CHXJ_VERSION        PACKAGE_VERSION

/**
 * It converts it from CHTML into ML corresponding to each model. 
 *
 * @param r   [i]
 * @param src [i]   It is former HTML character string. 
 * @param len [i/o] It is length of former HTML character string. 
 */
static char* 
chxj_exchange(request_rec *r, const char** src, apr_size_t* len)
{
  char *user_agent;
  char *dst = apr_pstrcat(r->pool, (char*)*src, NULL);

  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "User-Agent:[%s]", user_agent);

  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, 
          "start chxj_exchange()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, 
          "content type is %s", r->content_type);

  if (strncmp(r->content_type, "text/html",   9) != 0)
  {
    ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "content type is %s", r->content_type);
    return (char*)*src;
  }

  if (!r->header_only) 
  {
    device_table *spec = chxj_specified_device(r, user_agent);
    if (spec->html_spec_type == CHXJ_SPEC_Chtml_1_0) 
    {
      /*----------------------------------------------------------------------*/
      /* DoCoMo i-Mode 1.0                                                    */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select DoCoMo i-Mode 1.0 ");
      dst = chxj_exchange_chtml10(r, spec, *src, *len, len);

    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_Chtml_2_0) 
    {
      /*----------------------------------------------------------------------*/
      /* DoCoMo i-Mode 2.0                                                    */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select DoCoMo i-Mode 2.0 ");
      dst = chxj_exchange_chtml20(r, spec, *src, *len, len);
    }
    else 
    if (spec->html_spec_type == CHXJ_SPEC_Chtml_3_0) 
    {
      /*----------------------------------------------------------------------*/
      /* DoCoMo i-Mode 3.0                                                    */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select DoCoMo i-Mode 3.0 ");
      dst = chxj_exchange_chtml30(r, spec, *src, *len, len);
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_Chtml_4_0) 
    {
      /*----------------------------------------------------------------------*/
      /* DoCoMo i-Mode 4.0                                                    */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select DoCoMo i-Mode 4.0 ");
      dst = chxj_exchange_chtml30(r, spec, *src, *len, len);
    }
    else 
    if (spec->html_spec_type == CHXJ_SPEC_Chtml_5_0) 
    {
      /*----------------------------------------------------------------------*/
      /* DoCoMo i-Mode 5.0                                                    */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select DoCoMo i-Mode 5.0 ");
      dst = chxj_exchange_chtml30(r, spec, *src, *len, len);
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0) 
    {
      /*----------------------------------------------------------------------*/
      /* AU XHtml Mobile 1.0 (XHtml Basic 1.0 extended)                       */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select XHTML Mobile 1.0");
      dst = chxj_exchange_xhtml_mobile_1_0(r, spec, *src, *len, len);
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_Hdml) 
    {
      /*----------------------------------------------------------------------*/
      /* AU HDML Version 3.0 Only                                             */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select HDML");
      dst = chxj_exchange_hdml(r, spec, *src, *len, len);
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_Jhtml) 
    {
      /*----------------------------------------------------------------------*/
      /* J-Phone and Vodaphone                                                */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select JHTML");
      dst = chxj_exchange_jhtml(r, spec, *src, *len, len);
    }
    else 
    {
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "select ?????");
      ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "html_spec_type[%d]", 
                      spec->html_spec_type);
      ap_set_content_type(r, "text/html; charset=Windows-31J");
    }
  }

  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "end chxj_exchange()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "Length=[%d]", *len);
  if (*len == 0)
  {
    dst = apr_psprintf(r->pool, "\n");
    *len = 1;
  }
  dst[*len] = 0;
  return dst;
}
/**
 * It converts it from POSTDATA .
 *
 * @param r   [i]
 * @param src [i]   It is POSTDATA character string.
 * @param len [i/o] It is length of former HTML character string.
 */
static char*
chxj_input_exchange(request_rec *r, const char** src, apr_size_t* len)
{
  char* pair;
  char* name;
  char* value;
  char* pstate;
  char* vstate;
  char* s = apr_pstrdup(r->pool, *src);

  char* result;

  result = qs_alloc_zero_byte_string(r);

  /* _chxj_dmy */
  /* _chxj_c_ */
  /* _chxj_r_ */
  /* _chxj_s_ */
  for (;;) 
  {
    pair = apr_strtok(s, "&", &pstate);
    if (pair == NULL)
    {
      break;
    } 
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strncasecmp(name, "_chxj", 5) != 0) 
    {
      if (strlen(result) != 0)
      {
        result = apr_pstrcat(r->pool, result, "&", NULL);
      }
      result = apr_pstrcat(r->pool, result, name, "=", value, NULL);
    }
    else
    if (strncasecmp(name, "_chxj_c_", 8) == 0 
    ||  strncasecmp(name, "_chxj_r_", 8) == 0
    ||  strncasecmp(name, "_chxj_s_", 8) == 0)
    {
      if (value == NULL)
      {
        continue;
      }

      if (strlen(value) == 0)
      {
        continue;
      }

      if (strlen(result) != 0)
      {
        result = apr_pstrcat(r->pool, result, "&", NULL);
      }
      result = apr_pstrcat(r->pool, result, &name[8], "=", value, NULL);
    }
  }
  *len = strlen(result);
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
  request_rec*        r = f->r;
  conn_rec*           c = r->connection;
  apr_status_t        rv;
  apr_bucket_brigade* bb;
  apr_bucket*         b;

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "start pass_data_to_filter()");

  bb = apr_brigade_create(r->pool, c->bucket_alloc);
  b  = apr_bucket_transient_create(data, len, c->bucket_alloc);

  APR_BRIGADE_INSERT_TAIL(bb, b);
  b = apr_bucket_eos_create(f->c->bucket_alloc);
  APR_BRIGADE_INSERT_TAIL(bb, b);

  rv = ap_pass_brigade(f->next, bb);
  if (rv != APR_SUCCESS) 
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, rv, r,
                   "ap_pass_brigade()");
    return rv;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, rv, r, "end pass_data_to_filter()");
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
  request_rec* r = f->r;
  apr_status_t rv = APR_SUCCESS;
  apr_bucket*  b;
  const char*  data;
  char*       contentLength;
  apr_size_t  len;
  mod_chxj_ctx* ctx;


  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
          "start of chxj_output_filter()");
  if (!f->ctx) 
  {
    if ((f->r->proto_num >= 1001) 
    &&  !f->r->main 
    &&  !f->r->prev) 
    {
      f->r->chunked = 1;
    }
  }


  for (b = APR_BRIGADE_FIRST(bb);
       b != APR_BRIGADE_SENTINEL(bb); 
       b = APR_BUCKET_NEXT(b)) 
  {
    if (APR_BUCKET_IS_EOS(b)) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "eos");
      /*----------------------------------------------------------------------*/
      /* End Of File                                                          */
      /*----------------------------------------------------------------------*/
      if (f->ctx) 
      {
        ctx = (mod_chxj_ctx*)f->ctx;
        if (strncmp(r->content_type, "text/html",   9) == 0)
        {
          if (ctx->len)
          {
            char* tmp = apr_palloc(r->pool, ctx->len + 1);
            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "input data=[%s]", tmp);
            ctx->buffer = chxj_exchange(r, (const char**)&tmp, (apr_size_t*)&ctx->len);
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "output data=[%.*s]", ctx->len,ctx->buffer);
          }
          else 
          {
            ctx->buffer = apr_psprintf(r->pool, "\n");
            ctx->len += 1;
          }
        }
        if (strncmp(r->content_type, "image/", 6) == 0)
        {
          if (ctx->len)
          {
            char* tmp = apr_palloc(r->pool, ctx->len + 1);
            memset(tmp, 0, ctx->len + 1);
            memcpy(tmp, ctx->buffer, ctx->len);
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "input data=[%s]", tmp);
            ctx->buffer = chxj_exchange_image(r, (const char**)&tmp, (apr_size_t*)&ctx->len);
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "output data=[%.*s]", ctx->len,ctx->buffer);
          }
        }
        contentLength = apr_psprintf(r->pool, "%d", ctx->len);
        apr_table_setn(r->headers_out, "Content-Length", contentLength);
        
        if (ctx->len > 0)
        {
          rv = pass_data_to_filter(f, (const char*)ctx->buffer, (apr_size_t)ctx->len);
        }
        f->ctx = NULL;
        return rv;
      }
      else
      {
        apr_table_setn(r->headers_out, "Content-Length", "0");
        rv = pass_data_to_filter(f, (const char*)"", (apr_size_t)0);
        return rv;
      }
    }
    else
    if (apr_bucket_read(b, &data, &len, APR_BLOCK_READ) == APR_SUCCESS) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "read data[%.*s]",len, data);

      if (f->ctx == NULL) 
      {
        /*--------------------------------------------------------------------*/
        /* Start                                                              */
        /*--------------------------------------------------------------------*/
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "new context");
        ctx = (mod_chxj_ctx*)apr_palloc(r->pool, sizeof(mod_chxj_ctx));
        if (len > 0)
        {
          ctx->buffer = apr_palloc(r->pool, len);
          memcpy(ctx->buffer, data, len);
        }
        else 
        {
          ctx->buffer = apr_palloc(r->pool, 1);
          ctx->buffer = '\0';
        }
        ctx->len = len;
        f->ctx = (void*)ctx;
      }
      else 
      {
        /*--------------------------------------------------------------------*/
        /* append data                                                        */
        /*--------------------------------------------------------------------*/
        char* tmp;
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "append data start");
        ctx = (mod_chxj_ctx*)f->ctx;

        if (len > 0)
        {
          tmp = apr_palloc(r->pool, ctx->len);
          memcpy(tmp, ctx->buffer, ctx->len);
          ctx->buffer = apr_palloc(r->pool, ctx->len + len);
          memcpy(ctx->buffer, tmp, ctx->len);
          memcpy(&ctx->buffer[ctx->len], data, len);
          ctx->len += len;
        }
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "append data end");
      }
    }
  }
  apr_brigade_destroy(bb);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "end of output filter");
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
chxj_input_filter(ap_filter_t*        f, 
                 apr_bucket_brigade*  bb,
                 ap_input_mode_t      mode, 
                 apr_read_type_e      block,
                 apr_off_t            readbytes)
{
  request_rec*        r = f->r;
  apr_status_t        rv;
  conn_rec*           c = r->connection;
  apr_bucket*         b;
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for output                                        */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade* ibb;            
  /*--------------------------------------------------------------------------*/
  /* It is the brigade area for input                                         */
  /*--------------------------------------------------------------------------*/
  apr_bucket_brigade* obb;
  apr_size_t          len;
  apr_bucket*         tmp_heap;
  apr_bucket*         eos;
  const char*         data;
  char*               data_bucket;
  char*               data_brigade;

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "start of chxj_input_filter()");

  data_brigade = qs_alloc_zero_byte_string(r);


  ibb = apr_brigade_create(r->pool, c->bucket_alloc);
  obb = apr_brigade_create(r->pool, c->bucket_alloc);

  rv = ap_get_brigade(f->next, ibb, mode, block, readbytes);
  if (rv != APR_SUCCESS)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, rv, r,
                    "ap_get_brigade() failed");
    return rv;
  }

  APR_BRIGADE_FOREACH(b, ibb) 
  {
    rv = apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
    if (rv != APR_SUCCESS) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r,
                     "apr_bucket_read() failed");
      return rv;
    }

    if (data != NULL)
    {
      data_bucket = apr_palloc(r->pool, len+1);
      memset((void*)data_bucket, 0, len+1);
      memcpy(data_bucket, data, len);
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, f->r, "(in)POSTDATA:[%s]", data_bucket);
  
      data_brigade = apr_pstrcat(r->pool, data_brigade, data_bucket, NULL);
    }

    if (APR_BUCKET_IS_EOS(b)) 
    {
      break;
    }
  }
  apr_brigade_cleanup(ibb);

  len = strlen(data_brigade);
  if (len == 0)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, rv, r,
                    "data_brigade length is 0");
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "end of chxj_input_filter()");
    ap_remove_input_filter(f);
    return ap_get_brigade(f->next, bb, mode, block, readbytes);
  }
  data_brigade = chxj_input_exchange(r, (const char**)&data_brigade, (apr_size_t*)&len);

  if (len > 0)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, f->r, "(in:exchange)POSTDATA:[%s]", data_brigade);

    obb = apr_brigade_create(r->pool, c->bucket_alloc);
    tmp_heap = apr_bucket_heap_create(data_brigade, len, NULL, f->c->bucket_alloc);
    eos      = apr_bucket_eos_create(f->c->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(obb, tmp_heap);
    APR_BRIGADE_INSERT_TAIL(obb, eos);
    APR_BRIGADE_CONCAT(bb, obb);
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "end of chxj_input_filter()");
  return APR_SUCCESS;
}

/**
 * It is processing when the module ends. The shared memory etc. are liberated.
 *
 * @param data [i]  The pointer to the server_rec structure is specified. 
 * @return APR_SUCCESS
 *
 */
apr_status_t 
chxj_init_module_kill(void *data)
{
  server_rec *base_server = (server_rec *)data;
  mod_chxj_global_config* conf;

  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, base_server, 
                  "start chxj_init_module_kill()");

  /*--------------------------------------------------------------------------*/
  /* The setting of each server is acquired.                                  */
  /*--------------------------------------------------------------------------*/
  conf = ap_get_module_config(base_server->module_config, &chxj_module);

  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, base_server, 
                  "end chxj_init_module_kill()");
  return APR_SUCCESS;
}

static mod_chxj_global_config*
chxj_global_config_create(apr_pool_t* pool, server_rec* s)
{
  mod_chxj_global_config* conf;
  void*           param;

  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s,
                  "start chxj_global_config_create()");


  apr_pool_userdata_get(&param, CHXJ_MOD_CONFIG_KEY, pool);
  conf = (mod_chxj_global_config*)param;
  apr_pool_cleanup_register(pool, s,
                               chxj_init_module_kill,
                               apr_pool_cleanup_null);
  if (conf)
  {
    /*------------------------------------------------------------------------*/
    /* reused for lifetime of the server                                      */
    /*------------------------------------------------------------------------*/
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s, 
                   "end chxj_global_config_create() reused.");
    return conf; 
  }

  /*--------------------------------------------------------------------------*/
  /* allocate an own subpool which survives server restarts                   */
  /*--------------------------------------------------------------------------*/
  conf = (mod_chxj_global_config*)apr_palloc(pool, 
                  sizeof(mod_chxj_global_config));
  conf->client_shm  = NULL;
  conf->client_lock = NULL;
  memset(conf->client_lock_file_name, 0, sizeof(conf->client_lock_file_name));


  apr_pool_userdata_set(conf, CHXJ_MOD_CONFIG_KEY,
                              apr_pool_cleanup_null,
                              pool);
  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s,
                  "end chxj_global_config_create()");
  return conf;
}
/**
 * initialize chxj module
 */
static int 
chxj_init_module(apr_pool_t *p, 
                  apr_pool_t *plog, 
                  apr_pool_t *ptemp, 
                  server_rec *s)
{
  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s,
                  "start chxj_init_module()");

  ap_add_version_component(p, CHXJ_VERSION_PREFIX CHXJ_VERSION);

  ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, s,
                  "end chxj_init_module()");
  return OK;
}
/**
 * A set structure of each server is generated. 
 * 
 * @param p
 * @param s
 */
static void*
chxj_config_server_create(apr_pool_t *p, server_rec *s)
{
  mod_chxj_global_config *gc;

  gc = chxj_global_config_create(p,s);

  return gc;
}


static int
chxj_translate_name(request_rec *r)
{
  return chxj_trans_name(r);
}

/**
 * The hook is registered.
 *
 * @param p
 */
static void 
chxj_register_hooks(apr_pool_t *p)
{
  ap_hook_post_config(chxj_init_module,
                      NULL,
                      NULL,
                      APR_HOOK_MIDDLE);
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
  ap_hook_handler(chxj_img_conv_format_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_handler(chxj_qr_code_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_translate_name(chxj_translate_name, NULL, NULL, APR_HOOK_MIDDLE);
}

/**
 * A set structure according to the directory is generated. 
 *
 * @param p
 * @param arg
 */
static void* 
chxj_create_per_dir_config(apr_pool_t *p, char *arg) 
{
  mod_chxj_config* conf;

  conf = apr_pcalloc(p, sizeof(mod_chxj_config));
  conf->device_data_file = NULL;
  conf->devices          = NULL;
  conf->emoji_data_file  = NULL;
  conf->emoji            = NULL;
  conf->emoji_tail       = NULL;
  conf->image            = CHXJ_IMG_OFF;
  conf->image_cache_dir  = apr_psprintf(p, "%s",DEFAULT_IMAGE_CACHE_DIR);

  /* Default is copyleft */
  conf->image_copyright = NULL; 
  return conf;
}
/*
 *  Merge per-directory CHXJ configurations
 */
static void*
chxj_merge_per_dir_config(apr_pool_t *p, void *basev, void *addv)
{
  mod_chxj_config *base = (mod_chxj_config*)basev;
  mod_chxj_config *add  = (mod_chxj_config*)addv;
  mod_chxj_config *mrg  = (mod_chxj_config*)apr_palloc(p, sizeof(mod_chxj_config));

  mrg->device_data_file = NULL;
  mrg->devices          = NULL;
  mrg->emoji_data_file  = NULL;
  mrg->image            = CHXJ_IMG_OFF;
  mrg->image_cache_dir  = NULL;
  mrg->image_copyright  = NULL;
  mrg->emoji            = NULL;
  mrg->emoji_tail       = NULL;
  if (add->device_data_file == NULL)
  {
    mrg->devices = base->devices;
    mrg->device_data_file = apr_pstrdup(p, base->device_data_file);
  }
  else
  {
    mrg->devices = add->devices;
    mrg->device_data_file = apr_pstrdup(p, add->device_data_file);
  }

  if (add->emoji_data_file == NULL)
  {
    mrg->emoji = base->emoji;
    mrg->emoji_tail = base->emoji_tail;
    mrg->emoji_data_file = apr_pstrdup(p, base->emoji_data_file);
  }
  else
  {
    mrg->emoji = add->emoji;
    mrg->emoji_tail = add->emoji_tail;
    mrg->emoji_data_file = apr_pstrdup(p, add->emoji_data_file);
  }

  if (add->image == CHXJ_IMG_OFF)
  {
    mrg->image = base->image;
  }
  else
  {
    mrg->image = add->image;
  }

  if (strcasecmp(add->image_cache_dir ,DEFAULT_IMAGE_CACHE_DIR)==0)
  {
    mrg->image_cache_dir = apr_pstrdup(p, base->image_cache_dir);
  }
  else
  {
    mrg->image_cache_dir = apr_pstrdup(p, add->image_cache_dir);
  }

  if (add->image_copyright == NULL && base->image_copyright != NULL)
  {
    mrg->image_copyright = apr_pstrdup(p, base->image_copyright);
  }
  else
  if (add->image_copyright != NULL)
  {
    mrg->image_copyright = apr_pstrdup(p, add->image_copyright);
  }
  return mrg;
}
/**
 * The device definition file is loaded. 
 *
 * @param arg     [i]   The name of the device definition file is specified.
 * @param mconfig [i/o] The pointer to a set structure is specified. 
 * @param parms   [i]   
 */
static const char* 
cmd_load_device_data(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;
  doc.r = NULL;
  if (strlen(arg) > 256) 
  {
    return "device data filename too long.";
  }

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
static const char* 
cmd_load_emoji_data(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  char* rtn;
  Doc doc;
  doc.r = NULL;

  if (strlen(arg) > 256) 
  {
    return "emoji data filename too long.";
  }

  conf = (mod_chxj_config*)mconfig;
  conf->emoji_data_file = apr_pstrdup(parms->pool, arg);
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  qs_parse_file((Doc*)&doc, (const char*)arg);
  rtn = chxj_load_emoji_data(&doc,parms->pool, conf);
  qs_all_free(&doc, QX_LOGMARK);

  return rtn;
}

static const char* 
cmd_set_image_engine(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;
  doc.r = NULL;
  if (strlen(arg) > 256) 
  {
    return "image uri is too long.";
  }

  conf = (mod_chxj_config*)mconfig;
  if (strcasecmp("ON", arg) == 0)
  {
    conf->image = CHXJ_IMG_ON;
  }
  else
  {
    conf->image = CHXJ_IMG_OFF;
  }
  return NULL;
}

static const char* 
cmd_set_image_cache_dir(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;
  doc.r = NULL;
  if (strlen(arg) > 256) 
  {
    return "cache dir name is too long.";
  }

  conf = (mod_chxj_config*)mconfig;
  conf->image_cache_dir = apr_pstrdup(parms->pool, arg);
  return NULL;
}

static const char* 
cmd_set_image_copyright(cmd_parms *parms, void *mconfig, const char* arg) 
{
  mod_chxj_config* conf;
  Doc doc;

  doc.r = NULL;
  if (strlen(arg) > 256) 
  {
    return "Copyright Flag is too long.";
  }

  conf = (mod_chxj_config*)mconfig;
  conf->image_copyright = apr_pstrdup(parms->pool, arg);
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
    "ChxjImageCopyright",
    cmd_set_image_copyright,
    NULL,
    OR_ALL,
    "Copyright Flag"),
  { NULL },
};


/*----------------------------------------------------------------------------*/
/* Dispatch list for API hooks                                                */
/*----------------------------------------------------------------------------*/
module AP_MODULE_DECLARE_DATA chxj_module = 
{
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
