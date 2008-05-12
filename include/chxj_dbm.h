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
#ifndef __CHXJ_DBM_H__
#define __CHXJ_DBM_H__

#include "mod_chxj.h"
#include "apr.h"
#include "apu.h"
#include "apr_dbm.h"
#include "apr_uuid.h"
#include "apr_md5.h"
#include "apr_base64.h"
#include "apr_uri.h"
#include "apr_time.h"
#include "apr_date.h"

extern void chxj_cookie_db_unlock(request_rec *r, apr_file_t *file);
extern apr_file_t *chxj_cookie_db_lock(request_rec *r);
extern char *chxj_cookie_db_lock_name_create(request_rec *r, const char *dir);
extern char *chxj_cookie_db_name_create(request_rec *r, const char *dir);
extern char *chxj_cookie_expire_db_lock_name_create(request_rec *r, const char *dir);
extern apr_file_t *chxj_cookie_expire_db_lock(request_rec *r);
extern void chxj_cookie_expire_db_unlock(request_rec *r, apr_file_t *file);

/* operation for cookie */
extern int chxj_save_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern int chxj_update_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id, const char *store_string);
extern char *chxj_load_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_save_cookie_expire_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_delete_cookie_expire_dbm(request_rec *r, mod_chxj_config *m, const char *cookie_id);
extern int chxj_cookie_expire_gc_dbm(request_rec *r, mod_chxj_config *m);
extern int chxj_cookie_lock_dbm(request_rec *r, mod_chxj_config *m);
extern int chxj_cookie_unlock_dbm(request_rec *r, mod_chxj_config *m);
#endif
