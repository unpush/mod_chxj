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

#define CHXJ_SPEC_UNKNOWN          (0x00)
#define CHXJ_SPEC_Chtml_1_0        (0x11)
#define CHXJ_SPEC_Chtml_2_0        (0x12)
#define CHXJ_SPEC_Chtml_3_0        (0x13)
#define CHXJ_SPEC_Chtml_4_0        (0x14)
#define CHXJ_SPEC_Chtml_5_0        (0x15)
#define CHXJ_SPEC_XHtml_Mobile_1_0 (0x20)
#define CHXJ_SPEC_Hdml             (0x30)
#define CHXJ_SPEC_Jhtml            (0x40)
#define CHXJ_PIC_OK                (0x01)
#define CHXJ_PIC_NG                (0x00)


typedef struct device_table_t {
  struct device_table_t* next;
  const char* device_id;
  const char* device_name;
  int html_spec_type;
  int width;
  int heigh;
  int available_gif;
  int available_jpeg;
  int available_png;
  int available_bmp2;
  int available_bmp4;
  int color;
  char* emoji_type;
} device_table;

typedef struct device_table_list_t {
  struct device_table_list_t* next;
  char* pattern;
  device_table* table;
  device_table* tail;
} device_table_list;

device_table* chxj_specified_device(request_rec* r, const char* user_agent);

#endif
