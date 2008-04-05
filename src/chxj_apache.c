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
#include "chxj_apache.h"

const char *
chxj_apache_run_http_scheme(request_rec *r)
{
#if defined(CHXJ_TEST)
  return test_run_http_scheme(r);
#else
#  if AP_SERVER_MAJORVERSION_NUMBER == 2 && AP_SERVER_MINORVERSION_NUMBER == 2
  return ap_run_http_scheme(r);
#  else
  return ap_run_http_method(r);
#  endif
#endif
}

char *chxj_os_escape_path(apr_pool_t *p, const char *path, int partial) 
{
#if defined(CHXJ_TEST)
  return test_os_escape_path(p, path, partial);
#else
  return ap_os_escape_path(p, path, partial);
#endif
}


void
chxj_set_content_type(request_rec *r, const char *ct)
{
#if defined(CHXJ_TEST)
  test_set_content_type(r, ct);
#else
  ap_set_content_type(r, ct);
#endif
}


void *
chxj_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
#if defined(CHXJ_TEST)
  return test_get_module_config(cv,m);
#else
  return ap_get_module_config(cv,m);
#endif
}
/*
 * vim:ts=2 et
 */
