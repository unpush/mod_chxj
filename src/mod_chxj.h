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
#ifndef __MOD_CHXJ_H__
#define __MOD_CHXJ_H__


#include <string.h>

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_log.h"
#include "ap_config.h"
#include "apr_strings.h"
#include "apr_buckets.h"
#include "apr_lib.h"
#include "apr_tables.h"
#include "apr_dso.h"
#include "apr_general.h"
#include "apr_pools.h"
#include "util_filter.h"

#include "qs_ignore_sp.h"
#include "qs_log.h"
#include "qs_malloc.h"
#include "qs_parse_attr.h"
#include "qs_parse_file.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"

#ifndef __CONFIG_H__
#define __CONFIG_H__

#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include "config.h"
#endif
#ifdef HAVE_AP_REGEX_H
#  include "ap_regex.h"
#else
#  include "pcreposix.h"
#  include "pcre.h"
#  if !defined(AP_REG_ASSERT)
#    define AP_REG_ASSERT   REG_ASSERT
#  endif
#  if !defined(AP_REG_BADBR)
#    define AP_REG_BADBR    REG_BADBR
#  endif
#  if !defined(AP_REG_BADPAT)
#    define AP_REG_BADPAT   REG_BADPAT
#  endif
#  if !defined(AP_REG_BADRPT)
#    define AP_REG_BADRPT   REG_BADRPT
#  endif
#  if !defined(AP_REG_EBRACE)
#    define AP_REG_EBRACE   REG_EBRACE
#  endif
#  if !defined(AP_REG_EBRACK)
#    define AP_REG_EBRACK   REG_EBRACK
#  endif
#  if !defined(AP_REG_ECOLLATE)
#    define AP_REG_ECOLLATE REG_ECOLLATE
#  endif
#  if !defined(AP_REG_ECTYPE)
#    define AP_REG_ECTYPE   REG_ECTYPE
#  endif
#  if !defined(AP_REG_EESCAPE)
#    define AP_REG_EESCAPE  REG_EESCAPE
#  endif
#  if !defined(AP_REG_EMPTY)
#    define AP_REG_EMPTY    REG_EMPTY
#  endif
#  if !defined(AP_REG_EPAREN)
#    define AP_REG_EPAREN   REG_EPAREN
#  endif
#  if !defined(AP_REG_ERANGE)
#    define AP_REG_ERANGE   REG_ERANGE
#  endif
#  if !defined(AP_REG_ESIZE)
#    define AP_REG_ESIZE    REG_ESIZE
#  endif
#  if !defined(AP_REG_ESPACE)
#    define AP_REG_ESPACE   REG_ESPACE
#  endif
#  if !defined(AP_REG_ESUBREG)
#    define AP_REG_ESUBREG  REG_ESUBREG
#  endif
#  if !defined(AP_REG_INVARG)
#    define AP_REG_INVARG   REG_INVARG
#  endif
#  if !defined(AP_REG_NOMATCH)
#    define AP_REG_NOMATCH  REG_NOMATCH
#  endif

#  if !defined(AP_REG_ICASE)
#    define AP_REG_ICASE    REG_ICASE
#  endif
#  if !defined(AP_REG_NEWLINE)
#    define AP_REG_NEWLINE  REG_NEWLINE
#  endif
#  if !defined(AP_REG_NOTBOL)
#    define AP_REG_NOTBOL   REG_NOTBOL
#  endif
#  if !defined(AP_REG_NOTEOL)
#    define AP_REG_NOTEOL   REG_NOTEOL
#  endif

#  if !defined(AP_REG_EXTENDED)
#    define AP_REG_EXTENDED REG_EXTENDED
#  endif
#  if !defined(AP_REG_NOSUB)
#    define AP_REG_NOSUB    REG_NOSUB
#  endif

#  define ap_regex_t      regex_t
#  define ap_regmatch_t   regmatch_t
#endif


#include "chxj_specified_device.h"


#ifdef HAVE_APR_GLOBAL_MUTEX_H
#  include "apr_global_mutex.h"
#endif

#ifdef HAVE_APR_SHM_H
#  include "apr_shm.h"
#endif


typedef struct _imode_emoji_t {
  char  hex1byte;
  char  hex2byte;
  char* string;
  char *description;
} imode_emoji_t;

typedef struct _ezweb_emoji_t {
  char* typeA;
  char* typeB;
  char* typeC;
  char* typeD;
} ezweb_emoji_t;

typedef struct _jphone_emoji_t {
  char* string;
} jphone_emoji_t;

typedef struct _emoji_t {
  struct _emoji_t* next;
  int no;
  imode_emoji_t* imode;
  ezweb_emoji_t* ezweb;
  jphone_emoji_t* jphone;
} emoji_t;

typedef struct _mod_chxj_config_t 
{
  int                   image;

  char*                 device_data_file;
  char*                 emoji_data_file;

  char*                 image_cache_dir;
  char*                 image_copyright;
  device_table_list_t*  devices;
  emoji_t*              emoji;
  emoji_t*              emoji_tail;
  char*                 server_side_encoding;

} mod_chxj_config_t;

typedef struct _mod_chxj_global_config_t
{
  apr_shm_t*          client_shm;
  apr_global_mutex_t* client_lock;
  char                client_lock_file_name[256];
} mod_chxj_global_config_t;

typedef struct _mod_chxj_ctx_t
{
  unsigned int len;

  apr_bucket_brigade *bb;

  char* buffer;
} mod_chxj_ctx_t;

#include "chxj_tag_util.h"

#define CHXJ_MOD_CONFIG_KEY   "chxj_module"

#define HTTP_USER_AGENT       "User-Agent"

module AP_MODULE_DECLARE_DATA chxj_module;

#define CHXJ_IMG_ON     (1)
#define CHXJ_IMG_OFF    (0)


#endif
/*
 * vim:ts=2 et
 */
