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
#include <time.h>

#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_apply_convrule.h"
#include "chxj_str_util.h"

#include "ap_release.h"

#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"
#include "apr_time.h"
#include "apr_date.h"

#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#if defined(USE_MEMCACHE_COOKIE)
#  include "chxj_memcache.h"
#endif
#include "chxj_dbm.h"


static char* s_get_hostname_from_url(request_rec* r, char* value);
static char* s_cut_until_end_hostname(request_rec*, char* value);
static int valid_domain(request_rec *r, const char *value);
static int valid_path(request_rec *r, const char *value);
static int valid_expires(request_rec *r, const char *value);
static int valid_secure(request_rec *r, const char *value);
static int check_valid_cookie_attribute(request_rec *r, const char *pair);


static char *
alloc_cookie_id(request_rec *r)
{
  char                *cookie_id;
  char                *uuid_string;
  unsigned char       *md5_value;
  apr_uuid_t          uuid;
  apr_status_t        retval;

  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);

  retval = apr_md5(md5_value, 
                   (const char*)uuid_string, 
                   APR_UUID_FORMATTED_LENGTH);
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    return NULL;
  }

  cookie_id = apr_palloc(r->pool, apr_base64_encode_len(APR_MD5_DIGESTSIZE)+1);
  memset(cookie_id, 0, APR_MD5_DIGESTSIZE+1);
  apr_base64_encode(cookie_id, (char*)md5_value, APR_MD5_DIGESTSIZE);

  DBG(r, "cookie_id=[%s]", cookie_id);

  cookie_id = chxj_url_encode(r,cookie_id);

  DBG(r, "cookie_id=[%s]", cookie_id);
  return cookie_id;
}


/*
 *
 */
cookie_t*
chxj_save_cookie(request_rec* r)
{
  int                 ii;
  apr_array_header_t  *headers;
  apr_table_entry_t   *hentryp;
  apr_array_header_t  *err_headers;
  apr_table_entry_t   *err_hentryp;
  char                *old_cookie_id;
  char                *store_string;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;
  apr_table_t*        new_cookie_table;
  int                 has_cookie = 0;
  cookie_t*           cookie;
  cookie_t*           old_cookie;
  char*               refer_string;
  apr_uri_t           parsed_uri;
  int                 has_refer;
  apr_pool_t          *pool;


  DBG(r, "start chxj_save_cookie()");

  apr_pool_create(&pool, r->pool);
  cookie = (cookie_t*)apr_palloc(pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;

  has_cookie = 0;
  has_refer = 0;

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp) {
    DBG(r, "end chxj_save_cookie() no pattern");
    return NULL;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT)) {
    DBG(r, "end chxj_save_cookie() CookieOff");
    return NULL;
  }

  headers = (apr_array_header_t*)apr_table_elts(r->headers_out);
  hentryp = (apr_table_entry_t*)headers->elts;
  err_headers = (apr_array_header_t*)apr_table_elts(r->err_headers_out);
  err_hentryp = (apr_table_entry_t*)err_headers->elts;

  new_cookie_table = apr_table_make(pool, 0);

  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "=====================================");
      DBG(r, "cookie=[%s:%s]", hentryp[ii].key, hentryp[ii].val);

      char* key;
      char* val;
      char* buff;


      buff = apr_pstrdup(pool, hentryp[ii].val);
      val = strchr(buff, '=');
      if (val) {
        key = buff;
        *val++ = 0;
        apr_table_add(new_cookie_table, apr_pstrdup(pool, key), apr_pstrdup(pool, val));
        if (strcasecmp(REFERER_COOKIE_KEY, key) == 0) has_refer++;

      }

      has_cookie = 1;
      DBG(r, "=====================================");
    }
  }
  for (ii=0; ii<err_headers->nelts; ii++) {
    if (strcasecmp(err_hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "=====================================");
      DBG(r, "cookie=[%s:%s]", err_hentryp[ii].key, err_hentryp[ii].val);

      char* key;
      char* val;
      char* buff;


      buff = apr_pstrdup(pool, err_hentryp[ii].val);
      val = strchr(buff, '=');
      if (val) {
        key = buff;
        *val++ = 0;
        apr_table_add(new_cookie_table, apr_pstrdup(pool, key), apr_pstrdup(pool, val));
        if (strcasecmp(REFERER_COOKIE_KEY, key) == 0) has_refer++;

      }

      has_cookie = 1;
      DBG(r, "=====================================");
    }
  }
  apr_table_unset(r->headers_out, "Set-Cookie");
  apr_table_unset(r->err_headers_out, "Set-Cookie");


  if (! has_refer) {
    apr_uri_parse(pool,r->uri, &parsed_uri);
    refer_string = apr_psprintf(pool, 
                                "%s://%s%s", 
                                chxj_run_http_scheme(r),
                                r->hostname,
                                apr_uri_unparse(pool,
                                                &parsed_uri,
                                                APR_URI_UNP_OMITSITEPART));
    if (r->args && strlen(r->args)) {
      refer_string = apr_pstrcat(pool, refer_string, "?", r->args, NULL);
    }
    apr_table_setn(new_cookie_table, REFERER_COOKIE_KEY, apr_pstrdup(pool, refer_string));
    DBG(r, "ADD REFER[%s]", refer_string);
    has_cookie++;
  }


  /*
   * check input parameters
   */
  old_cookie_id = (char*)apr_table_get(r->headers_in, "CHXJ_COOKIE_ID");
  if (old_cookie_id) {
    old_cookie = chxj_load_cookie(r, old_cookie_id); 
    if (old_cookie && old_cookie->cookie_headers) {
      hentryp = (apr_table_entry_t*)old_cookie->cookie_headers->elts;
      for (ii=0; ii<old_cookie->cookie_headers->nelts; ii++) {
        if (hentryp && apr_table_get(new_cookie_table, hentryp[ii].key) == NULL) {
          apr_table_add(new_cookie_table, apr_pstrdup(pool, hentryp[ii].key), apr_pstrdup(pool, hentryp[ii].val));
          has_cookie = 1;
        }
      }
      chxj_delete_cookie(r,        old_cookie_id);
      chxj_delete_cookie_expire(r, old_cookie_id);
    }
  }

  if (! has_cookie) {
    DBG(r, "no cookie");
    DBG(r, "end chxj_save_cookie()");
    return NULL;
  }

  /*
   * create val
   */
  cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(new_cookie_table);
  hentryp = (apr_table_entry_t*)cookie->cookie_headers->elts;

  {
    apr_pool_t *ppp;
    apr_pool_create(&ppp, r->pool);
    apr_size_t store_string_len = 0;
    apr_size_t npos = 0;
    for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
      if (ii) store_string_len++;
      store_string_len += (strlen(hentryp[ii].key) + strlen(hentryp[ii].val) + 1);
    }
    store_string = apr_palloc(ppp, store_string_len + 1);
    memset(store_string, 0, store_string_len + 1);
    
    for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
      if (ii) store_string[npos++] = '\n';
      memcpy(&store_string[npos], hentryp[ii].key, strlen(hentryp[ii].key));
      npos += strlen(hentryp[ii].key); 
      store_string[npos++] = '=';
      memcpy(&store_string[npos], hentryp[ii].val, strlen(hentryp[ii].val));
      npos += strlen(hentryp[ii].val); 
    }
  }

  if (old_cookie_id && IS_COOKIE_LAZY(dconf)) {
    DBG(r, "LAZY COOKIE save");
    cookie->cookie_id = apr_pstrdup(r->pool, old_cookie_id);
  }
  else {
    DBG(r, "NO LAZY COOKIE save");
    cookie->cookie_id = alloc_cookie_id(r);
  }

DBG(r, "TYPE:[%d]", dconf->cookie_store_type);
  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (! chxj_save_cookie_mysql(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "faild: chxj_save_cookie_mysql() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif
#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! chxj_save_cookie_memcache(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "failed: chxj_save_cookie_memcache() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif
    if (IS_COOKIE_STORE_DBM(dconf->cookie_store_type) || ! done_proc) {
      if (! chxj_save_cookie_dbm(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "failed: chxj_save_cookie_dbm() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
    }
  }


  chxj_save_cookie_expire(r, cookie);



on_error:
  DBG(r, "end chxj_save_cookie()");
  return cookie;
}

/*
 *
 */
cookie_t*
chxj_update_cookie(request_rec* r, cookie_t* old_cookie)
{
  int                 ii;
  apr_array_header_t* headers;
  apr_table_entry_t*  hentryp;
  char*               store_string;
  mod_chxj_config*    dconf;
  chxjconvrule_entry* entryp;
  cookie_t*           cookie;


  DBG(r, "start chxj_update_cookie()");
  if (!old_cookie || ! old_cookie->cookie_headers || ! old_cookie->cookie_id) {
    DBG(r, "end chxj_update_cookie() (old_cookie is null)");
    return  NULL;
  }

  cookie = (cookie_t*)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;


  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp) {
    DBG(r, "end chxj_update_cookie() no pattern");
    return NULL;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT)) {
    DBG(r, "end chxj_update_cookie() CookieOff");
    return NULL;
  }


  headers = (apr_array_header_t*)apr_table_elts(r->headers_out);
  hentryp = (apr_table_entry_t*)headers->elts;


  chxj_delete_cookie(r,        old_cookie->cookie_id);
  chxj_delete_cookie_expire(r, old_cookie->cookie_id);

  if (IS_COOKIE_LAZY(dconf)) {
    DBG(r, "LAZY MODE");
    cookie->cookie_id = apr_pstrdup(r->pool, old_cookie->cookie_id);
  }
  else {
    DBG(r, "NO LAZY MODE");
    cookie->cookie_id = alloc_cookie_id(r);
  }

  cookie->cookie_headers = old_cookie->cookie_headers;
  store_string = apr_palloc(r->pool, 1);
  store_string[0] = 0;
  hentryp = (apr_table_entry_t*)cookie->cookie_headers->elts;

  for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
    if (ii) store_string = apr_pstrcat(r->pool,
                               store_string, 
                               "\n",
                               NULL);

    DBG(r, "OLD COOKIE VALUE=[%s][%s]", hentryp[ii].key, hentryp[ii].val);
    store_string = apr_pstrcat(r->pool, 
                               store_string, 
                               hentryp[ii].key, 
                               "=",
                               hentryp[ii].val, 
                               NULL);
  }

  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (!chxj_update_cookie_mysql(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "failed: chxj_update_cookie_mysql() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif

#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! chxj_update_cookie_memcache(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "failed: chxj_update_cookie_memcache() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
      done_proc = 1;
    }
#endif
    if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
      if (! chxj_update_cookie_dbm(r, dconf, cookie->cookie_id, store_string)) {
        ERR(r, "failed: chxj_update_cookie_dbm() cookie_id:[%s]", cookie->cookie_id);
        goto on_error;
      }
    }
  }

  chxj_save_cookie_expire(r, cookie);

  apr_table_setn(r->headers_in, "CHXJ_COOKIE_ID", cookie->cookie_id);


on_error:
  DBG(r, "end   chxj_update_cookie()");
  return cookie;
}


/*
 *
 * @return loaded data.
 */
cookie_t*
chxj_load_cookie(request_rec* r, char* cookie_id)
{
  mod_chxj_config         *dconf;
  chxjconvrule_entry      *entryp;
  cookie_t                *cookie;
  apr_table_t             *load_cookie_table;
  char                    *load_string = NULL;
  char                    *pstat;
  char                    *key;
  char                    *val;
  char                    *pair;
  char                    *header_cookie;
  apr_pool_t              *pool;

  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "start chxj_load_cookie() cookie_id=[%s]", cookie_id);
  chxj_cookie_expire_gc(r);

  apr_pool_create(&pool, r->pool);
  cookie = (cookie_t*)apr_palloc(pool, sizeof(cookie_t));
  cookie->cookie_headers = NULL;
  cookie->cookie_id = apr_pstrdup(pool, cookie_id);

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp) {
    DBG(r, "end chxj_load_cookie() no pattern");
    goto on_error0;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT)) {
    DBG(r, "end chxj_load_cookie() CookieOff");
    goto on_error0;
  }
  load_cookie_table = apr_table_make(pool, 1);

  {
    int done_proc = 0;
#if defined(USE_MYSQL_COOKIE)
    if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_mysql(r, dconf, cookie->cookie_id))) {
        ERR(r, "failed: chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
        goto on_error0;
      }
      done_proc = 1;
    }
#endif
#if defined(USE_MEMCACHE_COOKIE)
    if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_memcache(r, dconf, cookie->cookie_id))) {
        ERR(r, "failed: chxj_load_cookie_memcache() cookie_id:[%s]", cookie_id);
        goto on_error0;
      }
      done_proc = 1;
    }
#endif
    if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
      if (! (load_string = chxj_load_cookie_dbm(r, dconf, cookie->cookie_id))) {
        ERR(r, "failed: chxj_load_cookie_dbm() cookie_id:[%s]", cookie_id);
        goto on_error0;
      }
    }
  }

  if (load_string) {
    DBG(r, "load_string=[%s]", load_string);
    header_cookie = apr_palloc(pool, 1);
    header_cookie[0] = 0;
    for (;;) {
      char* tmp_sem;
      pair = apr_strtok(load_string, "\n", &pstat);  
      load_string = NULL;
      if (!pair) break;

      DBG(r, "Cookie:[%s]", pair);
      char* tmp_pair;

      tmp_pair = apr_pstrdup(pool, pair);
      val = strchr(tmp_pair, '=');
      if (val) {
        key = tmp_pair;
        *val++ = 0;
        apr_table_add(load_cookie_table, apr_pstrdup(pool, key), apr_pstrdup(pool, val));
        DBG(r, "ADD key:[%s] val:[%s]", key, val);
      }
      tmp_pair = apr_pstrdup(pool, pair);
      tmp_sem = strchr(tmp_pair, ';'); 
      if (tmp_sem)
        *tmp_sem = '\0';

      if (check_valid_cookie_attribute(r, pair)) {
        if (strlen(header_cookie)) 
          header_cookie = apr_pstrcat(pool, header_cookie, ";", NULL);
  
        header_cookie = apr_pstrcat(pool, header_cookie, tmp_pair, NULL);
      }
    }
    if (strlen(header_cookie)) {
      DBG(r, "ADD COOKIE to REQUEST HEADER:[%s]", header_cookie);
      apr_table_add(r->headers_in, apr_pstrdup(pool, "Cookie"), apr_pstrdup(pool, header_cookie));
    }
  
    cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(load_cookie_table);

    if (apr_table_get(r->headers_in, "referer") == NULL) {
      apr_table_setn(r->headers_in, 
                     apr_pstrdup(pool, "referer"),
                     apr_pstrdup(pool, apr_table_get(load_cookie_table, REFERER_COOKIE_KEY)));
    }
  
    /*
     * save cookie_id to request header.
     */
    apr_table_setn(r->headers_in, apr_pstrdup(pool, "CHXJ_COOKIE_ID"), apr_pstrdup(pool, cookie->cookie_id));
  }

  DBG(r, "end   chxj_load_cookie()");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");

  return cookie;


on_error0:

  DBG(r, "end   chxj_load_cookie()");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  return NULL;
}

static int
check_valid_cookie_attribute(request_rec *r, const char *value)
{
  char *pstat;
  char *pair;
  char *first_pair;
  char *domain_pair;
  char *path_pair;
  char *expire_pair;
  char *secure_pair;
  char *p;

  DBG(r, "start check_valid_cookie_attribute() value:[%s]", value);

  domain_pair = path_pair = expire_pair = secure_pair = NULL;
  p = apr_pstrdup(r->pool, value);

  /* pass first pair */
  first_pair = apr_strtok(p, ";", &pstat);  

  for (;;) {
    pair = apr_strtok(NULL, ";", &pstat);
    if (! pair) break;
    pair = qs_trim_string(r->pool, pair);
    if (STRNCASEEQ('d','D',"domain", pair, sizeof("domain")-1)) {
      domain_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('p','P',"path", pair, sizeof("path")-1)) {
      path_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('e','E',"expires", pair, sizeof("expires")-1)) {
      expire_pair = apr_pstrdup(r->pool, pair);
    }
    else if (STRNCASEEQ('s','S',"secure", pair, sizeof("secure")-1)) {
      secure_pair = apr_pstrdup(r->pool, pair);
    }
  }

  if (domain_pair) {
    if (!valid_domain(r, domain_pair)) {
      DBG(r, "invalid domain. domain_pair:[%s]", domain_pair);
      return CHXJ_FALSE;
    }
  }
  if (path_pair) {
    if (!valid_path(r, path_pair)) {
      DBG(r, "invalid path. path_pair:[%s]", path_pair);
      return CHXJ_FALSE;
    }
  }
  if (expire_pair) {
    if (!valid_expires(r, expire_pair)) {
      DBG(r, "invalid expire. expire_pair:[%s]", expire_pair);
      return CHXJ_FALSE;
    }
  }
  if (secure_pair) {
    if (!valid_secure(r, secure_pair)) {
      DBG(r, "invalid secure. secure_pair:[%s]", secure_pair);
      return CHXJ_FALSE;
    }
  }
  DBG(r, "end check_valid_cookie_attribute() value:[%s]", value);
  return CHXJ_TRUE;
}

static int
valid_domain(request_rec *r, const char *value)
{
  int len;
  char *name;
  char *val;
  char *pstat;
  char *p = apr_pstrdup(r->pool, value);
  const char *host = apr_table_get(r->headers_in, HTTP_HOST);

  DBG(r, "start valid_domain() value:[%s]", value);
  DBG(r, "host:[%s]", host);
  if (!host)
    return CHXJ_TRUE;

  name = apr_strtok(p,"=", &pstat);
  name = qs_trim_string(r->pool, name);
  val = apr_strtok(NULL, "=", &pstat);
  val = qs_trim_string(r->pool, val);
  len = strlen(host);
  if (len) {
    if (chxj_strcasenrcmp(r->pool, host, val, strlen(val))) {
      DBG(r, "not match domain. host domain:[%s] vs value:[%s]", host, val);
      return CHXJ_FALSE;
    }
  }
  DBG(r, "end valid_domain() value:[%s]", value);
  return CHXJ_TRUE;
}

static int
valid_path(request_rec *r, const char *value)
{
  char *p = apr_pstrdup(r->pool, value);
  char *uri;
  char *tmp;
  char *name;
  char *val;
  char *pstat;

  DBG(r, "start valid_path() unparsed_uri:[%s] value:[%s]", r->unparsed_uri, value);
  if (chxj_starts_with(r->unparsed_uri, "http://")) {
    uri = strchr(&r->unparsed_uri[sizeof("http://")], '/');
    if (uri != NULL) {
      uri = apr_pstrdup(r->pool, uri);
    }
  }
  else if (chxj_starts_with(r->unparsed_uri, "https://")) {
    uri = strchr(&r->unparsed_uri[sizeof("https://")], '/');
    if (uri != NULL) {
      uri = apr_pstrdup(r->pool, uri);
    }
  }
  else if (chxj_starts_with(r->unparsed_uri, "/")) {
    uri = apr_pstrdup(r->pool, r->unparsed_uri);
  }
  else {
    uri = apr_pstrdup(r->pool, "/");
  }
  
  if ((tmp = strchr(uri, '?'))) {
    *tmp = '\0';
  }
  DBG(r, "uri=[%s]", uri);
  name = apr_strtok(p, "=", &pstat);
  val = apr_strtok(NULL, "=", &pstat);
  name = qs_trim_string(r->pool, name);
  val = qs_trim_string(r->pool, val);
  DBG(r, "name=[%s] val=[%s]", name, val);
  
  DBG(r, "val:[%s] vs uri:[%s]", val, uri);
  if (! chxj_starts_with(uri, val)) {
    DBG(r, "end valid_path() unparsed_uri:[%s] value:[%s] (false)", r->unparsed_uri, value);
    return CHXJ_FALSE;
  }
  DBG(r, "end valid_path() unparsed_uri:[%s] value:[%s] (true)", r->unparsed_uri, value);
  return CHXJ_TRUE;
}

static int
valid_expires(request_rec *r, const char *value)
{
  char *name;
  char *val;
  char *p = apr_pstrdup(r->pool, value);
  char *pstat;
  apr_time_t expires;
  apr_time_t now;
  DBG(r, "start valid_expire() value:[%s]", value);

  name = apr_strtok(p, "=", &pstat);
  val  = apr_strtok(NULL, "=", &pstat);
  DBG(r, "name=[%s] val=[%s]", name, val);
  now = apr_time_now();
  expires = chxj_parse_cookie_expires(val);
  if (expires < now) {
    DBG(r, "end valid_expire() value:[%s] (expired)", value);
    return CHXJ_FALSE;
  }
  
  DBG(r, "end valid_expire() value:[%s] (non expired)", value);
  return CHXJ_TRUE;
}

static int
valid_secure(request_rec *r, const char *value)
{
  const char *scheme;
  DBG(r, "start valid_secure() value:[%s]", value);
  scheme = chxj_apache_run_http_scheme(r);
  if (strcasecmp("https", scheme)) {
    DBG(r, "end valid_secure() value:[%s] (non secure)", value);
    return CHXJ_FALSE;
  }
  DBG(r, "end valid_secure() value:[%s] (secure)", value);
  return CHXJ_TRUE;
}


char *
chxj_add_cookie_parameter(request_rec *r, char *value, cookie_t *cookie)
{
  char *qs;
  char *dst;
  char *name = "";

  DBG(r, "start chxj_add_cookie_parameter() cookie_id=[%s]", (cookie) ? cookie->cookie_id : NULL);

  dst = apr_pstrdup(r->pool, value);

  if (!cookie)
    goto on_error;

  if (!cookie->cookie_id)
    goto on_error;

  if (chxj_cookie_check_host(r, value) != 0) {
    DBG(r, "end chxj_add_cookie_parameter()(check host)");
    goto on_error;
  }

  qs = strchr(dst, '#');
  if (qs) {
    name = apr_pstrdup(r->pool, qs);
    *qs = 0;
  }

  qs = strchr(dst, '?');
  if (qs) {
    dst = apr_psprintf(r->pool, "%s&%s=%s%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id, name);
  }
  else {
    dst = apr_psprintf(r->pool, "%s?%s=%s%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id, name);
  }

  DBG(r, "end   chxj_add_cookie_parameter() dst=[%s]", dst);

  return dst;

on_error:
  DBG(r, "end   chxj_add_cookie_parameter() (on_error)");
  return dst;
}


int
chxj_cookie_check_host(request_rec* r, char* value) 
{
  char* hostnm;

  DBG(r, "hostname=[%s]", r->hostname);

  hostnm = s_get_hostname_from_url(r, value);
  if (hostnm) {
    if (strcasecmp(hostnm, r->hostname) == 0)
      return 0;
    else
      return 1;
  }
  return 0;
}


static char*
s_get_hostname_from_url(request_rec* r, char* value)
{
  if (!value) 
    return NULL; 

  if (strncasecmp(value, "http://",  7) == 0 )
    return s_cut_until_end_hostname(r, &value[7]);

  if (strncasecmp(value, "https://", 8) == 0)
    return s_cut_until_end_hostname(r, &value[8]);

  return NULL;
}


static char* 
s_cut_until_end_hostname(request_rec* r, char* value)
{
  char* sp;
  char* hostnm;

  hostnm = sp = apr_pstrdup(r->pool, value);
  for (;*sp; sp++) {
    if (*sp == '/'|| *sp == '?') {
      *sp = '\0';
      break;
    }
  }
  return hostnm;
}



void
chxj_delete_cookie(request_rec *r, const char *cookie_id)
{
  int done_proc = 0;
  mod_chxj_config *dconf;

  DBG(r, "start chxj_delete_cookie()");
  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_mysql(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_mysql() cookie_id:[%s]", cookie_id);
      DBG(r, "end   chxj_delete_cookie()");
      return;
    }
    done_proc = 1;
  }
#endif

#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_memcache(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_memcache() cookie_id:[%s]", cookie_id);
      DBG(r, "end   chxj_delete_cookie()");
      return;
    }
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_dbm(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_dbm() cookie_id:[%s]", cookie_id);
      DBG(r, "end   chxj_delete_cookie()");
      return;
    }
  }

  DBG(r, "end   chxj_delete_cookie()");
}


/*
 *
 */
void
chxj_save_cookie_expire(request_rec* r, cookie_t* cookie)
{
  int done_proc = 0;
  mod_chxj_config         *dconf;

  DBG(r, "start chxj_save_cookie_expire()");
  if (!cookie) {
    DBG(r, "cookie is NULL");
    return;
  }
  if (!cookie->cookie_id) {
    DBG(r, "cookie->cookie_id is NULL");
    return;
  }

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (!dconf) {
    DBG(r, "dconf is NULL");
    return;
  }

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_mysql(r, dconf, cookie->cookie_id)) {
      ERR(r, "failed: chxj_save_cookie_expire_mysql() cookie_id:[%s]", cookie->cookie_id);
      DBG(r, "end   chxj_save_cookie_expire()");
      return;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_memcache(r, dconf, cookie->cookie_id)) {
      ERR(r, "failed: chxj_save_cookie_expire_memcache() cookie_id:[%s]", cookie->cookie_id);
      DBG(r, "end   chxj_save_cookie_expire()");
      return;
    }
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (! chxj_save_cookie_expire_dbm(r, dconf, cookie->cookie_id)) {
      ERR(r, "failed: chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie->cookie_id);
      DBG(r, "end   chxj_save_cookie_expire()");
      return;
    }
  }

  DBG(r, "end   chxj_save_cookie_expire()");
}





void
chxj_delete_cookie_expire(request_rec* r, char* cookie_id)
{
  int done_proc = 0;
  mod_chxj_config*  dconf;

  DBG(r, "start chxj_delete_cookie_expire()");

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);

#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_delete_cookie_expire_mysql(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_expire_mysql() cookie_id:[%s]", cookie_id);
      return;
    }
    done_proc = 1;
  }
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (!chxj_delete_cookie_expire_memcache(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_expire_memcache() cookie_id:[%s]", cookie_id);
      return;
    } 
    done_proc = 1;
  }
#endif
  if (!done_proc || IS_COOKIE_STORE_DBM(dconf->cookie_store_type)) {
    if (!chxj_delete_cookie_expire_dbm(r, dconf, cookie_id)) {
      ERR(r, "failed: chxj_delete_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
      return;
    } 
  }

  DBG(r, "end   chxj_delete_cookie_expire()");

  return;
}


void
chxj_cookie_expire_gc(request_rec* r)
{
  mod_chxj_config   *dconf;
  int done_proc = 0;

  DBG(r, "start chxj_cookie_expire_gc()");

  dconf = chxj_get_module_config(r->per_dir_config, &chxj_module);
#if defined(USE_MYSQL_COOKIE)
  if (IS_COOKIE_STORE_MYSQL(dconf->cookie_store_type)) {
    if (! chxj_cookie_expire_gc_mysql(r, dconf)) {
      ERR(r, "failed: chxj_cookie_expire_gc_mysql()");
      return;
    }
    done_proc = 1;
  } 
#endif
#if defined(USE_MEMCACHE_COOKIE)
  if (IS_COOKIE_STORE_MEMCACHE(dconf->cookie_store_type)) {
    if (! chxj_cookie_expire_gc_memcache(r, dconf)) {
      ERR(r, "failed: chxj_cookie_expire_gc_memcache()");
      return;
    }
    done_proc = 1;
  } 
#endif
  if (!done_proc) {
    if (! chxj_cookie_expire_gc_dbm(r, dconf)) {
      ERR(r, "failed: chxj_cookie_expire_gc_dbm()");
      return;
    }
  }
  DBG(r, "end   chxj_cookie_expire_gc()");
}

apr_time_t
chxj_parse_cookie_expires(const char *s)
{
  if (!s) return (apr_time_t)0;
  return apr_date_parse_rfc(s);
}
/*
 * vim:ts=2 et
 */
