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
#ifndef __CHXJ_COOKIE_H__
#define __CHXJ_COOKIE_H__

#include "mod_chxj.h"
#include "apr_file_io.h"
#include "apr_tables.h"

#define CHXJ_COOKIE_PARAM  "_chxj_cc"


typedef struct cookie_t cookie_t;


struct cookie_t {
  char*               cookie_id;
  apr_array_header_t* cookie_headers;
};

extern cookie_t* 
chxj_save_cookie(request_rec *r);
extern char*     chxj_add_cookie_parameter(request_rec* r, char* value, cookie_t* cookie);
extern int       chxj_cookie_check_host(request_rec* r, char* value);
extern cookie_t* chxj_load_cookie(request_rec* r, char* cookie_id);
extern apr_file_t* chxj_cookie_db_lock(request_rec* r);
extern void chxj_cookie_db_unlock(request_rec* r, apr_file_t* file);
extern void chxj_delete_cookie(request_rec* r, char* cookie_id);

#endif
/*
 * vim:ts=2 et
 */
