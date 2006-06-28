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
#include <stdio.h>

void
chxj_dump_out(const char* title, const char* s, int slen)
{
#ifdef DUMP_LOG
  FILE* fp=fopen(DUMP_LOG, "a");
  if (fp != NULL) {
    fprintf(fp, "%s\n", title);
    fprintf(fp, "[%.*s]\n", slen, s);
    fprintf(fp, "-----------------------------\n");
    fclose(fp);
  }
#endif
}
/*
 * vim:ts=2 et
 */
