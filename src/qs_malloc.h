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
#ifndef __QS_MALLOC_H__
#define __QS_MALLOC_H__
#include "qs_parse_string.h"

#define QX_ALLOC_MAX_SIZE (1024*1024)

/**
 * Not return NULL;
 */
QS_EXPORT void* qs_malloc(Doc* doc, int size, const char* fname, int line) ;
QS_EXPORT void qs_init_malloc(Doc* doc);
QS_EXPORT void qs_free(Doc* doc,void *s, const char* fname, int line) ;
QS_EXPORT void qs_all_free(Doc* doc, const char* fname, int line);
#endif
