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
#include "qs_log.h"
#include "qs_malloc.h"

/**
 * Init
 */
void
qs_init_malloc(Doc* doc) 
{
  int ii;
  doc->alloc_size = 0;
  doc->pointer_table = (Pointer_Table*)malloc(sizeof(Pointer_Table)* QX_ALLOC_MAX);
  if (doc->pointer_table == NULL)
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  for (ii=0; ii<QX_ALLOC_MAX; ii++) {
    doc->pointer_table[ii].address = 0;
    doc->pointer_table[ii].size    = 0;
  }
  doc->do_init_flag = 1;
}

/**
 *it is a wrapper of "malloc()" function. 
 */
void*
qs_malloc(Doc* doc, int size, const char* fname, int line) 
{
  int ii;

  if (doc->do_init_flag == 0) 
  {
    qs_init_malloc(doc);
  }

  if (doc->alloc_size + size >= QX_ALLOC_MAX_SIZE) 
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  void* src = (void*)malloc(size);
  if (src == NULL) 
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }
#ifdef MALLOC_TRACE
  char buffer[BUFSIZ];
  sprintf(buffer,"malloc Address:[0x%x]", src);
  qs_log(doc,QX_LOG_DEBUG,fname,line,buffer);
#endif
  for (ii=0; ii<QX_ALLOC_MAX; ii++) 
  {
    if (doc->pointer_table[ii].address == 0) 
    {
      doc->pointer_table[ii].address = (unsigned int)src;
      doc->pointer_table[ii].size = size;
      doc->alloc_size += size;
      QX_LOGGER_DEBUG("use new space");
      break;
    }
  }
  if (ii == QX_ALLOC_MAX) 
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  QX_LOGGER_DEBUG_INT("allocated memory size",doc->alloc_size);
  return src;
}

/**
 *it is a wrapper of "free()" function. 
 */
void
qs_free(Doc* doc, void *s, const char* fname, int line) 
{
  int ii;
#ifdef MALLOC_TRACE
  char buffer[BUFSIZ];
  sprintf(buffer,"free Address:[0x%x]", s);
  qs_log(doc,QX_LOG_DEBUG,fname,line,buffer);
#endif
  for (ii=0; ii<QX_ALLOC_MAX; ii++) 
  {
    if (doc->pointer_table[ii].address == (unsigned int)s) 
    {
      doc->pointer_table[ii].address = 0;
      doc->alloc_size -= doc->pointer_table[ii].size;
      doc->pointer_table[ii].size = 0;
    }
  }
  free(s);
}


void
qs_all_free(Doc* doc, const char* fname, int line) 
{
  int ii;

  if (doc->do_init_flag) 
  {
    for (ii=0; ii < QX_ALLOC_MAX; ii++) 
    {
      if (doc->pointer_table != NULL 
      &&  doc->pointer_table[ii].address != 0) 
      {
        free((void*)(doc->pointer_table[ii].address));
        doc->pointer_table[ii].address = 0;
        doc->alloc_size -= doc->pointer_table[ii].size;
        doc->pointer_table[ii].size =0;
      }
    }
    free(doc->pointer_table);
    doc->pointer_table = NULL;
  }

  doc->do_init_flag = 0;
}
/*
 * vim:ts=2 et
 */
