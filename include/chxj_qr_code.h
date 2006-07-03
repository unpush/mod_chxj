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
#ifndef __CHXJ_QR_CODE_H__
#define __CHXJ_QR_CODE_H__

#include "httpd.h"
#include "qs_parse_string.h"

typedef enum _qr_ver_t {
  QR_VER_1=0,
  QR_VER_2,
  QR_VER_3,
  QR_VER_4,
  QR_VER_5,
  QR_VER_6,
  QR_VER_7,
  QR_VER_8,
  QR_VER_9,
  QR_VER_10,
  QR_VER_11,
  QR_VER_12,
  QR_VER_13,
  QR_VER_14,
  QR_VER_15,
  QR_VER_16,
  QR_VER_17,
  QR_VER_18,
  QR_VER_19,
  QR_VER_20,
  QR_VER_21,
  QR_VER_22,
  QR_VER_23,
  QR_VER_24,
  QR_VER_25,
  QR_VER_26,
  QR_VER_27,
  QR_VER_28,
  QR_VER_29,
  QR_VER_30,
  QR_VER_31,
  QR_VER_32,
  QR_VER_33,
  QR_VER_34,
  QR_VER_35,
  QR_VER_36,
  QR_VER_37,
  QR_VER_38,
  QR_VER_39,
  QR_VER_40,
} qr_ver_t;

typedef enum _qr_level_t {
  QR_LEVEL_L=0,
  QR_LEVEL_M,
  QR_LEVEL_Q,
  QR_LEVEL_H,
} qr_level_t;

/*----------------------------------------------------------------------------*/
/* ¥â¡¼¥É                                                                     */
/*----------------------------------------------------------------------------*/
typedef enum _qr_mode_t {
  QR_NUM_MODE = 0,
  QR_ALPHA_MODE,
  QR_8BIT_MODE,
  QR_KANJI_MODE,
  QR_ECI_MODE,
  QR_FNC1_1_MODE,
  QR_FNC1_2_MODE,
  QR_STR_CAT_MODE,
  QR_END_PTN_MODE,
  QR_RSV1_MODE,
  QR_RSV2_MODE,
  QR_RSV3_MODE,
  QR_RSV4_MODE,
  QR_RSV5_MODE,
  QR_RSV6_MODE,
  QR_RSV7_MODE,
} qr_mode_t;

typedef enum _qr_code_found_t {
  QR_NOT_FOUND = 0,
  QR_FOUND,
} qr_code_found_t;

typedef enum _qr_mode_change_t {
  QR_NOT_CHANGE = 0,
  QR_CHANGE,
} qr_mode_change_t;

typedef struct qr_code_t qr_code_t;

struct qr_code_t {
  qr_code_found_t  found;
  qr_mode_change_t mode_change;

  qr_ver_t         version;
  qr_level_t       level;
  qr_mode_t        mode;
  request_rec*     r;
  Doc*             doc;
  int              size;
  char*            indata;
};

extern int     chxj_qr_code_handler            (request_rec* r);
extern int     chxj_qrcode_create_image_data   (qr_code_t* qrcode, char** img, size_t* img_len);
void    chxj_qrcode_node_to_qrcode      (qr_code_t* qrcode, Node* root);
char*   chxj_qr_code_blob_handler       (request_rec* r, const char* indata, size_t* len);

#endif
