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
#include "mod_chxj.h"
#include "ap_regex.h"

static device_table_t  UNKNOWN_DEVICE      = {
    NULL, "","UNKNOWN", CHXJ_SPEC_UNKNOWN,  0,  0,0,0,0,0,0,0,0,0,0,0,0, ""};

/**
 * The device is specified from UserAgent. 
 * @param r Request_rec is appointed.
 * @param userAgent UserAgent is appointed here,
 * @return The style which corresponds is returned.
 */
device_table_t*
chxj_specified_device(request_rec* r, const char* user_agent) 
{
  ap_regex_t*          regexp;
  ap_regmatch_t        match[10];
  device_table_t*      returnType = &UNKNOWN_DEVICE;
  device_table_list_t* dtl;
  device_table_t* dt;
  mod_chxj_config_t* conf; 
  int rtn;
  char* device_id;

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "start chxj_specified_device()");
  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  for (dtl = conf->devices; dtl; dtl = dtl->next) 
  {
    if (dtl->pattern == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "pattern is null");
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "pattern is [%s]", dtl->pattern);
    regexp = ap_pregcomp(r->pool, (const char*)dtl->pattern, AP_REG_EXTENDED|AP_REG_ICASE);
    if (regexp == NULL) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "compile failed.");
      return returnType;
    }

    rtn = ap_regexec(regexp, user_agent, regexp->re_nsub + 1, match, 0);
    if (rtn == 0) 
    {
      device_id = ap_pregsub(r->pool, "$1", user_agent, regexp->re_nsub + 1, match);
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "device_id:[%s]", device_id);
      for (dt = dtl->table; dt; dt = dt->next) 
      {
        if (strcasecmp(device_id, dt->device_id) == 0) 
        {
          ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "device_name:[%s]", dt->device_name);
          returnType = dt;
          break;
        }
      }
      if (dt == NULL) 
      {
        for (dt = dtl->table; dt; dt = dt->next) 
        {
          if (dt->next == NULL) {
            break;
          }
        }
        if (dt != NULL)
        {
          returnType = dt;
        }
      }
    }
    ap_pregfree(r->pool, regexp);
    if (returnType != &UNKNOWN_DEVICE) 
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "end chxj_specified_device()");
      return returnType;
    }
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "end chxj_specified_device()");

  return returnType;
}

/*
 * vim:ts=2 et
 */

