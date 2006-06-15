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
#ifndef __CHXJ_SPECIFIED_DEVICE_H__
#define __CHXJ_SPECIFIED_DEVICE_H__

typedef enum _spec_type_t {
  CHXJ_SPEC_UNKNOWN=0,
  CHXJ_SPEC_Chtml_1_0,
  CHXJ_SPEC_Chtml_2_0,
  CHXJ_SPEC_Chtml_3_0,
  CHXJ_SPEC_Chtml_4_0,
  CHXJ_SPEC_Chtml_5_0,
  CHXJ_SPEC_XHtml_Mobile_1_0,
  CHXJ_SPEC_Hdml,
  CHXJ_SPEC_Jhtml,
  CHXJ_SPEC_HTML,
} spec_type_t;

#define CHXJ_PIC_OK                (0x01)
#define CHXJ_PIC_NG                (0x00)


typedef struct device_table_t {
  struct device_table_t* next;
  const char* device_id;
  const char* device_name;
  spec_type_t html_spec_type;
  int width;
  int heigh;
  /*--------------------------------------------------------------------------*/
  /* Walll Paper Size                                                         */
  /*--------------------------------------------------------------------------*/
  int wp_width;
  int wp_heigh;
  /*--------------------------------------------------------------------------*/
  /* Cache Size                                                               */
  /*--------------------------------------------------------------------------*/
  int cache;
  /*--------------------------------------------------------------------------*/
  /* Correspondence image format                                              */
  /* 1: It is possible to display it.                                         */
  /* 0: It is not possible to display it.                                     */
  /*--------------------------------------------------------------------------*/
  int available_gif;
  int available_jpeg;
  int available_png;
  int available_bmp2;
  int available_bmp4;
  /*--------------------------------------------------------------------------*/
  /* Resolution                                                               */
  /*--------------------------------------------------------------------------*/
  int dpi_width;
  int dpi_heigh;
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
  int color;
  char* emoji_type;
} device_table_t;

typedef struct device_table_list_t {
  struct device_table_list_t* next;
  char* pattern;
  device_table_t* table;
  device_table_t* tail;
} device_table_list_t;

device_table_t* chxj_specified_device(request_rec* r, const char* user_agent);

#endif
