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
#include <apr.h>
#include <ap_config.h>
#include <apr_strings.h>
#include <httpd.h>

#include "mod_chxj.h"

#include "chxj_preg_replace.h"

static inline char *
s_init_pattern(apr_pool_t *p, const char *old)
{
  return apr_psprintf(p, "^(.*)(%s)(.*)$", old);
}


static inline ap_regex_t *
s_compile_regex(apr_pool_t *p, const char *pattern)
{
  char *new_pat;
  new_pat = s_init_pattern(p, pattern);
  return ap_pregcomp(p, new_pat, AP_REG_EXTENDED|AP_REG_ICASE);
}

ap_regex_t *
chxj_compile_for_preg_replace(apr_pool_t *p, const char *pattern)
{
  return s_compile_regex(p, pattern);
}


static inline char *
s_one_time_replace(apr_pool_t *p, ap_regex_t *regexp, const char *replacement, const char *str)
{
  ap_regmatch_t match[10];

  if (ap_regexec(regexp, str, regexp->re_nsub + 1, match, 0) == 0) {
    /* Match */
    char *one = ap_pregsub(p, "$1", str, regexp->re_nsub + 1, match);
    char *three = ap_pregsub(p, "$3", str, regexp->re_nsub + 1, match);
    if (strlen(replacement)) {
      return apr_pstrcat(p, one, replacement, three, NULL);
    }
    return apr_pstrcat(p, one, three, NULL);
  }
  /* Not Match */
  return NULL;
}


char *
chxj_preg_str_replace(apr_pool_t *p, const char *pattern, const char *replacement, const char *str)
{
  return chxj_preg_replace(p, s_compile_regex(p, pattern), replacement, str);
}


char *
chxj_preg_str_replace_all(apr_pool_t *p, const char *pattern, const char *replacement, const char *str)
{
  return chxj_preg_replace_all(p, s_compile_regex(p, pattern), replacement, str);
}


char *
chxj_preg_replace(apr_pool_t *p, ap_regex_t *regexp, const char *replacement, const char *str)
{
  char *result;

  result = s_one_time_replace(p, regexp, replacement, str);
  if (result == NULL)
    return apr_pstrdup(p, str);

  return result;
}


char *
chxj_preg_replace_all(apr_pool_t *p, ap_regex_t *regexp, const char *replacement, const char *str)
{
  char *result;
  char *pre_result;

  result = apr_pstrdup(p, str);
  for (;;) {
    pre_result = s_one_time_replace(p, regexp, replacement, result);
    if (pre_result == NULL)
      break;
    result = pre_result;
  }

  return result;
}


