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
#include <ctype.h>
#include "apr.h"
#include "mod_chxj.h"
#include "chxj_apply_convrule.h"
#include "chxj_emoji.h"

request_rec *dbg_r;

#define NONE  (-1)
#define SJIS  (0)
#define EUCJP (1)
#define UTF8  (2)

static emoji_t *chxj_bsearch_emoji(
  request_rec *r,
  const char *src, 
  int (*cmp)(const char *, emoji_t *));

static emoji_t *is_emoji_bin(request_rec *, const char *, int (*cmp)(const char *, emoji_t *));
static emoji_t *is_emoji_as_sjis_bin(request_rec *,const char *src);
static emoji_t *is_emoji_as_eucjp_bin(request_rec *,const char *src);
static emoji_t *is_emoji_as_utf8_bin(request_rec *,const char *src);

static emoji_t *
is_emoji_refstring(
  request_rec *r,
  const char *src,
  int *nextpos,
  int (*cmp)(const char *, emoji_t *));
static emoji_t *is_emoji_as_sjis_refstring(request_rec *,const char *, int *);
static emoji_t *is_emoji_as_eucjp_refstring(request_rec *, const char *, int *);
static emoji_t *is_emoji_as_unicode_refstring(request_rec *,const char *, int *);
static emoji_t *is_emoji_as_utf8_refstring(request_rec *,const char *, int *);

static int cmp_as_sjis_bin(const char *, emoji_t *);
static int cmp_as_eucjp_bin(const char *, emoji_t *);
static int cmp_as_utf8_bin(const char *, emoji_t *);

static int cmp_refstring(const char *, const char *, const char *);
static int cmp_as_sjis_refstring(const char *, emoji_t *);
static int cmp_as_eucjp_refstring(const char *, emoji_t *);
static int cmp_as_unicode_refstring(const char *, emoji_t *);
static int cmp_as_utf8_refstring(const char *, emoji_t *);

static int  decide_terminal_code(char *, char *);
static char *docomo_meta_emoji_to_utf8_emoji(  request_rec*, emoji_t **, device_table *,const char *);
static char *softbank_meta_emoji_to_utf8_emoji(request_rec*, emoji_t **, device_table *,const char *);
static char *au_meta_emoji_to_utf8_emoji(      request_rec*, emoji_t **, device_table *,const char *);
static char *docomo_meta_emoji_to_sjis_emoji(  request_rec*, emoji_t **, device_table *,const char *);
static char *softbank_meta_emoji_to_sjis_emoji(request_rec*, emoji_t **, device_table *,const char *);
static char *au_meta_emoji_to_sjis_emoji(      request_rec*, emoji_t **, device_table *,const char *);
static char *meta_emoji_to_emoji(request_rec *,emoji_t **,device_table *,const char *,char *(*callback)(request_rec *,emoji_t *, device_table *));
static char *callback_meta_emoji_imode_utf8_emoji(request_rec *, emoji_t *, device_table *);
static char *callback_meta_emoji_imode_sjis_emoji(request_rec *, emoji_t *, device_table *);
static char *callback_meta_emoji_au_utf8_emoji(request_rec *,emoji_t *, device_table *);
static char *callback_meta_emoji_au_sjis_emoji(request_rec *, emoji_t *, device_table *);
static char *callback_meta_emoji_softbank_utf8_emoji(request_rec *,emoji_t *, device_table *);
static char *callback_meta_emoji_softbank_sjis_emoji(request_rec *, emoji_t *, device_table *);
static char *au_meta_emoji_to_sjis_hdml_emoji(request_rec  *r, emoji_t **, device_table *, const char *);
static char *callback_meta_emoji_au_sjis_hdml_emoji(request_rec  *r, emoji_t *emoji, device_table *spec);
static int cmp_qsort_no_index(const void *a, const void *b);
static int cmp_qsort_eucjp_index(const void *a, const void *b);
static int cmp_qsort_sjis_index(const void *a, const void *b);
static int cmp_qsort_utf8_index(const void *a, const void *b);



void
chxj_emoji_init(
  mod_chxj_config *conf)
{
  int i = 0;
  emoji_t *e;
  for (e = conf->emoji; e; e = e->next) {
    conf->emoji_index[i] = e;
    conf->emoji_index_eucjp[i] = e;
    conf->emoji_index_sjis[i] = e;
    conf->emoji_index_utf8[i] = e;
    i++;
  }
  qsort(conf->emoji_index,
        EMOJI_COUNT, 
        sizeof(emoji_t *),
        cmp_qsort_no_index);
  qsort(conf->emoji_index_eucjp, 
        EMOJI_COUNT, 
        sizeof(emoji_t *),
        cmp_qsort_eucjp_index);
  qsort(conf->emoji_index_sjis, 
        EMOJI_COUNT, 
        sizeof(emoji_t *),
        cmp_qsort_sjis_index);
  qsort(conf->emoji_index_utf8, 
        EMOJI_COUNT, 
        sizeof(emoji_t *),
        cmp_qsort_utf8_index);
}


static int
cmp_qsort_no_index(const void *a, const void *b)
{
  emoji_t *aa = *(emoji_t **)a;
  emoji_t *bb = *(emoji_t **)b;
  return aa->no - bb->no;
}


static int
cmp_qsort_eucjp_index(const void *a, const void *b)
{
  emoji_t *aa = *(emoji_t **)a;
  emoji_t *bb = *(emoji_t **)b;
  long aal = strtol(aa->imode.euc.hex_string, NULL, 16);
  long bbl = strtol(bb->imode.euc.hex_string, NULL, 16);
  return aal - bbl;
}


static int
cmp_qsort_sjis_index(const void *a, const void *b)
{
  emoji_t *aa = *(emoji_t **)a;
  emoji_t *bb = *(emoji_t **)b;
  long aal = strtol(aa->imode.sjis.hex_string, NULL, 16);
  long bbl = strtol(bb->imode.sjis.hex_string, NULL, 16);
  return aal - bbl;
}


static int
cmp_qsort_utf8_index(const void *a, const void *b)
{
  emoji_t *aa = *(emoji_t **)a;
  emoji_t *bb = *(emoji_t **)b;
  long aal = strtol(aa->imode.utf8.hex_string, NULL, 16);
  long bbl = strtol(bb->imode.utf8.hex_string, NULL, 16);
  return aal - bbl;
}


char *
chxj_emoji_to_meta_emoji(
  request_rec         *r,
  chxjconvrule_entry  *entryp,
  const char          *src,
  apr_size_t          *iolen) 
{
  char *server_side_encoding = entryp->encoding; 
  apr_size_t ilen = *iolen;
  char *outbuf;
  apr_size_t i;
  int enc;
dbg_r = r;
  /* 
   * note: server side encoding : default is ``NONE'' 
   */
  enc = SJIS;
  if (STRCASEEQ('n','N',"none",server_side_encoding)) {
    return apr_pstrdup(r->pool, src);
  } else if (IS_SJIS_STRING(server_side_encoding)) {
    enc = SJIS;
  } else if (IS_EUCJP_STRING(server_side_encoding)) {
    enc = EUCJP;
  } else if (IS_UTF8_STRING(server_side_encoding)) {
    enc = UTF8;
  }

  /* 
   * Now because the character-code was decided 
   * seeing one character.
   */
  outbuf = apr_palloc(r->pool, 1);
  outbuf[0] = 0;

  for (i = 0; i < ilen; i++) {
    if (isascii(src[i])) {
      DBG2(r, "ascii src[%d]=[%x]", i, (unsigned char)src[i]);
      /*
       * alphabet or number
       */
      if (src[i] == '&') {
        emoji_t *tmp;
        int nextpos = 0;
        switch (enc) {
        case EUCJP:
          tmp = is_emoji_as_eucjp_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(r->pool, 
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                             "%s", 
                                             meta_emoji), 
                                 NULL);
            i += nextpos;
            continue;
          }
          break;
        case UTF8:
          tmp = is_emoji_as_unicode_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(r->pool,
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                              "%s", 
                                              meta_emoji), 
                                 NULL);
            i += nextpos;
            continue;
          }
          tmp = is_emoji_as_utf8_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(r->pool,
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                              "%s", 
                                              meta_emoji), 
                                 NULL);
            i += nextpos;
            continue;
          }
          break;

        case SJIS:
        default:
          tmp = is_emoji_as_sjis_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(r->pool, 
                                            "%s%d;", 
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                              "%s", 
                                              meta_emoji), 
                                 NULL);
            i += nextpos;
            continue;
          }
          break;
        }
      }
      outbuf = apr_pstrcat(r->pool, 
                           outbuf,
                           apr_psprintf(r->pool, 
                                        "%c",
                                        src[i]), 
                           NULL);
    }
    else {
      /* multibyte charactor */
      char mb[3];
      emoji_t *tmp;
      switch (enc) {
      case EUCJP:
        DBG4(r, "mb src[%d]=[\\x%x][\\x%x][\\x%x]", i, (unsigned char)src[i], (unsigned char)src[i+1], (unsigned char)src[i+2]);
        DBG(r, "DETECT EUCJP BIN");
        tmp = is_emoji_as_eucjp_bin(r, &src[i]);
        if (tmp) {
          char *meta_emoji = apr_psprintf(r->pool, 
                                          "%s%d;", 
                                          META_EMOJI_PREFIX,
                                          tmp->no);
          DBG1(r, "FOUND EMOJI :[%s]", meta_emoji);
          outbuf = apr_pstrcat(r->pool, 
                               outbuf, 
                               apr_psprintf(r->pool, 
                                            "%s", 
                                            meta_emoji), 
                               NULL);
          i += 2;
          continue;
        }
        DBG(r, "NOT FOUND EMOJI");
        break;

      case UTF8:
        DBG4(r, "mb src[%d]=[\\x%x][\\x%x][\\x%x]", i, (unsigned char)src[i], (unsigned char)src[i+1], (unsigned char)src[i+2]);
        DBG(r, "DETECT UTF8 BIN");
        tmp = is_emoji_as_utf8_bin(r, &src[i]);
        if (tmp) {
          char *meta_emoji = apr_psprintf(r->pool, 
                                          "%s%d;", 
                                          META_EMOJI_PREFIX,
                                          tmp->no);
          DBG1(r, "FOUND EMOJI :[%s]", meta_emoji);
          outbuf = apr_pstrcat(r->pool, 
                               outbuf, 
                               apr_psprintf(r->pool, 
                                            "%s", 
                                            meta_emoji), 
                               NULL);
          i+=2;
          continue;
        }
        DBG(r, "NOT FOUND EMOJI");
        break;

      case SJIS:
      default:
        DBG3(r, "mb src[%d]=[\\x%x][\\x%x]", i, (unsigned char)src[i], (unsigned char)src[i+1]);
        DBG(r, "DETECT SJIS BIN");
        tmp = is_emoji_as_sjis_bin(r, &src[i]);
        if (tmp) {
          char *meta_emoji = apr_psprintf(r->pool, 
                                          "%s%d;", 
                                          META_EMOJI_PREFIX,
                                          tmp->no);
          DBG1(r, "FOUND EMOJI :[%s]", meta_emoji);
          outbuf = apr_pstrcat(r->pool, 
                               outbuf, 
                               apr_psprintf(r->pool, 
                                            "%s", 
                                            meta_emoji), 
                               NULL);
          i++;
          continue;
        }
        DBG(r, "NOT FOUND EMOJI");
        break;
      }
      mb[0] = src[i++]; 
      mb[1] = src[i]; 
      mb[2] = 0;
      outbuf = apr_pstrcat(r->pool, outbuf, mb, NULL);
    }
  }
  *iolen = strlen(outbuf);
  DBG1(r, "convert EMOJI to META EMOJI.  [%s]", outbuf);
  return outbuf;
}


static int
cmp_as_sjis_bin(
  const char *src, 
  emoji_t *target)
{
  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);

  target_value = (long)(0xff & target->imode.sjis.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->imode.sjis.hex[1]);
  
  return src_value - target_value;
}


static int
cmp_as_eucjp_bin(
  const char *src, 
  emoji_t *target)
{
  if (strlen(src) < 3) return -1;

  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[2]);

  target_value = (long)(0xff & target->imode.euc.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->imode.euc.hex[1]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->imode.euc.hex[2]);
  
  return src_value - target_value;
}

static int
cmp_as_utf8_bin(
  const char *src,
  emoji_t *target)
{
  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[2]);

  target_value = (long)(0xff & target->imode.utf8.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->imode.utf8.hex[1]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->imode.utf8.hex[2]);
  
  return src_value - target_value;
}


static emoji_t *
is_emoji_as_sjis_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_bin(r, src, cmp_as_sjis_bin);
}


static emoji_t *
is_emoji_as_eucjp_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_bin(r, src, cmp_as_eucjp_bin);
}


static emoji_t *
is_emoji_as_utf8_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_bin(r, src, cmp_as_utf8_bin);
}


static emoji_t *
is_emoji_bin(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, emoji_t *))
{
  char    test[EMOJI_BINCODE_MAX_LEN];
  emoji_t *rtn;
  memset(test, 0, EMOJI_BINCODE_MAX_LEN);
  strncpy(test, src, EMOJI_BINCODE_MAX_LEN);
  rtn = chxj_bsearch_emoji(r, test, cmp);
  if (rtn) {
    return rtn;
  }
  return NULL;
}


static emoji_t *
chxj_bsearch_emoji(
  request_rec *r,
  const char *src, 
  int (*cmp)(const char *, emoji_t *))
{
  int left = 0;
  int right = EMOJI_COUNT - 1;
  int mid; 
  int ret;
  mod_chxj_config *dconf;

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  while(left <= right) {
    mid = (left + right) / 2; /* calc of middle key */
    if (cmp == cmp_as_eucjp_bin) {
      ret = cmp(src, dconf->emoji_index_eucjp[mid]);
      if (! ret) {
        return dconf->emoji_index_eucjp[mid];
      }
    }
    else if (cmp == cmp_as_sjis_bin) {
      ret = cmp(src, dconf->emoji_index_sjis[mid]);
      if (! ret) {
        return dconf->emoji_index_sjis[mid];
      }
    }
    else {
      ret = cmp(src, dconf->emoji_index_utf8[mid]);
      if (! ret) {
        return dconf->emoji_index_utf8[mid];
      }
    }
    if (ret > 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return NULL;
}


static int
cmp_as_sjis_refstring(
  const char *src, 
  emoji_t *target)
{
  return cmp_refstring(src,
                       target->imode.sjis.hex_string, 
                       target->imode.sjis.dec_string);
}


static int
cmp_as_eucjp_refstring(
  const char *src,
  emoji_t *target)
{
  return cmp_refstring(src,
                       target->imode.euc.hex_string, 
                       target->imode.euc.dec_string);
}


static int
cmp_as_unicode_refstring(
  const char *src, 
  emoji_t *target)
{
  return cmp_refstring(src,
                       target->imode.unicode.hex_string, 
                       target->imode.unicode.dec_string);
}


static int
cmp_as_utf8_refstring(
  const char *src, 
  emoji_t *target)
{
  return cmp_refstring(src,
                       target->imode.utf8.hex_string, 
                       target->imode.utf8.dec_string);
}

static int
cmp_refstring(
  const char *src,
  const char *hex_string,
  const char *dec_string)
{
  char target_ref[REFSTRING_MAX_LEN];
  memset(target_ref, 0, REFSTRING_MAX_LEN);
  if (strstr(src, "&#x")) {
    /* 
     * hex
     */
    target_ref[0] = '&';
    target_ref[1] = '#';
    target_ref[2] = 'x';
    strcpy(&target_ref[3], hex_string);
    strcat(target_ref, ";");
    return strncasecmp(src, target_ref, REFSTRING_MAX_LEN);
  }
  /* decimal */
  target_ref[0] = '&';
  target_ref[1] = '#';
  strcpy(&target_ref[2], dec_string);
  strcat(target_ref, ";");
  return strncasecmp(src, target_ref, REFSTRING_MAX_LEN);
}


static emoji_t *
is_emoji_as_sjis_refstring(
  request_rec *r,
  const char *src,
  int  *nextpos)
{
  return is_emoji_refstring(r,src,nextpos, cmp_as_sjis_refstring);
}


static emoji_t *
is_emoji_as_eucjp_refstring(
  request_rec *r,
  const char *src,
  int  *nextpos)
{
  return is_emoji_refstring(r,src,nextpos, cmp_as_eucjp_refstring);
}


static emoji_t *
is_emoji_as_unicode_refstring(
  request_rec *r,
  const char *src,
  int  *nextpos)
{
  return is_emoji_refstring(r,src,nextpos, cmp_as_unicode_refstring);
}


static emoji_t *
is_emoji_as_utf8_refstring(
  request_rec *r,
  const char *src,
  int  *nextpos)
{
  return is_emoji_refstring(r,src,nextpos, cmp_as_utf8_refstring);
}

static emoji_t *
is_emoji_refstring(
  request_rec *r,
  const char *src,
  int *nextpos,
  int (*cmp)(const char *, emoji_t *)) 
{
  char test[REFSTRING_MAX_LEN];
  emoji_t *rtn;
  char *endp;
  memset(test, 0, REFSTRING_MAX_LEN);
  strncpy(test, src, REFSTRING_MAX_LEN);
  endp = strchr(test, ';');
  if (endp) {
    test[endp - test + 1] = 0;
    rtn = chxj_bsearch_emoji(r, test, cmp);
    if (rtn) {
      /* found emoji */
      *nextpos = endp - test;
      return rtn;
    }
  }
  /* not emoji */
  return NULL;
}
  

char *
chxj_meta_emoji_to_emoji(
  request_rec         *r,
  device_table        *spec,
  chxjconvrule_entry  *entryp,
  emoji_t             **emoji_table,
  char                *src)
{
  int enc = decide_terminal_code(GET_SPEC_CHARSET(spec), entryp->encoding);
  DBG(r,"start meta emoji --> emoji");
  switch (enc) {
  case UTF8:
    DBG(r, "DETECT: Client is UTF8");
    {
      switch (GET_HTML_SPEC_TYPE(spec)) {
      case CHXJ_SPEC_Jhtml:
        return softbank_meta_emoji_to_utf8_emoji(r, emoji_table,spec,src);
      case CHXJ_SPEC_XHtml_Mobile_1_0:
      case CHXJ_SPEC_Hdml: 
        return au_meta_emoji_to_utf8_emoji(r,emoji_table, spec,src);
      case CHXJ_SPEC_Chtml_1_0:
      case CHXJ_SPEC_Chtml_2_0:
      case CHXJ_SPEC_Chtml_3_0:
      case CHXJ_SPEC_Chtml_4_0:
      case CHXJ_SPEC_Chtml_5_0:
      case CHXJ_SPEC_Chtml_6_0:
      case CHXJ_SPEC_Chtml_7_0:
      default:
        break;
      }
      return docomo_meta_emoji_to_utf8_emoji(r,emoji_table,spec,src);
    }
  case NONE:
    DBG(r, "DETECT: Client is NONE");
    break;

  case SJIS:
  default:
    DBG(r, "DETECT: Client is SJIS");
    {
      switch (GET_HTML_SPEC_TYPE(spec)) {
      case CHXJ_SPEC_Jhtml:
        DBG(r, "DETECT: html_spec_type: JHTML");
        return softbank_meta_emoji_to_sjis_emoji(r,emoji_table,spec,src);
      case CHXJ_SPEC_XHtml_Mobile_1_0:
        DBG(r, "DETECT: html_spec_type: XHTML");
        return au_meta_emoji_to_sjis_emoji(r,emoji_table, spec,src);
      case CHXJ_SPEC_Hdml: 
        DBG(r, "DETECT: html_spec_type: HDML");
        return au_meta_emoji_to_sjis_hdml_emoji(r,emoji_table, spec,src);
      case CHXJ_SPEC_Chtml_1_0:
      case CHXJ_SPEC_Chtml_2_0:
      case CHXJ_SPEC_Chtml_3_0:
      case CHXJ_SPEC_Chtml_4_0:
      case CHXJ_SPEC_Chtml_5_0:
      case CHXJ_SPEC_Chtml_6_0:
      case CHXJ_SPEC_Chtml_7_0:
      default:
        break;
      }
      DBG(r, "DETECT: html_spec_type: CHTML");
      return docomo_meta_emoji_to_sjis_emoji(r,emoji_table,spec,src);
    }
    break;
  }
  return src;
}



static int
decide_terminal_code(
  char *term_encoding,
  char *server_encoding)
{
  
  if (IS_SJIS_STRING(term_encoding))  return SJIS;
  if (IS_EUCJP_STRING(term_encoding)) return SJIS;
  if (IS_UTF8_STRING(term_encoding))  return UTF8;

  if (IS_SJIS_STRING(server_encoding))  return SJIS;
  if (IS_EUCJP_STRING(server_encoding)) return SJIS;
  if (IS_UTF8_STRING(server_encoding))  return UTF8;

  return NONE;
}


static char *
meta_emoji_to_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src,
  char         *(*callback)(request_rec *,emoji_t *, device_table *))
{
  int i;
  int len = strlen(src);
  char *outbuf = apr_palloc(r->pool, 1);
  outbuf[0] = 0;

  for (i=0; i<len; i++) {
    if (is_sjis_kanji(src[i])) {
      char tmp[3];
      tmp[0] = src[i+0];
      tmp[1] = src[i+1];
      tmp[2] = 0;
      outbuf = apr_pstrcat(r->pool, outbuf, tmp, NULL);
      i++;
      continue;
    }
    if (src[i] == '&') {
      if (strncasecmp(META_EMOJI_PREFIX, &src[i], META_EMOJI_PREFIX_LEN) == 0) {
        char *npos = strchr(&src[i], ';');
        char *strno;
        int cl;
        int err;
        int emoji_len = (npos - &src[i]) + 1;
        strno = apr_psprintf(r->pool, 
                             "%.*s",
                             emoji_len - META_EMOJI_PREFIX_LEN - 1, 
                             &src[i + META_EMOJI_PREFIX_LEN]);
        DBG3(r, "Found EMOJI:[%.*s] no:[%s]", emoji_len, &src[i], strno);
        err = 0;
        for (cl=0; cl<(int)strlen(strno); cl++) {
          if (! isdigit(strno[cl])) {
            ERR1(r, "Invalid Emoji Number:[%s]", strno);
            err = 1;
            break;
          }
        }
        if (! err) {
          outbuf = apr_pstrcat(r->pool,
                               outbuf,
                               callback(r, emoji_table[atoi(strno) - 1], spec),
                               NULL);
        }
        i += (emoji_len - 1);
        continue;
      }
    }
    outbuf = apr_pstrcat(r->pool, 
                         outbuf,
                         apr_psprintf(r->pool, 
                                      "%c",
                                      src[i]), 
                         NULL);
  }
  return outbuf;
}


static char *
callback_meta_emoji_imode_utf8_emoji(
  request_rec *r,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode utf8");
  return apr_psprintf(r->pool,
                      "&#x%s;",
                      emoji->imode.unicode.hex_string);
}


static char *
callback_meta_emoji_softbank_utf8_emoji(
  request_rec *r,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use SoftBank utf8");
  return apr_psprintf(r->pool,
                      "&#x%s;",
                      emoji->softbank.unicode.hex_string);
}


static char *
callback_meta_emoji_au_utf8_emoji(
  request_rec *r,
  emoji_t *emoji,
  device_table *spec)
{
  DBG(r, "use KDDI utf8");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      return apr_psprintf(r->pool,
                          "%.*s",
                          3,
                          emoji->ezweb.typeA.utf8.hex);
    case 'B':
    case 'b':
      return apr_psprintf(r->pool,
                          "%.*s",
                          3,
                          emoji->ezweb.typeB.utf8.hex);
    case 'C':
    case 'c':
      return apr_psprintf(r->pool,
                          "%.*s",
                          3,
                          emoji->ezweb.typeC.utf8.hex);
    case 'D':
    case 'd':
      return apr_psprintf(r->pool,
                          "%.*s",
                          3,
                          emoji->ezweb.typeD.utf8.hex);
    default:
      break;
    }
  }
  ERR1(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(r->pool,
                      "&#x%s;",
                      emoji->ezweb.typeD.unicode.hex_string);
}


static char *
callback_meta_emoji_imode_sjis_emoji(
  request_rec *r,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode SJIS");
  return apr_psprintf(r->pool, "%.*s", 2, emoji->imode.sjis.hex);
}


static char *
callback_meta_emoji_softbank_sjis_emoji(
  request_rec *r,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use SoftBank SJIS");
  char *tmp = apr_palloc(r->pool, 6);
  memset(tmp, 0, 6);
  memcpy(tmp, emoji->softbank.sjis.hex, 5);
  DBG1(r, "use Softbank Code:[%s]", tmp);
  return tmp;
}


static char *
callback_meta_emoji_au_sjis_emoji(
  request_rec  *r,
  emoji_t      *emoji,
  device_table *spec)
{
  DBG(r, "use KDDI SJIS");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      return apr_psprintf(r->pool,
                          "%.*s",
                          2,
                          emoji->ezweb.typeA.sjis.hex);
    case 'B':
    case 'b':
      return apr_psprintf(r->pool,
                          "%.*s",
                          2,
                          emoji->ezweb.typeB.sjis.hex);
    case 'C':
    case 'c':
      return apr_psprintf(r->pool,
                          "%.*s",
                          2,
                          emoji->ezweb.typeC.sjis.hex);
    case 'D':
    case 'd':
      return apr_psprintf(r->pool,
                          "%.*s",
                          2,
                          emoji->ezweb.typeD.sjis.hex);
    default:
      break;
    }
  }
  ERR1(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(r->pool,
                      "%.*s",
                      2,
                      emoji->ezweb.typeD.sjis.hex);
}


static char *
callback_meta_emoji_au_sjis_hdml_emoji(
  request_rec  *r,
  emoji_t      *emoji,
  device_table *spec)
{
  DBG(r, "use KDDI SJIS(HDML)");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      return apr_psprintf(r->pool,
                          "<IMG ICON=%d>",
                          emoji->ezweb.typeA.no);
    case 'B':
    case 'b':
      return apr_psprintf(r->pool,
                          "<IMG ICON=%d>",
                          emoji->ezweb.typeB.no);
    case 'C':
    case 'c':
      return apr_psprintf(r->pool,
                          "<IMG ICON=%d>",
                          emoji->ezweb.typeC.no);
    case 'D':
    case 'd':
      return apr_psprintf(r->pool,
                          "<IMG ICON=%d>",
                          emoji->ezweb.typeD.no);
    default:
      break;
    }
  }
  ERR1(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(r->pool,
                      "%.*s",
                      2,
                      emoji->ezweb.typeD.sjis.hex);
}


static char *
docomo_meta_emoji_to_utf8_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r, 
                             emoji_table, 
                             spec,
                             src,
                             callback_meta_emoji_imode_utf8_emoji);
}


static char *
softbank_meta_emoji_to_utf8_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r, 
                             emoji_table,
                             spec,
                             src, 
                             callback_meta_emoji_softbank_utf8_emoji);
}


static char *
au_meta_emoji_to_utf8_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r,
                             emoji_table,
                             spec,
                             src,
                             callback_meta_emoji_au_utf8_emoji);
}


static char *
docomo_meta_emoji_to_sjis_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r, 
                             emoji_table,
                             spec,
                             src,
                             callback_meta_emoji_imode_sjis_emoji);
}


static char *
softbank_meta_emoji_to_sjis_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r,
                             emoji_table,
                             spec,
                             src,
                             callback_meta_emoji_softbank_sjis_emoji);
}


static char *
au_meta_emoji_to_sjis_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r,
                             emoji_table,
                             spec,
                             src,
                             callback_meta_emoji_au_sjis_emoji);
}


static char *
au_meta_emoji_to_sjis_hdml_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src)
{
  return meta_emoji_to_emoji(r,
                             emoji_table,
                             spec,
                             src,
                             callback_meta_emoji_au_sjis_hdml_emoji);
}
