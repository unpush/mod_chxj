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
#include "chxj_str_util.h"

int
chxj_chk_numeric(const char *s)
{
  int len;
  int ii;

  if (! s) return -1; /* NG(not numeric) */

  len = strlen(s);
  if (len == 0) return -1; /* NG(not numeric) */

  for (ii=0; ii<len; ii++) {
    if (ii == 0 && (s[ii] < '0' || s[ii] > '9') && s[ii] != '-') {
      return -1; /* NG(not numeric) */
    }
    if (ii != 0 && (s[ii] < '0' || s[ii] > '9')) {
      return -1; /* NG(not numeric) */
    }
  }

  return 0; /* OK */
}



int
chxj_atoi(const char *s)
{
  int len;
  int ii;
  int result;
  int mflag = 0;
  int break_flag = 0;

  if (! s) return 0;

  result = 0;

  len = strlen(s);

  for (ii=0; ii < len && break_flag == 0; ii++) {
    result *= 10;
    switch(s[ii]) {
    case '0': result += 0; break; 
    case '1': result += 1; break;
    case '2': result += 2; break;
    case '3': result += 3; break;
    case '4': result += 4; break;
    case '5': result += 5; break;
    case '6': result += 6; break;
    case '7': result += 7; break;
    case '8': result += 8; break;
    case '9': result += 9; break;
    case '-': 
      result /= 10;
      if (ii != 0) {
        break_flag = 1;
        break;
      }
      mflag = 1;
      break;

    case '+':
      result /= 10;
      if (ii != 0) {
        break_flag = 1;
        break;
      }
      break;

    default: 
      break_flag = 1;
      result /= 10;
      break;
    }
  }

  if (mflag)
    result *= -1;

  return result;
}

int
chxj_strcasenrcmp(apr_pool_t *p, const char *s1, const char *s2, int n)
{
    register int s1_len;
    register int s2_len;
    char *ss1;
    char *ss2;
    register char *ss1p;
    register char *ss2p;

    s1_len = strlen(s1) - 1;
    s2_len = strlen(s2) - 1;

    ss1 = (char *)apr_palloc(p, s1_len + 2);
    if (!ss1) {
      return -1;
    }
    ss2 = (char *)apr_palloc(p, s2_len + 2);
    if (!ss2) {
      return -1;
    }

    strcpy(&ss1[1], s1);
    strcpy(&ss2[1], s2);
    ss1[0] = 0;
    ss2[0] = 0;
    ss1p = &ss1[s1_len+1];
    ss2p = &ss2[s2_len+1];

    for (;*ss1p && *ss2p && *ss1p == *ss2p && n - 1 > 0; ss1p--, ss2p--, n--);

    return (int)(*ss1p - *ss2p);
}


int
chxj_starts_with(const char *str, const char *word)
{
  int len = strlen(word);
  return strncasecmp(str, word, len) == 0;
}
/*
 * vim:ts=2 et
 */
