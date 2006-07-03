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
#include "mod_chxj.h"
#include "chxj_encoding.h"
#include "chxj_apply_convrule.h"
#include "chxj_url_encode.h"

#if defined(HAVE_LIBICONV_HOOK)
#  include "iconv_hook/iconv.h"
#else
#  if defined(HAVE_LIBICONV)
#    include "iconv.h"
#  else
#    error "Please install libiconv or libiconv_hook. and Please set LD_LIBRARY_PATH."
#  endif
#endif


char*
chxj_encoding(request_rec *r, const char* src, apr_size_t* len)
{
  char*               obuf;
  char*               ibuf;
  char*               spos;
  
  iconv_t             cd;
  size_t              result;
  apr_size_t          ilen;
  apr_size_t          olen;
  mod_chxj_config*    dconf;
  chxjconvrule_entry* entryp;

  DBG(r,"start chxj_encoding()");

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  if (dconf == NULL) {
    DBG(r,"none encoding.");
    return (char*)src;
  }

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (entryp->encoding == NULL) {
    DBG(r,"none encoding.");
    return (char*)src;
  }

  if ((*(entryp->encoding) == 'n' || *(entryp->encoding) == 'N') 
  &&   strcasecmp(entryp->encoding, "NONE") == 0) {
    DBG(r,"none encoding.");
    return (char*)src;
  }
  ilen = *len;
  ibuf = apr_palloc(r->pool, ilen+1);
  if (ibuf == NULL) {
    DBG(r,"end   chxj_encoding()");
    return (char*)src;
  }
  memset(ibuf, 0, ilen+1);
  memcpy(ibuf, src, ilen);

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(r->pool, olen);
  if (obuf == NULL) {
    DBG(r,"end   chxj_encoding()");
    return ibuf;
  }
  DBG2(r,"encode convert [%s] -> [%s]", entryp->encoding, "CP932");

  memset(obuf, 0, olen);
  cd = iconv_open("CP932", entryp->encoding);
  if (cd == (iconv_t)-1) {
    DBG(r,"end   chxj_encoding()");
    return ibuf;
  }
  while (ilen > 0) {
    result = iconv(cd, &ibuf, &ilen, &obuf, &olen);
    if (result == (size_t)(-1)) {
      break;
    }
  }
  *len = olen;
  iconv_close(cd);

  DBG3(r,"end   chxj_encoding() len=[%d] obuf=[%.*s]", *len, *len, spos);
  return spos;
}


char*
chxj_rencoding(request_rec *r, const char* src, apr_size_t* len)
{
  char*               obuf;
  char*               ibuf;
  char*               spos;
  
  iconv_t             cd;
  size_t              result;
  apr_size_t          ilen;
  apr_size_t          olen;
  mod_chxj_config*    dconf;
  chxjconvrule_entry* entryp;

  DBG(r,"start chxj_rencoding()");

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (! dconf) {
    DBG(r,"none encoding.");
    return (char*)src;
  }

  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp->encoding) {
    DBG(r,"none encoding.");
    return (char*)src;
  }

  if ((*(entryp->encoding) == 'n' || *(entryp->encoding) == 'N') 
  &&   strcasecmp(entryp->encoding, "NONE") == 0) {
    DBG(r,"none encoding.");
    return (char*)src;
  }

  ilen = *len;
  ibuf = apr_palloc(r->pool, ilen+1);
  if (ibuf == NULL) {
    DBG(r,"end   chxj_rencoding()");
    return (char*)src;
  }
  memset(ibuf, 0, ilen+1);
  memcpy(ibuf, src, ilen);

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(r->pool, olen);
  if (obuf == NULL) {
    DBG(r,"end   chxj_rencoding()");
    return ibuf;
  }
  DBG2(r,"encode convert [%s] -> [%s]", "CP932", entryp->encoding);

  memset(obuf, 0, olen);
  cd = iconv_open(entryp->encoding, "CP932");
  if (cd == (iconv_t)-1) {
    DBG(r,"end   chxj_rencoding()");
    return ibuf;
  }
  while (ilen > 0) {
    result = iconv(cd, &ibuf, &ilen, &obuf, &olen);
    if (result == (size_t)(-1)) {
      break;
    }
  }
  *len = olen;
  iconv_close(cd);

  DBG3(r,"end   chxj_rencoding() len=[%d] obuf=[%.*s]", *len, *len, spos);

  return spos;
}

char*
chxj_encoding_parameter(request_rec* r, const char* value)
{
  char* src;
  char* src_sv;
  char* pstat;
  char* spos;
  char* pair;
  char* key;
  char* val;
  char* vstat;
  char* param;

  int use_amp_flag;
  
  DBG(r, "start chxj_encoding_parameter()");
  src = apr_pstrdup(r->pool, value);

  spos = strchr(src, '?');
  if (!spos) {
    DBG(r, "end   chxj_encoding_parameter()");
    return src;
  }
  *spos++ = 0;
  src_sv = apr_pstrdup(r->pool, src);
  param = apr_palloc(r->pool, 1);
  param[0] = 0;

  for (;;) {
    apr_size_t len;

    use_amp_flag = 0;

    pair = apr_strtok(spos, "&", &pstat);
    spos = NULL;
    if (!pair) break;
    if (strncasecmp(pair, "amp;", 4) == 0) {
      pair += 4;
      use_amp_flag = 1;
    }
    key = apr_strtok(pair, "=", &vstat);
    val = apr_strtok(NULL, "=", &vstat);
    if (val) {
      val = chxj_url_decode(r, val);
      len = (apr_size_t)strlen(val);
      val = chxj_encoding(r, val, &len);
      val = chxj_url_encode(r, val);
      if (strlen(param) == 0) {
        param = apr_pstrcat(r->pool, param, key, "=", val, NULL);
      }
      else {
        if (use_amp_flag) {
          param = apr_pstrcat(r->pool, param, "&amp;", key, "=", val, NULL);
        }
        else {
          param = apr_pstrcat(r->pool, param, "&", key, "=", val, NULL);
        }
      }
    }
    else {
      if (strlen(param) == 0) {
        param = apr_pstrcat(r->pool, param, key,  NULL);
      }
      else {
        if (use_amp_flag) {
          param = apr_pstrcat(r->pool, param, "&amp;", key, NULL);
        }
        else {
          param = apr_pstrcat(r->pool, param, "&", key, NULL);
        }
      }
    }
  }
  DBG(r, "end   chxj_encoding_parameter()");
  return apr_pstrcat(r->pool, src_sv, "?", param, NULL);
}
/*
 * vim:ts=2 et
 */
