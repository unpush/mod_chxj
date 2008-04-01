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
#include <apr.h>
#include <apr_pools.h>

#include "mod_chxj.h"
#include "chxj_apply_convrule.h"
#include "chxj_emoji.h"
#include "chxj_str_util.h"
#include "chxj_encoding.h"

request_rec *dbg_r;

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

static int decide_terminal_code(char *, char *);
static int decide_server_code(char *server_encoding);
static char *docomo_meta_emoji_to_utf8_emoji(  request_rec*, emoji_t **, device_table *,const char *);
static char *softbank_meta_emoji_to_utf8_emoji(request_rec*, emoji_t **, device_table *,const char *);
static char *au_meta_emoji_to_utf8_emoji(      request_rec*, emoji_t **, device_table *,const char *);
static char *docomo_meta_emoji_to_sjis_emoji(  request_rec*, emoji_t **, device_table *,const char *);
static char *softbank_meta_emoji_to_sjis_emoji(request_rec*, emoji_t **, device_table *,const char *);
static char *au_meta_emoji_to_sjis_emoji(      request_rec*, emoji_t **, device_table *,const char *);
static char *meta_emoji_to_emoji(request_rec *,emoji_t **,device_table *,const char *,char *(*callback)(request_rec *,apr_pool_t *,emoji_t *, device_table *));

static char *docomo_postdata_meta_emoji_to_sjis_emoji(request_rec  *, emoji_t **, device_table *, const char   *);
static char *docomo_postdata_meta_emoji_to_euc_emoji(request_rec  *, emoji_t **, device_table *, const char   *);
static char *docomo_postdata_meta_emoji_to_utf8_emoji(request_rec  *, emoji_t **, device_table *, const char   *);

static char *callback_meta_emoji_imode_utf8_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *callback_meta_emoji_imode_sjis_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *callback_meta_emoji_au_utf8_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *callback_meta_emoji_au_sjis_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *callback_meta_emoji_softbank_utf8_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *callback_meta_emoji_softbank_sjis_emoji(request_rec *,apr_pool_t*,emoji_t *, device_table *);
static char *au_meta_emoji_to_sjis_hdml_emoji(request_rec  *r, emoji_t **, device_table *, const char *);
static char *callback_meta_emoji_au_sjis_hdml_emoji(request_rec  *r,apr_pool_t*,emoji_t *emoji, device_table *spec);

/*============================================================================*/
/* for POSTDATA                                                               */
/*============================================================================*/
static char *callback_postdata_meta_emoji_imode_utf8_emoji(request_rec *, apr_pool_t*,emoji_t *, device_table *UNUSED(spec));
static char *callback_postdata_meta_emoji_imode_sjis_emoji(request_rec *, apr_pool_t*,emoji_t *, device_table *UNUSED(spec));
static char *callback_postdata_meta_emoji_imode_euc_emoji(request_rec *, apr_pool_t*,emoji_t *, device_table *UNUSED(spec));

static int cmp_qsort_no_index(const void *a, const void *b);
static int cmp_qsort_eucjp_index(const void *a, const void *b);
static int cmp_qsort_sjis_index(const void *a, const void *b);
static int cmp_qsort_utf8_index(const void *a, const void *b);

static int cmp_qsort_ezweb2imode_sjis(const void *a, const void *b);
static int cmp_qsort_ezweb2imode_utf8(const void *a, const void *b);
static int cmp_qsort_softbank2imode_webcode(const void *, const void *);
static int cmp_qsort_softbank2imode_sjis(const void *a, const void *b);
static int cmp_qsort_softbank2imode_utf8(const void *a, const void *b);


/*============================================================================*/
/* for POSTDATA (EZWEB)                                                       */
/*============================================================================*/
static ezweb2imode_t *chxj_bsearch_ezweb2imode_emoji(
  request_rec *r, 
  const char  *src,
  int (*cmp)(const char *, ezweb2imode_t *));
static ezweb2imode_t *is_emoji_ezweb2imode_bin(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, ezweb2imode_t *));
static ezweb2imode_t *is_emoji_as_ezweb2imode_sjis_bin(request_rec *r, const char *src);
static ezweb2imode_t *is_emoji_as_ezweb2imode_utf8_bin(request_rec *r, const char *src);
static int cmp_as_ezweb2imode_utf8_bin(const char *, ezweb2imode_t *);
static int cmp_as_ezweb2imode_sjis_bin(const char *, ezweb2imode_t *);

/*============================================================================*/
/* for POSTDATA (SoftBank)                                                    */
/*============================================================================*/
static softbank2imode_t *chxj_bsearch_softbank2imode_emoji(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, softbank2imode_t *));
static softbank2imode_t *is_emoji_softbank2imode_bin(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, softbank2imode_t *));
static softbank2imode_t *is_emoji_as_softbank2imode_utf8_bin(request_rec *, const char *);
static softbank2imode_t *is_emoji_as_softbank2imode_sjis_bin(request_rec *, const char *);
static softbank2imode_t *is_emoji_as_softbank2imode_webcode_bin(request_rec *, const char *);
static int cmp_as_softbank2imode_utf8_bin(const char *, softbank2imode_t *);
static int cmp_as_softbank2imode_sjis_bin(const char *, softbank2imode_t *);
static int cmp_as_softbank2imode_webcode_bin(const char *, softbank2imode_t *);

/*============================================================================*/
/* for POSTDATA (i-MODE)                                                      */
/*============================================================================*/
static emoji_t *chxj_bsearch_imode2imode_emoji(
  request_rec *r, 
  const char  *src,
  int (*cmp)(const char *, emoji_t *));
static emoji_t *is_emoji_imode2imode_bin(
  request_rec *r,
  const char  *src,
  int         (*cmp)(const char *, emoji_t *));
static emoji_t *is_emoji_as_imode2imode_sjis_bin(request_rec *r, const char *src);
static emoji_t *is_emoji_as_imode2imode_utf8_bin(request_rec *r, const char *src);
static int cmp_as_imode2imode_utf8_bin(const char *, emoji_t *);
static int cmp_as_imode2imode_sjis_bin(const char *, emoji_t *);


void
chxj_emoji_init(
  mod_chxj_config *conf)
{
  int              i = 0;
  emoji_t          *e;
  ezweb2imode_t    *e2i;
  softbank2imode_t *s2i;

  for (e = conf->emoji; e; e = e->next) {
    conf->emoji_index[i] = e;
    conf->emoji_index_eucjp[i] = e;
    conf->emoji_index_sjis[i] = e;
    conf->emoji_index_utf8[i] = e;
    i++;
  }
  i = 0;
  for (e2i = conf->ezweb2imode; 
       e2i && i<EMOJI_EZWEB2IMODE_COUNT; 
       e2i = e2i->next) {
    conf->emoji_ezweb2imode_sjis[i] = e2i;
    conf->emoji_ezweb2imode_utf8[i] = e2i;
    i++;
  }
  i = 0;
  for (s2i = conf->softbank2imode; 
       s2i && i<EMOJI_SOFTBANK2IMODE_COUNT; 
       s2i = s2i->next) {
    conf->emoji_softbank2imode_webcode[i] = s2i;
    conf->emoji_softbank2imode_sjis[i] = s2i;
    conf->emoji_softbank2imode_utf8[i] = s2i;
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
  qsort(conf->emoji_ezweb2imode_sjis,
        EMOJI_EZWEB2IMODE_COUNT,
        sizeof(ezweb2imode_t *),
        cmp_qsort_ezweb2imode_sjis);
  qsort(conf->emoji_ezweb2imode_utf8,
        EMOJI_EZWEB2IMODE_COUNT,
        sizeof(ezweb2imode_t *),
        cmp_qsort_ezweb2imode_utf8);
  qsort(conf->emoji_softbank2imode_webcode,
        EMOJI_SOFTBANK2IMODE_COUNT,
        sizeof(softbank2imode_t *),
        cmp_qsort_softbank2imode_webcode);
  qsort(conf->emoji_softbank2imode_sjis,
        EMOJI_SOFTBANK2IMODE_COUNT,
        sizeof(softbank2imode_t *),
        cmp_qsort_softbank2imode_sjis);
  qsort(conf->emoji_softbank2imode_utf8,
        EMOJI_SOFTBANK2IMODE_COUNT,
        sizeof(softbank2imode_t *),
        cmp_qsort_softbank2imode_utf8);
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
  return (int)(aal - bbl);
}


static int
cmp_qsort_ezweb2imode_sjis(const void *a, const void *b)
{
  ezweb2imode_t *aa = *(ezweb2imode_t **)a;
  ezweb2imode_t *bb = *(ezweb2imode_t **)b;
  long aal = strtol(aa->ezweb_sjis.hex_string, NULL, 16);
  long bbl = strtol(bb->ezweb_sjis.hex_string, NULL, 16);
  return (int)(aal - bbl);
}


static int
cmp_qsort_ezweb2imode_utf8(const void *a, const void *b)
{
  ezweb2imode_t *aa = *(ezweb2imode_t **)a;
  ezweb2imode_t *bb = *(ezweb2imode_t **)b;
  long aal = strtol(aa->ezweb_utf8.hex_string, NULL, 16);
  long bbl = strtol(bb->ezweb_utf8.hex_string, NULL, 16);
  return (int)(aal - bbl);
}


static int
cmp_qsort_softbank2imode_webcode(const void *a, const void *b)
{
  softbank2imode_t *aa = *(softbank2imode_t **)a;
  softbank2imode_t *bb = *(softbank2imode_t **)b;
  long aal = strtol(aa->softbank_webcode.hex_string, NULL, 16);
  long bbl = strtol(bb->softbank_webcode.hex_string, NULL, 16);
  return (int)(aal - bbl);
}


static int
cmp_qsort_softbank2imode_sjis(const void *a, const void *b)
{
  softbank2imode_t *aa = *(softbank2imode_t **)a;
  softbank2imode_t *bb = *(softbank2imode_t **)b;
  long aal = strtol(aa->softbank_sjis.hex_string, NULL, 16);
  long bbl = strtol(bb->softbank_sjis.hex_string, NULL, 16);
  return (int)(aal - bbl);
}


static int
cmp_qsort_softbank2imode_utf8(const void *a, const void *b)
{
  softbank2imode_t *aa = *(softbank2imode_t **)a;
  softbank2imode_t *bb = *(softbank2imode_t **)b;
  long aal = strtol(aa->softbank_utf8.hex_string, NULL, 16);
  long bbl = strtol(bb->softbank_utf8.hex_string, NULL, 16);
  return (int)(aal - bbl);
}


struct emoji_buffer {
  char *buffer;
  int  use_len;
};
static char * buffered_flush(apr_pool_t *pool, char *outbuf, struct emoji_buffer *buf);

static char *
buffered_out(apr_pool_t *pool, char *outbuf, struct emoji_buffer *buf, const char *s, apr_size_t len)
{
  if (buf->use_len + len < EMOJI_BUFFER_SIZE - 1) {
    memcpy(&buf->buffer[buf->use_len], s, len);
    buf->use_len += len;
  }
  else {
    outbuf = buffered_flush(pool, outbuf, buf);
    outbuf = buffered_out(pool, outbuf, buf, s, len);
  }
  return outbuf;
}


static char *
buffered_flush(apr_pool_t *pool, char *outbuf, struct emoji_buffer *buf)
{
  char *tmp;
  if (buf->use_len == 0) {
    return outbuf;
  }
  buf->buffer[buf->use_len] = 0;
  tmp = apr_pstrcat(pool, outbuf, buf->buffer, NULL);
  memset(buf->buffer, 0, EMOJI_BUFFER_SIZE);
  buf->use_len = 0;
  return tmp;
}


char *
chxj_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen) 
{
  apr_size_t ilen = *iolen;
  char *outbuf;
  char *result;
  apr_size_t i;
  int enc;
  struct emoji_buffer ebuffer; 
  apr_pool_t *np;
  apr_status_t rv;

  if ((rv = apr_pool_create(&np, r->pool)) != APR_SUCCESS) {
    ERR(r, "failed: new pool create. rv:[%d]", rv);
    return NULL;
  }

dbg_r = r;
  DBG(r, "start chxj_emoji_to_meta_emoji()");
  /* 
   * note: server side encoding : default is ``NONE'' 
   */
  enc = SJIS;
  if (STRCASEEQ('n','N',"none",encoding)) {
    return apr_pstrdup(r->pool, src);
  } else if (IS_SJIS_STRING(encoding)) {
    enc = SJIS;
  } else if (IS_EUCJP_STRING(encoding)) {
    enc = EUCJP;
  } else if (IS_UTF8_STRING(encoding)) {
    enc = UTF8;
  }

  memset(&ebuffer, 0, sizeof(struct emoji_buffer));
  ebuffer.buffer = apr_palloc(np, EMOJI_BUFFER_SIZE);
  if (ebuffer.buffer == NULL) {
    ERR(r, "out of memory");
    apr_pool_destroy(np);
    return NULL;
  }

  /* 
   * Now because the character-code was decided 
   * seeing one character.
   */
  outbuf = apr_palloc(np, 1);
  outbuf[0] = 0;

  for (i = 0; i < ilen; i++) {
    if (isascii(src[i])) {
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
            char *meta_emoji = apr_psprintf(np, 
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i += nextpos;
            continue;
          }
          break;
        case UTF8:
          tmp = is_emoji_as_unicode_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np,
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i += nextpos;
            continue;
          }
          tmp = is_emoji_as_utf8_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np,
                                            "%s%d;",
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i += nextpos;
            continue;
          }
          break;

        case SJIS:
        default:
          tmp = is_emoji_as_sjis_refstring(r, &src[i], &nextpos);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np, 
                                            "%s%d;", 
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i += nextpos;
            continue;
          }
          break;
        }
      }
      outbuf = buffered_out(np, outbuf, &ebuffer, &src[i], 1);
    }
    else {
      /* multibyte charactor */
      char mb[8];
      emoji_t *tmp;
      switch (enc) {
      case EUCJP:
        DBG(r, "DETECT EUCJP BIN");
        if ((unsigned char)src[i] == 0x8F) {
          /* 3byte charactor */
          tmp = is_emoji_as_eucjp_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np, 
                                            "%s%d;", 
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i += 2;
            continue;
          }
          DBG(r, "NOT FOUND EMOJI");
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = 0;
          i+=2;
        }
        else {
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = 0;
          i++;
        }
        break;

      case UTF8:
        DBG(r, "DETECT UTF8 BIN");
        if ((0xe0 & src[i]) == 0xc0) { 
          /* 2byte charactor */
          DBG(r, "UTF-8 2byte charactor");
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = 0;
          i++;
        }
        else if ((0xf0 & src[i]) == 0xe0) {
          /* 3byte charactor */
          DBG(r, "UTF-8 3byte charactor");
          tmp = is_emoji_as_utf8_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np, 
                                            "%s%d;", 
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i+=2;
            continue;
          }
          DBG(r, "NOT FOUND EMOJI");
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = 0;
          i+=2;
        }
        else if ((0xf8 & src[i]) == 0xf0) {
          /* 4byte charactor */
          DBG(r, "UTF-8 4byte charactor");
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = src[i+3];
          mb[4] = 0;
          i+=3;
        }
        else if ((0xc0 & src[i]) == 0x80) { 
          /* unknown charactor */
          DBG(r, "UTF-8 unknown charactor");
          mb[0] = src[i+0];
          mb[1] = 0;
        }
        break;

      case SJIS:
      default:
        DBG(r, "DETECT SJIS BIN");
        if ((0xff & src[i]) == 0xf8 || (0xff & src[i]) == 0xf9) {
          tmp = is_emoji_as_sjis_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji = apr_psprintf(np, 
                                            "%s%d;", 
                                            META_EMOJI_PREFIX,
                                            tmp->no);
            DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
            outbuf = buffered_out(np, outbuf, &ebuffer, meta_emoji, strlen(meta_emoji));
            i++;
            continue;
          }
          DBG(r, "NOT FOUND EMOJI");
        }
        if ( ( ((0x81 <= (unsigned char)src[i]) && (0x9f >= (unsigned char)src[i]))
             ||((0xe0 <= (unsigned char)src[i]) && (0xfc >= (unsigned char)src[i]))
             )
             && 
             ( ((0x40 <= (unsigned char)src[i+1]) && (0x7e >= (unsigned char)src[i+1]))
             ||((0x80 <= (unsigned char)src[i+1]) && (0xfc >= (unsigned char)src[i+1])))) {
          /* SJIS MultiByte */
          mb[0] = src[i++]; 
          mb[1] = src[i]; 
          mb[2] = 0;
          DBG(r, "SJIS multi byte:[%s]", mb);
        }
        else {
          /* SJIS single Byte */
          mb[0] = src[i];
          mb[1] = 0;
          DBG(r, "SJIS single byte:[%s]", mb);
        }
        break;
      }
      outbuf = buffered_out(np, outbuf, &ebuffer, mb, strlen(mb));
    }
  }
  outbuf = buffered_flush(np, outbuf, &ebuffer);
  *iolen = strlen(outbuf);
  result = apr_palloc(r->pool, *iolen + 1);
  memcpy(result, outbuf, *iolen);
  result[*iolen] = 0;
  apr_pool_destroy(np);
  DBG(r, "convert EMOJI to META EMOJI.  outbuf:[%s]", result);
  DBG(r, "convert EMOJI to META EMOJI.  len:[%d]", *iolen);
  DBG(r, "end chxj_emoji_to_meta_emoji()");
  return result;
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
cmp_as_ezweb2imode_sjis_bin(
  const char *src, 
  ezweb2imode_t *target)
{
  long src_value = 0;
  long target_value = 0;
  DBG(dbg_r, "src:[%s] target:[%s]", src, target->ezweb_sjis.hex_string);
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);

  target_value = (long)(0xff & target->ezweb_sjis.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->ezweb_sjis.hex[1]);
  
  return src_value - target_value;
}

static int
cmp_as_softbank2imode_sjis_bin(
  const char *src, 
  softbank2imode_t *target)
{
  long src_value = 0;
  long target_value = 0;
  DBG(dbg_r, "src:[%s] target:[%.2s]", src, target->softbank_sjis.hex);
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);

  target_value = (long)(0xff & target->softbank_sjis.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->softbank_sjis.hex[1]);
  DBG(dbg_r, "srcl:[%x] targetl:[%x]", (unsigned int)src_value, (unsigned int)target_value);
  
  return (int)(src_value - target_value);
}


static int
cmp_as_softbank2imode_webcode_bin(
  const char       *src, 
  softbank2imode_t *target)
{
  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);
  target_value = (long)(0xff & target->softbank_webcode.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->softbank_webcode.hex[1]);
  return (int)(src_value - target_value);
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


static int
cmp_as_ezweb2imode_utf8_bin(
  const char *src,
  ezweb2imode_t *target)
{
  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[2]);

  target_value = (long)(0xff & target->ezweb_utf8.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->ezweb_utf8.hex[1]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->ezweb_utf8.hex[2]);
  
  return src_value - target_value;
}


static int
cmp_as_softbank2imode_utf8_bin(
  const char *src,
  softbank2imode_t *target)
{
  long src_value = 0;
  long target_value = 0;
  src_value = (long)(0xff & src[0]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[1]);
  src_value <<= 8;
  src_value |= (long)(0xff & src[2]);

  target_value = (long)(0xff & target->softbank_utf8.hex[0]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->softbank_utf8.hex[1]);
  target_value <<= 8;
  target_value |= (long)(0xff & target->softbank_utf8.hex[2]);
  
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


static ezweb2imode_t *
is_emoji_ezweb2imode_bin(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, ezweb2imode_t *))
{
  char    test[EMOJI_BINCODE_MAX_LEN];
  ezweb2imode_t *rtn;
  memset(test, 0, EMOJI_BINCODE_MAX_LEN);
  strncpy(test, src, EMOJI_BINCODE_MAX_LEN);
  rtn = chxj_bsearch_ezweb2imode_emoji(r, test, cmp);
  if (rtn) {
    return rtn;
  }
  return NULL;
}


static softbank2imode_t *
is_emoji_softbank2imode_bin(
  request_rec *r,
  const char *src,
  int (*cmp)(const char *, softbank2imode_t *))
{
  char    test[EMOJI_BINCODE_MAX_LEN];
  softbank2imode_t *rtn;
  memset(test, 0, EMOJI_BINCODE_MAX_LEN);
  strncpy(test, src, EMOJI_BINCODE_MAX_LEN);
  rtn = chxj_bsearch_softbank2imode_emoji(r, test, cmp);
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


static ezweb2imode_t *
chxj_bsearch_ezweb2imode_emoji(
  request_rec *r,
  const char *src, 
  int (*cmp)(const char *, ezweb2imode_t *))
{
  int left = 0;
  int right = EMOJI_EZWEB2IMODE_COUNT - 1;
  int mid; 
  int ret;
  mod_chxj_config *dconf;

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  while(left <= right) {
    mid = (left + right) / 2; /* calc of middle key */
    if (cmp == cmp_as_ezweb2imode_sjis_bin) {
      ret = cmp(src, dconf->emoji_ezweb2imode_sjis[mid]);
      if (! ret) {
        return dconf->emoji_ezweb2imode_sjis[mid];
      }
    }
    else {
      ret = cmp(src, dconf->emoji_ezweb2imode_utf8[mid]);
      if (! ret) {
        return dconf->emoji_ezweb2imode_utf8[mid];
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


static softbank2imode_t *
chxj_bsearch_softbank2imode_emoji(
  request_rec *r,
  const char *src, 
  int (*cmp)(const char *, softbank2imode_t *))
{
  int left = 0;
  int right = EMOJI_SOFTBANK2IMODE_COUNT - 1;
  int mid; 
  int ret;
  mod_chxj_config *dconf;

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  while(left <= right) {
    mid = (left + right) / 2; /* calc of middle key */
    if (cmp == cmp_as_softbank2imode_sjis_bin) {
      ret = cmp(src, dconf->emoji_softbank2imode_sjis[mid]);
      if (! ret) {
        return dconf->emoji_softbank2imode_sjis[mid];
      }
    }
    else if (cmp == cmp_as_softbank2imode_webcode_bin) {
      ret = cmp(src, dconf->emoji_softbank2imode_webcode[mid]);
      if (! ret) {
        return dconf->emoji_softbank2imode_webcode[mid];
      }
    }
    else {
      ret = cmp(src, dconf->emoji_softbank2imode_utf8[mid]);
      if (! ret) {
        return dconf->emoji_softbank2imode_utf8[mid];
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
  int enc;

  DBG(r,"start meta emoji --> emoji");

  enc = decide_terminal_code(GET_SPEC_CHARSET(spec), entryp->encoding);
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


char *
chxj_postdata_meta_emoji_to_emoji(
  request_rec         *r,
  char                *encoding,
  const char          *src,
  device_table        *UNUSED(spec))
{
  mod_chxj_config *conf;
  int enc = decide_server_code(encoding);
  conf = ap_get_module_config(r->per_dir_config, &chxj_module);

  DBG(r,"****************** start meta emoji --> emoji [%s]", encoding);
  switch (enc) {
  case UTF8:
    DBG(r, "DETECT: to UTF8");
    return docomo_postdata_meta_emoji_to_utf8_emoji(r,conf->emoji_index,NULL,src);

  case EUCJP:
    DBG(r, "DETECT: to EUCJP");
    return docomo_postdata_meta_emoji_to_euc_emoji(r,conf->emoji_index,NULL,src);

  case NONE:
    DBG(r, "DETECT: to NONE");
    break;

  case SJIS:
  default:
    DBG(r, "DETECT: to SJIS");
    return docomo_postdata_meta_emoji_to_sjis_emoji(r,conf->emoji_index,NULL,src);
  }
  return apr_pstrdup(r->pool, src);
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


static int
decide_server_code(
  char *server_encoding)
{
  if (IS_EUCJP_STRING(server_encoding)) return EUCJP;
  if (IS_UTF8_STRING(server_encoding))  return UTF8;
  return SJIS;
}


static char *
meta_emoji_to_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *spec,
  const char   *src,
  char         *(*callback)(request_rec *,apr_pool_t*,emoji_t *, device_table *))
{
  int i;
  int len = strlen(src);
  char *outbuf = apr_palloc(r->pool, 1);
  char *result;
  outbuf[0] = 0;
  int enc = SJIS;
  struct emoji_buffer ebuf;
  apr_pool_t *np;
  apr_status_t rv;


  DBG(r, "=> meta_emoji_to_emoji()");
  if ((rv = apr_pool_create(&np, r->pool)) != APR_SUCCESS) {
    ERR(r, "failed: new pool create. rv:[%d]", rv);
    return NULL;
  }
  memset(&ebuf, 0, sizeof(ebuf));
  ebuf.buffer = apr_palloc(np, EMOJI_BUFFER_SIZE);
  if (ebuf.buffer == NULL) {
    ERR(r, "failed: out of memory.");
    apr_pool_destroy(np);
    return NULL;
  }
  memset(ebuf.buffer, 0, EMOJI_BUFFER_SIZE);

  if (spec) {
    if (IS_SJIS_STRING(GET_SPEC_CHARSET(spec))) {
      enc = SJIS;
    }
    else {
      enc = UTF8;
    }
  }
  else {
    DBG(r, "call from postadata callback");
    if (callback == callback_postdata_meta_emoji_imode_utf8_emoji) {
      enc = UTF8;
    }
    else if (callback == callback_postdata_meta_emoji_imode_euc_emoji) {
      enc = EUCJP;
    }
    else {
      enc = SJIS;
    }
  }
  
  for (i=0; i<len; i++) {
    /* 
     * The character-code has the possibility of UTF-8 and SJIS here. 
     */
    if (enc == SJIS && is_sjis_kanji(src[i])) {
      char tmp[3];
      tmp[0] = src[i+0];
      tmp[1] = src[i+1];
      tmp[2] = 0;
      outbuf = buffered_out(np, outbuf, &ebuf, tmp, 2);
      i++;
      continue;
    }
    if (enc == UTF8) {
      if ((0xe0 & src[i]) == 0xc0) { 
        /* 2byte charactor */
        char tmp[3];
        tmp[0] = src[i+0];
        tmp[1] = src[i+1];
        tmp[2] = 0;
        DBG(r, "DETECT UTF8 2BYTE CHARACTOR=[%s]", tmp);
        outbuf = buffered_out(np, outbuf, &ebuf, tmp, 2);
        i++;
        continue;
      }
      if ((0xf0 & src[i]) == 0xe0) {
        /* 3byte charactor */
        char tmp[4];
        tmp[0] = src[i+0];
        tmp[1] = src[i+1];
        tmp[2] = src[i+2];
        tmp[3] = 0;
        DBG(r, "DETECT UTF8 3BYTE CHARACTOR=[%s]", tmp);
        outbuf = buffered_out(np, outbuf, &ebuf, tmp, 3);
        i+=2;
        continue;
      }
      if ((0xf8 & src[i]) == 0xf0) {
        /* 4byte charactor */
        char tmp[5];
        tmp[0] = src[i+0];
        tmp[1] = src[i+1];
        tmp[2] = src[i+2];
        tmp[3] = src[i+3];
        tmp[4] = 0;
        DBG(r, "DETECT UTF8 4BYTE CHARACTOR=[%.*s]", 4, tmp);
        outbuf = buffered_out(np, outbuf, &ebuf, tmp, 4);
        i+=3;
        continue;
      }
      if ((0xc0 & src[i]) == 0x80) { 
        /* unknown charactor */
        char tmp[2];
        tmp[0] = src[i+0];
        tmp[1] = 0;
        DBG(r, "DETECT UTF8 OTHER CHARACTOR=[%s]", tmp);
        outbuf = buffered_out(np, outbuf, &ebuf, tmp, 1);
        continue;
      }
    }
    if (src[i] == '&') {
      if (strncasecmp(META_EMOJI_PREFIX, &src[i], META_EMOJI_PREFIX_LEN) == 0) {
        char *npos = strchr(&src[i], ';');
        char *strno;
        int cl;
        int err;
        int emoji_len = (npos - &src[i]) + 1;
        int no_len;
        strno = apr_psprintf(np, 
                             "%.*s",
                             emoji_len - META_EMOJI_PREFIX_LEN - 1, 
                             &src[i + META_EMOJI_PREFIX_LEN]);
        DBG(r, "Found EMOJI:[%.*s] no:[%s] emoji_len:[%d]", emoji_len, &src[i], strno, emoji_len);
        err = 0;
        no_len = strlen(strno);
        for (cl=0; cl<no_len; cl++) {
          if (! isdigit(strno[cl])) {
            ERR(r, "Invalid Emoji Number:[%s]", strno);
            err = 1;
            break;
          }
        }
        if (! no_len) {
          ERR(r, "Invalid Emoji Number:[%s]", strno);
          err = 1;
        }
        DBG(r, "Emoji Number:[%s]", strno);
        if (! err) {
          char *cret;
          DBG(r, "Found EMOJI src:[%.*s] --> to:[%s]", emoji_len, &src[i], callback(r,np,emoji_table[atoi(strno) - 1], spec));
          cret = callback(r,np,emoji_table[atoi(strno) - 1], spec);
          outbuf = buffered_out(np, outbuf, &ebuf, cret, strlen(cret));
          DBG(r, "Found EMOJI substitute result:[%s]", outbuf);
        }
        i += (emoji_len - 1);
        continue;
      }
    }
    outbuf = buffered_out(np, outbuf, &ebuf, &src[i], 1);
  }
  outbuf = buffered_flush(np, outbuf, &ebuf);
  result = apr_palloc(r->pool, strlen(outbuf)+1);
  strcpy(result, outbuf);
  apr_pool_destroy(np);

  return result;
}


static char *
callback_meta_emoji_imode_utf8_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode utf8");
  return apr_psprintf(pool,
                      "&#x%s;",
                      emoji->imode.unicode.hex_string);
}


static char *
callback_postdata_meta_emoji_imode_utf8_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode utf8");
  return apr_psprintf(pool,
                      "%.3s",
                      emoji->imode.utf8.hex);
}


static char *
callback_postdata_meta_emoji_imode_sjis_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode sjis");
  return apr_psprintf(pool,
                      "%.2s",
                      emoji->imode.sjis.hex);
}


static char *
callback_postdata_meta_emoji_imode_euc_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode euc");
  return apr_psprintf(pool,
                      "%.*s",
                      3,
                      emoji->imode.euc.hex);
}


static char *
callback_meta_emoji_softbank_utf8_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  apr_size_t len;
  DBG(r, "use SoftBank utf8");
  if (!chxj_chk_numeric(emoji->softbank.no)) {
    return apr_psprintf(pool,
                        "&#x%s;",
                        emoji->softbank.unicode.hex_string);
  }
  len = strlen(emoji->softbank.no);
  return apr_psprintf(pool,
                      "%s",
                      chxj_convert_encoding(r, 
                                            MOD_CHXJ_INTERNAL_ENCODING,
                                            "UTF-8",
                                            emoji->softbank.no,
                                            &len));
}


static char *
callback_meta_emoji_au_utf8_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *spec)
{
  apr_size_t dmy_len;
  DBG(r, "use KDDI utf8");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      if (!chxj_chk_numeric(emoji->ezweb.typeA.no)) {
        return apr_psprintf(pool,
                            "%.*s",
                            3,
                            emoji->ezweb.typeA.utf8.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeA.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "UTF-8",
                                                emoji->ezweb.typeA.no,
                                                &dmy_len));
    case 'B':
    case 'b':
      if (!chxj_chk_numeric(emoji->ezweb.typeB.no)) {
        return apr_psprintf(pool,
                            "%.*s",
                            3,
                            emoji->ezweb.typeB.utf8.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeB.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "UTF-8",
                                                emoji->ezweb.typeB.no,
                                                &dmy_len));
    case 'C':
    case 'c':
      if (!chxj_chk_numeric(emoji->ezweb.typeC.no)) {
        return apr_psprintf(pool,
                            "%.*s",
                            3,
                            emoji->ezweb.typeC.utf8.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeC.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "UTF-8",
                                                emoji->ezweb.typeC.no,
                                                &dmy_len));
    case 'D':
    case 'd':
      if (!chxj_chk_numeric(emoji->ezweb.typeD.no)) {
        return apr_psprintf(pool,
                            "%.*s",
                            3,
                            emoji->ezweb.typeD.utf8.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeD.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "UTF-8",
                                                emoji->ezweb.typeD.no,
                                                &dmy_len));
    default:
      break;
    }
  }
  ERR(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(pool,
                      "&#x%s;",
                      emoji->ezweb.typeD.unicode.hex_string);
}


static char *
callback_meta_emoji_imode_sjis_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use imode SJIS");
  return apr_psprintf(pool, "%.*s", 2, emoji->imode.sjis.hex);
}


static char *
callback_meta_emoji_softbank_sjis_emoji(
  request_rec *r,
  apr_pool_t *pool,
  emoji_t *emoji,
  device_table *UNUSED(spec))
{
  DBG(r, "use SoftBank SJIS");
  char *tmp;
  if (strlen(emoji->softbank.no) && chxj_chk_numeric(emoji->softbank.no)) {
    tmp = apr_pstrdup(pool, emoji->softbank.no);
  }
  else {
    tmp = apr_palloc(pool, 6);
    tmp[0] = 0x1b;
    tmp[1] = 0x24;
    tmp[2] = emoji->softbank.sjis.hex[0];
    tmp[3] = emoji->softbank.sjis.hex[1];
    tmp[4] = 0x0f;
    tmp[5] = 0;
  }
  DBG(r, "use Softbank Code:[%s]", tmp);
  return tmp;
}


static char *
callback_meta_emoji_au_sjis_emoji(
  request_rec  *r,
  apr_pool_t *pool,
  emoji_t      *emoji,
  device_table *spec)
{
  apr_size_t dmy_len;
  DBG(r, "use KDDI SJIS");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      if (!chxj_chk_numeric(emoji->ezweb.typeA.no)) {
        return apr_psprintf(pool,
                            "%.2s",
                            emoji->ezweb.typeA.sjis.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeA.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "CP932",
                                                emoji->ezweb.typeA.no,
                                                &dmy_len));
    case 'B':
    case 'b':
      if (!chxj_chk_numeric(emoji->ezweb.typeB.no)) {
        return apr_psprintf(pool,
                            "%.2s",
                            emoji->ezweb.typeB.sjis.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeB.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "CP932",
                                                emoji->ezweb.typeB.no,
                                                &dmy_len));
    case 'C':
    case 'c':
      if (!chxj_chk_numeric(emoji->ezweb.typeC.no)) {
        return apr_psprintf(pool,
                            "%.2s",
                            emoji->ezweb.typeC.sjis.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeC.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "CP932",
                                                emoji->ezweb.typeC.no,
                                                &dmy_len));
    case 'D':
    case 'd':
      if (!chxj_chk_numeric(emoji->ezweb.typeD.no)) {
        return apr_psprintf(pool,
                            "%.2s",
                            emoji->ezweb.typeD.sjis.hex);
      }
      dmy_len = strlen(emoji->ezweb.typeD.no);
      return apr_psprintf(pool,
                          "%s",
                          chxj_convert_encoding(r,
                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                "CP932",
                                                emoji->ezweb.typeD.no,
                                                &dmy_len));
    default:
      break;
    }
  }
  ERR(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(pool,
                      "%.2s",
                      emoji->ezweb.typeD.sjis.hex);
}


static char *
callback_meta_emoji_au_sjis_hdml_emoji(
  request_rec  *r,
  apr_pool_t *pool,
  emoji_t      *emoji,
  device_table *spec)
{
  DBG(r, "use KDDI SJIS(HDML)");
  char *emoji_type = GET_EMOJI_TYPE(spec);
  if (emoji_type) {
    switch (*emoji_type) {
    case 'A':
    case 'a':
      if (!chxj_chk_numeric(emoji->ezweb.typeA.no)) {
        return apr_psprintf(pool,
                            "<IMG ICON=%s>",
                            emoji->ezweb.typeA.no);
      }
      return apr_psprintf(pool,
                          "%s",
                          emoji->ezweb.typeA.no);
    case 'B':
    case 'b':
      if (!chxj_chk_numeric(emoji->ezweb.typeB.no)) {
        return apr_psprintf(pool,
                            "<IMG ICON=%s>",
                            emoji->ezweb.typeB.no);
      }
      return apr_psprintf(pool,
                          "%s",
                          emoji->ezweb.typeB.no);
    case 'C':
    case 'c':
      if (!chxj_chk_numeric(emoji->ezweb.typeC.no)) {
        return apr_psprintf(pool,
                            "<IMG ICON=%s>",
                            emoji->ezweb.typeC.no);
      }
      return apr_psprintf(pool,
                          "%s",
                          emoji->ezweb.typeC.no);
    case 'D':
    case 'd':
      if (!chxj_chk_numeric(emoji->ezweb.typeD.no)) {
        return apr_psprintf(pool,
                            "<IMG IDON=%s>",
                            emoji->ezweb.typeD.no);
      }
      return apr_psprintf(pool,
                          "%s",
                          emoji->ezweb.typeD.no);
    default:
      break;
    }
  }
  ERR(r, "Invalid KDDI spec(emoji_type) [%s]", emoji_type);
  return apr_psprintf(pool,
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
docomo_postdata_meta_emoji_to_utf8_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *UNUSED(spec),
  const char   *src)
{
  return meta_emoji_to_emoji(r, 
                             emoji_table, 
                             NULL,
                             src,
                             callback_postdata_meta_emoji_imode_utf8_emoji);
}


static char *
docomo_postdata_meta_emoji_to_sjis_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *UNUSED(spec),
  const char   *src)
{
  DBG(r, "=> docomo_postdata_meta_emoji_to_sjis_emoji");
  return meta_emoji_to_emoji(r, 
                             emoji_table, 
                             NULL,
                             src,
                             callback_postdata_meta_emoji_imode_sjis_emoji);
}


static char *
docomo_postdata_meta_emoji_to_euc_emoji(
  request_rec  *r,
  emoji_t      **emoji_table,
  device_table *UNUSED(spec),
  const char   *src)
{
  return meta_emoji_to_emoji(r, 
                             emoji_table, 
                             NULL,
                             src,
                             callback_postdata_meta_emoji_imode_euc_emoji);
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

/**
 * Emoji Convert to Meta Emoji for POSTDATA.
 *
 * @param r        [i] The request_rec
 * @param encoding [i] client encoding
 */
char *
chxj_postdata_ezweb_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen) 
{
  apr_size_t ilen = *iolen;
  char *outbuf;
  apr_size_t i;
  int enc;
dbg_r = r;

  DBG(r, "start convert EMOJI to META EMOJI");

  enc = SJIS;
  if (STRCASEEQ('n','N',"none",encoding)) {
    return apr_pstrdup(r->pool, src);
  } else if (IS_SJIS_STRING(encoding)) {
    enc = SJIS;
  } else if (IS_UTF8_STRING(encoding)) {
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
      DBG(r, "ascii src[%d]=[%x]", i, (unsigned char)src[i]);
      outbuf = apr_pstrcat(r->pool, 
                           outbuf,
                           apr_psprintf(r->pool, 
                                        "%c",
                                        src[i]), 
                           NULL);
    }
    else {
      /* multibyte charactor */
      char mb[8];
      ezweb2imode_t *tmp;
      switch (enc) {
      case UTF8:
        DBG(r, "mb src[%d]=[\\x%x][\\x%x][\\x%x]", 
            i, (unsigned char)src[i], (unsigned char)src[i+1], (unsigned char)src[i+2]);
        if ((0xe0 & src[i]) == 0xc0) { 
          /* 2byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = 0;
          i++;
        }
        else if ((0xf0 & src[i]) == 0xe0) {
          DBG(r, "DETECT UTF8 BIN");
          tmp = is_emoji_as_ezweb2imode_utf8_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji;
            if (!chxj_chk_numeric(tmp->imode_no)) {
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s%s;", 
                                        META_EMOJI_PREFIX,
                                        tmp->imode_no);
            }
            else {
              apr_size_t imode_no_len = strlen(tmp->imode_no);
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s", 
                                        chxj_convert_encoding(r, 
                                                              MOD_CHXJ_INTERNAL_ENCODING,
                                                              encoding,
                                                              tmp->imode_no,
                                                              &imode_no_len));
            }
            DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
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
          /* 3byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = 0;
          i+=2;
        }
        else if ((0xf8 & src[i]) == 0xf0) {
          /* 4byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = src[i+3];
          mb[4] = 0;
          i+=3;
        }
        else if ((0xc0 & src[i]) == 0x80) { 
          /* unknown charactor */
          mb[0] = src[i+0];
          mb[1] = 0;
        }
        break;

      case SJIS:
      default:
        DBG(r, "mb src[%d]=[%x][%x]", i, (unsigned char)src[i], (unsigned char)src[i+1]);
        DBG(r, "DETECT SJIS BIN");
        tmp = is_emoji_as_ezweb2imode_sjis_bin(r, &src[i]);
        if (tmp) {
          char *meta_emoji;
          if (!chxj_chk_numeric(tmp->imode_no)) {
            meta_emoji = apr_psprintf(r->pool, 
                                      "%s%s;", 
                                      META_EMOJI_PREFIX,
                                      tmp->imode_no);
          }
          else {
            apr_size_t imode_no_len = strlen(tmp->imode_no);
            meta_emoji = apr_psprintf(r->pool, 
                                      "%s", 
                                      chxj_convert_encoding(r, MOD_CHXJ_INTERNAL_ENCODING, encoding, tmp->imode_no, &imode_no_len));
          }
          DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
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
        mb[0] = src[i++]; 
        mb[1] = src[i]; 
        mb[2] = 0;
        break;
      }
      outbuf = apr_pstrcat(r->pool, outbuf, mb, NULL);
    }
  }
  *iolen = strlen(outbuf);
  DBG(r, "end convert EMOJI to META EMOJI.  [%s]", outbuf);
  return outbuf;
}


static ezweb2imode_t *
is_emoji_as_ezweb2imode_utf8_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_ezweb2imode_bin(r, src, cmp_as_ezweb2imode_utf8_bin);
}


static ezweb2imode_t *
is_emoji_as_ezweb2imode_sjis_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_ezweb2imode_bin(r, src, cmp_as_ezweb2imode_sjis_bin);
}


static softbank2imode_t *
is_emoji_as_softbank2imode_utf8_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_softbank2imode_bin(r, src, cmp_as_softbank2imode_utf8_bin);
}


static softbank2imode_t *
is_emoji_as_softbank2imode_webcode_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_softbank2imode_bin(r, src, cmp_as_softbank2imode_webcode_bin);
}

static softbank2imode_t *
is_emoji_as_softbank2imode_sjis_bin(
  request_rec *r,
  const char *src)
{
  return is_emoji_softbank2imode_bin(r, src, cmp_as_softbank2imode_sjis_bin);
}


char *
chxj_postdata_softbank_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen) 
{
  apr_size_t ilen = *iolen;
  char *outbuf;
  apr_size_t i;
  int enc;
dbg_r = r;

  DBG(r, "start convert EMOJI to META EMOJI");

  enc = SJIS;
  if (STRCASEEQ('n','N',"none",encoding)) {
    return apr_pstrdup(r->pool, src);
  } else if (IS_SJIS_STRING(encoding)) {
    enc = SJIS;
  } else if (IS_UTF8_STRING(encoding)) {
    enc = UTF8;
  } 

  /* 
   * Now because the character-code was decided 
   * seeing one character.
   */
  outbuf = apr_palloc(r->pool, 1);
  outbuf[0] = 0;

  for (i = 0; i < ilen; i++) {
    if (! (src[i] == 0x1b && i < ilen - 1 && src[i+1] == 0x24)
        && isascii(src[i])) {
      DBG(r, "ascii src[%d]=[%x]", i, (unsigned char)src[i]);
      outbuf = apr_pstrcat(r->pool, 
                           outbuf,
                           apr_psprintf(r->pool, 
                                        "%c",
                                        src[i]), 
                           NULL);
    }
    else {
      /* multibyte charactor */
      char mb[8];
      softbank2imode_t *tmp;
      switch (enc) {
      case UTF8:
        DBG(r, "mb src[%d]=[\\x%x][\\x%x][\\x%x]", 
            i, (unsigned char)src[i], (unsigned char)src[i+1], (unsigned char)src[i+2]);
        DBG(r, "DETECT UTF8 BIN");
        tmp = is_emoji_as_softbank2imode_utf8_bin(r, &src[i]);
        if (tmp) {
          char *meta_emoji = NULL;
          if (!chxj_chk_numeric(tmp->imode_no)) {
            meta_emoji = apr_psprintf(r->pool, 
                                      "%s%s;", 
                                      META_EMOJI_PREFIX,
                                      tmp->imode_no);
          }
          else {
            apr_size_t imode_no_len = strlen(tmp->imode_no);
            if (imode_no_len) {
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s", 
                                        chxj_convert_encoding(r, 
                                                              MOD_CHXJ_INTERNAL_ENCODING,
                                                              encoding,
                                                              tmp->imode_no,
                                                              &imode_no_len));
            }
          }
          DBG(r, "FOUND EMOJI :[%s]", (meta_emoji) ? meta_emoji : "no emoji");
          if (meta_emoji) {
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                              "%s", 
                                              meta_emoji), 
                                 NULL);
          }
          i+=2;
          continue;
        }
        DBG(r, "NOT FOUND EMOJI");
        if ((0xe0 & src[i]) == 0xc0) { 
          /* 2byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = 0;
          i++;
        }
        else if ((0xf0 & src[i]) == 0xe0) {
          /* 3byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = 0;
          i+=2;
        }
        else if ((0xf8 & src[i]) == 0xf0) {
          /* 4byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = src[i+3];
          mb[4] = 0;
          i+=3;
        }
        else if ((0xc0 & src[i]) == 0x80) { 
          /* unknown charactor */
          mb[0] = src[i+0];
          mb[1] = 0;
        }
        break;

      case SJIS:
      default:
        DBG(r, "mb src[%d]=[%x][%x]", i, (unsigned char)src[i], (unsigned char)src[i+1]);
        DBG(r, "DETECT SJIS BIN");
        if (IS_SOFTBANK_WEBCODE(&src[i])) {
          char dig_char;  /* [G|E|F|O|P|Q] */
          i += 2;
          if (i >= ilen)
            break;
          dig_char = src[i];
          i++;
          for (;src[i];) {
            char web_code[3];
            if (IS_SOFTBANK_WEBCODE_ENDMARK(src[i]))
              break;
            web_code[0] = dig_char;
            web_code[1] = src[i];
            web_code[2] = 0;
            tmp = is_emoji_as_softbank2imode_webcode_bin(r, web_code);
            if (tmp) {
              char *meta_emoji = NULL;
              if (!chxj_chk_numeric(tmp->imode_no)) {
                meta_emoji = apr_psprintf(r->pool, 
                                          "%s%s;", 
                                          META_EMOJI_PREFIX,
                                          tmp->imode_no);
              }
              else {
                apr_size_t imode_no_len = strlen(tmp->imode_no);
                meta_emoji = apr_psprintf(r->pool, 
                                          "%s", 
                                          chxj_convert_encoding(r, 
                                                                MOD_CHXJ_INTERNAL_ENCODING,
                                                                encoding,
                                                                tmp->imode_no,
                                                                &imode_no_len));
              }
              DBG(r, "FOUND EMOJI :[%s]", (meta_emoji) ? meta_emoji : "no emoji");
              if (meta_emoji) {
                outbuf = apr_pstrcat(r->pool, 
                                     outbuf, 
                                     apr_psprintf(r->pool, 
                                                  "%s", 
                                                  meta_emoji), 
                                     NULL);
              }
              i++;
            }
            else {
              break;
            }
          }
          continue;
        }
        else {
          tmp = is_emoji_as_softbank2imode_sjis_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji = NULL;
            if (!chxj_chk_numeric(tmp->imode_no)) {
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s%s;", 
                                        META_EMOJI_PREFIX,
                                        tmp->imode_no);
            }
            else {
              apr_size_t imode_no_len = strlen(tmp->imode_no);
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s", 
                                        chxj_convert_encoding(r, 
                                                              MOD_CHXJ_INTERNAL_ENCODING,
                                                              encoding,
                                                              tmp->imode_no,
                                                              &imode_no_len));
            }
            DBG(r, "FOUND EMOJI :[%s]", (meta_emoji) ? meta_emoji : "no emoji");
            if (meta_emoji) {
              outbuf = apr_pstrcat(r->pool, 
                                   outbuf, 
                                   apr_psprintf(r->pool, 
                                                "%s", 
                                                meta_emoji), 
                                   NULL);
            }
            i++;
            continue;
          }
        }
        DBG(r, "NOT FOUND EMOJI");
        mb[0] = src[i++]; 
        mb[1] = src[i]; 
        mb[2] = 0;
        break;
      }
      outbuf = apr_pstrcat(r->pool, outbuf, mb, NULL);
    }
  }
  *iolen = strlen(outbuf);
  DBG(r, "end convert EMOJI to META EMOJI.  [%s]", outbuf);
  return outbuf;
}




/*===========================================================================*/
/* I-Mode POSTDATA Converter                                                 */
/*===========================================================================*/

/**
 * Emoji Convert to Meta Emoji for POSTDATA.
 *
 * @param r        [i] The request_rec
 * @param encoding [i] client encoding
 */
char *
chxj_postdata_imode_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen) 
{
  apr_size_t ilen = *iolen;
  char *outbuf;
  apr_size_t i;
  int enc;
dbg_r = r;
  DBG(r, "start convert EMOJI to META EMOJI");
  enc = SJIS;
  if (STRCASEEQ('n','N',"none",encoding)) {
    return apr_pstrdup(r->pool, src);
  } else if (IS_SJIS_STRING(encoding)) {
    enc = SJIS;
  } else if (IS_UTF8_STRING(encoding)) {
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
      outbuf = apr_pstrcat(r->pool, 
                           outbuf,
                           apr_psprintf(r->pool, 
                                        "%c",
                                        src[i]), 
                           NULL);
    }
    else {
      /* 
       * multibyte charactor 
       */
      char mb[8];
      emoji_t *tmp;

      switch (enc) {
      case UTF8:
        if ((0xe0 & src[i]) == 0xc0) { 
          /* 2byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = 0;
          i++;
        }
        else if ((0xf0 & src[i]) == 0xe0) {
          DBG(r, "DETECT UTF8 BIN");
          if ((0xff & src[i]) == 0xee) {
DBG(r, "Yes EMOJI KAMO");
            tmp = is_emoji_as_imode2imode_utf8_bin(r, &src[i]);
            if (tmp) {
              char *meta_emoji;
              meta_emoji = apr_psprintf(r->pool, 
                                        "%s%d;", 
                                        META_EMOJI_PREFIX,
                                        tmp->no);
              DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
              outbuf = apr_pstrcat(r->pool, 
                                   outbuf, 
                                   apr_psprintf(r->pool, 
                                                "%s", 
                                                meta_emoji), 
                                   NULL);
              i+=2;
              continue;
            }
          }
          DBG(r, "NOT FOUND EMOJI");
          /* 3byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = 0;
          i+=2;
        }
        else if ((0xf8 & src[i]) == 0xf0) {
          /* 4byte charactor */
          mb[0] = src[i+0];
          mb[1] = src[i+1];
          mb[2] = src[i+2];
          mb[3] = src[i+3];
          mb[4] = 0;
          i+=3;
        }
        else if ((0xc0 & src[i]) == 0x80) { 
          /* unknown charactor */
          mb[0] = src[i+0];
          mb[1] = 0;
        }
        break;

      case SJIS:
      default:
        DBG(r, "DETECT SJIS BIN");
        if ((src[i] & 0xff) == 0xf8 || (src[i] & 0xff) == 0xf9) {
          tmp = is_emoji_as_imode2imode_sjis_bin(r, &src[i]);
          if (tmp) {
            char *meta_emoji;
            meta_emoji = apr_psprintf(r->pool, 
                                      "%s%d;", 
                                      META_EMOJI_PREFIX,
                                      tmp->no);
            DBG(r, "FOUND EMOJI :[%s]", meta_emoji);
            outbuf = apr_pstrcat(r->pool, 
                                 outbuf, 
                                 apr_psprintf(r->pool, 
                                              "%s", 
                                              meta_emoji), 
                                 NULL);
            i++;
            continue;
          }
        }
        DBG(r, "NOT FOUND EMOJI");
        mb[0] = src[i++]; 
        mb[1] = src[i]; 
        mb[2] = 0;
        break;
      }
      outbuf = apr_pstrcat(r->pool, outbuf, mb, NULL);
    }
  }
  *iolen = strlen(outbuf);
  DBG(r, "end convert EMOJI to META EMOJI.  [%s]", outbuf);
  return outbuf;
}


static emoji_t *
is_emoji_as_imode2imode_utf8_bin(
  request_rec *r,
  const char  *src)
{
  return is_emoji_imode2imode_bin(r, src, cmp_as_imode2imode_utf8_bin);
}


static emoji_t *
is_emoji_imode2imode_bin(
  request_rec *r,
  const char  *src,
  int         (*cmp)(const char *, emoji_t *))
{
  char          test[EMOJI_BINCODE_MAX_LEN];
  emoji_t       *rtn;

  memset(test, 0, EMOJI_BINCODE_MAX_LEN);
  strncpy(test, src, EMOJI_BINCODE_MAX_LEN);
  rtn = chxj_bsearch_imode2imode_emoji(r, test, cmp);
  if (rtn) {
    return rtn;
  }
  return NULL;
}


static emoji_t *
chxj_bsearch_imode2imode_emoji(
  request_rec *r,
  const char  *src, 
  int         (*cmp)(const char *, emoji_t *))
{
  int left = 0;
  int right = EMOJI_COUNT - 1;
  int mid; 
  int ret;
  mod_chxj_config *dconf;

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  while(left <= right) {
    mid = (left + right) / 2;
    if (cmp == cmp_as_imode2imode_sjis_bin) {
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
cmp_as_imode2imode_utf8_bin(
  const char    *src,
  emoji_t       *target)
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
is_emoji_as_imode2imode_sjis_bin(
  request_rec *r,
  const char  *src)
{
  return is_emoji_imode2imode_bin(r, src, cmp_as_imode2imode_sjis_bin);
}


static int
cmp_as_imode2imode_sjis_bin(
  const char *src, 
  emoji_t    *target)
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
