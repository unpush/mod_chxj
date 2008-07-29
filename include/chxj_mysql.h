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
#ifndef __CHXJ_MYSQL_H__
#define __CHXJ_MYSQL_H__

#include <mysql.h>
#include "mod_chxj.h"
#include "apr.h"


typedef struct {
  char *host;
  int  port;
  char *database;
  char *username;
  char *password;
  char *tablename;
  int  keep_alive;
  char *socket_path;
  char *charset;
} mysql_t;

#define DEFAULT_MYSQL_HOST "localhost"
#define DEFAULT_MYSQL_CHARSET "utf8"

extern int chxj_open_mysql_handle(request_rec *r, mod_chxj_config *m);
extern void chxj_close_mysql_handle();
extern int chxj_mysql_exist_cookie_table(request_rec *r, mod_chxj_config *m);
extern int chxj_mysql_create_cookie_table(request_rec *r, mod_chxj_config *m);
extern char *chxj_mysql_get_cookie_from_cookie_id(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_mysql_insert_or_update_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *data);
extern int chxj_mysql_rollback(request_rec *r, mod_chxj_config *m);
extern char *chxj_mysql_load_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_mysql_delete_cookie(request_rec *r, mod_chxj_config *m, const char *cookie_id);

/* expire table */
extern int chxj_mysql_create_cookie_expire_table(request_rec *r, mod_chxj_config *m);
extern char *chxj_mysql_get_cookie_expire_from_cookie_id(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_mysql_insert_or_update_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern char *chxj_mysql_load_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_mysql_delete_cookie_expire(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_mysql_exist_cookie_table_expire(request_rec *r, mod_chxj_config *m);
extern char *chxj_mysql_get_timeout_localtime(request_rec *r, mod_chxj_config *m);
extern int chxj_mysql_delete_expired_cookie(request_rec *r, mod_chxj_config *m);


/* operation for cookie */
extern int chxj_save_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern int chxj_update_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern char *chxj_load_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_save_cookie_expire_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_expire_mysql(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_cookie_expire_gc_mysql(request_rec *r, mod_chxj_config *m);

/* lock for cookie */
extern int chxj_cookie_lock_mysql(request_rec *r, mod_chxj_config *m);
extern int chxj_cookie_unlock_mysql(request_rec *r, mod_chxj_config *UNUSED(m));
#endif
#endif
