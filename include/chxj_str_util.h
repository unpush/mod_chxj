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
#ifndef __CHXJ_STR_UTIL_H__
#define __CHXJ_STR_UTIL_H__

#include <string.h>
#include <apr_pools.h>

extern int chxj_chk_numeric(
  const char*          s);

extern int chxj_atoi(
  const char*          s);

extern int chxj_strcasenrcmp(apr_pool_t *p, const char *s1, const char *s2, int n);
extern int chxj_starts_with(const char *str, const char *word);
#endif
/*
 * vim:ts=2 et
 */
