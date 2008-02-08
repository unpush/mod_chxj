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
#ifndef __CHXJ_PREG_REPLACE_H__
#define __CHXJ_PREG_REPLACE_H__

#include "mod_chxj.h"

extern ap_regex_t *chxj_compile_for_preg_replace(apr_pool_t *p, const char *pattern);
extern char *chxj_preg_str_replace(apr_pool_t *p, const char *pattern, const char *replacement, const char *str);
extern char *chxj_preg_str_replace_all(apr_pool_t *p, const char *pattern, const char *replacement, const char *str);
extern char *chxj_preg_replace(apr_pool_t *p, ap_regex_t *regexp, const char *replacement, const char *str);
extern char *chxj_preg_replace_all(apr_pool_t *p, ap_regex_t *regexp, const char *replacement, const char *str);

#endif
