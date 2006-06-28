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


char* 
chxj_img_conv(request_rec* r, device_table* spec, const char* s)
{
  char* dst;

  dst = apr_pstrdup(r->pool, s);

  if (spec) {
    switch (spec->color) {
    case 2       : dst = apr_pstrcat(r->pool, dst,".10", NULL); break;
    case 4       : dst = apr_pstrcat(r->pool, dst,".20", NULL); break;
    case 256     : dst = apr_pstrcat(r->pool, dst,".30", NULL); break;
    case 4096    : dst = apr_pstrcat(r->pool, dst,".40", NULL); break;
    case 65536   : dst = apr_pstrcat(r->pool, dst,".50", NULL); break;
    case 262144  : dst = apr_pstrcat(r->pool, dst,".60", NULL); break;
    case 15680000: dst = apr_pstrcat(r->pool, dst,".70", NULL); break;
    default:       dst = apr_pstrcat(r->pool, dst,".30", NULL); break;
    }

    if (spec->available_jpeg != 0)
      return apr_pstrcat(r->pool, dst,".jpg", NULL);

    if (spec->available_png != 0)
      return apr_pstrcat(r->pool, dst,".png", NULL);

    if (spec->available_gif != 0)
      return apr_pstrcat(r->pool, dst, ".gif", NULL);

    if (spec->available_bmp4 != 0 || spec->available_bmp2 != 0)
      return apr_pstrcat(r->pool, dst, ".bmp", NULL);
  }

  return dst;
}
