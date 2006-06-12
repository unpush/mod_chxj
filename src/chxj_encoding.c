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
#include "chxj_encoding.h"
#include "iconv_hook/iconv.h"

char*
chxj_encoding(request_rec *r, const char* src, apr_size_t* len)
{
  char* obuf;
  char* spos;
  
  iconv_t cd;
  size_t result;
  apr_size_t ilen;
  apr_size_t olen;

  ap_log_rerror(
    APLOG_MARK,APLOG_DEBUG, 0, r, "start chxj_encoding()");

  ilen = *len;

  olen = ilen * 4 + 1;
  spos = obuf = apr_palloc(r->pool, olen);
  if (obuf == NULL) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding()");
    return (char*)src;
  }

  memset(obuf, 0, olen);
  cd = iconv_open("CP932", "EUC-JP");
  if (cd == (iconv_t)-1) {
    ap_log_rerror(
      APLOG_MARK,APLOG_DEBUG, 0, r, "end   chxj_encoding()");
    return (char*)src;
  }
  while (ilen > 0) {
    result = iconv(cd, &src, &ilen, &obuf, &olen);
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
