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
#ifdef USE_MYSQL_COOKIE
#include "mod_chxj.h"
#include "chxj_cookie.h"
#include "chxj_url_encode.h"
#include "chxj_apply_convrule.h"

#include "ap_release.h"

#include "apu.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"

#include <unistd.h>

/* for MySQL */
#include <mysql.h>
#include <errmsg.h>


#define CHXJ_MYSQL_RECONNECT_WAIT_TIME (1)
#define CHXJ_MYSQL_RECONNECT_COUNT (3)

typedef struct {
  MYSQL *handle;
  char host[255];
  char username[255];
  char database[255];
  time_t last_used;
  int reconnect;
} mysql_connection;

static mysql_connection connection = {NULL, "", "", "", 0, 0};


static apr_status_t
_mysql_cleanup(void *UNUSED(notused))
{
  chxj_close_mysql_handle();
  return APR_SUCCESS;
}

static apr_status_t
_mysql_cleanup_child(void *UNUSED(notused))
{
  return APR_SUCCESS;
}


void
chxj_close_mysql_handle()
{
  if (connection.handle) {
    mysql_close(connection.handle);
    connection.handle = NULL;
  }
}

int
chxj_open_mysql_handle(request_rec *r, mod_chxj_config *m)
{
  static MYSQL mysql_conn;
  char query[MAX_STRING_LEN];

  if (connection.handle && connection.reconnect == 0) {
    if ((!m->mysql.host || (strcasecmp(m->mysql.host, "localhost") == 0)) && connection.host[0] == '\0'
        &&  (m->mysql.username && strcmp(m->mysql.username, connection.username) == 0)) {

      if (m->mysql.database && strcmp(m->mysql.database, connection.database) == 0) {
        DBG(r, "already connected");
        DBG(r, "end chxj_open_mysql_handle()");
        return 1;
      }
      else {
        if (mysql_select_db(connection.handle,m->mysql.database) != 0) {
          ERR(r, "%s:%d MySQL ERROR: %s", APLOG_MARK, mysql_error(connection.handle));
          return 0;
        }
        else {
          strcpy (connection.database, m->mysql.database);
          DBG(r, "already connected. new database:[%s]", m->mysql.database);
          return 1;
        }
      }
    }
  }

  chxj_close_mysql_handle();
  connection.handle = mysql_init(&mysql_conn);
  if (! connection.handle) {
    ERR(r, "%s:%d MySQL ERROR: %s", APLOG_MARK, mysql_error(&mysql_conn));
    return 0;
  }

  if (!m->mysql.host || strcmp(m->mysql.host,"localhost") == 0) {
    connection.host[0] = '\0';
  } else {
    strcpy(connection.host, m->mysql.host);
  }

  connection.handle = mysql_real_connect(&mysql_conn,connection.host,m->mysql.username,
		  		  m->mysql.password, NULL, m->mysql.port, m->mysql.socket_path, 0);
  if (!connection.handle) {
    ERR(r, "MySQL ERROR: %s. host:[%s] username:[%s] password:[%s] port:[%d] socket_path:[%s]", mysql_error(&mysql_conn), 
       connection.host, 
       m->mysql.username,
       m->mysql.password,
       m->mysql.port,
       m->mysql.socket_path);
    DBG(r, "end chxj_open_mysql_handle()");
    return 0;
  }

  apr_pool_cleanup_register(r->pool, (void *)NULL, _mysql_cleanup, _mysql_cleanup_child);
  if (m->mysql.username) {
    strcpy(connection.username, m->mysql.username);
  }
  else {
    connection.username[0] = '\0';
  }

  if (mysql_select_db(connection.handle,m->mysql.database) != 0) {
    ERR(r, "%s:%d MySQL ERROR: %s", APLOG_MARK, mysql_error(connection.handle));
    return 0;
  }

  strcpy (connection.database, m->mysql.database);
  if (m->mysql.charset) {
    apr_snprintf(query, sizeof(query)-1, "SET CHARACTER SET %s", m->mysql.charset);
    if (mysql_query(connection.handle, query) != 0) {
      ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }

  DBG(r, "end chxj_open_mysql_handle()");
  return 1;
}


int
chxj_mysql_exist_cookie_table(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  int retry_count = 0;

  apr_snprintf(query, sizeof(query)-1, "desc %s", m->mysql.tablename);
  DBG(r, "start chxj_mysql_exist_cookie_table() query:[%s]", query);
  do { 
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return 0;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_exist_cookie_table() query:[%s]", query);

  return 1;
}


int
chxj_mysql_exist_cookie_table_expire(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  int retry_count = 0;

  apr_snprintf(query, sizeof(query)-1, "desc %s_expire", m->mysql.tablename);

  DBG(r, "start chxj_mysql_exist_cookie_table_expire() query:[%s]", query);

  do { 
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return 0;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  } while(0);

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);

  DBG(r, "end chxj_mysql_exist_cookie_table_expire() query:[%s]", query);

  return 1;
}


int
chxj_mysql_create_cookie_table(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  int retry_count = 0;

  apr_snprintf(query, sizeof(query)-1, "CREATE TABLE %s  (cookie_id VARCHAR(%d) NOT NULL, data TEXT, PRIMARY KEY(cookie_id)) TYPE=InnoDB;",
    m->mysql.tablename,
    apr_base64_encode_len(APR_MD5_DIGESTSIZE) * 3);
  DBG(r, "start chxj_mysql_create_cookie_table() query:[%s]", query);
  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return 0;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_create_cookie_table() query:[%s]", query);

  return 1;
}

int
chxj_mysql_create_cookie_expire_table(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  int retry_count = 0;

  apr_snprintf(query, sizeof(query)-1, "CREATE TABLE %s_expire  (cookie_id VARCHAR(%d) NOT NULL, created_at DATETIME, PRIMARY KEY(cookie_id)) TYPE=InnoDB;",
    m->mysql.tablename,
    apr_base64_encode_len(APR_MD5_DIGESTSIZE) * 3);

  DBG(r, "start chxj_mysql_create_cookie_expire_table() query:[%s]", query);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return 0;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_create_cookie_expire_table() query:[%s]", query);

  return 1;
}


char *
chxj_mysql_get_cookie_from_cookie_id(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *retval = NULL;
  apr_size_t clen = strlen(cookie_id);
  char *sql_safe_cookie_id = apr_palloc(r->pool, clen*2+1);
  int retry_count = 0;

  mysql_escape_string(sql_safe_cookie_id,cookie_id,clen);

  apr_snprintf(query, sizeof(query)-1, "SELECT data, length(data) FROM %s WHERE cookie_id = '%s'", m->mysql.tablename, sql_safe_cookie_id);
  DBG(r, "start chxj_mysql_get_cookie_from_cookie_id() query:[%s]", query);
  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return 0;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return NULL;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result && (mysql_num_rows(result) >= 1)) {
    MYSQL_ROW data = mysql_fetch_row(result);
    int len;
    if (! data[0]) {
      mysql_free_result(result);
      ERR(r, "%s:%d MySQL cookie_id:[%s] has no valid cookie_id. %s", APLOG_MARK, cookie_id, r->uri);
      return NULL;
    }
    len = atoi(data[1]);
    retval = (char *) apr_palloc(r->pool, len + 1);
    memcpy(retval, data[0], len);
  }
  if (result) mysql_free_result(result);

  
  DBG(r, "end chxj_mysql_get_cookie_from_cookie_id() query:[%s]", query);

  return retval;
}


char *
chxj_mysql_get_cookie_expire_from_cookie_id(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  apr_size_t clen = strlen(cookie_id);
  char *retval = NULL;
  char *sql_safe_cookie_id = apr_palloc(r->pool, clen*2+1);
  int retry_count = 0;

  mysql_escape_string(sql_safe_cookie_id,cookie_id,clen);

  apr_snprintf(query, sizeof(query)-1, "SELECT DATE_FORMAT(created_at, '%%Y%%m%%d%%H%%i%%s') FROM %s_expire WHERE cookie_id = '%s'", 
    m->mysql.tablename, sql_safe_cookie_id);

  DBG(r, "start chxj_mysql_get_cookie_expire_from_cookie_id() query:[%s]", query);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return NULL;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return NULL;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result && (mysql_num_rows(result) >= 1)) {
    MYSQL_ROW data = mysql_fetch_row(result);
    if (! data[0]) {
      mysql_free_result(result);
      ERR(r, "%s:%d MySQL cookie_id:[%s] has no valid cookie_id. %s", APLOG_MARK, cookie_id, r->uri);
      return NULL;
    }
    retval = (char *) apr_palloc(r->pool, 19 + 1);
    memset(retval, 0, 19+1);
    memcpy(retval, data[0], 19);
  }
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_get_cookie_expire_from_cookie_id() query:[%s] retval:[%s]", query, retval);

  return retval;
}


int
chxj_mysql_insert_or_update_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *data)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *cid = ap_escape_logitem(r->pool, cookie_id);
  char *cdt = ap_escape_logitem(r->pool, data);
  int retry_count = 0;

  DBG(r, "start chxj_mysql_insert_or_update_cookie() data:[%s]", data);
  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0;
    apr_snprintf(query, sizeof(query)-1, "BEGIN;");
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  apr_snprintf(query, sizeof(query)-1, "INSERT INTO %s (cookie_id, data) VALUES ('%s','%s');", m->mysql.tablename, cid, cdt);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    WRN(r, "MySQL WARN: %s: %s", mysql_error(connection.handle), r->uri);
    if (!chxj_mysql_get_cookie_from_cookie_id(r, m, cookie_id)) {
      ERR(r, "%s:%d failed chxj_mysql_get_cookie_from_cookie_id() cookie_id:[%s]", APLOG_MARK, cookie_id);
      return 0;
    }
    apr_snprintf(query, sizeof(query)-1, "UPDATE %s set data = '%s' WHERE cookie_id = '%s';", m->mysql.tablename, cdt, cid);
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      chxj_mysql_rollback(r, m);
      return 0;
    }
  }

  apr_snprintf(query, sizeof(query)-1, "COMMIT;");
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_get_cookie_from_cookie_id()");

  return 1;
}


int
chxj_mysql_insert_or_update_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *cid = ap_escape_logitem(r->pool, cookie_id);
  int retry_count = 0;

  DBG(r, "start chxj_mysql_insert_or_update_cookie_expire()");

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0; 
    apr_snprintf(query, sizeof(query)-1, "BEGIN;");
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK,mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  apr_snprintf(query, sizeof(query)-1, "INSERT INTO %s_expire (cookie_id, created_at) VALUES ('%s',localtime);", m->mysql.tablename, cid);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    WRN(r, "MySQL WARN: %s: %s", mysql_error(connection.handle), r->uri);
    if (!chxj_mysql_get_cookie_from_cookie_id(r, m, cookie_id)) {
      ERR(r, "%s:%d failed chxj_mysql_get_cookie_from_cookie_id() cookie_id:[%s]", APLOG_MARK, cookie_id);
      return 0;
    }
    apr_snprintf(query, sizeof(query)-1, "UPDATE %s_expire set created_at = localtime WHERE cookie_id = '%s';", m->mysql.tablename, cid);
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      chxj_mysql_rollback(r, m);
      return 0;
    }
  }

  apr_snprintf(query, sizeof(query)-1, "COMMIT;");
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_insert_or_update_cookie_expire()");

  return 1;
}


int
chxj_mysql_rollback(request_rec *r, mod_chxj_config *m)
{
  char query[MAX_STRING_LEN];

  DBG(r, "start chxj_mysql_rollback()");

  apr_snprintf(query, sizeof(query)-1, "ROLLBACK;");

  if (!chxj_open_mysql_handle(r, m)) {
    DBG(r, "end chxj_mysql_rollback()");
    return 1; /* TRUE */
  }
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    return 0; /* FALSE */
  }
  DBG(r, "end chxj_mysql_rollback()");
  return 1;
}

char *
chxj_mysql_load_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  apr_size_t clen = strlen(cookie_id);
  char *retval = NULL;
  char *sql_safe_cookie_id = apr_palloc(r->pool, clen*2+1);
  int retry_count = 0;
  
  mysql_escape_string(sql_safe_cookie_id,cookie_id,clen);

  apr_snprintf(query, sizeof(query)-1, "SELECT data, length(data) FROM %s WHERE cookie_id = '%s';", m->mysql.tablename, sql_safe_cookie_id);

  DBG(r, "start chxj_mysql_load_cookie() query:[%s]", query);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK, query);
      return NULL;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return NULL;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return NULL;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result && (mysql_num_rows(result) >= 1)) {
    MYSQL_ROW data = mysql_fetch_row(result);
    int len;
    if (! data[0]) {
      ERR(r, "%s:%d MySQL cookie_id:[%s] has no valid cookie_id. %s", APLOG_MARK, cookie_id, r->uri);
      mysql_free_result(result);
      return NULL;
    }
    len = atoi(data[1]);
    retval = (char *) apr_palloc(r->pool, len + 1);
    memset(retval, 0, len + 1);
    memcpy(retval, data[0], len);
  }
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_load_mysql_cookie() query:[%s]", query);

  return retval;
}


char *
chxj_mysql_load_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *retval = NULL;
  apr_size_t clen = strlen(cookie_id);
  char *sql_safe_cookie_id = apr_palloc(r->pool, clen*2+1);
  int retry_count = 0;

  mysql_escape_string(sql_safe_cookie_id,cookie_id,clen);

  apr_snprintf(query, 
               sizeof(query)-1, 
               "SELECT DATE_FORMAT(created_at, '%%Y%%m%%d%%H%%i%%s') FROM %s_expire WHERE cookie_id = '%s';", 
               m->mysql.tablename, 
               sql_safe_cookie_id);

  DBG(r, "start chxj_mysql_load_cookie_expire() query:[%s]", query);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle() query:[%s]", APLOG_MARK,query);
      return NULL;
    }
    connection.reconnect = 0;
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK,mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      WRN(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return NULL;
    }
  }
  while(0);

  result = mysql_store_result(connection.handle);
  if (result && (mysql_num_rows(result) >= 1)) {
    MYSQL_ROW data = mysql_fetch_row(result);
    if (! data[0]) {
      ERR(r, "%s:%d MySQL cookie_id:[%s] has no valid cookie_id. %s", APLOG_MARK,cookie_id, r->uri);
      mysql_free_result(result);
      return NULL;
    }
    retval = (char *) apr_palloc(r->pool, 14 + 1);
    memcpy(retval, data[0], 14);
  }
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_load_cookie_expire() query:[%s]", query);

  return retval;
}


int
chxj_mysql_delete_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *cid = ap_escape_logitem(r->pool, cookie_id);
  int retry_count = 0;

  DBG(r, "start chxj_mysql_delete_cookie() cookie_id:[%s]", cookie_id);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0;
    apr_snprintf(query, sizeof(query)-1, "BEGIN;");
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK,mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  if (!chxj_mysql_get_cookie_from_cookie_id(r, m, cookie_id)) {
    ERR(r, "%s:%d failed chxj_mysql_get_cookie_from_cookie_id() cookie_id:[%s]", APLOG_MARK,cookie_id);
    return 0;
  }
  apr_snprintf(query, sizeof(query)-1, "DELETE FROM %s WHERE cookie_id = '%s';", m->mysql.tablename, cid);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  apr_snprintf(query, sizeof(query)-1, "COMMIT;");
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_delete_cookie()");

  return 1;
}


int
chxj_mysql_delete_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *cid = ap_escape_logitem(r->pool, cookie_id);
  int retry_count = 0;

  DBG(r, "start chxj_mysql_delete_cookie_expire() cookie_id:[%s]", cookie_id);

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0; 
    apr_snprintf(query, sizeof(query)-1, "BEGIN;");
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK,mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK,mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while(0);

  if (!chxj_mysql_get_cookie_expire_from_cookie_id(r, m, cookie_id)) {
    ERR(r, "%s:%d failed chxj_mysql_get_cookie_expire_from_cookie_id() cookie_id:[%s]", APLOG_MARK,cookie_id);
    return 0;
  }
  apr_snprintf(query, sizeof(query)-1, "DELETE FROM %s_expire WHERE cookie_id = '%s';", m->mysql.tablename, cid);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK,mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  apr_snprintf(query, sizeof(query)-1, "COMMIT;");
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_mysql_delete_cookie_expire() cookie_id:[%s]", cookie_id);

  return 1;
}

char *
chxj_mysql_get_timeout_localtime(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *retval = NULL;
  int retry_count = 0;

  DBG(r, "start chxj_mysql_get_timeout_localtime()");
  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0;
    apr_snprintf(query, sizeof(query)-1, "SELECT DATE_SUB(localtime, interval %ld second);",
      (m->cookie_timeout == 0) ? DEFAULT_COOKIE_TIMEOUT : m->cookie_timeout);
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK, mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      return NULL;
    }
  }
  while(0);
  result = mysql_store_result(connection.handle);
  if (result && (mysql_num_rows(result) >= 1)) {
    MYSQL_ROW data = mysql_fetch_row(result);
    if (! data[0]) {
      ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
      mysql_free_result(result);
      return NULL;
    }
    retval = (char *) apr_palloc(r->pool, 19 + 1);
    memset(retval, 0, 19+1);
    memcpy(retval, data[0], 19);
  }
  if (result) mysql_free_result(result);
   
  DBG(r, "end chxj_mysql_get_timeout_localtime()");

  return retval;
}

int
chxj_mysql_delete_expired_cookie(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  char *timeout;
  int retry_count = 0;
  DBG(r, "start chxj_mysql_delete_expired_cookie()");

  do {
    if (!chxj_open_mysql_handle(r, m)) {
      ERR(r, "%s:%d failed chxj_open_mysql_handle()", APLOG_MARK);
      return 0;
    }
    connection.reconnect = 0;
    apr_snprintf(query, sizeof(query)-1, "BEGIN;");
    DBG(r, "query:[%s]", query);
    if (mysql_query(connection.handle, query) != 0) {
      if (mysql_errno(connection.handle) == CR_SERVER_GONE_ERROR) {
        connection.reconnect = 1;
        retry_count++;
        if (retry_count >= CHXJ_MYSQL_RECONNECT_COUNT) {
          ERR(r, "%s:%d MySQL ERROR: %s: %s(retry over)", APLOG_MARK,mysql_error(connection.handle), r->uri);
          return 0;
        }
        sleep(CHXJ_MYSQL_RECONNECT_WAIT_TIME);
        continue;
      }
      ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK,mysql_error(connection.handle), r->uri);
      return 0;
    }
  }
  while (0);

  timeout = chxj_mysql_get_timeout_localtime(r, m);
  if (! timeout) {
    ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    return 0;
  }

  apr_snprintf(query, sizeof(query)-1, "SELECT * FROM %s_expire WHERE created_at <= '%s'", m->mysql.tablename, timeout);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK,mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }
  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  result = NULL;

  /* delete from chxj_cookie */
  apr_snprintf(query, sizeof(query)-1, "DELETE %s FROM %s, %s_expire WHERE %s_expire.created_at <= '%s' AND %s.cookie_id = %s_expire.cookie_id;", 
     m->mysql.tablename, 
     m->mysql.tablename,
     m->mysql.tablename,
     m->mysql.tablename,
     timeout,
     m->mysql.tablename,
     m->mysql.tablename);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }
  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  result = NULL;

  /* delete from chxj_cookie_expire */
  apr_snprintf(query, sizeof(query)-1, "DELETE %s_expire FROM %s_expire WHERE %s_expire.created_at <= '%s';", 
     m->mysql.tablename, 
     m->mysql.tablename,
     m->mysql.tablename,
     timeout);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL ERROR: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }
  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  result = NULL;

  apr_snprintf(query, sizeof(query)-1, "COMMIT;");
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    ERR(r, "%s:%d MySQL WARN: %s: %s", APLOG_MARK, mysql_error(connection.handle), r->uri);
    chxj_mysql_rollback(r, m);
    return 0;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);

  DBG(r, "end chxj_mysql_delete_expired_cookie()");
  return 1;
}











int
chxj_save_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  DBG(r, "start chxj_save_cookie_mysql()");
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "Cannot open mysql connection");
    DBG(r, "end chxj_save_cookie_mysql()");
    return CHXJ_FALSE;
  }

  if (!chxj_mysql_exist_cookie_table(r, m)) {
    DBG(r, "not found cookie table:[%s]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_table(r, m)) {
      ERR(r, "cannot create cookie table:[%s]", m->mysql.tablename);
      DBG(r, "end chxj_save_cookie_mysql()");
      return CHXJ_FALSE;
    }
  }
  if (! chxj_mysql_insert_or_update_cookie(r, m, cookie_id, store_string)) {
    ERR(r, "cannot store to cookie table:[%s]", m->mysql.tablename);
    DBG(r, "end chxj_save_cookie_mysql()");
    return CHXJ_FALSE;
  }

  /* *NEED NOT* close database. */
  DBG(r, "end chxj_save_cookie_mysql()");
  return CHXJ_TRUE;
}

int
chxj_update_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string)
{
  DBG(r, "start chxj_update_cookie_mysql() cookie_id:[%s]", cookie_id);
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "Cannot open mysql connection");
    DBG(r, "end chxj_update_cookie_mysql() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  if (!chxj_mysql_exist_cookie_table(r, m)) {
    DBG(r, "not found cookie table:[%s]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_table(r, m)) {
      ERR(r, "cannot create cookie table:[%s]", m->mysql.tablename);
      DBG(r, "end chxj_update_cookie_mysql() cookie_id:[%s]", cookie_id);
      return CHXJ_FALSE;
    }
  }
  if (! chxj_mysql_insert_or_update_cookie(r, m, cookie_id, store_string)) {
    ERR(r, "cannot create cookie table:[%s]", m->mysql.tablename);
    DBG(r, "end chxj_update_cookie_mysql() cookie_id:[%s]", cookie_id);
    return CHXJ_FALSE;
  }

  /* *NEED NOT* close database. */
  /* chxj_close_mysql_handle(); */
  DBG(r, "end chxj_update_cookie_mysql() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


char *
chxj_load_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  char *load_string;

  DBG(r, "start chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "Cannot open mysql connection");
    DBG(r, "end   chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
    return NULL;
  }

  if (!chxj_mysql_exist_cookie_table(r, m)) {
    DBG(r, "not found cookie table:[%s]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_table(r, m)) {
      ERR(r, "cannot create cookie table:[%s]", m->mysql.tablename);
      DBG(r, "end   chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
      return NULL;
    }
  }
  if (!(load_string = chxj_mysql_load_cookie(r, m, cookie_id))) {
    ERR(r, "%s:%d not found cookie. cookie_id:[%s]", APLOG_MARK, cookie_id);
    DBG(r, "end   chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
    return NULL;
  }

  /* *NEED NOT* close database. */
  /* chxj_close_mysql_handle(); */
  DBG(r, "end   chxj_load_cookie_mysql() cookie_id:[%s]", cookie_id);
  return load_string;
}


int
chxj_delete_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  DBG(r, "start chxj_delete_cookie_mysql() cookie_id=[%s]", cookie_id);
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "%s:%d Cannot open mysql connection cookie_id=[%s]", APLOG_MARK, cookie_id);
    return CHXJ_FALSE;
  }
  if (!chxj_mysql_exist_cookie_table(r, m)) {
    DBG(r, "not found cookie table:[%s]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_table(r, m)) {
      ERR(r, "%s:%d cannot create cookie table:[%s]", APLOG_MARK, m->mysql.tablename);
      return CHXJ_FALSE;
    }
  }
  else {
    if (!chxj_mysql_delete_cookie(r, m, cookie_id)) {
      ERR(r, "%s:%d failed: chxj_mysql_delete_cookie() cookie_id:[%s]", APLOG_MARK, cookie_id);
      return CHXJ_FALSE;
    }
  }
  DBG(r, "end chxj_delete_cookie_mysql() cookie_id=[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_save_cookie_expire_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  DBG(r, "start chxj_save_cookie_expire_mysql() cookie_id:[%s]", cookie_id);
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "%s:%d Cannot open mysql connection cookie_id=[%s]",APLOG_MARK, cookie_id);
    return CHXJ_FALSE;
  }

  if (!chxj_mysql_exist_cookie_table_expire(r, m)) {
    DBG(r, "not found cookie table:[%s_expire]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_expire_table(r, m)) {
      ERR(r, "%s:%d cannot create cookie table:[%s_expire] cookie_id:[%s]", APLOG_MARK, m->mysql.tablename, cookie_id);
      return CHXJ_FALSE;
    }
  }
  if (! chxj_mysql_insert_or_update_cookie_expire(r, m, cookie_id)) {
    ERR(r, "%s:%d cannot create cookie table:[%s_expire] cookie_id:[%s]", APLOG_MARK, m->mysql.tablename, cookie_id);
    return CHXJ_FALSE;
  }

  /* *NEED NOT* close database. */
  /* chxj_close_mysql_handle(); */

  DBG(r, "end   chxj_save_cookie_expire_mysql() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_delete_cookie_expire_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id)
{
  DBG(r, "start chxj_delete_cookie_expire_mysql() cookie_id:[%s]", cookie_id);
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "%s:%d Cannot open mysql connection", APLOG_MARK);
    return CHXJ_FALSE;
  }

  if (!chxj_mysql_exist_cookie_table_expire(r, m)) {
    DBG(r, "not found cookie table:[%s_expire]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_expire_table(r, m)) {
      ERR(r, "%s:%d cannot create cookie table:[%s_expire] cookie_id:[%s]", APLOG_MARK, m->mysql.tablename, cookie_id);
      return CHXJ_FALSE;
    }
  } else {
    if (!chxj_mysql_delete_cookie_expire(r, m, cookie_id)) {
      ERR(r, "%s:%d failed: chxj_mysql_delete_cookie() cookie_id:[%s]", APLOG_MARK, cookie_id);
      return CHXJ_FALSE;
    }
  }
  DBG(r, "end   chxj_delete_cookie_expire_mysql() cookie_id:[%s]", cookie_id);
  return CHXJ_TRUE;
}


int
chxj_cookie_expire_gc_mysql(request_rec *r, mod_chxj_config *m)
{
  DBG(r, "start chxj_cookie_expire_gc_mysql()");
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "Cannot open mysql connection");
    DBG(r, "end chxj_cookie_expire_gc_mysql()");
    return CHXJ_FALSE;
  }
  if (!chxj_mysql_exist_cookie_table_expire(r, m)) {
    DBG(r, "not found cookie table:[%s_expire]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_expire_table(r, m)) {
      ERR(r, "%s:%d cannot create cookie table:[%s_expire]", APLOG_MARK, m->mysql.tablename);
      return CHXJ_FALSE;
    }
  } else {
    if (!chxj_mysql_delete_expired_cookie(r, m)) {
      ERR(r, "%s:%d failed: chxj_mysql_delete_expired_cookie()", APLOG_MARK);
      return CHXJ_FALSE;
    }
  }
  DBG(r, "end   chxj_cookie_expire_gc_mysql()");
  return CHXJ_TRUE;
}


int
chxj_cookie_lock_mysql(request_rec *r, mod_chxj_config *m)
{
  MYSQL_RES *result;
  char query[MAX_STRING_LEN];
  DBG(r, "start chxj_cookie_lock_mysql()");
  if (! chxj_open_mysql_handle(r, m)) {
    ERR(r, "Cannot open mysql connection");
    DBG(r, "end   chxj_save_cookie_expire_mysql()");
    return CHXJ_FALSE;
  }
  if (!chxj_mysql_exist_cookie_table_expire(r, m)) {
    DBG(r, "not found cookie table:[%s_expire]", m->mysql.tablename);
    if (!chxj_mysql_create_cookie_expire_table(r, m)) {
      ERR(r, "cannot create cookie table:[%s_expire]", m->mysql.tablename);
      DBG(r, "end chxj_cookie_expire_gc_mysql()");
      return CHXJ_FALSE;
    }
  }
  apr_snprintf(query, sizeof(query)-1, "LOCK TABLES %s WRITE", m->mysql.tablename);
  DBG(r, "query:[%s]", query);
  if (mysql_query(connection.handle, query) != 0) {
    chxj_mysql_rollback(r, m);
    ERR(r, "MySQL WARN: %s: %s", mysql_error(connection.handle), r->uri);
    return CHXJ_FALSE;
  }

  result = mysql_store_result(connection.handle);
  if (result) mysql_free_result(result);
  
  DBG(r, "end chxj_cookie_lock_mysql()");
  return CHXJ_TRUE;
}


int
chxj_cookie_unlock_mysql(request_rec *r, mod_chxj_config *UNUSED(m))
{
  char query[MAX_STRING_LEN];
  if (r) DBG(r, "start chxj_cookie_unlock_mysql()");
  apr_snprintf(query, sizeof(query)-1, "UNLOCK TABLES");
  if (mysql_query(connection.handle, query) != 0) {
     if (r) {
       ERR(r, "MySQL WARN: %s: %s", mysql_error(connection.handle), r->uri);
       DBG(r, "end chxj_cookie_unlock_mysql()");
     }
    return CHXJ_FALSE;
  }
  if (r) DBG(r, "end chxj_cookie_unlock_mysql()");
  return CHXJ_TRUE;
}
#endif
/*
 * vim:ts=2 et
 */
