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
#ifdef USE_MEMCACHE_COOKIE
#ifndef __CHXJ_MEMCACHE_H__
#define __CHXJ_MEMCACHE_H__

#include "mod_chxj.h"
#include "apr.h"
#include "apr_memcache.h"

#define DEFAULT_MEMCACHE_HOST "localhost"

typedef struct {
  char *host;
  apr_port_t port;
} memcache_t;


extern int chxj_memcache_init(request_rec *r, mod_chxj_config *m);
extern int chxj_memcache_and_memcache_server_create(request_rec *r, mod_chxj_config *m, apr_memcache_server_t **memcache_server, apr_memcache_t **memcache);
extern int chxj_memcache_set_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern char *chxj_memcache_get_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_memcache_delete_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_memcache_reset_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id);


/* operation for cookie */
extern int chxj_save_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern int chxj_update_cookie_memcache(request_rec *, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern char *chxj_load_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_save_cookie_expire_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_expire_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_cookie_expire_gc_memcache(request_rec *r, mod_chxj_config *m);

extern int chxj_cookie_lock_memcache(request_rec *r, mod_chxj_config *m);
extern int chxj_cookie_unlock_memcache(request_rec *r, mod_chxj_config *UNUSED(m));
#endif
#endif
