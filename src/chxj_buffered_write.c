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
#include <apr_pools.h>

#include "mod_chxj.h"
#include "chxj_buffered_write.h"


int
chxj_buffered_write_init(apr_pool_t *parent_pool, buf_object *buf)
{
  apr_status_t rv;
  if ((rv = apr_pool_create(&buf->pool, parent_pool)) != APR_SUCCESS) {
    return CHXJ_FALSE;
  }
  buf->buffer = apr_palloc(buf->pool, CHXJ_BUFFERED_OBJECT_SIZE);
  if (! buf->buffer) {
    return CHXJ_FALSE;
  }
  memset(buf->buffer, 0, CHXJ_BUFFERED_OBJECT_SIZE);
  buf->use_len = 0;
  return CHXJ_TRUE;
}


char *
chxj_buffered_write(const char *src, buf_object *buf, const char *add, int add_len)
{
  char *result;

  if (buf->use_len + add_len < CHXJ_BUFFERED_OBJECT_SIZE - 1) {
    memcpy(&buf->buffer[buf->use_len], add, add_len);
    buf->use_len += add_len;
    buf->buffer[buf->use_len] = 0;
    return (char *)src;
  }
  else if (buf->use_len < CHXJ_BUFFERED_OBJECT_SIZE - 1) {
    int new_add_len = CHXJ_BUFFERED_OBJECT_SIZE - 1 - buf->use_len;
    memcpy(&buf->buffer[buf->use_len], add, new_add_len);
    buf->use_len += new_add_len;
    buf->buffer[buf->use_len] = 0;
    result = chxj_buffered_write_flush(src, buf);
    result = chxj_buffered_write(result, buf, add + new_add_len, add_len - new_add_len);
    return result;
  }
  result = chxj_buffered_write_flush(src, buf);
  result = chxj_buffered_write(result, buf, add, add_len);
  return result;
}


char *
chxj_buffered_write_flush(const char *src, buf_object *buf)
{
  char *result;

  buf->buffer[buf->use_len] = 0;
  result = apr_pstrcat(buf->pool, src, buf->buffer, NULL);
  buf->use_len = 0;
  buf->buffer[0] = 0;

  return result;
}


int
chxj_buffered_write_terminate(buf_object *buf)
{
  apr_pool_destroy(buf->pool);
  buf->pool = NULL;
  buf->buffer = NULL;
  buf->use_len = 0;
  return CHXJ_TRUE;
}
