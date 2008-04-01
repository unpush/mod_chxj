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
#ifndef __CHXJ_IMG_CONV_FORMAT_H__
#define __CHXJ_IMG_CONV_FORMAT_H__


#define DEFAULT_IMAGE_CACHE_DIR "/tmp"

/* default: 10MByte */
#define DEFAULT_IMAGE_CACHE_LIMIT (10*1024*1024)    


extern int chxj_img_conv_format_handler(
  request_rec *r);

extern char *chxj_convert_image(
  request_rec *r, 
  const char  **src, 
  apr_size_t  *len);

extern int chxj_trans_name(
  request_rec *r);


/* micro second */
#define CACHE_RECHECK_WAIT        (200000)

/* retry count of creating cache file */
#define CACHE_RETRY_COUNT         (5)

#endif
/*
 * vim:ts=4 et
 */
