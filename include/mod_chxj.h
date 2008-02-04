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

#define DEBUG_FD(format, args...) do { \
  FILE*fp=fopen("/tmp/mod_chxj.log","a");       \
  fprintf(fp,"%s:%d ",__FILE__,__LINE__);       \
  fprintf(fp,format, ##args);                   \
  fprintf(fp,"\n");                             \
  fclose(fp);                                   \
} while(0)

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#define STRCASEEQ(a,b,c,d) \
  ((((a) == *(d))|| ((b) == *(d))) && strcasecmp((c),(d)) == 0)
#define STRNCASEEQ(a,b,c,d,e) \
  ((((a) == *(d))|| ((b) == *(d))) && strncasecmp((c),(d),(e)) == 0)

#define IS_SJIS_STRING(str) \
  (  STRCASEEQ('s','S',"sjis",           (str)) \
  || STRCASEEQ('s','S',"shift-jis",      (str)) \
  || STRCASEEQ('s','S',"shift_jis",      (str)) \
  || STRCASEEQ('s','S',"shift_jisx0213", (str)) \
  || STRCASEEQ('s','S',"sjis-open",      (str)) \
  || STRCASEEQ('s','S',"sjis-win",       (str)) \
  || STRCASEEQ('c','C',"cp932",          (str)) \
  || STRCASEEQ('m','M',"ms_kanji",       (str)) \
  || STRCASEEQ('m','M',"ms932",          (str)) \
  || STRCASEEQ('c','C',"csshiftjis",     (str)) \
  || STRCASEEQ('c','C',"cswindows31j",   (str)) \
  || STRCASEEQ('w','W',"windows-31j",    (str)))

#define IS_EUCJP_STRING(str) \
  (  STRCASEEQ('e','E',"euc-jp",              (str)) \
  || STRCASEEQ('e','E',"eucjp-win",           (str)) \
  || STRCASEEQ('e','E',"eucjp-ms",            (str)) \
  || STRCASEEQ('e','E',"eucjp-open",          (str)) \
  || STRCASEEQ('e','E',"euc-jp-win",          (str)) \
  || STRCASEEQ('c','C',"cseucpkdfmtjapanese", (str)) \
  || STRCASEEQ('e','E',"euc-jisx0213",        (str)) \
  || STRCASEEQ('e','E',"euc-jp-ms",           (str)) \
  || STRCASEEQ('e','E',"cseucpkdfmtjapanese", (str)))

#define IS_UTF8_STRING(str) \
  (  STRCASEEQ('u','U',"utf8",                (str)) \
  || STRCASEEQ('u','U',"utf-8",               (str)))

#define MOD_CHXJ_INTERNAL_ENCODING      "CP932"

#define GET_SPEC_CHARSET(spec) ((spec) ? (spec)->charset : "")
#define GET_EMOJI_INDEX(conf) ((conf)->emoji_index)

#define NONE  (-1)
#define SJIS  (0)
#define EUCJP (1)
#define UTF8  (2)

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
#if !defined(AP_NEED_SET_MUTEX_PERMS)
#  if !defined(OS2) && !defined(WIN32) && !defined(BEOS) && !defined(NETWARE)
#    define AP_NEED_SET_MUTEX_PERMS
#  endif
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
  char *(*start_tag_handler)(void *doc, Node *node); 
  char *(*end_tag_handler)(void *doc, Node *node); 
};

#include "chxj_specified_device.h"

typedef struct {
  spec_type    type; 
  tag_handler *handler;
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

#include "chxj_emoji.h"

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
  ezweb2imode_t         *ezweb2imode;
  ezweb2imode_t         *ezweb2imode_tail;
  softbank2imode_t      *softbank2imode;
  softbank2imode_t      *softbank2imode_tail;
  char                  *server_side_encoding;

  char                  *dir; /* for LOG */

  apr_array_header_t    *convrules;

  char                  *cookie_db_dir;
  long                  cookie_timeout;

  /* Index array for Emoji retrieval */
  emoji_t               *emoji_index[EMOJI_COUNT]; 
  emoji_t               *emoji_index_eucjp[EMOJI_COUNT]; 
  emoji_t               *emoji_index_sjis[EMOJI_COUNT]; 
  emoji_t               *emoji_index_utf8[EMOJI_COUNT]; 

  ezweb2imode_t         *emoji_ezweb2imode_sjis[EMOJI_EZWEB2IMODE_COUNT];
  ezweb2imode_t         *emoji_ezweb2imode_utf8[EMOJI_EZWEB2IMODE_COUNT];
  softbank2imode_t      *emoji_softbank2imode_webcode[EMOJI_SOFTBANK2IMODE_COUNT];
  softbank2imode_t      *emoji_softbank2imode_sjis[EMOJI_SOFTBANK2IMODE_COUNT];
  softbank2imode_t      *emoji_softbank2imode_utf8[EMOJI_SOFTBANK2IMODE_COUNT];
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
  apr_global_mutex_t    *cookie_db_lock;
} mod_chxj_global_config;

typedef struct {
  apr_size_t len;

  apr_bucket_brigade *bb;

  char *buffer;
} mod_chxj_ctx;

#include "chxj_tag_util.h"

#define CHXJ_MOD_CONFIG_KEY   "chxj_module_key"

#define HTTP_USER_AGENT       "User-Agent"
#define CHXJ_HTTP_USER_AGENT  "CHXJ_HTTP_USER_AGENT"

module AP_MODULE_DECLARE_DATA chxj_module;

#define CHXJ_IMG_ON     (1)
#define CHXJ_IMG_OFF    (0)


#define DBG(rec,format, args...)  \
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG,0,(request_rec*)(rec),(format), ##args)
#define SDBG(rec,format, args...) \
  ap_log_error(APLOG_MARK,APLOG_DEBUG,0,(rec),(format), ##args)
#define ERR(rec,format, args...)  \
  ap_log_rerror(APLOG_MARK,APLOG_ERR,0,(request_rec*)(rec),(format), ##args)
#define SERR(rec,format, args...) \
  ap_log_error(APLOG_MARK,APLOG_ERR,0,(rec),(format), ##args)

extern tag_handlers chxj_tag_handlers[];
extern tag_handler  chtml10_handler[];
extern tag_handler  chtml20_handler[];
extern tag_handler  chtml30_handler[];
extern tag_handler  xhtml_handler[];
extern tag_handler  hdml_handler[];
extern tag_handler  jhtml_handler[];

extern char *chxj_node_exchange( 
  device_table *spec,
  request_rec  *r,
  void         *pdoc,
  Doc          *doc,
  Node         *node,
  int         indent
);



#endif
/*
 * vim:ts=2 et
 */
