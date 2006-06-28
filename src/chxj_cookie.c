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

#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"

/*
 *
 */
void
chxj_save_cookie(request_rec* r)
{
  int   ii;
  char* cookie;
  apr_array_header_t* headers;
  apr_table_entry_t* entryp;
  apr_status_t        retval;
  apr_datum_t dbmkey;
  apr_datum_t dbmval;
  apr_dbm_t*          f;
  apr_uuid_t        uuid;
  char*             uuid_string;
  unsigned char*    md5_value;
  char*             md5_string;


  DBG(r, "start chxj_save_cookie()");

  headers = (apr_array_header_t*)apr_table_elts(r->headers_out);
  entryp = (apr_table_entry_t*)headers->elts;

  retval = apr_dbm_open_ex(&f, 
                           "default", 
                           "/tmp/cookie.db", 
                           APR_DBM_RWCREATE, 
                           APR_OS_DEFAULT, 
                           r->pool);
  if (retval != APR_SUCCESS) {
    ERR2(r, "could not open dbm (type %s) auth file: %s", "default", "/tmp/cookie.db");
    return;
  }

  cookie = apr_palloc(r->pool, 1);
  cookie[0] = 0;

  for (ii=0; ii<headers->nelts; ii++) {
    if (strcasecmp(entryp[ii].key, "Set-Cookie") == 0) {
      DBG(r, "=====================================");
      DBG2(r, "cookie=[%s:%s]", entryp[ii].key, entryp[ii].val);
      cookie = apr_pstrcat(r->pool, cookie, "\r\n", entryp[ii].val, NULL);
      DBG(r, "=====================================");
    }
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

  DBG(r, "end   chxj_save_cookie()");
}

/*
 * vim:ts=2 et
 */
