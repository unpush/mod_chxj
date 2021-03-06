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
#include <stdio.h>
#include "qs_parse_string.h"
#include "qs_parse_attr.h"
#include "qs_log.h"
#include "qs_ignore_sp.h"


Attr *
qs_parse_attr(Doc *doc, const char *s, int len, int *pos) 
{
  int   ii;
  int   start_pos;
  int   size;
  int   novalue;
  char  *name;
  char  *value;
  Attr  *attr;
  int   use_quote_sq;
  int   use_quote_dq;
  int   backslash;

  if (! doc) {
    QX_LOGGER_FATAL("runtime exception: qs_parse_attr(): doc is null");
    return NULL;
  }
  if (! doc->pool) {
    QX_LOGGER_FATAL("runtime exception: qs_parse_attr(): doc->pool is null");
    return NULL;
  }
  if (! s) return NULL;

  use_quote_sq = 0;
  use_quote_dq = 0;
  backslash = 0;

  QX_LOGGER_DEBUG("start qs_parse_attr()");

  /* ignore space */
  ii = start_pos = qs_ignore_sp_and_quote(doc, s, len);
  QX_LOGGER_DEBUG_INT("len",len);

  /* get attr name */
  for (;ii<len; ii++) {
    if (is_white_space(s[ii])) 
      break;

    if (s[ii] == '=')
      break;
  }

  size = ii - start_pos;
  QX_LOGGER_DEBUG_INT("size", size);

  /* 
   * not found 
   */
  if (size == 0) {
    *pos = ii;
    return NULL;
  }

  name = (char *)apr_palloc(doc->pool,size+1);
  memset(name, 0, size+1);
  memcpy(name, &s[start_pos], size);

  QX_LOGGER_DEBUG((char *)name);

  novalue = 0;
  /* find '=' */
  for (;ii<len; ii++) {
    if (is_white_space(s[ii])) 
      /* ignore */
      continue;
    if (s[ii] == '=') 
      ii++;
    else 
      /* novalue */
      novalue = 1;
    break;
  }

  if (ii == len) 
    novalue = 1;

  size = 0;
  if (!novalue) {

    /* 
     * ignore space
     */
    ii += qs_ignore_sp(doc, &s[ii], len-ii);

    backslash = 0;
    for (;ii<len; ii++) {
      if (s[ii] == '\\') {
        backslash = 1;
        break;
      }
      if (s[ii] == '\'') {
        use_quote_sq = 1;
        ii++;
        break;
      }
      if (s[ii] == '"') {
        use_quote_dq = 1;
        ii++;
        break;
      }
      if (!is_white_space(s[ii]))
        break;
    }
  
    start_pos = ii;
    if (backslash && ii + 2 < len)
      ii+=2;
    
    backslash = 0;
    /* 
     * get attr value 
     */
    for (;ii<len; ii++) {
      if (is_sjis_kanji(s[ii])) {
        ii++;
        continue;
      }

      if (is_sjis_kana(s[ii])) 
        continue;

      if (is_white_space(s[ii])) {
        if (! use_quote_sq && ! use_quote_dq) 
          break;
      }

      if (s[ii] == '\\') {
        ii++;
        continue;
      }

      if (s[ii] == '"' && use_quote_dq) 
        break;

      if (s[ii] == '\'' && use_quote_sq) 
        break;
    }
    size = ii - start_pos;

    QX_LOGGER_DEBUG_INT("size",size);
  }

  value = (char *)apr_palloc(doc->pool, size+1);
  memset(value, 0, size+1);
  if (size != 0) 
    memcpy(value, &s[start_pos], size);

  attr = qs_new_attr(doc);

  attr->name  = name;
  attr->value = value;

  QX_LOGGER_DEBUG("end qs_parse_attr()");
  *pos = ii;

  return attr;
}


Attr *
qs_new_attr(Doc *doc) 
{
  Attr *attr;

  if (!doc) {
    QX_LOGGER_FATAL("runtime exception: qs_new_attr(): doc is null");
    return NULL;
  }
  if (!doc->pool) {
    QX_LOGGER_FATAL("runtime exception: qs_new_attr(): doc->pool is null");
    return NULL;
  }

  attr = (Attr *)apr_palloc(doc->pool,sizeof(Attr));
  if (attr == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
    return NULL;
  }

  attr->next   = NULL;
  attr->parent = NULL;
  attr->name   = NULL;
  attr->value  = NULL;

  return attr;
}
/*
 * vim:ts=2 et
 */
