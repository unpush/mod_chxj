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
#ifndef __CHXJ_EMOJI_H__
#define __CHXJ_EMOJI_H__

#define EMOJI_COUNT                (252)
#define REFSTRING_MAX_LEN          (16)
#define EMOJI_BINCODE_MAX_LEN      (16)
#define META_EMOJI_PREFIX          "&chxjEmoji"
#define META_EMOJI_PREFIX_LEN      (10)
#define EMOJI_EZWEB2IMODE_COUNT    (646)
#define EMOJI_SOFTBANK2IMODE_COUNT (485)

#define IS_SOFTBANK_WEBCODE(X)          \
   ((X) && *(X) &&                      \
    strlen((X)) >= 3 &&                 \
    *(X) == (unsigned char)0x1b &&      \
    *((X)+1) == (unsigned char)0x24 &&  \
      (*((X)+2) == (unsigned char)'G'   \
    || *((X)+2) == (unsigned char)'E'   \
    || *((X)+2) == (unsigned char)'F'   \
    || *((X)+2) == (unsigned char)'O'   \
    || *((X)+2) == (unsigned char)'P'   \
    || *((X)+2) == (unsigned char)'Q'))
#define IS_SOFTBANK_WEBCODE_ENDMARK(X) \
   ((X) == (unsigned char)0x0f)

/*===============================================================*/
/* Emoji Data Structure.                                         */
/*===============================================================*/
typedef struct emoji_data_t {
  char *hex_string;
  char *dec_string;
  char hex[8];
} emoji_data_t;

typedef struct imode_emoji_t {
  emoji_data_t sjis;
  emoji_data_t euc;
  emoji_data_t unicode;
  emoji_data_t utf8;
  char *description;
} imode_emoji_t;

typedef struct ezweb_type_t {
  char *no;
  emoji_data_t sjis;
  emoji_data_t unicode;
  emoji_data_t utf8;
} ezweb_type_t;

typedef struct ezweb_emoji_t {
  ezweb_type_t typeA;
  ezweb_type_t typeB;
  ezweb_type_t typeC;
  ezweb_type_t typeD;
} ezweb_emoji_t;

typedef struct softbank_emoji_t {
  char *no;
  emoji_data_t sjis;
  emoji_data_t unicode;
  emoji_data_t utf8;
} softbank_emoji_t;

typedef struct emoji_t {
  struct emoji_t *next;
  int             no;
  imode_emoji_t  imode;
  ezweb_emoji_t  ezweb;
  softbank_emoji_t softbank;
} emoji_t;


/**
 * &lt;ezweb2imode&gt;
 *   &lt;set&gt;
 *     &lt;no&gt;1&lt;/no&gt;
 *     &lt;ezweb&gt;
 *       &lt;sjis-hex&gt;F659&lt;/sjis-hex&gt;
 *       &lt;utf8-hex&gt;eebd99&lt;/utf-hex&gt;
 *     &lt;/ezweb&gt;
 *     &lt;imode&gt;
 *       &lt;no&gt;220&lt;/no&gt;
 *     &lt;/imode&gt;
 *    &lt;/set&gt;
 * &lt;/ezweb2imode&gt;
 */
typedef struct ezweb2imode_t {
  struct ezweb2imode_t *next;
  int no;
  emoji_data_t ezweb_sjis;
  emoji_data_t ezweb_utf8;
  char *imode_no;
} ezweb2imode_t;


/**
 * &lt;softbank2imode&gt;
 *   &lt;set&gt;
 *     &lt;no&gt;1&lt;/no&gt;
 *     &lt;softbank&gt;
 *       &lt;sjis-hex&gt;1b2447210f&lt;/sjis-hex&gt;
 *       &lt;utf8-hex&gt;ee8081&lt;/utf-hex&gt;
 *     &lt;/softbank&gt;
 *     &lt;imode&gt;
 *       &lt;no&gt;140&lt;/no&gt;
 *     &lt;/imode&gt;
 *   &lt;/set&gt;
 * &lt;/softbank2imode&gt;
 */
typedef struct softbank2imode_t {
  struct softbank2imode_t *next;
  int no;
  emoji_data_t softbank_webcode;
  emoji_data_t softbank_sjis;
  emoji_data_t softbank_utf8;
  char *imode_no;
} softbank2imode_t;



extern void
chxj_emoji_init(
  mod_chxj_config *conf);

extern char *
chxj_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *ilen);

extern char *
chxj_meta_emoji_to_emoji(
  request_rec         *r,
  device_table        *spec,
  chxjconvrule_entry  *entryp,
  emoji_t             **emoji_table,
  char                *src);


extern char *
chxj_postdata_ezweb_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen);

extern char *
chxj_postdata_softbank_emoji_to_meta_emoji(
  request_rec         *r,
  const char          *encoding,
  const char          *src,
  apr_size_t          *iolen);

extern char *
chxj_postdata_meta_emoji_to_emoji(
  request_rec         *r,
  char                *encoding,
  const char          *src,
  device_table        *spec);

#endif
