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
#ifndef __CHXJ_SERF_H__
#define __CHXJ_SERF_H__

#include "chxj_apache.h"

#include "apr.h"
#include "apr_uri.h"
#include "apr_strings.h"
#include "apr_atomic.h"
#include "apr_base64.h"
#include "apr_getopt.h"
#include "apr_version.h"
#include "apr_pools.h"
/*
 * libserf CHXJ Interface.
 */
#include "serf.h"
#if APR_MAJOR_VERSION == 0
#define apr_atomic_inc32 apr_atomic_inc
#define apr_atomic_dec32 apr_atomic_dec
#define apr_atomic_read32 apr_atomic_read
#endif


extern char *default_chxj_serf_get(request_rec *r, apr_pool_t *ppool, const char *url_path, int set_headers_flag, apr_size_t *res_len);
extern char *(*chxj_serf_get)(request_rec *r, apr_pool_t *ppool, const char *url_path, int set_headers_flag, apr_size_t *res_len);

extern char *default_chxj_serf_post(request_rec *r, apr_pool_t *ppool, const char *url_path, char *post_data, apr_size_t post_data_len, int set_headers_flag, apr_size_t *res_len);
extern char *(*chxj_serf_post)(request_rec *r, apr_pool_t *ppool, const char *url_path, char *post_data, apr_size_t post_data_len, int set_headers_flag, apr_size_t *res_len);


#endif
/*
 * vim:ts=2 et
 */
