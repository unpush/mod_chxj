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
#include "chxj_jreserved_tag.h"

typedef struct r_table {
  char lower;
  char upper;
  char *name;
} r_table_t;

static r_table_t reserved_start_with_table[] = {
  {
    .lower = 'j',
    .upper = 'J',
    .name  = "jsky",
  },
};


#define RESERVED_NELT            (16)
#define RESERVED_NELT_START_WITH (1)

static r_table_t reserved_table[] = {
  {
    .lower = 'p',
    .upper = 'P',
    .name = "pid",
  },
  {
    .lower = 's',
    .upper = 'S',
    .name = "sid",
  },
  { 
    .lower = 'u',
    .upper = 'U',
    .name  = "uid",
  },
  {
    .lower = 'l',
    .upper = 'L',
    .name  = "lid",
  },
  {
    .lower = 'g',
    .upper = 'G',
    .name  = "gid",
  },
  {
    .lower = 'r',
    .upper = 'R',
    .name  = "rpid",
  },
  {
    .lower = 'r',
    .upper = 'R',
    .name  = "rsid",
  },
  {
    .lower = 'n',
    .upper = 'N',
    .name  = "nl",
  },
  {
    .lower = 'c',
    .upper = 'C',
    .name  = "cl",
  },
  {
    .lower = 'o',
    .upper = 'O',
    .name  = "ol",
  },
  {
    .lower = 'p',
    .upper = 'P',
    .name  = "pl",
  },
  {
    .lower = 'p',
    .upper = 'P',
    .name  = "prc",
  },
  {
    .lower = 'c',
    .upper = 'C',
    .name  = "cnt",
  },
  {
    .lower = 'r',
    .upper = 'R',
    .name  = "reg",
  },
  {
    .lower = 'v',
    .upper = 'V',
    .name  = "vsekey",
  },
  {
    .lower = 'v',
    .upper = 'V',
    .name  = "vsernk",
  },
};


char *
chxj_jreserved_to_safe_tag(request_rec *r, const char *src)
{
  int ii;
  for (ii=0;ii<RESERVED_NELT;ii++) {
    if (STRCASEEQ(reserved_table[ii].lower,
                  reserved_table[ii].upper,
                  reserved_table[ii].name, 
                  src)) {
      return apr_psprintf(r->pool, "%s%s", CHXJ_SOFTBANK_RESERVED_TAG_PREFIX, reserved_table[ii].name);
    }
  }
  for (ii=0; ii<RESERVED_NELT_START_WITH; ii++) {
    if (strncasecmp(reserved_start_with_table[ii].name, src, sizeof(reserved_start_with_table[ii].name)) == 0) {
      return apr_psprintf(r->pool, "%s%s", CHXJ_SOFTBANK_RESERVED_TAG_PREFIX, src);
    }
  }
  return (char *)src;
}


char *
chxj_safe_to_jreserved_tag(request_rec *r, const char *src)
{
  if (strncasecmp(CHXJ_SOFTBANK_RESERVED_TAG_PREFIX, src, sizeof(CHXJ_SOFTBANK_RESERVED_TAG_PREFIX)-1) == 0) {
    return apr_pstrdup(r->pool, &src[sizeof(CHXJ_SOFTBANK_RESERVED_TAG_PREFIX)-1]);
  }
  return (char *)src;
}
