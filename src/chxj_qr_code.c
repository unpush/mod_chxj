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
#include <unistd.h>
#include <assert.h>
#include "apr_strings.h"
#include "httpd.h"
#include "http_protocol.h"
#include "http_log.h"

#include "chxj_str_util.h"
#include "chxj_qr_code.h"
#include "chxj_calc_ecc.h"
#include "mod_chxj.h"
#include "chxj_specified_device.h"
#include "qs_malloc.h"
#include "qs_log.h"
#include "qs_parse_file.h"

#include <wand/magick_wand.h>

/*
#define QR_CODE_DEBUG
*/
#define EXIT_MAGICK_ERROR() \
          do { \
             char *description; ExceptionType severity; \
             description=MagickGetException(magick_wand,&severity); \
             ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r,"%s %s %d %s\n",__FILE__,(__func__),__LINE__,description); \
             description=(char *) MagickRelinquishMemory(description); \
             DestroyMagickWand(magick_wand); \
          }while(1)

/*----------------------------------------------------------------------------*/
/* モード指示子テーブル                                                       */
/*----------------------------------------------------------------------------*/
static char* v_mode_table[] = {
/* 数字          */  "0001",
/* 英数字        */  "0010",
/* 8ビットバイト */  "0100",
/* 漢字          */  "1000",
/* ECI           */  "0111",
/* FNC1 1        */  "0101",
/* FNC1 2        */  "1001",
/* 構造的連接    */  "0011",
/* 終端パターン  */  "0000",
/* 予約          */  "0110",
/* 予約          */  "1010",
/* 予約          */  "1011",
/* 予約          */  "1100",
/* 予約          */  "1101",
/* 予約          */  "1110",
/* 予約          */  "1111",
};

/*----------------------------------------------------------------------------*/
/* 文字数指示子のビット数                                                     */
/*----------------------------------------------------------------------------*/
static char v_char_count_table[][4] = {
/* バージョン *//* 文字数指示子のビット数 */
/*  1         */  { 10, 9, 8, 8, },
/*  2         */  { 10, 9, 8, 8, },
/*  3         */  { 10, 9, 8, 8, },
/*  4         */  { 10, 9, 8, 8, },
/*  5         */  { 10, 9, 8, 8, },
/*  6         */  { 10, 9, 8, 8, },
/*  7         */  { 10, 9, 8, 8, },
/*  8         */  { 10, 9, 8, 8, },
/*  9         */  { 10, 9, 8, 8, },
/* 10         */  { 12,11,16,10, },
/* 11         */  { 12,11,16,10, },
/* 12         */  { 12,11,16,10, },
/* 13         */  { 12,11,16,10, },
/* 14         */  { 12,11,16,10, },
/* 15         */  { 12,11,16,10, },
/* 16         */  { 12,11,16,10, },
/* 17         */  { 12,11,16,10, },
/* 18         */  { 12,11,16,10, },
/* 19         */  { 12,11,16,10, },
/* 20         */  { 12,11,16,10, },
/* 21         */  { 12,11,16,10, },
/* 22         */  { 12,11,16,10, },
/* 23         */  { 12,11,16,10, },
/* 24         */  { 12,11,16,10, },
/* 25         */  { 12,11,16,10, },
/* 26         */  { 12,11,16,10, },
/* 27         */  { 14,13,16,12, },
/* 28         */  { 14,13,16,12, },
/* 29         */  { 14,13,16,12, },
/* 30         */  { 14,13,16,12, },
/* 31         */  { 14,13,16,12, },
/* 32         */  { 14,13,16,12, },
/* 33         */  { 14,13,16,12, },
/* 34         */  { 14,13,16,12, },
/* 35         */  { 14,13,16,12, },
/* 36         */  { 14,13,16,12, },
/* 37         */  { 14,13,16,12, },
/* 38         */  { 14,13,16,12, },
/* 39         */  { 14,13,16,12, },
/* 40         */  { 14,13,16,12, },
};
static int v_data_code_count[][4] = {
/* バージョン *//* データコード数 */
                /*  L,    M,   Q,   H, */
/* 1          */  {19,   16,  13,   9,},
/* 2          */  {34,   28,  22,  16,},
/* 3          */  {55,   44,  34,  26,},
/* 4          */  {80,   64,  48,  36,},
/* 5          */  {108,  86,  62,  46,},
/* 6          */  {136, 108,  76,  60,},
/* 7          */  {156, 124,  88,  66,},
/* 8          */  {194, 154, 110,  86,},
/* 9          */  {232, 182, 132, 100,},
/* 10         */  {274, 216, 154, 122,},
/* 11         */  {324, 254, 180, 140,},
/* 12         */  {370, 290, 206, 158,},
/* 13         */  {428, 334, 244, 180,},
/* 14         */  {461, 365, 261, 197,},
/* 15         */  {523, 415, 295, 223,},
/* 16         */  {589, 453, 325, 253,},
/* 17         */  {647, 507, 367, 283,},
/* 18         */  {721, 563, 397, 313,},
/* 19         */  {795, 627, 445, 341,},
/* 20         */  {861, 669, 485, 385,},
/* 21         */  {932, 714, 512, 406,},
/* 22         */  {1006,782, 568, 442,},
/* 23         */  {1094,860, 614, 464,},
/* 24         */  {1174,914, 664, 514,},
/* 25         */  {1276,1000,718, 538,},
/* 26         */  {1370,1062,754, 596,},
/* 27         */  {1468,1128,808, 628,},
/* 28         */  {1531,1193,871, 661,},
/* 29         */  {1631,1267,911, 701,},
/* 30         */  {1735,1373,985, 745,},
/* 31         */  {1843,1455,1033,793,},
/* 32         */  {1955,1541,1115,845,},
/* 33         */  {2071,1631,1171,901,},
/* 34         */  {2191,1725,1231,961,},
/* 35         */  {2306,1812,1286,986,},
/* 36         */  {2434,1914,1354,1054,},
/* 37         */  {2566,1992,1426,1096,},
/* 38         */  {2702,2102,1502,1142,},
/* 39         */  {2812,2216,1582,1222,},
/* 40         */  {2956,2334,1666,1276,},
};

/*----------------------------------------------------------------------------*/
/* あまった場合の付加ビット                                                   */
/*----------------------------------------------------------------------------*/
static char* v_pend_bit[]  = {
  "11101100",
  "00010001",
};

/*----------------------------------------------------------------------------*/
/* モジュール数                                                               */
/*----------------------------------------------------------------------------*/
static int v_module_count_table[] = {
  21, 25, 29, 33, 37, 41, 45, 49,
  53, 57, 61, 65, 69, 73, 77, 81,
  85, 89, 93, 97, 101, 105, 109, 113,
  117, 121, 125, 129, 133, 137, 141, 145,
  149, 153, 157, 161, 165, 169, 173, 177,
};
/*----------------------------------------------------------------------------*/
/* 位置あわせパターンの位置                                                   */
/*----------------------------------------------------------------------------*/
typedef struct _qr_position_pattern_t {
  int count;
  int position[7];
} qr_position_pattern_t;

static qr_position_pattern_t v_position_adjust_table[] = {
  { 0, {0,0,0,0,0,0,0,},},
  { 1, {6,18,0,0,0,0,0,},},
  { 1, {6,22,0,0,0,0,0,},},
  { 1, {6,26,0,0,0,0,0,},},
  { 1, {6,30,0,0,0,0,0,},},
  { 1, {6,34,0,0,0,0,0,},},
  { 6, {6,22,38,0,0,0,0,},},
  { 6, {6,24,42,0,0,0,0,},},
  { 6, {6,26,46,0,0,0,0,},},
  { 6, {6,28,50,0,0,0,0,},},
  { 6, {6,30,54,0,0,0,0,},},
  { 6, {6,32,58,0,0,0,0,},},
  { 6, {6,34,62,0,0,0,0,},},
  {13, {6,26,46,66,0,0,0,},},
  {13, {6,26,48,70,0,0,0,},},
  {13, {6,26,50,74,0,0,0,},},
  {13, {6,30,54,78,0,0,0,},},
  {13, {6,30,56,82,0,0,0,},},
  {13, {6,30,58,86,0,0,0,},},
  {13, {6,34,62,90,0,0,0,},},
  {22, {6,28,50,72,94,0,0,},},
  {22, {6,26,50,74,98,0,0,},},
  {22, {6,30,54,78,102,0,0,},},
  {22, {6,28,54,80,106,0,0,},},
  {22, {6,32,58,84,110,0,0,},},
  {22, {6,30,58,86,114,0,0,},},
  {22, {6,34,62,90,118,0,0,},},
  {33, {6,26,50,74,98,122,0,},},
  {33, {6,30,54,78,102,126,0,},},
  {33, {6,26,52,78,104,130,0,},},
  {33, {6,30,56,82,108,134,0,},},
  {33, {6,34,60,86,112,138,0,},},
  {33, {6,30,58,86,114,142,0,},},
  {33, {6,34,62,90,118,146,0,},},
  {46, {6,30,54,78,102,126,150,},},
  {46, {6,24,50,76,102,128,154,},},
  {46, {6,28,54,80,106,132,158,},},
  {46, {6,32,58,84,110,136,162,},},
  {46, {6,26,54,82,110,138,166,},},
  {46, {6,30,58,86,114,142,170,},},
};

/*----------------------------------------------------------------------------*/
/* マスク定義                                                                 */
/*----------------------------------------------------------------------------*/
typedef enum _qr_mask_pattern_t {
  QR_MASK_1 = 0,
  QR_MASK_2,
  QR_MASK_3,
  QR_MASK_4,
  QR_MASK_5,
  QR_MASK_6,
  QR_MASK_7,
  QR_MASK_8,
} 
qr_mask_pattern_t;

/*----------------------------------------------------------------------------*/
/* 白い画像(PNG)                                                              */
/*----------------------------------------------------------------------------*/
static unsigned char v_white_base_pic[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18,
  0x08, 0x02, 0x00, 0x00, 0x00, 0x6f, 0x15, 0xaa,
  0xaf, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59,
  0x73, 0x00, 0x00, 0x0b, 0x12, 0x00, 0x00, 0x0b,
  0x12, 0x01, 0xd2, 0xdd, 0x7e, 0xfc, 0x00, 0x00,
  0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xd5,
  0x04, 0x04, 0x17, 0x13, 0x07, 0x12, 0x27, 0x77,
  0x6f, 0x00, 0x00, 0x00, 0x22, 0x49, 0x44, 0x41,
  0x54, 0x78, 0x9c, 0x63, 0xfc, 0xff, 0xff, 0x3f,
  0x03, 0x35, 0x00, 0x13, 0x55, 0x4c, 0x19, 0x35, 
  0x68, 0xd4, 0xa0, 0x51, 0x83, 0x46, 0x0d, 0x1a, 
  0x35, 0x68, 0xd4, 0x20, 0x8a, 0x00, 0x00, 0x19,
  0xd1, 0x03, 0x2d, 0xc6, 0xa1, 0x61, 0x7f, 0x00, 
  0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
  0x42, 0x60, 0x82, 0x0a
};
/*----------------------------------------------------------------------------*/
/* 黒い画像(PNG)                                                              */
/*----------------------------------------------------------------------------*/
static unsigned char v_black_pixel_pic[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0x15, 0xc4,
  0x89, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47,
  0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
  0x00, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00,
  0xb1, 0x8f, 0x0b, 0xfc, 0x61, 0x05, 0x00, 0x00,
  0x00, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x00, 0x00,
  0x7a, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00,
  0xfa, 0x00, 0x00, 0x00, 0x80, 0xe8, 0x00, 0x00,
  0x75, 0x30, 0x00, 0x00, 0xea, 0x60, 0x00, 0x00,
  0x3a, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9c, 0xba,
  0x51, 0x3c, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x44,
  0x41, 0x54, 0x18, 0x57, 0x63, 0x60, 0x60, 0x60,
  0xf8, 0x0f, 0x00, 0x01, 0x04, 0x01, 0x00, 0x70,
  0x20, 0x65, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x49,
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82, 
};
/*----------------------------------------------------------------------------*/
/* 型番情報                                                                   */
/*----------------------------------------------------------------------------*/
typedef struct _qr_version_info_t {
  qr_ver_t version;
  char* bits;
} qr_version_info_t;

static qr_version_info_t v_version_info_table[] = {
  {QR_VER_1, NULL,},
  {QR_VER_2, NULL,},
  {QR_VER_3, NULL,},
  {QR_VER_4, NULL,},
  {QR_VER_5, NULL,},
  {QR_VER_6, NULL,},
  {QR_VER_7, "000111110010010100",},
  {QR_VER_8, "001000010110111100",},
  {QR_VER_9, "001001101010011001",},
  {QR_VER_10,"001010010011010011",},
  {QR_VER_11,"001011101111110110",},
  {QR_VER_12,"001100011101100010",},
  {QR_VER_13,"001101100001000111",},
  {QR_VER_14,"001110011000001101",},
  {QR_VER_15,"001111100100101000",},
  {QR_VER_16,"010000101101111000",},
  {QR_VER_17,"010001010001011101",},
  {QR_VER_18,"010010101000010111",},
  {QR_VER_19,"010011010100110010",},
  {QR_VER_20,"010100100110100110",},
  {QR_VER_21,"010101011010000011",},
  {QR_VER_22,"010110100011001001",},
  {QR_VER_23,"010111011111101100",},
  {QR_VER_24,"011000111011000100",},
  {QR_VER_25,"011001000111100001",},
  {QR_VER_26,"011010111110101011",},
  {QR_VER_27,"011011000010001110",},
  {QR_VER_28,"011100110000011010",},
  {QR_VER_29,"011101001100111111",},
  {QR_VER_30,"011110110101110101",},
  {QR_VER_31,"011111001001010000",},
  {QR_VER_32,"100000100111010101",},
  {QR_VER_33,"100001011011110000",},
  {QR_VER_34,"100010100010111010",},
  {QR_VER_35,"100011011110011111",},
  {QR_VER_36,"100100101100001011",},
  {QR_VER_37,"100101010000101110",},
  {QR_VER_38,"100110101001100100",},
  {QR_VER_39,"100111010101000001",},
  {QR_VER_40,"101000110001101001",},
};

/**
 * データ容量のテーブル
 */
/*----------------------------------------------------------------------------*/
/* バージョン別、レベル別、モード別データ容量                                 */
/*----------------------------------------------------------------------------*/
typedef struct _qr_capacity_t {
  qr_ver_t    version; /* 冗長情報 */
  qr_level_t  level;   /* 冗長情報 */
  int size[4];
} qr_capacity_t;

static qr_capacity_t v_capacity_table[] = {
  {QR_VER_1, QR_LEVEL_L, {  41,  25,  17,  10, },},
  {QR_VER_1, QR_LEVEL_M, {  34,  20,  14,   8, },},
  {QR_VER_1, QR_LEVEL_Q, {  27,  16,  11,   7, },},
  {QR_VER_1, QR_LEVEL_H, {  17,  10,   7,   4, },},

  {QR_VER_2, QR_LEVEL_L, {  77,  47,  32,  20, },},
  {QR_VER_2, QR_LEVEL_M, {  63,  38,  26,  16, },},
  {QR_VER_2, QR_LEVEL_Q, {  48,  29,  20,  12, },},
  {QR_VER_2, QR_LEVEL_H, {  34,  20,  14,   8, },},

  {QR_VER_3, QR_LEVEL_L, { 127,  77,  53,  32, },},
  {QR_VER_3, QR_LEVEL_M, { 101,  61,  42,  26, },},
  {QR_VER_3, QR_LEVEL_Q, {  77,  47,  32,  20, },},
  {QR_VER_3, QR_LEVEL_H, {  58,  35,  24,  15, },},

  {QR_VER_4, QR_LEVEL_L, { 187, 114,  78,  48, },},
  {QR_VER_4, QR_LEVEL_M, { 149,  90,  62,  38, },},
  {QR_VER_4, QR_LEVEL_Q, { 111,  67,  46,  28, },},
  {QR_VER_4, QR_LEVEL_H, {  82,  50,  34,  21, },},

  {QR_VER_5, QR_LEVEL_L, { 255, 154, 106,  65, },},
  {QR_VER_5, QR_LEVEL_M, { 202, 122,  84,  52, },},
  {QR_VER_5, QR_LEVEL_Q, { 144,  87,  60,  37, },},
  {QR_VER_5, QR_LEVEL_H, { 106,  64,  44,  27, },},

  {QR_VER_6, QR_LEVEL_L, { 322, 195, 134,  82, },},
  {QR_VER_6, QR_LEVEL_M, { 255, 154, 106,  65, },},
  {QR_VER_6, QR_LEVEL_Q, { 178, 108,  74,  45, },},
  {QR_VER_6, QR_LEVEL_H, { 139,  84,  58,  36, },},

  {QR_VER_7, QR_LEVEL_L, { 370, 224, 154,  95, },},
  {QR_VER_7, QR_LEVEL_M, { 293, 178, 122,  75, },},
  {QR_VER_7, QR_LEVEL_Q, { 207, 125,  86,  53, },},
  {QR_VER_7, QR_LEVEL_H, { 154,  93,  64,  39, },},

  {QR_VER_8, QR_LEVEL_L, { 461, 279, 192, 118, },},
  {QR_VER_8, QR_LEVEL_M, { 365, 221, 152,  93, },},
  {QR_VER_8, QR_LEVEL_Q, { 259, 157, 108,  66, },},
  {QR_VER_8, QR_LEVEL_H, { 202, 122,  84,  52, },},

  {QR_VER_9, QR_LEVEL_L, { 552, 335, 230, 141, },},
  {QR_VER_9, QR_LEVEL_M, { 432, 262, 180, 111, },},
  {QR_VER_9, QR_LEVEL_Q, { 312, 189, 130,  80, },},
  {QR_VER_9, QR_LEVEL_H, { 235, 143,  98,  60, },},

  {QR_VER_10, QR_LEVEL_L, { 652, 395, 271, 167, },},
  {QR_VER_10, QR_LEVEL_M, { 513, 311, 213, 131, },},
  {QR_VER_10, QR_LEVEL_Q, { 364, 221, 151,  93, },},
  {QR_VER_10, QR_LEVEL_H, { 288, 174, 119,  74, },},

  {QR_VER_11, QR_LEVEL_L, { 772, 468, 321, 198, },},
  {QR_VER_11, QR_LEVEL_M, { 604, 366, 251, 155, },},
  {QR_VER_11, QR_LEVEL_Q, { 427, 259, 177, 109, },},
  {QR_VER_11, QR_LEVEL_H, { 331, 200, 137,  85, },},

  {QR_VER_12, QR_LEVEL_L, { 883, 535, 367, 226, },},
  {QR_VER_12, QR_LEVEL_M, { 691, 419, 287, 177, },},
  {QR_VER_12, QR_LEVEL_Q, { 489, 296, 203, 125, },},
  {QR_VER_12, QR_LEVEL_H, { 374, 227, 155,  96, },},

  {QR_VER_13, QR_LEVEL_L, {1022, 619, 425, 262, },},
  {QR_VER_13, QR_LEVEL_M, { 796, 483, 331, 204, },},
  {QR_VER_13, QR_LEVEL_Q, { 580, 352, 241, 149, },},
  {QR_VER_13, QR_LEVEL_H, { 427, 259, 177, 109, },},

  {QR_VER_14, QR_LEVEL_L, {1101, 667, 458, 282, },},
  {QR_VER_14, QR_LEVEL_M, { 871, 528, 362, 223, },},
  {QR_VER_14, QR_LEVEL_Q, { 621, 376, 258, 159, },},
  {QR_VER_14, QR_LEVEL_H, { 468, 283, 194, 120, },},

  {QR_VER_15, QR_LEVEL_L, {1250, 758, 520, 320, },},
  {QR_VER_15, QR_LEVEL_M, { 991, 600, 412, 254, },},
  {QR_VER_15, QR_LEVEL_Q, { 703, 426, 292, 180, },},
  {QR_VER_15, QR_LEVEL_H, { 530, 321, 220, 136, },},

  {QR_VER_16, QR_LEVEL_L, {1408, 854, 586, 361, },},
  {QR_VER_16, QR_LEVEL_M, {1082, 656, 450, 277, },},
  {QR_VER_16, QR_LEVEL_Q, { 775, 470, 322, 198, },},
  {QR_VER_16, QR_LEVEL_H, { 602, 365, 250, 154, },},

  {QR_VER_17, QR_LEVEL_L, {1548, 938, 644, 397, },},
  {QR_VER_17, QR_LEVEL_M, {1212, 734, 504, 310, },},
  {QR_VER_17, QR_LEVEL_Q, { 876, 531, 364, 224, },},
  {QR_VER_17, QR_LEVEL_H, { 674, 408, 280, 173, },},

  {QR_VER_18, QR_LEVEL_L, {1725,1046, 718, 442, },},
  {QR_VER_18, QR_LEVEL_M, {1346, 816, 560, 345, },},
  {QR_VER_18, QR_LEVEL_Q, { 948, 574, 394, 243, },},
  {QR_VER_18, QR_LEVEL_H, { 746, 452, 310, 191, },},

  {QR_VER_19, QR_LEVEL_L, {1903,1153, 792, 488, },},
  {QR_VER_19, QR_LEVEL_M, {1500, 909, 624, 384, },},
  {QR_VER_19, QR_LEVEL_Q, {1063, 644, 442, 272, },},
  {QR_VER_19, QR_LEVEL_H, { 813, 493, 338, 208, },},

  {QR_VER_20, QR_LEVEL_L, {2061,1249, 858, 528, },},
  {QR_VER_20, QR_LEVEL_M, {1600, 970, 666, 410, },},
  {QR_VER_20, QR_LEVEL_Q, {1159, 702, 482, 297, },},
  {QR_VER_20, QR_LEVEL_H, { 919, 557, 382, 235, },},

  {QR_VER_21, QR_LEVEL_L, {2232,1352, 929, 572, },},
  {QR_VER_21, QR_LEVEL_M, {1708,1035, 711, 438, },},
  {QR_VER_21, QR_LEVEL_Q, {1224, 742, 509, 314, },},
  {QR_VER_21, QR_LEVEL_H, { 969, 587, 403, 248, },},

  {QR_VER_22, QR_LEVEL_L, {2409,1460,1003, 618, },},
  {QR_VER_22, QR_LEVEL_M, {1872,1134, 779, 480, },},
  {QR_VER_22, QR_LEVEL_Q, {1358, 823, 565, 348, },},
  {QR_VER_22, QR_LEVEL_H, {1056, 640, 439, 270, },},

  {QR_VER_23, QR_LEVEL_L, {2620,1588,1091, 672, },},
  {QR_VER_23, QR_LEVEL_M, {2059,1248, 857, 528, },},
  {QR_VER_23, QR_LEVEL_Q, {1468, 890, 611, 376, },},
  {QR_VER_23, QR_LEVEL_H, {1108, 672, 461, 284, },},

  {QR_VER_24, QR_LEVEL_L, {2812,1704,1171, 721, },},
  {QR_VER_24, QR_LEVEL_M, {2188,1326, 911, 561, },},
  {QR_VER_24, QR_LEVEL_Q, {1588, 963, 661, 407, },},
  {QR_VER_24, QR_LEVEL_H, {1228, 744, 511, 315, },},

  {QR_VER_25, QR_LEVEL_L, {3057,1853,1273, 784, },},
  {QR_VER_25, QR_LEVEL_M, {2395,1451, 997, 614, },},
  {QR_VER_25, QR_LEVEL_Q, {1718,1041, 715, 440, },},
  {QR_VER_25, QR_LEVEL_H, {1286, 779, 535, 330, },},

  {QR_VER_26, QR_LEVEL_L, {3283,1990,1367, 842, },},
  {QR_VER_26, QR_LEVEL_M, {2544,1542,1059, 652, },},
  {QR_VER_26, QR_LEVEL_Q, {1804,1094, 751, 462, },},
  {QR_VER_26, QR_LEVEL_H, {1425, 864, 593, 365, },},

  {QR_VER_27, QR_LEVEL_L, {3517,2132,1465, 902, },},
  {QR_VER_27, QR_LEVEL_M, {2701,1637,1125, 692, },},
  {QR_VER_27, QR_LEVEL_Q, {1933,1172, 805, 496, },},
  {QR_VER_27, QR_LEVEL_H, {1501, 910, 625, 385, },},

  {QR_VER_28, QR_LEVEL_L, {3669,2223,1528, 940, },},
  {QR_VER_28, QR_LEVEL_M, {2857,1732,1190, 732, },},
  {QR_VER_28, QR_LEVEL_Q, {2085,1263, 868, 534, },},
  {QR_VER_28, QR_LEVEL_H, {1581, 958, 658, 405, },},

  {QR_VER_29, QR_LEVEL_L, {3909,2369,1628,1002, },},
  {QR_VER_29, QR_LEVEL_M, {3035,1839,1264, 778, },},
  {QR_VER_29, QR_LEVEL_Q, {2181,1322, 908, 559, },},
  {QR_VER_29, QR_LEVEL_H, {1677,1016, 698, 430, },},

  {QR_VER_30, QR_LEVEL_L, {4158,2520,1732,1066, },},
  {QR_VER_30, QR_LEVEL_M, {3289,1994,1370, 843, },},
  {QR_VER_30, QR_LEVEL_Q, {2358,1429, 982, 604, },},
  {QR_VER_30, QR_LEVEL_H, {1782,1080, 742, 457, },},

  {QR_VER_31, QR_LEVEL_L, {4417,2677,1840,1132, },},
  {QR_VER_31, QR_LEVEL_M, {3486,2113,1452, 894, },},
  {QR_VER_31, QR_LEVEL_Q, {2473,1499,1030, 634, },},
  {QR_VER_31, QR_LEVEL_H, {1897,1150, 790, 486, },},

  {QR_VER_32, QR_LEVEL_L, {4686,2840,1952,1201, },},
  {QR_VER_32, QR_LEVEL_M, {3693,2238,1538, 947, },},
  {QR_VER_32, QR_LEVEL_Q, {2670,1618,1112, 684, },},
  {QR_VER_32, QR_LEVEL_H, {2022,1226, 842, 518, },},

  {QR_VER_33, QR_LEVEL_L, {4965,3009,2068,1273, },},
  {QR_VER_33, QR_LEVEL_M, {3909,2369,1628,1002, },},
  {QR_VER_33, QR_LEVEL_Q, {2805,1700,1168, 719, },},
  {QR_VER_33, QR_LEVEL_H, {2157,1307, 898, 553, },},

  {QR_VER_34, QR_LEVEL_L, {5253,3183,2188,1347, },},
  {QR_VER_34, QR_LEVEL_M, {4134,2506,1722,1060, },},
  {QR_VER_34, QR_LEVEL_Q, {2949,1787,1228, 756, },},
  {QR_VER_34, QR_LEVEL_H, {2301,1394, 958, 590, },},

  {QR_VER_35, QR_LEVEL_L, {5529,3351,2303,1417, },},
  {QR_VER_35, QR_LEVEL_M, {4343,2632,1809,1113, },},
  {QR_VER_35, QR_LEVEL_Q, {3081,1867,1283, 790, },},
  {QR_VER_35, QR_LEVEL_H, {2361,1431, 983, 605, },},

  {QR_VER_36, QR_LEVEL_L, {5836,3537,2431,1496, },},
  {QR_VER_36, QR_LEVEL_M, {4588,2780,1911,1176, },},
  {QR_VER_36, QR_LEVEL_Q, {3244,1966,1351, 832, },},
  {QR_VER_36, QR_LEVEL_H, {2524,1530,1051, 647, },},

  {QR_VER_37, QR_LEVEL_L, {6153,3729,2563,1577, },},
  {QR_VER_37, QR_LEVEL_M, {4775,2894,1989,1224, },},
  {QR_VER_37, QR_LEVEL_Q, {3417,2071,1423, 876, },},
  {QR_VER_37, QR_LEVEL_H, {2625,1591,1093, 673, },},

  {QR_VER_38, QR_LEVEL_L, {6479,3927,2699,1661, },},
  {QR_VER_38, QR_LEVEL_M, {5039,3054,2099,1292, },},
  {QR_VER_38, QR_LEVEL_Q, {3599,2181,1499, 923, },},
  {QR_VER_38, QR_LEVEL_H, {2735,1658,1139, 701, },},

  {QR_VER_39, QR_LEVEL_L, {6743,4087,2809,1729, },},
  {QR_VER_39, QR_LEVEL_M, {5313,3220,2213,1362, },},
  {QR_VER_39, QR_LEVEL_Q, {3791,2298,1579, 972, },},
  {QR_VER_39, QR_LEVEL_H, {2927,1774,1219, 750, },},

  {QR_VER_40, QR_LEVEL_L, {7089,4296,2953,1817, },},
  {QR_VER_40, QR_LEVEL_M, {5596,3391,2331,1435, },},
  {QR_VER_40, QR_LEVEL_Q, {3993,2420,1663,1024, },},
  {QR_VER_40, QR_LEVEL_H, {3057,1852,1273, 784, },},
};


static char*  s_get_mode_spec       (qr_code_t* qrcode);
static char*  s_get_char_bit_count  (qr_code_t* qrcode, int len);
static char*  s_data_to_bin_num     (qr_code_t* qrcode, int data_code_count);
static char*  s_data_to_bin_alpha   (qr_code_t* qrcode, int data_code_count);
static int    s_char_to_num_alpha   (qr_code_t* qrcode, char src);
static char*  s_data_to_bin_8bit    (qr_code_t* qrcode, int data_code_count);
static char*  s_data_to_bin_kanji   (qr_code_t* qrcode, int data_code_count);
static char*  s_tidy_8bit_code      (qr_code_t* qrcode, const char* indata, int data_code_count);
static char*  s_str_to_bin          (qr_code_t* qrcode, char* indata, int data_code_count);
static void   s_init_modules        (qr_code_t* qrcode, qr_mask_pattern_t pat, char* module[]);
static void   s_setup_probe_position(qr_code_t* qrcode, char* dst[], int pos_x, int pos_y);
static void   s_setup_timing_pattern(qr_code_t* qrcode, char* dst[]);
static void   s_setup_position_adjust(qr_code_t* qrcode, char* dst[]);
static void   s_setup_type_info     (qr_code_t* qrcode, char* dst[], qr_mask_pattern_t pat);
static void   s_setup_version_info  (qr_code_t* qrcode, char* dst[]);
static void   s_map_data            (qr_code_t* qrcode, char* dst[], unsigned char* indata,
                int data_count, qr_mask_pattern_t pat);
static int    s_get_mask            (qr_mask_pattern_t pat, int yy, int xx);
static int    s_get_bit_count       (int data);
static int    s_calc_lost_point     (qr_code_t* qrcode, char* dst[]);
static int    s_count_same_module   (qr_ver_t ver, char* dst[]);
static int    s_count_same_block    (qr_ver_t ver, char* dst[]);
static int    s_count_11311_pattern (qr_ver_t ver, char* dst[]);
static int    s_count_dark_ratio    (qr_ver_t ver, char* dst[]);

static void   chxj_qr_code          (qr_code_t* qrcode, char* module[]);

int
chxj_qr_code_handler(request_rec* r)
{
  int                sts;
  size_t             len;
  qr_code_t          qrcode;
  Doc                doc;
  char*              img;
  Node*              root;
  mod_chxj_config*   conf;

#ifdef QR_CODE_DEBUG
  DBG(r,"start chxj_qr_code_handler()");
#endif
  if (strcasecmp(r->handler, "chxj-qrcode")) {
    DBG(r, "end chxj_qr_code_handler()");
    return DECLINED;
  }

  /*--------------------------------------------------------------------------*/
  /* もし、イメージ変換ハンドラ中であれば、ここでは処理しない                 */
  /*--------------------------------------------------------------------------*/
  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (conf->image == CHXJ_IMG_ON)
    return DECLINED;

  memset(&doc, 0, sizeof(Doc));
  memset(&qrcode, 0, sizeof(qr_code_t));

  doc.r           = r;
  doc.parse_mode  = PARSE_MODE_CHTML;
  qrcode.doc      = &doc;
  qrcode.r        = r;

  qs_init_malloc(&doc);
  root = qs_parse_file(&doc, r->filename);
  qrcode.found = QR_NOT_FOUND;
  chxj_qrcode_node_to_qrcode(&qrcode, root);

  if (qrcode.found == QR_NOT_FOUND)
    return HTTP_NOT_FOUND;

  qs_all_free(&doc,QX_LOGMARK);

  sts = chxj_qrcode_create_image_data(&qrcode, &img, &len);
  if (sts != OK)
    return sts;

  ap_set_content_type(r, "image/jpg");

  ap_rwrite((void*)img, len, r);

  return OK;
}

char* 
chxj_qr_code_blob_handler(request_rec* r, const char* indata, size_t* len)
{
  int                sts;
  qr_code_t          qrcode;
  Doc                doc;
  char*              img;
  Node*              root;

  DBG(r, "start chxj_qr_code_blob_handler()");

  memset(&doc, 0, sizeof(Doc));
  memset(&qrcode, 0, sizeof(qr_code_t));

  doc.r           = r;
  doc.parse_mode  = PARSE_MODE_CHTML;
  qrcode.doc      = &doc;
  qrcode.r        = r;

  qs_init_malloc(&doc);
  root = qs_parse_string(&doc, indata, *len);
  qrcode.found = QR_NOT_FOUND;
  chxj_qrcode_node_to_qrcode(&qrcode, root);
  qs_all_free(&doc,QX_LOGMARK);

  if (qrcode.found == QR_NOT_FOUND)
    return NULL;

  sts = chxj_qrcode_create_image_data(&qrcode, &img, len);
  if (sts != OK) {
    DBG(r, "end chxj_qr_code_blob_handler()");
    return NULL;
  }

  ap_set_content_type(r, "image/jpg");

  DBG(r, "end chxj_qr_code_blob_handler()");

  return img;
}

int
chxj_qrcode_create_image_data(
  qr_code_t* qrcode,
  char** img,
  size_t* img_len)
{
  int                xx, yy;
  int                module_count;
  MagickBooleanType  status;
  request_rec*       r = qrcode->r;
  MagickWand*        magick_wand;
  MagickWand*        black_wand;
  unsigned char*     tmp;
  char**             module;

#ifdef QR_CODE_DEBUG
  DBG(r,"start s_create_image_file()");
#endif

  module_count  = v_module_count_table[qrcode->version];
  module = (char**)apr_palloc(r->pool, sizeof(char*)*(module_count+1));
  for (yy=0; yy<module_count; yy++) {
    module[yy] = (char*)apr_palloc(r->pool, module_count+1);
    memset(module[yy], -1, module_count+1);
  }

  chxj_qr_code(qrcode,  module);


  magick_wand = NewMagickWand();
  black_wand = NewMagickWand();


  if (MagickSetSize(magick_wand, 24, 24) == MagickFalse)
    goto on_error;

  if (MagickSetSize(black_wand, 24, 24) == MagickFalse)
    goto on_error;

  if (MagickReadImageBlob(magick_wand, v_white_base_pic, sizeof(v_white_base_pic)) == MagickFalse) 
    goto on_error;

  if (MagickReadImageBlob(black_wand, v_black_pixel_pic, sizeof(v_black_pixel_pic)) == MagickFalse)
    goto on_error;

  if (MagickSetImageUnits(magick_wand, PixelsPerInchResolution) == MagickFalse)
    goto on_error;

  if (MagickSetImageUnits(black_wand, PixelsPerInchResolution) == MagickFalse)
    goto on_error;

  if (MagickResizeImage(magick_wand, (module_count)+6, (module_count)+6 , LanczosFilter,0.0) == MagickFalse)
    goto on_error;

  if (MagickSetImageResolution(magick_wand, 96,96) == MagickFalse) 
    goto on_error;

  if (MagickSetImageResolution(black_wand, 96,96) == MagickFalse)
    goto on_error;

  if (MagickSetImageType(magick_wand, GrayscaleType) == MagickFalse)
    goto on_error;

  if (MagickSetImageType(black_wand, GrayscaleType) == MagickFalse)
    goto on_error;


  for (yy=0; yy<module_count; yy++) {
    for (xx=0; xx<module_count; xx++) {
      if (module[yy][xx] == '1') {
        if (MagickCompositeImage(magick_wand, black_wand, CopyCompositeOp, xx+3,yy+3) == MagickFalse)
          goto on_error;
      }
    }
  }

  if (qrcode->size <= 0) {
    status = MagickResizeImage(magick_wand, ((module_count)*4+6), ((module_count)*4+6) , LanczosFilter,0.0);
  }
  else {  
    status = MagickResizeImage(magick_wand, (module_count * qrcode->size + 6), (module_count * qrcode->size + 6) , LanczosFilter,0.0);
  }
  if (status == MagickFalse) 
    goto on_error;

  if (MagickSetImageFormat(magick_wand,"jpg") == MagickFalse)
    goto on_error;

#ifdef QR_CODE_DEBUG
  DBG(r,"convert to jpg");
#endif

  tmp = (unsigned char*)MagickGetImageBlob(magick_wand,img_len);
  *img = apr_palloc(r->pool, *img_len);
  memcpy(*img, tmp, *img_len);


  DestroyMagickWand(magick_wand);
  DestroyMagickWand(black_wand);


#ifdef QR_CODE_DEBUG
  DBG(r, "end chxj_qr_code_handler()");
#endif

  return OK;

on_error:
  EXIT_MAGICK_ERROR();
  return HTTP_NOT_FOUND;
}



static void
chxj_qr_code(qr_code_t* qrcode, char* module[])
{
  request_rec*      r     = qrcode->doc->r;
  unsigned char*    eccstr;
  int               ii;
  int               data_code_count;
  char*             real_data  = NULL;
  char*             decstr;
  char*             binstr;
  int               data_total_count;
  int               min_lost_point;
  int               min_mask_pattern;
  int               module_count = v_module_count_table[qrcode->version];

#ifdef QR_CODE_DEBUG
  DBG(r,"start chxj_qr_code()");
#endif
  do {
    /*------------------------------------------------------------------------*/
    /* モード指示子を取得します                                               */
    /*------------------------------------------------------------------------*/
    binstr = apr_psprintf(r->pool, "%s", s_get_mode_spec(qrcode));
#ifdef QR_CODE_DEBUG
    DBG(r,"s_get_mode_spec()");
#endif

    /*------------------------------------------------------------------------*/
    /* 文字数指示子を取得します                                               */
    /*------------------------------------------------------------------------*/
    binstr = apr_pstrcat(r->pool, binstr, 
                  s_get_char_bit_count(qrcode, strlen(qrcode->indata)), NULL);
#ifdef QR_CODE_DEBUG
    DBG1(r, "s_get_char_bit_count()[%s]", binstr);
#endif

    /*------------------------------------------------------------------------*/
    /* データコード数を算出                                                   */
    /*------------------------------------------------------------------------*/
    data_code_count = v_data_code_count[qrcode->version][qrcode->level];
#ifdef QR_CODE_DEBUG
    DBG1(r,"data_code_count=[%d]", data_code_count);
#endif

    /*------------------------------------------------------------------------*/
    /* 実データの２進化                                                       */
    /*------------------------------------------------------------------------*/
    qrcode->mode_change = QR_NOT_CHANGE;
    switch(qrcode->mode) {
    case QR_NUM_MODE:
      real_data = s_data_to_bin_num(qrcode, data_code_count);
      break;
    case QR_ALPHA_MODE:
      real_data = s_data_to_bin_alpha(qrcode, data_code_count);
      break;
    case QR_8BIT_MODE:
      real_data = s_data_to_bin_8bit(qrcode,data_code_count);
      break;
    case QR_KANJI_MODE:
      real_data = s_data_to_bin_kanji(qrcode,data_code_count);
      break;
    default:
      qrcode->mode = QR_8BIT_MODE;
      qrcode->mode_change = QR_CHANGE;
      break;
    }
  }
  while(qrcode->mode_change == QR_CHANGE);

  /*--------------------------------------------------------------------------*/
  /* 終端パターンの付加                                                       */
  /*--------------------------------------------------------------------------*/
  binstr = apr_pstrcat(r->pool, binstr, real_data, NULL);
#ifdef QR_CODE_DEBUG
  DBG1(r,"Before TERM BIT[%s]", binstr);
#endif
  if (data_code_count * 8 > strlen(binstr)) {
    int binstr_len = strlen(binstr);
    for (ii=0; ii< (data_code_count * 8) - binstr_len && ii < 4; ii++) {
      binstr = apr_pstrcat(r->pool, binstr, "0", NULL);
    }
  }
#ifdef QR_CODE_DEBUG
  DBG3(r, "After TERM BIT[%s] len[%d]bit [%f]byte", binstr, strlen(binstr), (float)(((float)strlen(binstr))/8.0));
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "add term data");
#endif

  /*--------------------------------------------------------------------------*/
  /* 8bit単位にそろえる                                                       */
  /*--------------------------------------------------------------------------*/
  binstr = s_tidy_8bit_code(qrcode, binstr, data_code_count);
#ifdef QR_CODE_DEBUG
  DBG1(r,"After Tidy 8 BIT[%s]", binstr);
  DBG(r,"s_tidy_8bit_code()");
#endif

  /*--------------------------------------------------------------------------*/
  /* 文字列をバイナリへ                                                       */
  /*--------------------------------------------------------------------------*/
  decstr = s_str_to_bin(qrcode, binstr, data_code_count);
#ifdef QR_CODE_DEBUG
  DBG(r,"s_str_to_bin()");
#endif

#ifdef QR_CODE_DEBUG
  /*--------------------------------------------------------------------------*/
  /* DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG  */
  /*--------------------------------------------------------------------------*/
  do {
    char* rows = apr_psprintf(r->pool, " ");
    for (ii=0; ii<data_code_count; ii++) {
      rows = apr_pstrcat(r->pool, rows, apr_psprintf(r->pool, "[%02x]\n", (unsigned char)decstr[ii]), NULL);
    }
    DBG1(r,"decstr[%s]", rows);
  }while(0);
#endif


  /*--------------------------------------------------------------------------*/
  /* ECCの追加                                                                */
  /* 出力としてのeccstrは、インタリーブ済みのデータがセットされます.          */
  /*--------------------------------------------------------------------------*/
  eccstr = apr_palloc(r->pool, module_count* module_count);
  data_total_count = chxj_calc_ecc(qrcode, (unsigned char*)decstr, eccstr);
#ifdef QR_CODE_DEBUG
  DBG1(r,"chxj_calc_ecc() total_count[%d]", data_total_count);
#endif

  /*--------------------------------------------------------------------------*/
  /* 最適のマスクを取得する                                                   */
  /* 全てのパターンを試し、最も失点の少ないマスクを選択します                 */
  /*--------------------------------------------------------------------------*/
  min_lost_point = -1;
  min_mask_pattern = -1;
  for (ii=0; ii<8; ii++)
  {
    int lost_point;
    s_init_modules(qrcode, ii, module);
    s_map_data(qrcode,  module, (unsigned char*)eccstr, data_total_count, ii);
    lost_point = s_calc_lost_point(qrcode, module);
    if (min_mask_pattern == -1)
    {
      min_mask_pattern = ii;
      min_lost_point = lost_point;
    }
    else
    {
      if (min_lost_point >= lost_point)
      {
        min_mask_pattern = ii;
        min_lost_point = lost_point;
      }
    }
  }

  /*--------------------------------------------------------------------------*/
  /* 最適マスクで再度作りなおし                                               */
  /*--------------------------------------------------------------------------*/
#ifdef QR_CODE_DEBUG
  DBG1(r, "selected mask[%d]", min_mask_pattern);
#endif
  s_init_modules(qrcode, min_mask_pattern, module);
  s_map_data(qrcode, 
             module, 
             (unsigned char*)eccstr, 
             data_total_count, 
             min_mask_pattern);


#ifdef QR_CODE_DEBUG
  /*--------------------------------------------------------------------------*/
  /* DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG  */
  /*--------------------------------------------------------------------------*/
  do {
    int xx, yy;
    int module_count = v_module_count_table[qrcode->version];
    DBG1(r,"loop to [%d]", module_count);
    for (yy=0; yy<module_count; yy++)
    {
      char *rows ;

      rows = apr_psprintf(r->pool, " ");
      
      for (xx=0; xx<module_count; xx++)
      {
        rows = apr_pstrcat(r->pool, rows, "[", apr_psprintf(r->pool, "%c", module[yy][xx]), "]", NULL);
      }
      DBG2(r,"%d:%s", yy,rows);
    }
  } while(0);
  /* DEBUG END */
#endif

#ifdef QR_CODE_DEBUG
  DBG(r,"end chxj_qr_code()");
#endif
}

void
chxj_qrcode_node_to_qrcode(qr_code_t* qrcode, Node* node)
{
  request_rec* r = qrcode->r;
  Doc* doc = qrcode->doc;
  Node* child;

  for (child = qs_get_child_node(doc,node);
       child ;
       child = qs_get_next_node(doc,child))
  {
    char* name = qs_get_node_name(doc,child);
    if (strcasecmp("qrcode",name) == 0)
    {
      qrcode->found       = QR_FOUND;
      qrcode->mode_change = QR_NOT_CHANGE;
      qrcode->version     = QR_VER_5;
      qrcode->level       = QR_LEVEL_L;
      qrcode->mode        = QR_8BIT_MODE;
      qrcode->size        = 0;
      qrcode->indata      = NULL;
      chxj_qrcode_node_to_qrcode(qrcode, child);
    }
    else
    if (strcasecmp("version", name) == 0)
    {
      int ver;
      Node* cchild = qs_get_child_node(doc, child);
      char* value;
      if (cchild == NULL)
      {
        qrcode->version = QR_VER_1;
        continue;
      }

      name = qs_get_node_name(doc, cchild);
      value = qs_get_node_value(doc, cchild);
      if (strcasecmp("TEXT", name) != 0)
      {
        qrcode->version = QR_VER_1;
        continue;
      }
      if (value == NULL || strlen(value) == 0)
      {
        qrcode->version = QR_VER_1;
        continue;
      }

      if (chxj_chk_numeric(value))
      {
        qrcode->version = QR_VER_1;
        continue;
      }
      ver = chxj_atoi(value);
      if (ver <= 0 || ver > 40)
      {
        qrcode->version = QR_VER_1;
        continue;
      }

      qrcode->version = ver - 1;
    }
    else
    if (strcasecmp("level", name) == 0)
    {
      Node* cchild = qs_get_child_node(doc, child);
      char* value;
      if (cchild == NULL)
      {
        qrcode->level = QR_LEVEL_L;
        continue;
      }
      name = qs_get_node_name(doc, cchild);
      value = qs_get_node_value(doc, cchild);
      if (strcasecmp("TEXT", name) != 0)
      {
        qrcode->level = QR_LEVEL_L;
        continue;
      }

      if (value == NULL || strlen(value) == 0)
      {
        qrcode->level = QR_LEVEL_L;
        continue;
      }

      if (strcasecmp("L", value) == 0)
      {
        qrcode->level = QR_LEVEL_L;
      }
      else
      if (strcasecmp("M", value) == 0)
      {
        qrcode->level = QR_LEVEL_M;
      }
      else
      if (strcasecmp("Q", value) == 0)
      {
        qrcode->level = QR_LEVEL_Q;
      }
      else
      if (strcasecmp("H", value) == 0)
      {
        qrcode->level = QR_LEVEL_H;
      }
      else
      {
        qrcode->level = QR_LEVEL_L;
      }
    }
    else
    if (strcasecmp("mode", name) == 0)
    {
      Node* cchild = qs_get_child_node(doc, child);
      char* value;
      if (cchild == NULL)
      {
        qrcode->mode = QR_NUM_MODE;
        continue;
      }
      name = qs_get_node_name(doc, cchild);
      value = qs_get_node_value(doc, cchild);
      if (strcasecmp("TEXT", name) != 0)
      {
        qrcode->level = QR_NUM_MODE;
        continue;
      }
      if (value == NULL || strlen(value) == 0)
      {
        qrcode->level = QR_NUM_MODE;
        continue;
      }

      /*----------------------------------------------------------------------*/
      /* 数字モード                                                           */
      /*----------------------------------------------------------------------*/
      if (strcasecmp("NUM", value) == 0)
      {
        qrcode->mode    = QR_NUM_MODE;
      }
      else
      /*----------------------------------------------------------------------*/
      /* 英数字モード                                                         */
      /*----------------------------------------------------------------------*/
      if (strcasecmp("ALPHA", value) == 0)
      {
        qrcode->mode    = QR_ALPHA_MODE;
      }
      else
      /*----------------------------------------------------------------------*/
      /* 8ビットバイトモード                                                  */
      /*----------------------------------------------------------------------*/
      if (strcasecmp("8BIT", value) == 0)
      {
        qrcode->mode    = QR_8BIT_MODE;
      }
      else
      /*----------------------------------------------------------------------*/
      /* 漢字モード                                                           */
      /*----------------------------------------------------------------------*/
      if (strcasecmp("KANJI", value) == 0)
      {
        qrcode->mode    = QR_KANJI_MODE;
      }
      else
      {
        qrcode->mode    = QR_NUM_MODE;
      }
    }
    else
    if (strcasecmp("data", name) == 0)
    {
      /* TODO: 改行も可能なように修正する */
      Node* cchild = qs_get_child_node(doc, child);
      char* value;

      qrcode->indata = apr_palloc(r->pool, 1);
      qrcode->indata[0] = 0;
      if (cchild == NULL)
      {
        continue;
      }
      for (;cchild; cchild = cchild->next)
      {
        name = qs_get_node_name(doc, cchild);
        value = qs_get_node_value(doc, cchild);
        if (strcasecmp("TEXT", name) != 0)
        {
          continue;
        }
        if (strlen(qrcode->indata) > 0)
        {
          qrcode->indata = apr_pstrcat(r->pool, qrcode->indata, "\r\n", NULL);
        }
        qrcode->indata = apr_pstrcat(r->pool, qrcode->indata, value, NULL);
      }
    }
    else
    if (strcasecmp("size", name) == 0)
    {
      int size;
      char* value;
      Node* cchild = qs_get_child_node(doc, child);

      if (cchild == NULL)
      {
        qrcode->size = 0;
        continue;
      }
      name = qs_get_node_name(doc, cchild);
      value = qs_get_node_value(doc, cchild);
      if (strcasecmp("TEXT", name) != 0)
      {
        qrcode->size = 0;
        continue;
      }
      if (value == NULL || strlen(value) == 0)
      {
        qrcode->size = 0;
        continue;
      }
      if (chxj_chk_numeric(value))
      {
        qrcode->size = 0;
        continue;
      }

      size = chxj_atoi(value);
      if (size <= 0)
      {
        qrcode->size = 0;
        continue;
      }
      if (size > 20)
      {
        qrcode->size = 20;
        continue;
      }
      qrcode->size = size;
    }
  }
#ifdef QR_CODE_DEBUG
  DBG1(r,"qrcode->version[%d]", qrcode->version);
  DBG1(r,"qrcode->level[%d]", qrcode->level);
  DBG1(r,"qrcode->mode[%d]", qrcode->mode);
  DBG1(r,"qrcode->indata[%s]", qrcode->indata);
#endif
}


/**
 * モード指示子を取得します.
 */
static char*
s_get_mode_spec(qr_code_t *qrcode)
{
  char* result = (char*)apr_palloc(qrcode->r->pool, 4+1);

  memset(result, 0, 5);
  memcpy(result, v_mode_table[qrcode->mode], 4);
#ifdef QR_CODE_DEBUG
  DBG1(r,"Mode Spec[%s]", result);
#endif

  return result;
}


/**
 * 文字数指示子を取得します.
 */
static char*
s_get_char_bit_count(qr_code_t* qrcode, int len)
{
  int bit_count = v_char_count_table[qrcode->version][qrcode->mode];
  int ii,jj;
  char* tmp;
  char* result;
  int data_capacity   = v_capacity_table[qrcode->version*4+qrcode->level].size[qrcode->mode];

  DBG1(qrcode->r, "len [%d]", len);
  if (qrcode->mode == QR_KANJI_MODE && data_capacity * 2 < len) {
    len = data_capacity * 2;
  }
  else
  if (qrcode->mode != QR_KANJI_MODE && data_capacity < len) {
    len = data_capacity;
  }

  if (qrcode->mode == QR_KANJI_MODE) {
    len /= 2;
  }

  DBG1(qrcode->r, "len [%d]", len);

  tmp = (char*)apr_palloc(qrcode->r->pool, bit_count + 1);
  for (ii=0; ii<bit_count; ii++) {
    tmp[ii] = (len & 0x01) ? '1' : '0';
    len = len >> 1;
  }

  tmp[bit_count] = '\0';

  result = (char*)apr_palloc(qrcode->r->pool, bit_count+1);
  for (jj=0,ii=bit_count-1; ii>=0 && jj < bit_count; ii--, jj++) {
    result[ii] = tmp[jj];
  }

  result[bit_count] = '\0';

#ifdef QR_CODE_DEBUG
  DBG1(qrcode->r, "Character Count bit [%s]", result);
#endif

  return result;
}

/**
 * 数字モード
 * 入力データから、２進文字列を取得します.
 */
static char*
s_data_to_bin_num(qr_code_t* qrcode, int data_code_count)
{
  int len = strlen(qrcode->indata);
  int setn;
  int modn;
  int ii;
  int jj;
  int kk;
  char* result;
  char  tmp[4];
  char  tmp_bit[11];
  int data_capacity   = v_capacity_table[qrcode->version*4+qrcode->level].size[qrcode->mode];

  if (data_capacity < len) {
    DBG(qrcode->r, "input data is too long");
    len = data_capacity;
  }
  setn = len / 3;
  modn = len % 3;

  if (chxj_chk_numeric(qrcode->indata) != 0) {
    qrcode->mode_change = QR_CHANGE;
    qrcode->mode        = QR_8BIT_MODE;
    return NULL;
  }

  result = (char*)apr_palloc(qrcode->r->pool, setn*10 + ((modn == 1) ? 4 : (modn == 2) ? 7 : 0) + 1); 
  kk = 0;
  for (ii=0; ii<len; ii++) {
    tmp[ii % 3] = qrcode->indata[ii];
    if ((ii % 3) == 2) {
      tmp[4] = 0;
      int n = atoi(tmp);
      for (jj=0; jj<10; jj++) {
        tmp_bit[jj] = (n & 0x01) ?  '1'  : '0';
        n = n >> 1;
      }
      tmp_bit[10] = 0;
      for (jj=10-1; jj>=0; jj--) {
        result[kk++] = tmp_bit[jj];
      }
    }
  }

  if (modn != 0) {
    tmp[modn] = 0;
    int n = atoi(tmp);
    int bb = (modn == 1) ? 4 : 7;

    for (jj=0; jj< bb; jj++) {
      tmp_bit[jj] = (n & 0x01) ?  '1'  : '0';
      n = n >> 1;
    }

    tmp_bit[(modn == 1) ? 4 : 7] = 0;

    for (jj=bb -1; jj>=0; jj--) {
      result[kk++] = tmp_bit[jj];
    }
  }
  result[kk] = 0;
  return result;
}


/**
 * 英数字モード
 * 入力データから、２進文字列を取得します.
 */
static char*
s_data_to_bin_alpha(qr_code_t* qrcode, int data_code_count)
{
  int len = strlen(qrcode->indata);
  int setn;
  int modn;
  int ii;
  int jj;
  int kk;
  char* result;
  char  tmp[2+1];
  char  tmp_bit[11+1];
  int data_capacity   = v_capacity_table[qrcode->version*4+qrcode->level].size[qrcode->mode];

  if (data_capacity < len) {
    DBG(qrcode->r, "input data is too long");
    len = data_capacity;
  }
  setn = len / 2;
  modn = len % 2;

  result = (char*)apr_palloc(qrcode->r->pool, setn*11 + ((modn == 1) ? 6 : 0) + 1); 

  kk = 0;
  for (ii=0; ii<len; ii++) {
    tmp[ii % 2] = qrcode->indata[ii];
    if ((ii % 2) == 1) {
      tmp[3] = 0;
      int n = s_char_to_num_alpha(qrcode,tmp[0])*45;

      if (qrcode->mode_change == QR_CHANGE)
        return NULL;

      n += s_char_to_num_alpha(qrcode,tmp[1]);
      if (qrcode->mode_change == QR_CHANGE)
        return NULL;

      for (jj=0; jj<11; jj++) {
        tmp_bit[jj] = (n & 0x01) ?  '1'  : '0';
        n = n >> 1;
      }

      tmp_bit[11] = 0;

      for (jj=11-1; jj>=0; jj--)
        result[kk++] = tmp_bit[jj];
    }
  }

  if (modn) {

    tmp[modn] = 0;
    int n = s_char_to_num_alpha(qrcode,tmp[0]);

    if (qrcode->mode_change == QR_CHANGE)
      return NULL;

    for (jj=0; jj< 6; jj++) {
      tmp_bit[jj] = (n & 0x01) ?  '1'  : '0';
      n = n >> 1;
    }

    tmp_bit[6] = 0;

    for (jj=6-1; jj>=0; jj--)
      result[kk++] = tmp_bit[jj];
  }
  result[kk] = 0;
  return result;
}


/**
 * 英数字モードの補助関数.
 * 英数字から、数値に変換します.
 */
static int
s_char_to_num_alpha(qr_code_t* qrcode, char src)
{
  switch(src) {
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  case 'A': return 10;
  case 'B': return 11;
  case 'C': return 12;
  case 'D': return 13;
  case 'E': return 14;
  case 'F': return 15;
  case 'G': return 16;
  case 'H': return 17;
  case 'I': return 18;
  case 'J': return 19;
  case 'K': return 20;
  case 'L': return 21;
  case 'M': return 22;
  case 'N': return 23;
  case 'O': return 24;
  case 'P': return 25;
  case 'Q': return 26;
  case 'R': return 27;
  case 'S': return 28;
  case 'T': return 29;
  case 'U': return 30;
  case 'V': return 31;
  case 'W': return 32;
  case 'X': return 33;
  case 'Y': return 34;
  case 'Z': return 35;
  case ' ': 
    return 36;
  case '$': 
    return 37;
  case '%': 
    return 38;
  case '*': 
    return 39;
  case '+': 
    return 40;
  case '-': 
    return 41;
  case '.': 
    return 42;
  case '/': 
    return 43;
  case ':': 
    return 44;
  default:
    qrcode->mode_change = QR_CHANGE;
    qrcode->mode = QR_8BIT_MODE;
    return -1;
  }
}

/**
 * 8bitバイトモード
 * 入力データから２進文字列を取得します.
 */
static char*
s_data_to_bin_8bit(qr_code_t* qrcode, int data_code_count)
{
  int len = strlen(qrcode->indata);
  int ii;
  int jj;
  int kk;
  char* result;
  char  tmp_bit[8+1];
  int data_capacity   = v_capacity_table[qrcode->version*4+qrcode->level].size[qrcode->mode];

  if (data_capacity < len) {
    DBG(qrcode->r, "input data is too long");
    len = data_capacity;
  }

  result = (char*)apr_palloc(qrcode->r->pool, len*8 + 1); 

  kk = 0;
  for (ii=0; ii<len; ii++) {
    int n;

    n = (int)qrcode->indata[ii];
    for (jj=0; jj<8; jj++) {
      tmp_bit[jj] = (n & 0x01) ? '1' : '0';
      n = n >> 1;
    }

    tmp_bit[8] = 0;

    for (jj=8-1; jj>=0; jj--)
      result[kk++] = tmp_bit[jj];
  }
  result[kk] = 0;

#ifdef QR_CODE_DEBUG
  DBG2(qrcode->r, "input data --> 2bin result[%s] len:[%d]", result, strlen(result));
#endif

  return result;
}

/**
 * Kanji mode.
 * A binary character string is acquired from input data.
 */
static char*
s_data_to_bin_kanji(qr_code_t* qrcode, int data_code_count)
{
  int len = strlen(qrcode->indata);
  int ii;
  int jj;
  int kk;
  char* result;
  char  tmp_bit[13+1];

  int data_capacity   = v_capacity_table[qrcode->version*4+qrcode->level].size[qrcode->mode];

  if (data_capacity * 2 < len) {
    DBG(qrcode->r, "input data is too long");
    len = data_capacity * 2;
  }

  if ((len % 2) != 0) {
    DBG(qrcode->r, "invalid data.");
    qrcode->mode_change = QR_CHANGE;
    qrcode->mode        = QR_8BIT_MODE;
    return NULL;
  }

  result = (char*)apr_palloc(qrcode->r->pool, (len/2)*13 + 1); 
  for (kk=0,ii=0; ii<len-1; ii++) {
    int c;
    int up_c;
    int dn_c;
    c = (qrcode->indata[ii+0] & 0xff)<< 8;
    c +=(qrcode->indata[ii+1] & 0xff);

    if (c >= 0x8140 && c <= 0x9FFC) {
      c -= 0x8140;
      up_c = ((c >> 8) & 0xff) * 0xC0;
      dn_c = ( c       & 0xff);
      c  = up_c;
      c += dn_c;
    }
    else
    if (c >= 0xE040 && c <= 0xEBBF) {
      c -= 0xC140;
      up_c = ((c >> 8) & 0xff) * 0xC0;
      dn_c = ( c       & 0xff);
      c  = up_c;
      c += dn_c;
    }
    else {
      qrcode->mode_change = QR_CHANGE;
      qrcode->mode        = QR_8BIT_MODE;
      return NULL;
    }

    memset(tmp_bit, 0, 13+1);
    for (jj=0; jj<13; jj++) {
      tmp_bit[jj] = (c & 0x01) ? '1' : '0';
      c = c >> 1;
    }

    tmp_bit[13] = 0;
    for (jj=13-1; jj>=0; jj--)
      result[kk++] = tmp_bit[jj];

    ii++;
  }
  result[kk] = 0;

#ifdef QR_CODE_DEBUG
  DBG2(qrcode->r, "input data --> 2bin result[%s] len:[%d]", result, strlen(result));
#endif

  return result;
}

/**
 * 8bit長にあわせます
 */
static char*
s_tidy_8bit_code(qr_code_t* qrcode, const char* indata, int data_code_count)
{
  int len = strlen(indata);
  int ii;
  int n;
  int rest;
  char* tmp = NULL;
  char* result;

#ifdef QR_CODE_DEBUG
  DBG2(qrcode->r, "len[%d] data_code_count * 8 [%d]", len, data_code_count * 8);
#endif

  assert (len <= data_code_count * 8);
  /* 8bitで割れない場合は、残りを０埋めし、8で割れるようにする */
  if (len % 8) {
    n = 8 - (len % 8);
    assert (len + n <= data_code_count * 8);

    tmp = (char*)apr_palloc(qrcode->r->pool, n);
    for (ii=0; ii<n; ii++)
      tmp[ii] = '0';

    tmp[ii] = 0;
  }

  result = (char*)apr_palloc(qrcode->r->pool, data_code_count * 8 + 1);
  memset(result, 0, data_code_count * 8+ 1);
  memcpy(result, indata, len);
  if (len % 8)
    memcpy(&result[len], tmp, strlen(tmp));

  len = strlen(result);
  rest = data_code_count - (len / 8);
  n = 0;
  while(rest>0) {
    for (ii=0; ii<8; ii++)
      result[len++] = v_pend_bit[n][ii];

    n = (n == 0) ? 1 : 0;
    rest--;
  }
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, qrcode->r, "tidy len[%d] data_code_count[%d]", strlen(result)/8, data_code_count);
#endif

  return result;
}

/**
 * ２進文字列をバイナリに変換します.
 */
static char* 
s_str_to_bin(qr_code_t* qrcode, char* indata, int data_code_count)
{
  char* result ;
  int pos;
  int len ;
  int ii;

  len = strlen(indata);
  result = (char*)apr_palloc(qrcode->r->pool, data_code_count);
  memset(result, 0, data_code_count);

  pos = 0;
  for (ii=0; ii<len; ii++)
  {
    result[pos] = (result[pos] << 1) | (indata[ii] - '0');
    if ((ii % 8) == 7)
    {
      pos ++;
    }
  }
  return result;
}

/**
 * 出力領域を初期化します.
 * 位置検出パターン、位置あわせ、タイミング、形式情報を設定し、
 * そのほかの場所には、-1を設定します.
 */
static void
s_init_modules(qr_code_t* qrcode, qr_mask_pattern_t pat, char* dst[])
{
  int module_count = v_module_count_table[qrcode->version];
  int yy;
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r,
                                    "start s_init_modules()");

  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r,
                                    "module_count[%d]", module_count);
#endif
  for (yy=0; yy<module_count; yy++)
  {
    memset(&dst[yy][0], -1, module_count+1);
  }

  s_setup_probe_position(qrcode, dst, 0, 0);
  s_setup_probe_position(qrcode, dst, 0, module_count - 7);
  s_setup_probe_position(qrcode, dst, module_count - 7, 0);
  s_setup_position_adjust(qrcode, dst);
  s_setup_timing_pattern(qrcode, dst);
  s_setup_type_info(qrcode,dst, pat);
  s_setup_version_info(qrcode, dst);

#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r,
                                    "end s_init_modules()");
#endif
}

/* 
 * 位置検出パターンとついでに分離パターンの出力
 */
static void
s_setup_probe_position(qr_code_t* qrcode, char* dst[], int pos_x, int pos_y)
{
  int module_count = v_module_count_table[qrcode->version];
  int xx;
  int yy;
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_probe_position()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "module_count[%d]", module_count);
#endif

  for (yy=-1; yy<=7; yy++)
  {
    for (xx=-1; xx<=7; xx++)
    {
      /* 位置検出パターン */
      if ((yy==0 || yy == 6) && xx != -1 && xx != 7)
      {
        assert(yy+pos_y >= 0 && yy+pos_y < module_count);
        assert(xx+pos_x >= 0 && xx+pos_x < module_count);
        dst[yy+pos_y][xx+pos_x] = '1';
      }
      else
      if ((xx == 0 && yy != -1 && yy != 7)
      ||  (xx >= 2 && xx <= 4 && yy >=2 && yy <= 4)
      ||  (xx == 6 && yy != -1 && yy != 7))
      {
        assert(yy+pos_y >= 0 && yy+pos_y < module_count);
        assert(xx+pos_x >= 0 && xx+pos_x < module_count);
        dst[yy+pos_y][xx+pos_x] = '1';
      }
      else
      {
        /* 分離パターン */
        if ((yy == 7  
        &&   yy + pos_y >= 0 
        &&   yy + pos_y < module_count 
        &&   xx + pos_x >= 0 
        &&   xx + pos_x < module_count)
        ||  (yy == -1  
        &&   yy + pos_y >= 0 
        &&   yy + pos_y < module_count
        &&   xx + pos_x >= 0 
        &&   xx + pos_x < module_count)
        ||  (xx == 7  
        &&   xx + pos_x >= 0 
        &&   xx + pos_x < module_count 
        &&   yy + pos_y >= 0 
        &&   yy + pos_y < module_count)
        ||  (xx == -1  
        &&   xx + pos_x < module_count 
        &&   xx + pos_x >= 0
        &&   yy + pos_y >= 0 
        &&   yy + pos_y < module_count))
        {
          assert(yy+pos_y >= 0 && yy+pos_y < module_count);
          assert(xx+pos_x >= 0 && xx+pos_x < module_count);
          dst[yy+pos_y][xx+pos_x] = '0';
        }
        else
        if (yy != -1 && yy != 7 && xx != -1 && xx != 7)
        {
          assert(yy+pos_y >= 0 && yy+pos_y < module_count);
          assert(xx+pos_x >= 0 && xx+pos_x < module_count);
          dst[yy+pos_y][xx+pos_x] = '0';
        }
      }
    }
  }
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "end s_setup_probe_position()");
#endif
}
/* 
 * タイミングパターン
 */
static void
s_setup_timing_pattern(qr_code_t* qrcode, char* dst[])
{
  int module_count = v_module_count_table[qrcode->version];
  int xx, yy;
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_timing_pattern()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "module_count[%d]", module_count);
#endif

  yy=6;
  for (xx=8; xx < module_count-8; xx++)
  {
    if (dst[yy][xx] != -1)
    {
      continue;
    }

    assert(yy >= 0 && yy < module_count);
    assert(xx >= 0 && xx < module_count);

    dst[yy][xx] = (xx % 2 == 0) ? '1' : '0';
  }

  xx=6;
  for (yy=8; yy < module_count-8; yy++)
  {
    if (dst[yy][xx] != -1)
    {
      continue;
    }

    assert(yy >= 0 && yy < module_count);
    assert(xx >= 0 && xx < module_count);

    dst[yy][xx] = (yy % 2 == 0) ? '1' : '0';
  }
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_timing_pattern()");
#endif
}

/*
 * 位置合わせパターン
 */
static void
s_setup_position_adjust(qr_code_t* qrcode, char* dst[])
{
  int module_count = v_module_count_table[qrcode->version];
  int* pos_list = v_position_adjust_table[qrcode->version].position;
  int xx, yy;
  int mxx, myy;
  int dxx, dyy;

#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_position_adjust()");
#endif

  for (yy=0; yy<7; yy++)
  {
    for (xx=0; xx<7; xx++)
    {
      mxx = pos_list[xx];
      myy = pos_list[yy];
      if (mxx == 0 || myy == 0)
      {
        continue;
      }
      if (dst[myy][mxx] != -1)
      {
        continue;
      }
      for (dyy = -2; dyy <= 2; dyy++)
      {
        for (dxx= -2; dxx <= 2; dxx++)
        {
          if (dyy == -2 
          ||  dxx == -2
          ||  dyy == 2
          ||  dxx == 2
          ||  (dyy == 0 && dxx == 0))
          {
            assert(myy+dyy >= 0);
            assert(myy+dyy < module_count);
            dst[myy+dyy][mxx+dxx] = '1';
          }
          else
          {
            assert(myy+dyy >= 0);
            assert(myy+dyy < module_count);
            dst[myy+dyy][mxx+dxx] = '0';
          }
        }
      }
    }
  }
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "end s_setup_position_adjust()");
#endif
}
static void
s_setup_type_info(qr_code_t* qrcode, char* dst[], qr_mask_pattern_t pat)
{
  int module_count = v_module_count_table[qrcode->version];
  int typedata;
  int sdata;
  int gf;
  int gfmask;
  int xx;
  int yy;

#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_type_info()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "module_count[%d]", module_count);
#endif

  /* 形式情報データの生成 */
  switch(qrcode->level) {
  case QR_LEVEL_L: sdata = 0x01 << 3; break;
  case QR_LEVEL_M: sdata = 0x00 << 3; break;
  case QR_LEVEL_Q: sdata = 0x03 << 3; break;
  case QR_LEVEL_H: sdata = 0x02 << 3; break;
  default:
    sdata = 0;
    break;
  }
  sdata |= pat;

  /* G(x)=x10+x8+x5+x4+x2+x+1 */
  gf = (0x01 << 10) 
     | (0x01 << 8) 
     | (0x01 << 5) 
     | (0x01 << 4) 
     | (0x01 << 2) 
     | (0x01 << 1)
     | (0x01 << 0);

  gfmask = (0x01 << 14) | (0x01 << 12) | (0x01 << 10)  | (0x01 << 4) | (0x01 << 1);


  typedata = sdata << 10;
  while (s_get_bit_count(typedata) - s_get_bit_count(gf) >= 0)
  {
    typedata ^= (gf << (s_get_bit_count(typedata) - s_get_bit_count(gf)));
  }
  typedata = (sdata << 10 | typedata);

  typedata ^= gfmask;

  for (yy=0; yy<6; yy++)
  {
    assert(yy >= 0 && yy < module_count);
    dst[yy][8] = (((typedata >> yy) & 0x01) == 1) ?  '1' : '0';
  }
  for (; yy<8; yy++)
  {
    assert(yy + 1 >= 0 && yy + 1 < module_count);
    dst[yy+1][8] = (((typedata >> yy) & 0x01) == 1) ?  '1' : '0';
  }
  for (; yy<15; yy++)
  {
    assert(module_count - 15 + yy >= 0 && module_count - 15 + yy < module_count);
    dst[module_count - 15 + yy][8] = (((typedata >> yy) & 0x01) == 1) ?  '1' : '0';
  }

  for (xx = 0; xx < 8; xx++)
  {
    assert(module_count - xx -1 >= 0 && module_count - xx - 1 < module_count);
    dst[8][module_count - xx - 1] = (((typedata >> xx) & 0x01) == 1) ? '1' : '0';
  }
  for (; xx<9; xx++)
  {
    assert(15 - xx >= 0 && 15 - xx < module_count);
    dst[8][15 - xx] = (((typedata >> xx) & 0x01) == 1) ? '1' : '0';
  }
  for (; xx<15; xx++)
  {
    assert(15 - xx - 1 >= 0 && 15 - xx - 1 < module_count);
    dst[8][15 - xx - 1] = (((typedata >> xx) & 0x01) == 1) ? '1' : '0';
  }
 
  assert(module_count - 8 >= 0 && module_count - 8 < module_count);
  dst[module_count - 8][8] = '1';

#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_setup_type_info()");
#endif
}

static int
s_get_bit_count(int data)
{
  int count = 0;

  while (data != 0) 
  {
    count++;
    data >>= 1;
  }

  return count;
}


static void
s_setup_version_info(qr_code_t* qrcode, char* dst[])
{
  char* bits = v_version_info_table[qrcode->version].bits;
  int module_count = v_module_count_table[qrcode->version];
  int xx;
  int yy;

  if (bits == NULL)
  {
    /* バージョン７以降のみ対象 */
    return;
  }

  /* 右上の型番情報の配置 */
  for (xx=0; xx<strlen(bits); xx++)
  {
    dst[(int)((double)xx/3.0)][(xx % 3) + module_count - 8 - 3] = bits[xx];
  }
  
  /* 左下の型番情報の配置 */
  for (yy=0; yy<strlen(bits); yy++) 
  {
    dst[(yy % 3) + module_count - 8 - 3][(int)((double)yy/3.0)] = bits[yy];
  } 
}

static void 
s_map_data(qr_code_t* qrcode,
           char* dst[], 
           unsigned char* indata, int data_count,qr_mask_pattern_t pat)
{
  int module_count = v_module_count_table[qrcode->version];
  int inc_yy_flag = -1;

  int xx, yy;
  int ii;
  int in_pos;
  int now_bit;
  int mask;

#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "start s_map_data()");
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "module_count[%d]", module_count);
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "data_count[%d]", data_count);
#endif
  in_pos = 0;
  now_bit = 7;

  yy = module_count - 1;

  for (xx = module_count - 1; xx > 0 ; xx -= 2)
  {
    if (xx == 6)
    {
      xx--;
    }
    do 
    {
      for (ii=0; ii < 2; ii++)
      {
        if (dst[yy][xx - ii] != -1)
        {
          /* 既に配置されている場合 */
          continue;
        }
        if (in_pos < data_count)
        {
          assert(yy >= 0 && yy < module_count);
          assert(xx-ii >= 0 && xx-ii < module_count);

          dst[yy][xx-ii] = (((indata[in_pos] >> now_bit) & 0x01) == 1) ? '1' : '0';
          mask = s_get_mask(pat, yy, xx - ii);
          if (mask == 1)
          {
            if (dst[yy][xx-ii] == '1')
            {
              dst[yy][xx-ii] = '0';
            }
            else
            {
              dst[yy][xx-ii] = '1';
            }
          }
        } 
        if (--now_bit < 0)
        {
          in_pos++;
          now_bit = 7;
        }
          
      }
      yy += inc_yy_flag;
    } while (yy >= 0 && yy < module_count) ;

    yy -= inc_yy_flag;
    inc_yy_flag = -inc_yy_flag;
  } 

  /* ??? */
  for (yy=0; yy<module_count; yy++)
  {
    for (xx=0; xx<module_count; xx++)
    {
      if (dst[yy][xx] == -1)
      {
        dst[yy][xx] = '0';
        mask = s_get_mask(pat, yy, xx);
        if (mask == 1)
        {
          dst[yy][xx] = '1';
        }
      }
    }
  }
#ifdef QR_CODE_DEBUG
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, qrcode->r, "end s_map_data()");
#endif
}

static int
s_get_mask(qr_mask_pattern_t pat, int yy, int xx)
{
  switch (pat) 
  {
  case QR_MASK_1: return ((yy + xx) % 2 == 0) ? 1 : 0;
  case QR_MASK_2: return (yy % 2 == 0) ? 1 : 0;
  case QR_MASK_3: return (xx % 3 == 0) ? 1 : 0;
  case QR_MASK_4: return ((yy + xx) % 3 == 0) ? 1 : 0;
  case QR_MASK_5: return (((yy / 2) + (xx / 3)) % 2 == 0) ? 1 : 0;
  case QR_MASK_6: return ((yy * xx) % 2 + (yy * xx) % 3 == 0) ? 1 : 0;
  case QR_MASK_7: return (((yy * xx) % 2 + (yy * xx) % 3) % 2 == 0) ? 1 : 0;
  case QR_MASK_8: return (((yy * xx) % 3 + (yy + xx) % 2) % 2 == 0) ? 1 : 0;
  default:
    return 0;
  }
}


static int
s_calc_lost_point(qr_code_t* qrcode, char* dst[])
{
  int point = 0;

  /* 同色の行／列の隣接モジュール モジュール数=(5+i) 3+i  */
  point = s_count_same_module(qrcode->version,dst);

  /* 同色のモジュールブロック ブロックサイズ 2×2 3  */
  point += s_count_same_block(qrcode->version, dst);

  /* 行／列における1:1:3:1:1(暗:明:暗:明:暗)のパターン */
  point += s_count_11311_pattern(qrcode->version, dst);

  /* 全体に占める暗モジュールの割合 50±(5+k)%〜50±(5+(k+1))% 10×k */
  point += s_count_dark_ratio(qrcode->version, dst);

  return point;
}

/* 
 * 同色の行／列の隣接モジュール モジュール数=(5+i) 
 * 失点=3+i
 */
static int
s_count_same_module(qr_ver_t ver, char* dst[])
{
  int module_count = v_module_count_table[ver];
  int point = 0;
  int xx, yy;
  int same_count = 0;
  int prev_value = -1;

  /* 横方向でチェック */
  for (yy=0; yy<module_count; yy++)
  {
    prev_value = -1;
    same_count = 0;
    for (xx=0; xx<module_count; xx++)
    {
      if (prev_value == (unsigned char)dst[yy][xx])
      {
        same_count++;
      }
      else
      {
        same_count = 0;
      }
      if (same_count >= 5)
      {
        point++;
      }
      prev_value = (unsigned char)dst[yy][xx];
    }
  }
  /* 縦方向でチェック */
  for (xx=0; xx<module_count; xx++)
  {
    prev_value = -1;
    same_count = 0;
    for (yy=0; yy<module_count; yy++)
    {
      if (prev_value == (unsigned char)dst[yy][xx])
      {
        same_count++;
      }
      else
      {
        same_count = 0;
      }
      if (same_count >= 5)
      {
        point++;
      }
      prev_value = (unsigned char)dst[yy][xx];
    }
  }
  if (point > 0)
  {
    point += 3;
  }

  return point;
}

/* 
 * 同色のモジュールブロック ブロックサイズ 2×2 
 */
static int
s_count_same_block(qr_ver_t ver, char* dst[])
{
  int module_count = v_module_count_table[ver];
  int point;
  int count;
  int xx,yy;

  point = 0;
  for (yy = 0; yy < module_count-1; yy++)
  {
    for (xx = 0; xx < module_count-1; xx++)
    {
      count = 0;
      if (dst[yy+0][xx+0] == '1') count++;
      if (dst[yy+0][xx+1] == '1') count++;
      if (dst[yy+1][xx+0] == '1') count++;
      if (dst[yy+1][xx+1] == '1') count++;
      if (count == 0 || count == 4)
      {
        point += 3;
      }
    }
  }
  return point;
}

static int
s_count_11311_pattern(qr_ver_t ver, char* dst[])
{
  int module_count = v_module_count_table[ver];
  int point;
  int xx,yy;

  point = 0;
  /* 横方向のチェック */
  for (yy = 0; yy < module_count; yy++)
  {
    for (xx = 0; xx < module_count-6; xx++)
    {
      if (dst[yy][xx+0] == '1'
      &&  dst[yy][xx+1] == '0'
      &&  dst[yy][xx+2] == '1'
      &&  dst[yy][xx+3] == '1'
      &&  dst[yy][xx+4] == '1'
      &&  dst[yy][xx+5] == '0'
      &&  dst[yy][xx+6] == '1')
      {
        point += 40;
      }
    }
  }

  /* 縦方向のチェック */
  for (xx = 0; xx < module_count; xx++)
  {
    for (yy = 0; yy < module_count-6; yy++)
    {
      if (dst[yy+0][xx] == '1'
      &&  dst[yy+1][xx] == '0'
      &&  dst[yy+2][xx] == '1'
      &&  dst[yy+3][xx] == '1'
      &&  dst[yy+4][xx] == '1'
      &&  dst[yy+5][xx] == '0'
      &&  dst[yy+6][xx] == '1')
      {
        point += 40;
      }
    }
  }

  return point;
}
/* 
 * 全体に占める暗モジュールの割合 50±(5+k)%〜50±(5+(k+1))% 
 */
static int
s_count_dark_ratio(qr_ver_t ver, char* dst[])
{
  int module_count = v_module_count_table[ver];
  int dark_count = 0;
  int xx, yy;
  int ratio;
  int all_count = module_count * module_count;

  /* 単に"暗"をカウントする */
  for (yy=0; yy<module_count; yy++)
  {
    for (xx=0; xx < module_count; xx++)
    {
      if (dst[yy][xx] == '1')
      {
        dark_count++;
      }
    }
  }

  /* % を求め、50％からの差を求める。 */
  ratio = (int)(((double)(dark_count) / (double)all_count)*100.0 - 50.0);
  ratio = abs(ratio);

  /* 5%毎に10点減点なので、まず5で割る */
  ratio = ratio / 5;
  return ratio * 10;
}



/*
 * vim:ts=2 et
 */
