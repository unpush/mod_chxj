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

/* for memcache */
#include <memcached.h>

#define MEMCACHE_MIN_CONNECTION (0)
#define MEMCACHE_SMAX_CONNECTION (1)
#define MEMCACHE_MAX_CONNECTION (1)
#define MEMCACHE_CONNECTION_TIMEOUT (60)
#define MEMCACHE_POLL_TIMEOUT       (60)
#define MEMCACHE_RETRY_TIMEOUT      (60)

#define MEMCACHE_LOCK_KEY "chxj::lock"
#define MEMCACHE_WAIT_MICRO_SECOND (5000)
#define MEMCACHE_LOCK_RETRY_COUNT (100)


#define MEMCACHE_MAX_SERVER (10)
#define MEMCACHE_FLAGS (0)

#define DEFAULT_MEMCACHE_TIMEOUT (1000)
#define DEFAULT_DELETE_TIMEOUT (0)

#define DEFAULT_COOKIE_DB_NAME "chxj_cookie"
#define DEFAULT_COOKIE_EXPIRE_DB_NAME "chxj_cookie_expire"

/* The underlying memcache system is thread safe. */
static memcached_st *memc = NULL;
static memcached_server_st *servers = NULL;

static apr_status_t
_memcache_cleanup(void *UNUSED(unused))
{
  if (servers) {
    memcached_server_list_free(servers);
    servers = NULL;
  }
  if (memc) {
    memcached_free(memc);
    memc = NULL;
  }
  return APR_SUCCESS;
}

int
chxj_memcache_init(request_rec *r, mod_chxj_config *m)
{
  DBG(r, "start chxj_memcache_init()");
  if (! memc) {
    if (!chxj_memcache_and_memcache_server_create(r, m)) {
      ERR(r, "%s:%d end chxj_memcache_init() failed: chxj_memcache_and_memcache_server_create()", APLOG_MARK);
      return CHXJ_FALSE;
    }
    apr_pool_cleanup_register(r->pool, NULL, _memcache_cleanup, _memcache_cleanup);
  }
  DBG(r, "end chxj_memcache_init()");
  return CHXJ_TRUE;
}

int
chxj_memcache_and_memcache_server_create(request_rec *r, mod_chxj_config *m)
{
  memcached_return rc;
  DBG(r, "start chxj_memcache_server_create()");
  
  memc = memcached_create(NULL);
  if (! memc) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): failed allocation of memcached_st.", APLOG_MARK);
    return CHXJ_FALSE;
  }
  servers = memcached_server_list_append(NULL, m->memcache.host, m->memcache.port, &rc);
  if (servers == NULL || rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): host:[%s] port:[%d]: %s", APLOG_MARK, m->memcache.host, m->memcache.port, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }
  rc = memcached_server_push(memc, servers);
  if (rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): host:[%s] port:[%d]: %s\n", APLOG_MARK, m->memcache.host, m->memcache.port, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }

  rc = memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_POLL_TIMEOUT, (uint64_t)MEMCACHE_POLL_TIMEOUT);
  if (rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): memcached_behavior_set(MEMCACHED_BEHAVIOR_POLL_TIMEOUT): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }
  rc = memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT, (uint64_t)MEMCACHE_CONNECTION_TIMEOUT);
  if (rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): memcached_behavior_set(MEMCACHED_BEHAVIOR_CONNECTION_TIMEOUT): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }
  rc = memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, (uint64_t)MEMCACHE_RETRY_TIMEOUT);
  if (rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_server_create(): memcached_behavior_set(MEMCACHED_BEHAVIOR_RETRY_TIMEOUT): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }
  
  DBG(r, "end chxj_memcache_server_create()");
  return CHXJ_TRUE;
}


int
chxj_memcache_set_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  memcached_return rc;
  time_t timeout = (time_t) ((m->cookie_timeout) ? m->cookie_timeout : DEFAULT_COOKIE_TIMEOUT);
  DBG(r, "start chxj_memcache_set_cookie()");

  rc= memcached_set(memc, (char *)cookie_id, strlen(cookie_id),
                    (char *)store_string, strlen(store_string),
                    (time_t)timeout, (uint32_t)0);
  if (rc != MEMCACHED_SUCCESS && rc != MEMCACHED_BUFFERED) {
    ERR(r, "%s:%d end chxj_memcache_set_cookie(): failed memcache_set(): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_set_cookie()");
  return CHXJ_TRUE;
}


int
chxj_memcache_reset_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  char *store_string;
  DBG(r, "start chxj_memcache_reset_cookie()");

  if (! (store_string = chxj_memcache_get_cookie(r, m, cookie_id))) {
    ERR(r, "%s:%d end chxj_memcache_reset_cookie() failed: chxj_memcache_get_cookie() cookie_id:[%s]", APLOG_MARK, cookie_id);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_set_cookie(r, m, cookie_id, store_string)) {
    ERR(r, "%s:%d end chxj_memcache_reset_cookie() failed: chxj_memcache_set_cookie() cookie_id:[%s]", APLOG_MARK, cookie_id);
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_reset_cookie()");
  return CHXJ_TRUE;
}


char *
chxj_memcache_get_cookie(request_rec *r, mod_chxj_config *UNUSED(m), const char *cookie_id)
{
  char *load_string;
  char *ret;
  size_t value_length;
  uint32_t flags;
  memcached_return rc;

  DBG(r, "start chxj_memcache_get_cookie()");

  load_string = memcached_get(memc, (char *)cookie_id, strlen(cookie_id), &value_length, &flags, &rc);
  if (rc != MEMCACHED_SUCCESS) {
    ERR(r, "%s:%d end chxj_memcache_get_cookie(): failed memcached_get(): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return NULL;
  }
  ret = apr_pstrdup(r->pool, load_string);
  free(load_string);
  
  DBG(r, "end chxj_memcache_get_cookie()");
  return ret;
}


int
chxj_memcache_delete_cookie(request_rec *r, mod_chxj_config *UNUSED(m),  const char *cookie_id)
{
  memcached_return rc;

  DBG(r, "start chxj_memcache_delete_cookie()");

  rc = memcached_delete(memc, (char *)cookie_id, strlen(cookie_id), (time_t)0);
  if (rc != MEMCACHED_SUCCESS && rc == MEMCACHED_BUFFERED) {
    ERR(r,"%s:%d end chxj_memcache_delete_cookie(): failed memcached_delete(): %s", APLOG_MARK, memcached_strerror(memc, rc));
    return CHXJ_FALSE;
  }

  DBG(r, "end chxj_memcache_delete_cookie()");
  return CHXJ_TRUE;
}



int
chxj_save_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  DBG(r, "start chxj_save_cookie_memcache() cookie_id:[%s]", cookie_id);

  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_save_cookie_memcache(): Cannot create memcache server: cookie_id:[%s] store_string:[%s]", APLOG_MARK, cookie_id, store_string);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_set_cookie(r, m, cookie_id, store_string)) {
    ERR(r, "%s:%d end chxj_save_cookie_memcache(): cannot store to memcache host:[%s] port:[%d] cookie_id:[%s] store_string:[%s]", 
        APLOG_MARK, m->memcache.host, m->memcache.port, cookie_id, store_string);
    return CHXJ_FALSE;
  }

  DBG(r, "stored DATA:[%s]", chxj_memcache_get_cookie(r, m, cookie_id));
  DBG(r, "end chxj_save_cookie_memcache() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_update_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  DBG(r, "start chxj_update_cookie_memcache() cookie_id:[%s]", cookie_id);
  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_update_cookie_memcache(): Cannot create memcache server: cookie_id:[%s] store_string:[%s]",APLOG_MARK,cookie_id, store_string);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_set_cookie(r, m, cookie_id, store_string)) {
    ERR(r, 
        "%s:%d end chxj_update_cookie_memcache(): cannot store to memcache host:[%s] port:[%d] cookie_id:[%s] store_string:[%s]", 
        APLOG_MARK, m->memcache.host, m->memcache.port, cookie_id, store_string);
    return CHXJ_FALSE;
  }
  DBG(r, "end chxj_update_cookie_memcache() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


char *
chxj_load_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  char *load_string;
  DBG(r, "start chxj_load_cookie_memcache() cookie_id:[%s]", cookie_id);

  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_load_cookie_memcache(): Cannot create memcache server: cookie_id:[%s]",APLOG_MARK, cookie_id);
    return NULL;
  }

  if (! (load_string = chxj_memcache_get_cookie(r, m, cookie_id))) {
    ERR(r, "%s:%d end chxj_load_cookie_memcache(): cannot store to memcache host:[%s] port:[%d] cookie_id:[%s]", APLOG_MARK, 
        m->memcache.host, m->memcache.port, cookie_id);
    return NULL;
  }
  DBG(r, "end   chxj_load_cookie_memcache() cookie_id:[%s]", cookie_id);
  return load_string;
}


int
chxj_delete_cookie_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  DBG(r, "start chxj_delete_cookie_memcache() cookie_id:[%s]", cookie_id);
  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_delete_cookie_memcache(): Cannot create memcache server: cookie_id:[%s]",APLOG_MARK, cookie_id);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_delete_cookie(r, m, cookie_id)) {
    ERR(r, "%s:%d end chxj_delete_cookie_memcache(): Cannot store to memcache host:[%s] port:[%d] cookie_id:[%s]", APLOG_MARK,
        m->memcache.host, m->memcache.port, cookie_id);
    return CHXJ_FALSE;
  }
  DBG(r, "end   chxj_delete_cookie_memcache() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_save_cookie_expire_memcache(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  DBG(r, "start chxj_save_cookie_expire_memcache() cookie_id:[%s]", cookie_id);
  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_save_cookie_expire_memcache(): Cannot create memcache server: cookie_id:[%s]",APLOG_MARK,cookie_id);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_reset_cookie(r, m, cookie_id)) {
    ERR(r, "%s:%d end chxj_save_cookie_expire_memcache(): Cannot store to memcache host:[%s] port:[%d] cookie_id:[%s]", APLOG_MARK, 
        m->memcache.host, m->memcache.port, cookie_id);
    return CHXJ_FALSE;
  }
  DBG(r, "end   chxj_save_cookie_expire_memcache() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_delete_cookie_expire_memcache(request_rec *r, mod_chxj_config *UNUSED(m), const char *cookie_id)
{
  DBG(r, "start chxj_delete_cookie_expire_memcache() cookie_id:[%s]", cookie_id);
  /* PASS */
  DBG(r, "end   chxj_delete_cookie_expire_memcache() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_cookie_expire_gc_memcache(request_rec *r, mod_chxj_config *UNUSED(m))
{
  DBG(r, "start chxj_cookie_expire_gc_memcache()");
  /* PASS */
  DBG(r, "end   chxj_cookie_expire_gc_memcache()");
  return CHXJ_TRUE;
}


int
chxj_cookie_lock_memcache(request_rec *r, mod_chxj_config *m)
{
  char baton[256];
  int retry_count = 0;
  apr_uint32_t timeout = (apr_uint32_t) ((m->cookie_timeout) ? m->cookie_timeout : DEFAULT_COOKIE_TIMEOUT);
  apr_interval_time_t wait_time = MEMCACHE_WAIT_MICRO_SECOND;

  DBG(r, "start chxj_cookie_lock_memcache()");

  if (! chxj_memcache_init(r, m)) {
    ERR(r, "%s:%d end chxj_cookie_lock_memcache(): Cannot create memcache server", APLOG_MARK);
    return CHXJ_FALSE;
  }

  apr_snprintf(baton, sizeof(baton)-1, "dummy");
  while(1) {
    memcached_return rc;
    rc = memcached_add(memc, MEMCACHE_LOCK_KEY, sizeof(MEMCACHE_LOCK_KEY)-1, baton, strlen(baton), (time_t)timeout, (uint32_t)0);
    if (rc != MEMCACHED_SUCCESS && rc != MEMCACHED_STORED && rc != MEMCACHED_NOTSTORED) {
      ERR(r, "%s:%d end chxj_cookie_lock_memcache(): failed memcached_add(): %s\n", APLOG_MARK, memcached_strerror(memc, rc));
      return CHXJ_FALSE;
    }
    if (rc == MEMCACHED_SUCCESS || rc == MEMCACHED_STORED) {
      /* got lock */
      DBG(r, "got lock");
      break;
    }
    retry_count++;
    if (retry_count >= MEMCACHE_LOCK_RETRY_COUNT) {
      ERR(r, "%s:%d end chxj_cookie_lock_memcache(): retry over.",APLOG_MARK);
      return CHXJ_FALSE;
    }
    apr_sleep(wait_time);
  }

  DBG(r, "end chxj_cookie_lock_memcache()");
  return CHXJ_TRUE;
}


int
chxj_cookie_unlock_memcache(request_rec *r, mod_chxj_config *m)
{
  DBG(r, "start chxj_cookie_unlock_memcache()");
  if (! memc) {
    ERR(r, "%s:%d end chxj_cookie_unlock_memcache(): runtime exception: programing failure.", APLOG_MARK);
    return CHXJ_FALSE;
  }

  if (! chxj_memcache_delete_cookie(r, m, MEMCACHE_LOCK_KEY)) {
    ERR(r, "%s:%d end chxj_cookie_unlock_memcache(): failed: chxj_memcache_delete_cookie() (lock data)", APLOG_MARK);
    return CHXJ_FALSE;
  }
  DBG(r, "end chxj_cookie_unlock_memcache()");
  return CHXJ_TRUE;
}


#endif
/*
 * vim:ts=2 et
 */
