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
#ifndef __QS_LOG_H__
#define __QS_LOG_H__
#include "qs_parse_string.h"

#define QX_LOGMARK   __FILE__,__LINE__
#define QX_LOG_DEBUG (0)
#define QX_LOG_INFO  (1)
#define QX_LOG_WARN  (2)
#define QX_LOG_ERROR (3)
#define QX_LOG_FATAL (4)


#define QX_LOGGER_DEBUG(c)  qs_log(doc,QX_LOG_DEBUG,QX_LOGMARK,(c))
#define QX_LOGGER_INFO(c)   qs_log(doc,QX_LOG_INFO,QX_LOGMARK, (c))
#define QX_LOGGER_WARN(c)   qs_log(doc,QX_LOG_WARN,QX_LOGMARK, (c))
#define QX_LOGGER_ERROR(c)  qs_log(doc,QX_LOG_ERROR,QX_LOGMARK, (c))
#define QX_LOGGER_FATAL(c)  qs_log(doc,QX_LOG_FATAL,QX_LOGMARK,(c))
#define QX_LOGGER_DEBUG_INT(x,y)  qs_log_int(doc,QX_LOG_DEBUG,QX_LOGMARK,(x),(y))

QS_EXPORT void qs_log(Doc* doc, int log_level,const char* f, int l, char* msg);
QS_EXPORT void qs_log_int(Doc* doc, int log_level,const char* f, int l, char* msg, int val);
#endif
