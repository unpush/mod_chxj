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
#include "chxj_str_util.h"

int
chxj_chk_numeric(const char* s)
{
  int len = strlen(s);
  int ii;

  for (ii=0; ii<len; ii++) {
    if (ii == 0 && (s[ii] < '0' || s[ii] > '9') && s[ii] != '-') {
      return -1; /* NG */
    }
    if (ii != 0 && (s[ii] < '0' || s[ii] > '9')) 
    {
      return -1; /* NG */
    }
  }
  return 0; /* OK */
}

int
chxj_atoi(const char* s)
{
  int len = strlen(s);
  int ii;
  int result;
  int mflag = 0;

  result = 0;
  for (ii=0; ii < len; ii++) 
  {
    result *= 10;
    switch(s[ii]) 
    {
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
      if (ii == 0)
      {
        mflag = 1;
      }
      break;
    default: break;
    }
  }
  if (mflag)
  {
    result *= -1;
  }
  return result;
}
/*
 * vim:ts=2 et
 */
