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

#include "ap_release.h"

#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"

static char *s_get_hostname_from_url(request_rec *r, char *value);
static char *s_cut_until_end_hostname(request_rec *, char *value);

/*
 *
 */
cookie_t *
chxj_save_cookie(request_rec *r)
{
  int                 ii;
  apr_array_header_t *headers;
  apr_table_entry_t   *hentryp;
  apr_status_t        retval;
  apr_datum_t         dbmkey;
  apr_datum_t         dbmval;
  apr_dbm_t           *f;
  apr_uuid_t          uuid;
  char                *uuid_string;
  unsigned char       *md5_value;
  char                *old_cookie_id;
  char                *store_string;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;
  apr_file_t          *file;
  apr_table_t         *new_cookie_table;
  int                 has_cookie = 0;
  cookie_t            *cookie;
  cookie_t            *old_cookie;
  char                *refer_string;
  apr_uri_t           parsed_uri;
  int                 has_refer;


  DBG(r, "start chxj_save_cookie()");

  cookie = (cookie_t*)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;

  has_cookie = 0;
  has_refer = 0;

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
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


  new_cookie_table = apr_table_make(r->pool, 0);

  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "=====================================");
      DBG(r, "cookie=[%s:%s]", hentryp[ii].key, hentryp[ii].val);

      char *key;
      char *val;
      char *buff;


      buff = apr_pstrdup(r->pool, hentryp[ii].val);
      val = strchr(buff, '=');
      if (val) {
        key = buff;
        *val++ = 0;
        apr_table_add(new_cookie_table, key, val);
        if (strcasecmp(REFERER_COOKIE_KEY, key) == 0) has_refer++;
        
      }

      has_cookie = 1;
      DBG(r, "=====================================");
    }
  }
  apr_table_unset(r->headers_out, "Set-Cookie");

  if (! has_refer) {
    apr_uri_parse(r->pool,r->uri, &parsed_uri);
    refer_string = apr_psprintf(r->pool, 
                                "%s://%s%s", 
#if AP_SERVER_MAJORVERSION_NUMBER == 2 && AP_SERVER_MINORVERSION_NUMBER == 2
                                ap_run_http_scheme(r),
#else
                                ap_run_http_method(r),
#endif
                                r->hostname,
                                apr_uri_unparse(r->pool,
                                                &parsed_uri,
                                                APR_URI_UNP_OMITSITEPART));
    if (r->args && strlen(r->args)) {
      refer_string = apr_pstrcat(r->pool, refer_string, "?", r->args, NULL);
    }
    apr_table_setn(new_cookie_table, REFERER_COOKIE_KEY, refer_string);
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
          apr_table_add(new_cookie_table, hentryp[ii].key, hentryp[ii].val);
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

  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end chxj_save_cookie()");
    return NULL;
  }

  DBG(r, " ");

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           chxj_cookie_db_name_create(r, dconf->cookie_db_dir), 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    DBG(r, "end chxj_save_cookie()");
    ERR(r, "could not open dbm (type %s) auth file: %s", 
            "default", 
            chxj_cookie_db_name_create(r,dconf->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    return NULL;
  }

  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);

  retval = apr_md5(md5_value, 
                   (const char*)uuid_string, 
                   (apr_size_t)APR_UUID_FORMATTED_LENGTH);
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    goto on_error;
  }

  cookie->cookie_id = apr_palloc(r->pool, apr_base64_encode_len(APR_MD5_DIGESTSIZE)+1);
  memset(cookie->cookie_id, 0, APR_MD5_DIGESTSIZE+1);
  apr_base64_encode(cookie->cookie_id, (char*)md5_value, APR_MD5_DIGESTSIZE);

  DBG(r, "cookie->cookie_id=[%s]", cookie->cookie_id);

  cookie->cookie_id = chxj_url_encode(r,cookie->cookie_id);

  DBG(r, "cookie->cookie_id=[%s]", cookie->cookie_id);

  /*
   * create key
   */

  dbmkey.dptr  = cookie->cookie_id;
  dbmkey.dsize = strlen(cookie->cookie_id);

  /*
   * create val
   */
  cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(new_cookie_table);
  store_string = apr_palloc(r->pool, 1);
  store_string[0] = 0;
  hentryp = (apr_table_entry_t*)cookie->cookie_headers->elts;

  for (ii=0; ii<cookie->cookie_headers->nelts; ii++) {
    if (ii) store_string = apr_pstrcat(r->pool,
                               store_string, 
                               "\n",
                               NULL);

    store_string = apr_pstrcat(r->pool, 
                               store_string, 
                               hentryp[ii].key, 
                               "=",
                               hentryp[ii].val, 
                               NULL);
  }
  dbmval.dptr  = store_string;
  dbmval.dsize = strlen(store_string);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR(r, "Cannot store Cookie data to DBM file `%s'",
            chxj_cookie_db_name_create(r, dconf->cookie_db_dir));
    goto on_error;
  }

  chxj_save_cookie_expire(r, cookie);


on_error:
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);

  DBG(r, "end chxj_save_cookie()");
  return cookie;
}

/*
 *
 */
cookie_t *
chxj_update_cookie(request_rec *r, cookie_t *old_cookie)
{
  int                 ii;
  apr_array_header_t  *headers;
  apr_table_entry_t   *hentryp;
  apr_status_t        retval;
  apr_datum_t         dbmkey;
  apr_datum_t         dbmval;
  apr_dbm_t           *f;
  apr_uuid_t          uuid;
  char                *uuid_string;
  unsigned char       *md5_value;
  char                *store_string;
  mod_chxj_config     *dconf;
  chxjconvrule_entry  *entryp;
  apr_file_t          *file;
  cookie_t            *cookie;


  DBG(r, "start chxj_update_cookie()");
  if (!old_cookie || ! old_cookie->cookie_headers || ! old_cookie->cookie_id) {
    DBG(r, "end chxj_update_cookie() (old_cookie is null)");
    return  NULL;
  }

  cookie = (cookie_t*)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_id = NULL;


  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
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

  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    return NULL;
  }

  DBG(r, " ");

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           chxj_cookie_db_name_create(r, dconf->cookie_db_dir), 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, "could not open dbm (type %s) auth file: %s", 
            "default", 
            chxj_cookie_db_name_create(r,dconf->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    return NULL;
  }
  DBG(r, " ");

  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);

  retval = apr_md5(md5_value, 
                   (const char*)uuid_string, 
                   (apr_size_t)APR_UUID_FORMATTED_LENGTH);
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    goto on_error;
  }
  DBG(r, " ");

  cookie->cookie_id = apr_palloc(r->pool, apr_base64_encode_len(APR_MD5_DIGESTSIZE)+1);
  memset(cookie->cookie_id, 0, APR_MD5_DIGESTSIZE+1);
  apr_base64_encode(cookie->cookie_id, (char*)md5_value, APR_MD5_DIGESTSIZE);

  cookie->cookie_id = chxj_url_encode(r,cookie->cookie_id);

  DBG(r, " ");

  /*
   * create key
   */

  dbmkey.dptr  = cookie->cookie_id;
  dbmkey.dsize = strlen(cookie->cookie_id);

  /*
   * create val
   */
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
  dbmval.dptr  = store_string;
  dbmval.dsize = strlen(store_string);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR(r, "Cannot store Cookie data to DBM file `%s'",
            chxj_cookie_db_name_create(r, dconf->cookie_db_dir));
    goto on_error;
  }

  chxj_save_cookie_expire(r, cookie);

  apr_table_setn(r->headers_in, "CHXJ_COOKIE_ID", cookie->cookie_id);


on_error:
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);

  DBG(r, "end   chxj_update_cookie()");
  return cookie;
}


/*
 *
 * @return loaded data.
 */
cookie_t *
chxj_load_cookie(request_rec *r, char *cookie_id)
{
  apr_status_t            retval;
  apr_datum_t             dbmkey;
  apr_datum_t             dbmval;
  apr_dbm_t               *f;
  mod_chxj_config         *dconf;
  chxjconvrule_entry      *entryp;
  apr_file_t              *file;
  cookie_t                *cookie;
  apr_table_t             *load_cookie_table;
  char                    *load_string;
  char                    *pstat;
  char                    *key;
  char                    *val;
  char                    *pair;

  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "start chxj_load_cookie() cookie_id=[%s]", cookie_id);
  chxj_cookie_expire_gc(r);

  cookie = (cookie_t*)apr_palloc(r->pool, sizeof(cookie_t));
  cookie->cookie_headers = NULL;
  cookie->cookie_id = apr_pstrdup(r->pool, cookie_id);

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  entryp = chxj_apply_convrule(r, dconf->convrules);
  if (! entryp) {
    DBG(r, "end chxj_load_cookie() no pattern");
    goto on_error0;
  }
  if (! (entryp->action & CONVRULE_COOKIE_ON_BIT)) {
    DBG(r, "end chxj_load_cookie() CookieOff");
    goto on_error0;
  }


  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    goto on_error0;
  }

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           chxj_cookie_db_name_create(r, dconf->cookie_db_dir),
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, 
        "could not open dbm (type %s) auth file: %s", 
        "default", 
        chxj_cookie_db_name_create(r, dconf->cookie_db_dir));
    goto on_error1;
  }

  /*
   * create key
   */
  dbmkey.dptr  = apr_pstrdup(r->pool, cookie->cookie_id);
  dbmkey.dsize = strlen(dbmkey.dptr);

  if (apr_dbm_exists(f, dbmkey)) {
  
    retval = apr_dbm_fetch(f, dbmkey, &dbmval);
    if (retval != APR_SUCCESS) {
      ERR(r, 
           "could not fetch dbm (type %s) auth file: %s", "default", 
           chxj_cookie_db_name_create(r, dconf->cookie_db_dir));
      goto on_error2;
    }
    load_cookie_table = apr_table_make(r->pool, 0);
    load_string = apr_palloc(r->pool, dbmval.dsize+1);

    memset(load_string, 0, dbmval.dsize+1);
    memcpy(load_string, dbmval.dptr, dbmval.dsize);

    char *header_cookie = apr_palloc(r->pool, 1);

    header_cookie[0] = 0;
    for (;;) {
      char *tmp_sem;
      pair = apr_strtok(load_string, "\n", &pstat);  
      load_string = NULL;
      if (!pair) break;

      DBG(r, "Cookie:[%s]", pair);
      char *tmp_pair;

      tmp_pair = apr_pstrdup(r->pool, pair);
      val = strchr(tmp_pair, '=');
      if (val) {
        key = tmp_pair;
        *val++ = 0;
        apr_table_add(load_cookie_table, key, val);
      }
      tmp_sem = strchr(pair, ';'); 
      if (tmp_sem)
        *tmp_sem = '\0';

      if (strlen(header_cookie)) 
        header_cookie = apr_pstrcat(r->pool, header_cookie, ";", NULL);

      header_cookie = apr_pstrcat(r->pool, header_cookie, pair, NULL);
    }
    if (strlen(header_cookie))
      apr_table_add(r->headers_in, "Cookie", header_cookie);
  
    cookie->cookie_headers = (apr_array_header_t*)apr_table_elts(load_cookie_table);

    if (apr_table_get(r->headers_in, "referer") == NULL) {
      apr_table_setn(r->headers_in, 
                     "referer",
                     apr_table_get(load_cookie_table, REFERER_COOKIE_KEY));
    }
  
    /*
     * save cookie_id to request header.
     */
    apr_table_setn(r->headers_in, "CHXJ_COOKIE_ID", cookie->cookie_id);
  }
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);
  DBG(r, "end   chxj_load_cookie()");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");

  return cookie;


on_error2:
  apr_dbm_close(f);

on_error1:
  chxj_cookie_db_unlock(r, file);

on_error0:

  DBG(r, "end   chxj_load_cookie()");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  DBG(r, "========================================================");
  return NULL;
}


char *
chxj_add_cookie_parameter(request_rec *r, char *value, cookie_t *cookie)
{
  char *qs;
  char *dst;

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

  qs = strchr(dst, '?');
  if (qs) {
    dst = apr_psprintf(r->pool, "%s&%s=%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id);
  }
  else {
    dst = apr_psprintf(r->pool, "%s?%s=%s", dst, CHXJ_COOKIE_PARAM, cookie->cookie_id);
  }

  DBG(r, "end   chxj_add_cookie_parameter() dst=[%s]", dst);

  return dst;

on_error:
  DBG(r, "end   chxj_add_cookie_parameter() (on_error)");
  return dst;
}


int
chxj_cookie_check_host(request_rec *r, char *value) 
{
  char *hostnm;

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


static char *
s_get_hostname_from_url(request_rec *r, char *value)
{
  if (!value) 
    return NULL; 

  if (strncasecmp(value, "http://",  7) == 0 )
    return s_cut_until_end_hostname(r, &value[7]);

  if (strncasecmp(value, "https://", 8) == 0)
    return s_cut_until_end_hostname(r, &value[8]);

  return NULL;
}


static char *
s_cut_until_end_hostname(request_rec *r, char *value)
{
  char *sp;
  char *hostnm;

  hostnm = sp = apr_pstrdup(r->pool, value);
  for (;*sp; sp++) {
    if (*sp == '/'|| *sp == '?') {
      *sp = '\0';
      break;
    }
  }
  return hostnm;
}


apr_file_t *
chxj_cookie_db_lock(request_rec *r)
{
  apr_file_t       *file;
  apr_status_t     rv;
  mod_chxj_config  *dconf;

  dconf = (mod_chxj_config *)ap_get_module_config(r->per_dir_config, &chxj_module);

  rv = apr_file_open(&file, 
                     chxj_cookie_db_lock_name_create(r, dconf->cookie_db_dir),
                     APR_CREATE|APR_WRITE, 
                     APR_OS_DEFAULT, 
                     r->pool);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    return NULL;
  }

  rv = apr_file_lock(file,APR_FLOCK_EXCLUSIVE);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    apr_file_close(file);
    return NULL;
  }

  return file;
}


void
chxj_cookie_db_unlock(request_rec *r, apr_file_t *file)
{
  apr_status_t rv;

  rv = apr_file_unlock(file);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    return;
  }

  apr_file_close(file);
}


void
chxj_delete_cookie(request_rec *r, char *cookie_id)
{
  apr_status_t      retval;
  apr_datum_t       dbmkey;
  apr_dbm_t         *f;
  apr_file_t        *file;
  mod_chxj_config   *dconf;

  DBG(r, "start chxj_delete_cookie()");

  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    goto on_error0;
  }
  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_db_name_create(r, dconf->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, 
        "could not open dbm (type %s) auth file: %s", 
        "default", 
        chxj_cookie_db_name_create(r,dconf->cookie_db_dir));
    goto on_error1;
  }

  /*
   * create key
   */
  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(dbmkey.dptr);
  if (apr_dbm_exists(f, dbmkey)) {
    apr_dbm_delete(f, dbmkey);
  }
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);

  DBG(r, "end   chxj_delete_cookie()");

  return;

on_error1:
  chxj_cookie_db_unlock(r, file);

on_error0:
  return;

}


char *
chxj_cookie_db_name_create(request_rec *r, const char *dir)
{
  char *dst;

  if (!dir) {
    dst = apr_pstrdup(r->pool, DEFAULT_COOKIE_DB_DIR);
  }
  else {
    dst = apr_pstrdup(r->pool, dir);
  }

  if (dst[strlen(dst)-1] != '/') {
    dst = apr_pstrcat(r->pool, dst, "/", COOKIE_DB_NAME, NULL);
  }
  else {
    dst = apr_pstrcat(r->pool, dst, COOKIE_DB_NAME, NULL);
  }

  return dst;
}


char *
chxj_cookie_db_lock_name_create(request_rec *r, const char *dir)
{
  char *dst;
  DBG(r, "start  chxj_cookie_db_lock_name_create()");

  if (!dir) {
    DBG(r, " ");
    dst = apr_pstrdup(r->pool, DEFAULT_COOKIE_DB_DIR);
    DBG(r, " ");
  }
  else {
    dst = apr_pstrdup(r->pool, dir);
    DBG(r, " ");
  }
  DBG(r, "dst[strlen(dst)-1]=[%c]", dst[strlen(dst)-1]);
  if (dst[strlen(dst)-1] != '/') {
    dst = apr_pstrcat(r->pool, dst, "/", COOKIE_DB_LOCK_NAME, NULL);
  }
  else {
    dst = apr_pstrcat(r->pool, dst, COOKIE_DB_LOCK_NAME, NULL);
  }
  DBG(r, "end  chxj_cookie_db_lock_name_create()");
  return dst;
}
/*
 *
 */
void
chxj_save_cookie_expire(request_rec *r, cookie_t *cookie)
{
  apr_status_t            retval;
  apr_datum_t             dbmkey;
  apr_datum_t             dbmval;
  apr_dbm_t               *f;
  apr_file_t              *file;
  mod_chxj_config         *dconf;
  char                    *store_string;

  DBG(r, "start chxj_save_cookie_expire()");
  if (!cookie) {
    DBG(r, "cookie is NULL");
    return;
  }

  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (!dconf) {
    DBG(r, "dconf is NULL");
    return;
  }

  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    return;
  }

  DBG(r, " ");

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           chxj_cookie_expire_db_name_create(r, dconf->cookie_db_dir), 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, "could not open dbm (type %s) auth file: %s", 
           "default", 
            chxj_cookie_expire_db_name_create(r,dconf->cookie_db_dir));
    chxj_cookie_expire_db_unlock(r, file);
    return;
  }
  /*
   * create key
   */

  dbmkey.dptr  = cookie->cookie_id;
  dbmkey.dsize = strlen(cookie->cookie_id);

  /*
   * create val
   */
  
  store_string = apr_psprintf(r->pool, "%d", (int)time(NULL));
  dbmval.dptr  = store_string;
  dbmval.dsize = strlen(store_string);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR(r, "Cannot store Cookie data to DBM file `%s'",
            chxj_cookie_db_name_create(r, dconf->cookie_db_dir));
  }


  apr_dbm_close(f);
  chxj_cookie_expire_db_unlock(r, file);

  DBG(r, "end   chxj_save_cookie_expire()");
}


char *
chxj_cookie_expire_db_name_create(request_rec *r, const char *dir)
{
  char *dst;

  if (!dir) {
    dst = apr_pstrdup(r->pool, DEFAULT_COOKIE_DB_DIR);
  }
  else {
    dst = apr_pstrdup(r->pool, dir);
  }

  if (dst[strlen(dst)-1] != '/') {
    dst = apr_pstrcat(r->pool, dst, "/", COOKIE_EXPIRE_DB_NAME, NULL);
  }
  else {
    dst = apr_pstrcat(r->pool, dst, COOKIE_EXPIRE_DB_NAME, NULL);
  }

  return dst;
}


char *
chxj_cookie_expire_db_lock_name_create(request_rec *r, const char *dir)
{
  char *dst;

  if (!dir) {
    dst = apr_pstrdup(r->pool, DEFAULT_COOKIE_DB_DIR);
  }
  else {
    dst = apr_pstrdup(r->pool, dir);
  }
  if (dst[strlen(dst)-1] != '/') {
    dst = apr_pstrcat(r->pool, dst, "/", COOKIE_EXPIRE_DB_LOCK_NAME, NULL);
  }
  else {
    dst = apr_pstrcat(r->pool, dst, COOKIE_EXPIRE_DB_LOCK_NAME, NULL);
  }

  return dst;
}


apr_file_t *
chxj_cookie_expire_db_lock(request_rec *r)
{
  apr_file_t       *file;
  apr_status_t     rv;
  mod_chxj_config  *dconf;

  dconf = (mod_chxj_config *)ap_get_module_config(r->per_dir_config, &chxj_module);

  rv = apr_file_open(&file, 
                     chxj_cookie_expire_db_lock_name_create(r, dconf->cookie_db_dir),
                     APR_CREATE|APR_WRITE, 
                     APR_OS_DEFAULT, 
                     r->pool);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    return NULL;
  }

  rv = apr_file_lock(file,APR_FLOCK_EXCLUSIVE);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    apr_file_close(file);
    return NULL;
  }

  return file;
}


void
chxj_cookie_expire_db_unlock(request_rec *r, apr_file_t *file)
{
  apr_status_t rv;

  rv = apr_file_unlock(file);
  if (rv != APR_SUCCESS) {
    ERR(r, "cookie lock file open failed.");
    return;
  }

  apr_file_close(file);
}

void
chxj_delete_cookie_expire(request_rec *r, char *cookie_id)
{
  apr_status_t      retval;
  apr_datum_t       dbmkey;
  apr_dbm_t         *f;
  apr_file_t        *file;
  mod_chxj_config   *dconf;

  DBG(r, "start chxj_delete_cookie_expire()");

  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    goto on_error0;
  }
  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_expire_db_name_create(r, dconf->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, 
        "could not open dbm (type %s) auth file: %s", 
        "default", 
        chxj_cookie_expire_db_name_create(r,dconf->cookie_db_dir));
    goto on_error1;
  }

  /*
   * create key
   */
  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(dbmkey.dptr);
  if (apr_dbm_exists(f, dbmkey)) {
    apr_dbm_delete(f, dbmkey);
  }
  apr_dbm_close(f);
  chxj_cookie_expire_db_unlock(r, file);

  DBG(r, "end   chxj_delete_cookie_expire()");

  return;

on_error1:
  chxj_cookie_expire_db_unlock(r, file);

on_error0:
  return;

}


void
chxj_cookie_expire_gc(request_rec *r)
{
  apr_status_t      retval;
  apr_datum_t       dbmkey;
  apr_datum_t       dbmval;
  apr_dbm_t         *f;
  apr_file_t        *file;
  mod_chxj_config   *dconf;
  time_t            now_time;

  DBG(r, "start chxj_cookie_expire_gc()");

  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    goto on_error0;
  }
  dconf = ap_get_module_config(r->per_dir_config, &chxj_module);

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_expire_db_name_create(r, dconf->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, 
        "could not open dbm (type %s) auth file: %s", 
        "default", 
        chxj_cookie_expire_db_name_create(r,dconf->cookie_db_dir));
    goto on_error1;
  }

  /*
   * create key
   */
  memset(&dbmkey, 0, sizeof(apr_datum_t));

  now_time = time(NULL);

  retval = apr_dbm_firstkey(f, &dbmkey);
  if (retval == APR_SUCCESS) {
    DBG(r, "firstkey=[%.*s]", (int)dbmkey.dsize, dbmkey.dptr);
    do {
      char *tmp;
      char *old_cookie_id;
      int   val_time;
      int   cmp_time;

      retval = apr_dbm_fetch(f, dbmkey, &dbmval);
      if (retval != APR_SUCCESS) {
        break;
      }
      tmp = apr_palloc(r->pool, dbmval.dsize+1);
      memset(tmp, 0, dbmval.dsize+1);
      memcpy(tmp, dbmval.dptr, dbmval.dsize);


      val_time = atoi(tmp);

      if (dconf->cookie_timeout == 0)
        cmp_time = now_time - DEFAULT_COOKIE_TIMEOUT;
      else
        cmp_time = now_time - dconf->cookie_timeout;

      DBG(r, "dconf->cookie_timeout=[%d]", (int)dconf->cookie_timeout);
      DBG(r, "key=[%.*s] cmp_time=[%d] val_time=[%d]", (int)dbmkey.dsize, dbmkey.dptr, cmp_time, val_time);
      if (cmp_time >= val_time) {
        apr_dbm_delete(f, dbmkey);

        old_cookie_id = apr_palloc(r->pool, dbmkey.dsize+1);
        memset(old_cookie_id, 0, dbmkey.dsize+1);
        memcpy(old_cookie_id, dbmkey.dptr, dbmkey.dsize);

        chxj_delete_cookie(r,old_cookie_id);
        DBG(r, "detect timeout cookie [%s]", old_cookie_id);
      }

      retval = apr_dbm_nextkey(f, &dbmkey);
    } while(retval == APR_SUCCESS && dbmkey.dptr != NULL);
  }

  apr_dbm_close(f);
  chxj_cookie_expire_db_unlock(r, file);

  DBG(r, "end   chxj_cookie_expire_gc()");

  return;

on_error1:
  chxj_cookie_expire_db_unlock(r, file);

on_error0:
  return;

}
/*
 * vim:ts=2 et
 */
