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
#ifndef __CHXJ_CHTM_1_0_H__
#define __CHXJ_CHTM_1_0_H__

/*#define DUMP_CHTML10 "" */
#include "mod_chxj.h"
#include "chxj_cookie.h"

/*----------------------------------------------------------------------------*/
/* Structure for HDML                                                         */
/*----------------------------------------------------------------------------*/
typedef struct _ctml10_t {
    Doc*               doc;
    char*              out;
    int                 out_len;
    int                 pre_flag;
    int                 textarea_flag;

    device_table*       spec;
    mod_chxj_config*    conf;
    chxjconvrule_entry* entryp;
    cookie_t*           cookie;
} chtml10_t;

/*----------------------------------------------------------------------------*/
/* Prototype                                                                  */
/*----------------------------------------------------------------------------*/
char* chxj_exchange_chtml10( request_rec* r, device_table* spec, const char* src, apr_size_t srclen, apr_size_t *dstlen, chxjconvrule_entry* , cookie_t*);
#endif
