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
#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_apply_convrule.h"
#include "chxj_str_util.h"

#include "ap_release.h"

#include "apu.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"

#include <unistd.h>

/* for apr-memcache */
#include "apr_memcache.h"

#define MEMCACHE_MIN_CONNECTION (0)
#define MEMCACHE_SMAX_CONNECTION (1)
#define MEMCACHE_MAX_CONNECTION (1)
#define MEMCACHE_TTL_CONNECTION (60)


#define MEMCACHE_MAX_SERVER (10)
#define MEMCACHE_FLAGS (0)

#define DEFAULT_MEMCACHE_TIMEOUT (1000)
#define DEFAULT_DELETE_TIMEOUT (0)

#define DEFAULT_COOKIE_DB_NAME "chxj_cookie"
#define DEFAULT_COOKIE_EXPIRE_DB_NAME "chxj_cookie_expire"

/* The underlying apr_memcache system is thread safe. */
static apr_memcache_t* mc = NULL;

static apr_status_t
_memcache_cleanup(void *UNUSED(notused))
{
  mc = NULL;
  return APR_SUCCESS;
}

int
chxj_memcache_init(request_rec *r, mod_chxj_config *m)
{
  apr_memcache_server_t* st;
  DBG(r, "start chxj_memcache_init()");
  if (! mc) {
    if (!chxj_memcache_and_memcache_server_create(r, m, &st, &mc)) {
      ERR(r, "failed: chxj_memcache_and_memcache_server_create()");
      return CHXJ_FALSE;
    }
    apr_pool_cleanup_register(r->pool, (void *)NULL, _memcache_cleanup, _memcache_cleanup);
  }
  DBG(r, "end chxj_memcache_init()");
  return CHXJ_TRUE;
}

int
chxj_memcache_and_memcache_server_create(request_rec *r, mod_chxj_config *m, apr_memcache_server_t **memcache_server, apr_memcache_t **memcache)
{
  DBG(r, "start chxj_memcache_server_create()");
  if (apr_memcache_server_create(r->pool, 
                                 m->memcache.host,
                                 m->memcache.port,
                                 MEMCACHE_MIN_CONNECTION, 
                                 MEMCACHE_SMAX_CONNECTION,
                                 MEMCACHE_MAX_CONNECTION,
                                 MEMCACHE_TTL_CONNECTION,
                                 memcache_server) != APR_SUCCESS) {
    ERR(r, "failed apr_memcache_server_create() host:[%s] port:[%d]", m->memcache.host, m->memcache.port);
    return CHXJ_FALSE;
  }
  DBG(r, "done create_server");
  if (apr_memcache_create(r->pool, MEMCACHE_MAX_SERVER, MEMCACHE_FLAGS, memcache) != APR_SUCCESS) {
    ERR(r, "failed apr_memcache_create()");
    return CHXJ_FALSE;
  }
  DBG(r, "done create memcache");
  if (apr_memcache_add_server(*memcache, *memcache_server) != APR_SUCCESS) {
    ERR(r, "failed apr_memcache_add_server()");
    return CHXJ_FALSE;
  }
  DBG(r, "end chxj_memcache_server_create()");

  return CHXJ_TRUE;
}


int
chxj_memcache_set_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  apr_uint32_t timeout = (apr_uint32_t) ((m->cookie_timeout) ? m->cookie_timeout : DEFAULT_COOKIE_TIMEOUT);
  DBG(r, "start chxj_memcache_set_cookie()");

  if (apr_memcache_set(mc, cookie_id, (char *)store_string, strlen(store_string), timeout, 0) != APR_SUCCESS) {
    ERR(r, "failed: apr_memcache_set()");
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_set_cookie()");
  return CHXJ_TRUE;
}


int
chxj_memcache_reset_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  apr_uint32_t timeout = (apr_uint32_t) ((m->cookie_timeout) ? m->cookie_timeout : DEFAULT_COOKIE_TIMEOUT);
  char *store_string;
  DBG(r, "start chxj_memcache_reset_cookie()");


  if (! (store_string = chxj_memcache_get_cookie(r, m, cookie_id))) {
    ERR(r, "failed: chxj_memcache_get_cookie() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  if (apr_memcache_set(mc, cookie_id, (char *)store_string, strlen(store_string), timeout, 0) != APR_SUCCESS) {
    ERR(r, "failed: apr_memcache_set()");
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_reset_cookie()");
  return CHXJ_TRUE;
}


char *
chxj_memcache_get_cookie(request_rec *r, mod_chxj_config *UNUSED(m), const char *cookie_id)
{
  char *load_string;
  char *ret_value;
  apr_size_t len;
  apr_status_t ret;
  DBG(r, "start chxj_memcache_get_cookie()");

  if ((ret = apr_memcache_getp(mc, r->pool, cookie_id, &load_string, &len, 0)) != APR_SUCCESS) {
    ERR(r, "failed: apr_memcache_get() cookie_id:[%s] ret:[%d]", cookie_id, ret);
    return NULL;
  }
  
  ret_value = apr_palloc(r->pool, len+1);
  memset(ret_value, 0, len+1);
  memcpy(ret_value, load_string, len);

  DBG(r, "end chxj_memcache_get_cookie()");
  return ret_value;
}


int
chxj_memcache_delete_cookie(request_rec *r, mod_chxj_config *UNUSED(m),  const char *cookie_id)
{
  DBG(r, "start chxj_memcache_delete_cookie()");

  if (apr_memcache_delete(mc, cookie_id, 0) != APR_SUCCESS) {
    ERR(r, "failed: apr_memcache_delete() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_delete_cookie()");
  return CHXJ_TRUE;
}



#endif
/*
 * vim:ts=2 et
 */
