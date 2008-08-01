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
#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_dbm.h"
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

int
chxj_save_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  apr_status_t        retval;
  apr_datum_t         dbmkey;
  apr_datum_t         dbmval;
  apr_dbm_t           *f;
  apr_file_t          *file;

  DBG(r, "REQ[%X] start chxj_save_cookie_dbm() cookie_id:[%s] store_string:[%s]", (apr_size_t)r, cookie_id, store_string);

  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "REQ[%X] end chxj_save_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
    return CHXJ_FALSE;
  }

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, "could not open dbm (type %s) auth file: %s",
            "default",
            chxj_cookie_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    DBG(r, "REQ[%X] end chxj_save_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
    return CHXJ_FALSE;
  }


  /*
   * create key
   */

  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(cookie_id);
  dbmval.dptr  = apr_pstrdup(r->pool, store_string);
  dbmval.dsize = strlen(store_string);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR(r, "Cannot store Cookie data to DBM file `%s'",
            chxj_cookie_db_name_create(r, m->cookie_db_dir));
    apr_dbm_close(f);
    chxj_cookie_db_unlock(r, file);
    DBG(r, "REQ[%X] end chxj_save_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
    return CHXJ_FALSE;
  }

  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);
  DBG(r, "REQ[%X] end chxj_save_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
  return CHXJ_TRUE;
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


apr_file_t *
chxj_cookie_db_lock(request_rec *r)
{
  apr_file_t       *file;
  apr_status_t     rv;
  mod_chxj_config  *dconf;

  dconf = (mod_chxj_config*)chxj_get_module_config(r->per_dir_config, &chxj_module);

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

  dconf = (mod_chxj_config *)chxj_get_module_config(r->per_dir_config, &chxj_module);

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

int
chxj_update_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  apr_dbm_t           *f;
  apr_file_t          *file;
  apr_datum_t         dbmkey;
  apr_datum_t         dbmval;
  apr_status_t        retval;

  DBG(r, "start chxj_update_cookie_dbm() cookie_id:[%s]", cookie_id);

  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end   chxj_update_cookie_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, "could not open dbm (type %s) auth file: %s",
            "default",
            chxj_cookie_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    DBG(r, "end   chxj_update_cookie_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }


  /*
   * create key
   */

  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(cookie_id);

  /*
   * create val
   */
  dbmval.dptr  = apr_pstrdup(r->pool, store_string);
  dbmval.dsize = strlen(store_string);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR(r, "Cannot store Cookie data to DBM file `%s'",
            chxj_cookie_db_name_create(r, m->cookie_db_dir));
    apr_dbm_close(f);
    chxj_cookie_db_unlock(r, file);
    DBG(r, "end   chxj_update_cookie_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);
  DBG(r, "end   chxj_update_cookie_dbm() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


char *
chxj_load_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  char                    *load_string = NULL;
  apr_status_t            retval;
  apr_dbm_t               *f;
  apr_file_t              *file;
  apr_datum_t             dbmval;
  apr_datum_t             dbmkey;

  DBG(r, "REQ[%X] start chxj_load_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "REQ[%X] end   chxj_load_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
    return NULL;
  }

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r,
         "%s:%d"
         "could not open dbm (type %s) auth file: %s",
         __FILE__,__LINE__,
         "default",
         chxj_cookie_db_name_create(r, m->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    DBG(r, "REQ[%X] end   chxj_load_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
    return NULL;
  }

  /*
   * create key
   */
  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(dbmkey.dptr);

  if (apr_dbm_exists(f, dbmkey)) {
    retval = apr_dbm_fetch(f, dbmkey, &dbmval);
    if (retval != APR_SUCCESS) {
      ERR(r,
           "%s:%d"
           "could not fetch dbm (type %s) auth file: %s",
           __FILE__,__LINE__,
           "default",
           chxj_cookie_db_name_create(r, m->cookie_db_dir));
      apr_dbm_close(f);
      chxj_cookie_db_unlock(r, file);
      DBG(r, "REQ[%X] end   chxj_load_cookie_dbm() cookie_id:[%s]", (apr_size_t)r, cookie_id);
      return NULL;
    }
    load_string = apr_palloc(r->pool, dbmval.dsize+1);

    memset(load_string, 0, dbmval.dsize+1);
    memcpy(load_string, dbmval.dptr, dbmval.dsize);
  }
  apr_dbm_close(f);
  chxj_cookie_db_unlock(r, file);
  DBG(r, "REQ[%X] end   chxj_load_cookie_dbm() cookie_id:[%s] load_string:[%s]", (apr_size_t)r, cookie_id, load_string);
  return load_string;
}


int
chxj_delete_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  apr_status_t      retval;
  apr_file_t        *file;
  apr_datum_t       dbmkey;
  apr_dbm_t*        f;

  DBG(r, "start chxj_delete_cookie_dbm() cookie_id:[%s]", cookie_id);
  file = chxj_cookie_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end   chxj_delete_cookie_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r,
         "could not open dbm (type %s) auth file: %s",
         "default",
         chxj_cookie_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_db_unlock(r, file);
    DBG(r, "end   chxj_delete_cookie_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
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
  DBG(r, "end   chxj_delete_cookie_dbm() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_save_cookie_expire_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  apr_status_t            retval;
  char                    *store_string;
  apr_file_t              *file;
  apr_datum_t             dbmkey;
  apr_datum_t             dbmval;
  apr_dbm_t*              f;

  DBG(r, "start chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end   chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  DBG(r, " ");

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           chxj_cookie_expire_db_name_create(r, m->cookie_db_dir), 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, "could not open dbm (type %s) auth file: %s", 
            "default", 
            chxj_cookie_expire_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_expire_db_unlock(r, file);
    DBG(r, "end   chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }
  /*
   * create key
   */

  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(cookie_id);

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
            chxj_cookie_db_name_create(r, m->cookie_db_dir));
    DBG(r, "end   chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
    apr_dbm_close(f);
    chxj_cookie_expire_db_unlock(r, file);
    return CHXJ_FALSE;
  }


  apr_dbm_close(f);
  chxj_cookie_expire_db_unlock(r, file);
  DBG(r, "end   chxj_save_cookie_expire_dbm() cookie_id:[%s]", cookie_id);

  return CHXJ_TRUE;
}


int
chxj_delete_cookie_expire_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  apr_status_t      retval;
  apr_datum_t       dbmkey;
  apr_dbm_t*        f;
  apr_file_t*       file;

  DBG(r, "start chxj_delete_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end   chxj_delete_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }
  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_expire_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r,
         "could not open dbm (type %s) auth file: %s",
         "default",
         chxj_cookie_expire_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_expire_db_unlock(r, file);
    DBG(r, "end   chxj_delete_cookie_expire_dbm() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  /*
 *    * create key
 *       */
  dbmkey.dptr  = apr_pstrdup(r->pool, cookie_id);
  dbmkey.dsize = strlen(dbmkey.dptr);
  if (apr_dbm_exists(f, dbmkey)) {
    apr_dbm_delete(f, dbmkey);
  }
  apr_dbm_close(f);
  chxj_cookie_expire_db_unlock(r, file);
  DBG(r, "end   chxj_delete_cookie_expire_dbm() cookie_id:[%s]", cookie_id);

  return CHXJ_TRUE;
}


int
chxj_cookie_expire_gc_dbm(request_rec *r, mod_chxj_config *m)
{
  apr_status_t      retval;
  apr_datum_t       dbmkey;
  apr_datum_t       dbmval;
  apr_dbm_t*        f;
  apr_file_t*       file;
  time_t            now_time;

  DBG(r, "start chxj_cookie_expire_gc_dbm()");

  file = chxj_cookie_expire_db_lock(r);
  if (! file) {
    ERR(r, "mod_chxj: Can't lock cookie db");
    DBG(r, "end   chxj_cookie_expire_gc_dbm()");
    return CHXJ_FALSE;
  }

  retval = apr_dbm_open_ex(&f,
                           "default",
                           chxj_cookie_expire_db_name_create(r, m->cookie_db_dir),
                           APR_DBM_RWCREATE,
                           APR_OS_DEFAULT,
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR(r, 
         "could not open dbm (type %s) auth file: %s", 
         "default", 
         chxj_cookie_expire_db_name_create(r,m->cookie_db_dir));
    chxj_cookie_expire_db_unlock(r, file);
    DBG(r, "end   chxj_cookie_expire_gc_dbm()");
    return CHXJ_FALSE;
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
      char* tmp;
      char* old_cookie_id;
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

      if (m->cookie_timeout == 0)
        cmp_time = now_time - DEFAULT_COOKIE_TIMEOUT;
      else
        cmp_time = now_time - m->cookie_timeout;

      DBG(r, "m->cookie_timeout=[%d]", (int)m->cookie_timeout);
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
  DBG(r, "end   chxj_cookie_expire_gc_dbm()");
  return CHXJ_TRUE;
}


cookie_lock_t *
chxj_cookie_lock_dbm(request_rec *r, mod_chxj_config *UNUSED(m))
{
  cookie_lock_t *ret = apr_palloc(r->pool, sizeof(*ret));
  ret->file = chxj_cookie_db_lock(r);
  return ret;
}


int
chxj_cookie_unlock_dbm(request_rec *r, cookie_lock_t *lock, mod_chxj_config *UNUSED(m))
{
  chxj_cookie_expire_db_unlock(r, lock->file);
  return 1; /* allways true */
}
/*
 * vim:ts=2 et
 */
