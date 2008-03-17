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
#include "apr_pools.h"
#include "qs_log.h"
#include "qs_malloc.h"

/**
 * Init
 */
void
qs_init_malloc(Doc* doc) 
{
  apr_status_t rtn;

  rtn = apr_allocator_create(&(doc->allocator));
  if (rtn != APR_SUCCESS) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  rtn = apr_pool_create_ex(&(doc->pool), NULL, NULL, doc->allocator);
  if (rtn != APR_SUCCESS) {
    QX_LOGGER_FATAL("Out Of Memory");
  }

  doc->do_init_flag = 1;
}





void
qs_all_free(Doc* doc, const char* UNUSED(fname), int UNUSED(line)) 
{
  if (doc->do_init_flag) {
    apr_pool_destroy(doc->pool);

    apr_allocator_destroy(doc->allocator);
    doc->do_init_flag = 0;
  }
}


#if HAVE_MALLOC == 0
#include "malloc.c"
#endif
/*
 * vim:ts=2 et
 */
