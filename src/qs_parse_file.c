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
#include <strings.h>
#include "qs_malloc.h"
#include "qs_parse_string.h"
#include "qs_parse_file.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

QS_EXPORT Node*
qs_parse_file(Doc* doc, const char* filename) {
  FILE* fp;
  char buffer[256];
  char *tgt = NULL;
  char *tmp = NULL;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    return NULL;
  }

  tgt = (char*)qs_malloc(doc, 1, QX_LOGMARK);
  tgt[0] = 0;
  while((fgets(buffer, 256, fp)) != NULL) {
    tmp = (char*)qs_malloc(doc,strlen(tgt)+1, QX_LOGMARK);
    memset(tmp, 0, strlen(tgt)+1);
    memcpy(tmp, tgt, strlen(tgt));
    qs_free(doc,tgt, QX_LOGMARK);
    tgt = (char*)qs_malloc(doc,strlen(tmp)+ strlen(buffer)+1, QX_LOGMARK);
    memset(tgt, 0, strlen(tmp)+strlen(buffer)+1);
    memcpy(tgt, tmp,strlen(tmp));
    memcpy(&tgt[strlen(tmp)], buffer, strlen(buffer));
    qs_free(doc,tmp, QX_LOGMARK);
  }
  fclose(fp);

  return qs_parse_string(doc, tgt);
}
/*
 * vim:ts=2 et
 */
