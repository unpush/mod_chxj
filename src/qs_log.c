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
#include <stdio.h>
#include <unistd.h>
#include "qs_log.h"
#include "qs_malloc.h"
#include "http_log.h"
#define USE_LOG
/*
#ifndef DEBUG
#define DEBUG
#endif
*/

extern apr_status_t chxj_init_module_kill(void *data);

QS_EXPORT void
qs_log(Doc* doc, int log_level,const char* f, int l, char* msg) 
{
  switch(log_level) {
  case QX_LOG_DEBUG:
#ifdef USE_LOG
#ifdef DEBUG
    do {
      FILE* fp = fopen("/tmp/debug.log", "a");
      fprintf(fp, "DBG:[%20.20s][%10d]: - <%s>\n", f,l,msg);
      fclose(fp);
    }
    while(0);
#endif
#endif
    break;
  case QX_LOG_INFO:
#ifdef USE_LOG
    do {
      FILE* fp = fopen("/tmp/debug.log", "a");
      fprintf(fp, "INF:[%20.20s][%10d]: - <%s>\n", f,l,msg);
      fclose(fp);
    }
    while(0);
#endif
    break;
  case QX_LOG_WARN:
#ifdef USE_LOG
    do {
      FILE* fp = fopen("/tmp/debug.log", "a");
      fprintf(fp, "WRN:[%20.20s][%10d]: - <%s>\n", f,l,msg);
      fclose(fp);
    }
    while(0);
#endif
    break;
  case QX_LOG_ERROR:
#ifdef USE_LOG
    do {
      FILE* fp = fopen("/tmp/debug.log", "a");
      fprintf(fp, "ERR:[%20.20s][%10d]: - <%s>\n", f,l,msg);
      fclose(fp);
    }
    while(0);
#endif
    break;
  case QX_LOG_FATAL:
#ifdef USE_LOG
    do {
      FILE* fp = fopen("/tmp/debug.log", "a");
      fprintf(fp, "FTL:[%20.20s][%10d]: - <%s>\n", f,l,msg);
      fclose(fp);
    }
    while(0);
#endif
    if (doc->r != NULL)
    {
      ap_log_rerror(f,l, APLOG_CRIT, 0, doc->r, msg);
      chxj_init_module_kill((void*)doc->r->server);
    }
    qs_all_free(doc, QX_LOGMARK);
    _exit(0);
  }
}

void
qs_log_int(Doc* doc, int log_level,const char* f, int l, char*msg, int val) 
{
#if 0
#ifdef USE_LOG
  switch(log_level) {
  case QX_LOG_DEBUG:
#ifdef DEBUG
    fprintf(stderr, "DBG:[%20.20s][%10d]: - <%s:%d>\n", f,l,msg, val);
#endif
    break;
  case QX_LOG_INFO:
    fprintf(stderr, "INF:[%20.20s][%10d]: - <%s:%d>\n", f,l,msg,val);
    break;
  case QX_LOG_WARN:
    fprintf(stderr, "WRN:[%20.20s][%10d]: - <%s:%d>\n", f,l,msg,val);
    break;
  case QX_LOG_ERROR:
    fprintf(stderr, "ERR:[%20.20s][%10d]: - <%s:%d>\n", f,l,msg,val);
    break;
  case QX_LOG_FATAL:
    fprintf(stderr, "FTL:[%20.20s][%10d]: - <%s:%d>\n", f,l,msg,val);
    _exit(0);
    break;
  }
#endif
#endif
}
