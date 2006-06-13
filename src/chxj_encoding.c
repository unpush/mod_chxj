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
  char* obuf;
  char* ibuf;
  char* spos;
  
  iconv_t cd;
  size_t result;
  apr_size_t ilen;
  apr_size_t olen;
  mod_chxj_config_t* conf;

  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, "start chxj_encoding()");
  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, " ");

  if (conf == NULL) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "none encoding.");
    return (char*)src;
  }
  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, " ");

  if (conf->server_side_encoding == NULL) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "none encoding.");
    return (char*)src;
  }
  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, " ");

  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, "Encoding:[%.*s]", 4, conf->server_side_encoding);

  if ((*(conf->server_side_encoding) == 'n' || *(conf->server_side_encoding) == 'N') 
     &&   strcasecmp(conf->server_side_encoding, "NONE") == 0) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "none encoding.");
    return (char*)src;
  }
  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, " ");

  ilen = *len;
  ibuf = apr_palloc(r->pool, ilen+1);
  if (ibuf == NULL) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding()");
    return (char*)src;
  }
  memset(ibuf, 0, ilen+1);
  memcpy(ibuf, src, ilen);
  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, " ");

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(r->pool, olen);
  if (obuf == NULL) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding()");
    return ibuf;
  }
  ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "encode convert [%s] -> [%s]", conf->server_side_encoding, "CP932");

  memset(obuf, 0, olen);
  cd = iconv_open("CP932", conf->server_side_encoding );
  if (cd == (iconv_t)-1) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding()");
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

  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding() len=[%d] obuf=[%.*s]", *len, *len, obuf);
  return spos;
}
/*
 * vim:ts=2 et
 */
