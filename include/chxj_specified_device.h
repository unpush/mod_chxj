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
#ifndef __CHXJ_SPECIFIED_DEVICE_H__
#define __CHXJ_SPECIFIED_DEVICE_H__

typedef enum {
  CHXJ_SPEC_UNKNOWN=0,
  CHXJ_SPEC_Chtml_1_0,
  CHXJ_SPEC_Chtml_2_0,
  CHXJ_SPEC_Chtml_3_0,
  CHXJ_SPEC_Chtml_4_0,
  CHXJ_SPEC_Chtml_5_0,
  CHXJ_SPEC_Chtml_6_0, /* use XHTML */
  CHXJ_SPEC_Chtml_7_0, /* use XHTML */
  CHXJ_SPEC_XHtml_Mobile_1_0,
  CHXJ_SPEC_Hdml,
  CHXJ_SPEC_Jhtml,
  CHXJ_SPEC_Jxhtml,    /* use XHTML */
  CHXJ_SPEC_HTML,
} spec_type;

#define CHXJ_PIC_OK                (0x01)
#define CHXJ_PIC_NG                (0x00)

#include "mod_chxj.h"
#include "chxj_cookie.h"


typedef struct device_table_t device_table;

struct device_table_t {
  struct device_table_t* next;
  const char*            device_id;
  const char*            device_name;
  spec_type              html_spec_type;
  int                    width;
  int                    heigh;
  /*--------------------------------------------------------------------------*/
  /* Walll Paper Size                                                         */
  /*--------------------------------------------------------------------------*/
  int                    wp_width;
  int                    wp_heigh;
  /*--------------------------------------------------------------------------*/
  /* Cache Size                                                               */
  /*--------------------------------------------------------------------------*/
  int                    cache;
  /*--------------------------------------------------------------------------*/
  /* Correspondence image format                                              */
  /* 1: It is possible to display it.                                         */
  /* 0: It is not possible to display it.                                     */
  /*--------------------------------------------------------------------------*/
  int                    available_gif;
  int                    available_jpeg;
  int                    available_png;
  int                    available_bmp2;
  int                    available_bmp4;
  /*--------------------------------------------------------------------------*/
  /* Resolution                                                               */
  /*--------------------------------------------------------------------------*/
  int                    dpi_width;
  int                    dpi_heigh;
  /*--------------------------------------------------------------------------*/
  /* Color number type                                                        */
  /* 2       : 2        Colors                                                */
  /* 4       : 4        Colors                                                */
  /* 256     : 256      Colors                                                */
  /* 4096    : 4096     Colors                                                */
  /* 65536   : 65536    Colors                                                */
  /* 262144  : 262144   Colors                                                */
  /* 15680000: 15680000 over colors                                           */
  /*--------------------------------------------------------------------------*/
  int                    color;
  char*                  emoji_type;
};

typedef struct device_table_list_t  device_table_list;

struct device_table_list_t {
  struct device_table_list_t  *next;

  char                        *pattern;
  ap_regex_t                  *regexp;
  device_table                *table;
  device_table                *tail;
};

typedef struct converter_t converter_t;

struct converter_t {
  /* convert routine */
  char* (*converter)(request_rec                 *r,
                     struct device_table_t       *spec,
                     const char                  *src, 
                     apr_size_t                  srclen, 
                     apr_size_t                  *dstlen,
                     struct chxjconvrule_entry   *entryp,
                     cookie_t                    *cookie);

  char* (*encoder)(request_rec  *r,
                   const char   *src,
                   apr_size_t   *len);
};

extern converter_t convert_routine[];

extern device_table* chxj_specified_device(
  request_rec             *r, 
  const char              *user_agent);

#endif
