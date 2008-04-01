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
#ifndef __CHXJ_BUFFERED_WRITE_H__
#define __CHXJ_BUFFERED_WRITE_H__

#include <apr_pools.h>

#define CHXJ_BUFFERED_OBJECT_SIZE (8192)

typedef struct {
  char *buffer;
  int use_len;
  apr_pool_t *pool;
} buf_object;


extern int chxj_buffered_write_init(apr_pool_t *parent_pool, buf_object *buf);
extern char *chxj_buffered_write(const char *src, buf_object *buf, const char *add, int add_len);
extern char *chxj_buffered_write_flush(const char *src, buf_object *buf);
extern int chxj_buffered_write_terminate(buf_object *buf);

#define BUFFERED_WRITE_LITERAL(src, buf, add)     chxj_buffered_write((src), (buf), (add), sizeof(add)-1)
#define BUFFERED_WRITE_VALUE(src, buf, add)       chxj_buffered_write((src), (buf), (add), strlen(add))

#endif
/*
 * vim:ts=2 et
 */
