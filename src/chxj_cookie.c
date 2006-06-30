/*
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
 * Copyright (C) 2005 Atsushi Konno All rights reserved.
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
#include "chxj_url_encode.h"
#include "chxj_apply_convrule.h"

#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"

static char* s_get_hostname_from_url(request_rec* r, char* value);
static char* s_cut_until_end_hostname(request_rec*, char* value);

/*
 *
 */
char*
chxj_save_cookie(request_rec* r)
{
  int   ii;
  char* cookie;
  apr_array_header_t* headers;
  apr_table_entry_t*  hentryp;
  apr_status_t        retval;
  apr_datum_t dbmkey;
  apr_datum_t dbmval;
  apr_dbm_t*          f;
  apr_uuid_t        uuid;
  char*             uuid_string;
  unsigned char*    md5_value;
  char*             md5_string;
  mod_chxj_global_config* gconf;
  mod_chxj_config*        dconf;
  chxjconvrule_entry* entryp;



  DBG(r, "start chxj_save_cookie()");

  md5_string = NULL;

  gconf = ap_get_module_config(r->server->module_config, &chxj_module);
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


  cookie = apr_palloc(r->pool, 1);
  cookie[0] = 0;

  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(hentryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "=====================================");
      DBG2(r, "cookie=[%s:%s]", hentryp[ii].key, hentryp[ii].val);
      cookie = apr_pstrcat(r->pool, cookie, "\r\n", hentryp[ii].val, NULL);
      DBG(r, "=====================================");
    }
  }

  if (strlen(cookie) == 0) {
    DBG(r, "no cookie");
    return NULL;
  }

  apr_global_mutex_lock(gconf->cookie_db_lock);

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           "/tmp/cookie.db", 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR2(r, "could not open dbm (type %s) auth file: %s", "default", "/tmp/cookie.db");
    apr_global_mutex_unlock(gconf->cookie_db_lock);
    return NULL;
  }

  apr_uuid_get(&uuid);
  uuid_string = apr_palloc(r->pool, APR_UUID_FORMATTED_LENGTH + 1);
  memset(uuid_string, 0, APR_UUID_FORMATTED_LENGTH + 1);
  apr_uuid_format(uuid_string, &uuid);;

  md5_value = (unsigned char*)apr_palloc(r->pool, APR_MD5_DIGESTSIZE + 1);
  memset(md5_value, 0, APR_MD5_DIGESTSIZE + 1);
  retval = apr_md5(md5_value, 
          (const char*)uuid_string, (apr_size_t)APR_UUID_FORMATTED_LENGTH);
  if (retval != APR_SUCCESS) {
    ERR(r, "md5 failed.");
    goto on_error;
  }

  md5_string = apr_palloc(r->pool, apr_base64_encode_len(APR_MD5_DIGESTSIZE)+1);
  memset(md5_string, 0, APR_MD5_DIGESTSIZE+1);
  apr_base64_encode(md5_string, (char*)md5_value, APR_MD5_DIGESTSIZE);

  DBG1(r, "md5_string=[%s]", md5_string);

  md5_string = chxj_url_encode(r,md5_string);

  DBG1(r, "md5_string=[%s]", md5_string);

  /*
   * create key
   */

  dbmkey.dptr  = md5_string;
  dbmkey.dsize = strlen(md5_string);

  /*
   * create val
   */
  dbmval.dptr  = cookie;
  dbmval.dsize = strlen(cookie);

  /*
   * store to db
   */
  retval = apr_dbm_store(f, dbmkey, dbmval);
  if (retval != APR_SUCCESS) {
    ERR1(r, "Cannot store SSL session to DBM file `%s'","/tmp/cookie.db");
    goto on_error;
  }


on_error:
  apr_dbm_close(f);
  apr_global_mutex_unlock(gconf->cookie_db_lock);

  DBG(r, "end   chxj_save_cookie()");
  return md5_string;
}


char*
chxj_add_cookie_parameter(request_rec* r, char* value, char* cookie_id)
{
  char* qs;
  char* dst;

  DBG1(r, "start chxj_add_cookie_parameter() cookie_id=[%s]", cookie_id);
  if (!cookie_id) return value;
  if (chxj_cookie_check_host(r, value) != 0) {
    DBG(r, "end chxj_add_cookie_parameter()(check host)");
    return value;
  }

  dst = value;

  qs = strchr(dst, '?');
  if (qs) {
    dst = apr_psprintf(r->pool, "%s&%s=%s", dst, CHXJ_COOKIE_PARAM, cookie_id);
  }
  else {
    dst = apr_psprintf(r->pool, "%s?%s=%s", dst, CHXJ_COOKIE_PARAM, cookie_id);
  }

  DBG1(r, "end   chxj_add_cookie_parameter() dst=[%s]", dst);

  return dst;
}


int
chxj_cookie_check_host(request_rec* r, char* value) 
{
  char* hostnm;

  DBG1(r, "hostname=[%s]", r->hostname);

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
  if (!value) return NULL; 

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
/*
 * vim:ts=2 et
 */
