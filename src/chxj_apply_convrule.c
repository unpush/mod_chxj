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
#include "chxj_apply_convrule.h"

static int s_apply_rule(request_rec* r, chxjconvrule_entry* pp);

chxjconvrule_entry*
chxj_apply_convrule(request_rec* r, apr_array_header_t* convrules)
{
  chxjconvrule_entry *entries;
  chxjconvrule_entry *pp;
  int                ii;

  entries = (chxjconvrule_entry *)convrules->elts;
  for (ii = 0; ii < convrules->nelts; ii++) {
    pp = &entries[ii];

    if (r->main != NULL) continue;

    /* Match */
    if (s_apply_rule(r, pp)) 
      return pp;
  }
  return NULL;
}

static int
s_apply_rule(request_rec* r, chxjconvrule_entry* pp) 
{
  char* uri;
  int   rtn;
  ap_regmatch_t regmatch[AP_MAX_REG_MATCH];

  uri = r->uri;

  DBG2(r,"convert rule pattern=[%s] uri=[%s]", pp->pattern, uri);

  rtn = ap_regexec((const ap_regex_t*)pp->regexp, uri, AP_MAX_REG_MATCH, (ap_regmatch_t*)regmatch, 0);
  if (rtn == 0) {
    /* Match */
    if (pp->flags & CONVRULE_FLAG_NOTMATCH) {
      /* not match */
      return 0;
    }
  }
  else {
    /* Unmatch */
    if (!(pp->flags & CONVRULE_FLAG_NOTMATCH)) {
      /* not match */
      return 0;
    }
  }

  /* Match */
  return 1;
}
/*
 * vim:ts=2 et
 */
