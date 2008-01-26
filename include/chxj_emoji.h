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

#define EMOJI_COUNT     (252)
#define REFSTRING_MAX_LEN (16)
#define EMOJI_BINCODE_MAX_LEN (16)
#define META_EMOJI_PREFIX "&chxjEmoji"
#define META_EMOJI_PREFIX_LEN (10)

extern void
chxj_emoji_init(
  mod_chxj_config *conf);

extern char *
chxj_emoji_to_meta_emoji(
  request_rec         *r,
  chxjconvrule_entry  *entryp,
  const char          *src,
  apr_size_t          *ilen);

extern char *
chxj_meta_emoji_to_emoji(
  request_rec         *r,
  device_table        *spec,
  chxjconvrule_entry  *entryp,
  emoji_t             **emoji_table,
  char                *src);

#endif
