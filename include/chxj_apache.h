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
#ifndef __CHXJ_APACHE_H__
#define __CHXJ_APACHE_H__


#if defined(CHXJ_TEST)
#  define chxj_log_rerror test_log_rerror
#  define chxj_log_error test_log_error
#  define chxj_run_http_scheme test_run_http_scheme
#  include "apr.h"
#  include "apr_pools.h"
#  include "apr_tables.h"
#  define APLOG_EMERG 0   /* system is unusable */
#  define APLOG_ALERT 1   /* action must be taken immediately */
#  define APLOG_CRIT  2   /* critical conditions */
#  define APLOG_ERR   3   /* error conditions */
#  define APLOG_WARNING   4   /* warning conditions */
#  define APLOG_NOTICE    5   /* normal but significant condition */
#  define APLOG_INFO  6   /* informational */
#  define APLOG_DEBUG 7   /* debug-level messages */
#  define APLOG_LEVELMASK 7   /* mask off the level value */
#  ifndef __CONFIG_H__
#    define __CONFIG_H__
#    undef PACKAGE_NAME
#    undef PACKAGE_STRING
#    undef PACKAGE_TARNAME
#    undef PACKAGE_VERSION
#    include "test_config.h"
#  endif
#  define AP_MODULE_DECLARE_DATA
#  define APLOG_MARK   __FILE__,__LINE__
typedef struct module_struct module;
struct module_struct {
    int version;
    int minor_version;
    int module_index;
    const char *name;
    void *dynamic_load_handle;
    struct module_struct *next;
    unsigned long magic;
    void (*rewrite_args) (void *process);
    void *(*create_dir_config) (apr_pool_t *p, char *dir);
    void *(*merge_dir_config) (apr_pool_t *p, void *base_conf, void *new_conf);
    void *(*create_server_config) (apr_pool_t *p, void *s);
    void *(*merge_server_config) (apr_pool_t *p, void *base_conf, void *new_conf);
    const void *cmds;
    void (*register_hooks) (apr_pool_t *p);
};
typedef struct ap_conf_vector_t ap_conf_vector_t;
typedef struct test_request_rec {
  apr_pool_t *pool;
  ap_conf_vector_t *per_dir_config;
  apr_table_t *headers_in;
  apr_table_t *headers_out;
  char *uri;
  char *args;
  char *hostname;
  char *unparsed_uri;
} request_rec;


extern void test_log_error(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...);
extern void test_log_rerror(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...);
extern const char *test_run_http_scheme(request_rec *r);
extern char *test_os_escape_path(apr_pool_t *p, const char *path, int partial);
extern void test_set_content_type(request_rec *r, const char *ct);
extern void *test_get_module_config(const ap_conf_vector_t *cv, const module *m);
extern char *test_ap_escape_html(apr_pool_t *p, const char *s);

#else
#  include "httpd.h"
#  include "http_config.h"
#  include "http_protocol.h"
#  include "http_request.h"
#  include "http_log.h"
#  include "ap_config.h"
#  include "util_filter.h"
#  ifndef __CONFIG_H__
#    define __CONFIG_H__

#    undef PACKAGE_NAME
#    undef PACKAGE_STRING
#    undef PACKAGE_TARNAME
#    undef PACKAGE_VERSION
#    include "config.h"
#  endif
#  define chxj_log_rerror ap_log_rerror
#  define chxj_log_error ap_log_error
#  define chxj_run_http_scheme chxj_apache_run_http_scheme
#endif

#define chxj_escape_uri(p,path) chxj_os_escape_path(p,path,1)

extern const char *chxj_apache_run_http_scheme(request_rec *r);
extern char * chxj_os_escape_path(apr_pool_t *p, const char *path, int partial);
extern void chxj_set_content_type(request_rec *r, const char *ct);
extern void * chxj_get_module_config(const ap_conf_vector_t *cv, const module *m);
extern char *chxj_ap_escape_html(apr_pool_t *p, const char *s);


#endif
/*
 * vim:ts=2 et
 */
