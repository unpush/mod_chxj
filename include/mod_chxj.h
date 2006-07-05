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

#if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#define AP_NEED_SET_MUTEX_PERMS
#endif


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

#if defined(AP_NEED_SET_MUTEX_PERMS)
#  include "unixd.h"
#endif

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
  char       *pattern;
  ap_regex_t *regexp;
  int        flags;
  int        action;
  char       *encoding;
  int        pc_flag;
  char       *user_agent;
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
} tag_type;

typedef struct mod_chxj_config mod_chxj_config;

struct mod_chxj_config {
  int                   image;

  char*                 device_data_file;
  char*                 emoji_data_file;

  char*                 image_cache_dir;
  char*                 image_copyright;
  device_table_list*    devices;
  emoji_t*              emoji;
  emoji_t*              emoji_tail;
  char*                 server_side_encoding;

  char*                 dir; /* for LOG */

  apr_array_header_t   *convrules;

  char*                 cookie_db_dir;
  long                  cookie_timeout;
};


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
  unsigned int len;

  apr_bucket_brigade *bb;

  char* buffer;
} mod_chxj_ctx;

#include "chxj_tag_util.h"

#define CHXJ_MOD_CONFIG_KEY   "chxj_module_key"

#define HTTP_USER_AGENT       "User-Agent"
#define CHXJ_HTTP_USER_AGENT  "CHXJ_HTTP_USER_AGENT"

module AP_MODULE_DECLARE_DATA chxj_module;

#define CHXJ_IMG_ON     (1)
#define CHXJ_IMG_OFF    (0)


#define DBG(X,Y)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(request_rec*)(X),(Y))
#define DBG1(X,Y,Za)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za))
#define DBG2(X,Y,Za,Zb)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb))
#define DBG3(X,Y,Za,Zb,Zc)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc))
#define DBG4(X,Y,Za,Zb,Zc,Zd)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc),(Zd))
#define DBG5(X,Y,Za,Zb,Zc,Zd,Ze)  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc),(Zd),(Ze))
#define SDBG(X,Y)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y))
#define SDBG1(X,Y,Za)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za))
#define SDBG2(X,Y,Za,Zb)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb))
#define SDBG3(X,Y,Za,Zb,Zc)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc))
#define SDBG4(X,Y,Za,Zb,Zc,Zd)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc),(Zd))
#define SDBG5(X,Y,Za,Zb,Zc,Zd,Ze)  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(X),(Y),(Za),(Zb),(Zc),(Zd),(Ze))
#define ERR(X,Y)  ap_log_rerror(APLOG_MARK,APLOG_ERR,0,(X),(Y))
#define ERR1(X,Y,Za)  ap_log_rerror(APLOG_MARK,APLOG_ERR,0,(X),(Y),(Za))
#define ERR2(X,Y,Za,Zb)  ap_log_rerror(APLOG_MARK,APLOG_ERR,0,(X),(Y),(Za),(Zb))
#define SERR(X,Y)  ap_log_error(APLOG_MARK,APLOG_ERR,0,(X),(Y))
#define SERR1(X,Y,Za)  ap_log_error(APLOG_MARK,APLOG_ERR,0,(X),(Y),(Za))
#define SERR2(X,Y,Za,Zb)  ap_log_error(APLOG_MARK,APLOG_ERR,0,(X),(Y),(Za),(Zb))

extern tag_handlers chxj_tag_handlers[];
extern tag_handler chtml10_handler[];
extern tag_handler chtml20_handler[];
extern tag_handler chtml30_handler[];
extern tag_handler xhtml_handler[];
extern tag_handler hdml_handler[];
extern tag_handler jhtml_handler[];

extern char* chxj_node_exchange( 
  device_table* spec,
  request_rec*  r,
  void*         pdoc,
  Doc*          doc,
  Node*         node,
  int           indent
);



#endif
/*
 * vim:ts=2 et
 */
