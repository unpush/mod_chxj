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
#include "mod_chxj.h"

static device_table  UNKNOWN_DEVICE      = {
  .next               = NULL,
  .device_id          = "",
  .device_name        = "UNKNOWN",
  .html_spec_type     = CHXJ_SPEC_UNKNOWN,
  .width              = 640,
  .heigh              = 480,
  /*--------------------------------------------------------------------------*/
  /* Walll Paper Size                                                         */
  /*--------------------------------------------------------------------------*/
  .wp_width           = 640,
  .wp_heigh           = 480,
  /*--------------------------------------------------------------------------*/
  /* Cache Size                                                               */
  /*--------------------------------------------------------------------------*/
  .cache              = 10000000,
  /*--------------------------------------------------------------------------*/
  /* Correspondence image format                                              */
  /* 1: It is possible to display it.                                         */
  /* 0: It is not possible to display it.                                     */
  /*--------------------------------------------------------------------------*/
  .available_gif      = 1,
  .available_jpeg     = 1,
  .available_png      = 1,
  .available_bmp2     = 1,
  .available_bmp4     = 1,
  .dpi_width          = 96,
  .dpi_heigh          = 96,
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
  .color              = 15680000,
  .emoji_type         = NULL,
};

/**
 * The device is specified from UserAgent. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
device_table *
chxj_specified_device(request_rec *r, const char *user_agent) 
{
  ap_regmatch_t        match[10];
  device_table         *returnType = &UNKNOWN_DEVICE;
  device_table_list    *dtl;
  device_table         *dt;
  mod_chxj_config      *conf; 
  char                 *device_id;

  if (! user_agent) 
    return returnType;
            

  DBG(r, "start chxj_specified_device()");

  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (! conf->devices) {
    DBG(r, "device_data.xml load failure");
    return returnType;
  }

  for (dtl = conf->devices; dtl; dtl = dtl->next) {
    if (! dtl->pattern) {
      DBG(r, "pattern is null");
      continue;
    }

    DBG(r, "pattern is [%s]", dtl->pattern);
    if (! dtl->regexp) {
      DBG(r,"compile failed.");
      return returnType;
    }

    if (ap_regexec((const ap_regex_t *)dtl->regexp, user_agent, (apr_size_t)dtl->regexp->re_nsub + 1, match, 0) == 0) {
      device_id = ap_pregsub(r->pool, "$1", user_agent, dtl->regexp->re_nsub + 1, match);
      DBG(r, "device_id:[%s]", device_id);
      for (dt = dtl->table; dt; dt = dt->next) {
        if (strcasecmp(device_id, dt->device_id) == 0) {
          DBG(r, "device_name:[%s]", dt->device_name);
          returnType = dt;
          break;
        }
      }

      if (! dt) {
        for (dt = dtl->table; dt; dt = dt->next) {
          if (dt->next == NULL)
            break;
        }

        if (dt)
          returnType = dt;
      }
    }

    if (returnType != &UNKNOWN_DEVICE) {
      DBG(r,"end chxj_specified_device()");
      return returnType;
    }
  }

  DBG(r,"end chxj_specified_device()");

  return returnType;
}
/*
 * vim:ts=2 et
 */
