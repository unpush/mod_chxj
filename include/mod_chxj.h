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
#ifndef __MOD_CHXJ_H__
#define __MOD_CHXJ_H__

#define CHXJ_TRUE    (1)
#define CHXJ_FALSE   (0)

/* #define DEBUG_FD(X,args...) {FILE*fp=fopen("/tmp/error.log", "ab");fprintf(fp, X, ##args);fflush(fp);fclose(fp);} */

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


#include <string.h>

#include "chxj_apache.h"

#include "apr_strings.h"
#include "apr_buckets.h"
#include "apr_lib.h"
#include "apr_tables.h"
#include "apr_dso.h"
#include "apr_general.h"
#include "apr_pools.h"

#if defined(AP_NEED_SET_MUTEX_PERMS)
#  include "unixd.h"
#endif
#if !defined(AP_NEED_SET_MUTEX_PERMS)
#  if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#    define AP_NEED_SET_MUTEX_PERMS
#  endif
#endif

#define STRCASEEQ(a,b,c,d) \
  ((((a) == *(d))|| ((b) == *(d))) && strcasecmp((c),(d)) == 0)
#define STRNCASEEQ(a,b,c,d,e) \
  ((((a) == *(d))|| ((b) == *(d))) && strncasecmp((c),(d),(e)) == 0)

#include "qs_ignore_sp.h"
#include "qs_log.h"
#include "qs_malloc.h"
#include "qs_parse_attr.h"
#include "qs_parse_file.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"

#if defined(HAVE_AP_REGEX_H) && HAVE_AP_REGEX_H == 1
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




#ifdef HAVE_APR_GLOBAL_MUTEX_H
#  include "apr_global_mutex.h"
#endif

#ifdef HAVE_APR_SHM_H
#  include "apr_shm.h"
#endif


typedef struct imode_emoji_t imode_emoji_t;

struct imode_emoji_t {
  char  hex1byte;
  char  hex2byte;
  char* string;
  char *description;
};

typedef struct ezweb_emoji_t ezweb_emoji_t;

struct ezweb_emoji_t {
  char* typeA;
  char* typeB;
  char* typeC;
  char* typeD;
};

typedef struct jphone_emoji_t jphone_emoji_t;

struct jphone_emoji_t {
  char* string;
};

typedef struct emoji_t emoji_t;

struct emoji_t {
  struct emoji_t*  next;
  int              no;
  imode_emoji_t*   imode;
  ezweb_emoji_t*   ezweb;
  jphone_emoji_t*  jphone;
};

typedef struct chxjconvrule_entry chxjconvrule_entry;

struct chxjconvrule_entry {
  char         *pattern;
  ap_regex_t   *regexp;
  int          flags;
  int          action;
  char         *encoding;
  int          pc_flag;
  char         *user_agent;
};

typedef struct tag_handler tag_handler;

struct tag_handler {
  char* (*start_tag_handler)(void* doc, Node* node); 
  char* (*end_tag_handler)(void* doc, Node* node); 
};

#include "chxj_specified_device.h"

typedef struct {
  spec_type    type; 
  tag_handler* handler;
} tag_handlers;

typedef enum {
  tagHTML = 0,
  tagMETA,  
  tagTEXTAREA,
  tagP,
  tagPRE,
  tagUL,
  tagLI,
  tagOL,
  tagH1,
  tagH2,
  tagH3,
  tagH4,
  tagH5,
  tagH6,
  tagHEAD,
  tagTITLE,
  tagBASE,
  tagBODY,
  tagA,
  tagBR,
  tagTABLE,
  tagTR,
  tagTD,
  tagTBODY,
  tagFONT,
  tagFORM,
  tagINPUT,
  tagCENTER,
  tagHR,
  tagIMG,
  tagSELECT,
  tagOPTION,
  tagDIV,
  tagCHXJIF,  
  tagNOBR,  
  tagSMALL,  
  tagSTYLE,  
  tagSPAN,  
  tagTEXT,  
  tagTH,  
  tagB,
  tagFIELDSET,
  tagDT,
  tagLEGEND,
  tagLABEL,
  tagBLOCKQUOTE,
  tagDIR,
  tagDL,
} tag_type;

typedef struct mod_chxj_config mod_chxj_config;

#if defined(USE_MYSQL_COOKIE)
#  include "chxj_mysql.h"
#endif
#if defined(USE_MEMCACHE_COOKIE)
#  include "chxj_memcache.h"
#endif

/* cookie store type */
#define CHXJ_COOKIE_STORE_TYPE_DBM      "dbm"
#define CHXJ_COOKIE_STORE_TYPE_MYSQL    "mysql"
#define CHXJ_COOKIE_STORE_TYPE_MEMCACHE "memcache"
typedef enum {
  COOKIE_STORE_TYPE_NONE     = 0,
  COOKIE_STORE_TYPE_DBM      = 1,
  COOKIE_STORE_TYPE_MYSQL    = 2,
  COOKIE_STORE_TYPE_MEMCACHE = 3, 
} cookie_store_type_t;

struct mod_chxj_config {
  int                   image;

  char                  *device_data_file;
  char                  *emoji_data_file;

  char                  *image_cache_dir;
  char                  *image_copyright;
  unsigned long         image_cache_limit;
  device_table_list     *devices;
  emoji_t               *emoji;
  emoji_t               *emoji_tail;
  char                  *server_side_encoding;

  char                  *dir; /* for LOG */

  apr_array_header_t    *convrules;

  char                  *cookie_db_dir;
  long                  cookie_timeout;
  cookie_store_type_t   cookie_store_type;
  int                   cookie_lazy_mode;

#if defined(USE_MYSQL_COOKIE)
  mysql_t               mysql;
#endif
#if defined(USE_MEMCACHE_COOKIE)
  memcache_t            memcache;
#endif
};

#define IS_COOKIE_STORE_DBM(X)      ((X) == COOKIE_STORE_TYPE_DBM)
#define IS_COOKIE_STORE_MYSQL(X)    ((X) == COOKIE_STORE_TYPE_MYSQL)
#define IS_COOKIE_STORE_MEMCACHE(X) ((X) == COOKIE_STORE_TYPE_MEMCACHE)
#define IS_COOKIE_STORE_NONE(X)     ((X) == COOKIE_STORE_TYPE_NONE)

#define COOKIE_LAZY_ON                (2)
#define COOKIE_LAZY_OFF               (1)
#define IS_COOKIE_LAZY(X)             ((X)->cookie_lazy_mode == COOKIE_LAZY_ON)


#define CONVRULE_ENGINE_ON_BIT        (0x00000001)
#define CONVRULE_ENGINE_OFF_BIT       (0x00000002)
#define CONVRULE_COOKIE_ON_BIT        (0x00000004)

#define CONVRULE_ENGINE_ON_CMD        "EngineOn"
#define CONVRULE_ENGINE_OFF_CMD       "EngineOff"
#define CONVRULE_COOKIE_ON_CMD        "CookieOn"
#define CONVRULE_COOKIE_OFF_CMD       "CookieOff"


#define CONVRULE_FLAG_NOTMATCH        (0x00000001)

#define CONVRULE_PC_FLAG_ON_CMD       "PC"

#define CONVRULE_PC_FLAG_ON_BIT       (0x00000001)
#define CONVRULE_PC_FLAG_OFF_BIT      (0x00000002)


typedef struct {
  apr_global_mutex_t* cookie_db_lock;
} mod_chxj_global_config;

typedef struct {
  apr_size_t len;
  chxjconvrule_entry  *entryp;
  device_table        *spec;

  apr_bucket_brigade *bb;
  apr_pool_t *pool;

  char* buffer;
} mod_chxj_ctx;

#include "chxj_tag_util.h"

#define CHXJ_MOD_CONFIG_KEY   "chxj_module_key"

#define HTTP_USER_AGENT       "User-Agent"
#define HTTP_HOST             "Host"
#define CHXJ_HTTP_USER_AGENT  "CHXJ_HTTP_USER_AGENT"

module AP_MODULE_DECLARE_DATA chxj_module;

#define CHXJ_IMG_ON     (1)
#define CHXJ_IMG_OFF    (0)


#define DBG(X,args...)  chxj_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(request_rec*)(X),##args)
#define SDBG(X,Y)       chxj_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y))
#define ERR(X,args...)  chxj_log_rerror(APLOG_MARK,APLOG_ERR,0,(X), ##args)
#define SERR(X,Y)       chxj_log_error(APLOG_MARK,APLOG_ERR,0,(X),(Y))
#define WRN(rec,format,args...)  chxj_log_rerror(APLOG_MARK,APLOG_WARNING,0,(rec),(format), ##args)

extern tag_handlers chxj_tag_handlers[];
extern tag_handler  chtml10_handler[];
extern tag_handler  chtml20_handler[];
extern tag_handler  chtml30_handler[];
extern tag_handler  xhtml_handler[];
extern tag_handler  hdml_handler[];
extern tag_handler  jhtml_handler[];

extern char* chxj_node_exchange( 
  device_table* spec,
  request_rec*  r,
  void*         pdoc,
  Doc*          doc,
  Node*         node,
  int           indent
);

#define IMAGE_CACHE_LIMIT_FMT_LEN  (20)

#if HAVE_MALLOC == 0
extern void *rpl_malloc(size_t n);
#endif

#endif
/*
 * vim:ts=2 et
 */
