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

#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/mman.h>
#include <unistd.h> 

#include "qs_malloc.h"
#include "qs_parse_string.h"
#include "qs_parse_file.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

Node*
qs_parse_file(Doc* doc, const char* filename) 
{
  char*       tgt = NULL;
  Node*       return_value = NULL;
  struct stat st;
  int         rtn;
  int         fd;


  memset(&st, 0, sizeof(struct stat));

  rtn = stat(filename, &st); 
  if (rtn != 0)
    return return_value;

  fd = open(filename, O_RDONLY);
  if (fd == -1)
    return return_value;

  tgt = (char*)mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if ((int)tgt == -1) {
    close(fd);
    return return_value;
  }

  doc->parse_mode = PARSE_MODE_CHTML;
  return_value = qs_parse_string(doc, tgt, st.st_size);

  munmap(tgt,st.st_size);
  close(fd);

  return return_value;
}
/*
 * vim:ts=2 et
 */
