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

static Pointer_Table* s_get_free_pointer_table(Doc* doc);
static void s_add_to_allocated_list(Doc* doc, Pointer_Table* pt);
static void s_add_to_free_list(Doc* doc, Pointer_Table* pt);
/**
 * Init
 */
void
qs_init_malloc(Doc* doc) 
{
  int ii;
  doc->alloc_size = 0L;
  doc->pointer_table = (Pointer_Table*)malloc(sizeof(Pointer_Table)* QX_ALLOC_MAX);
  if (doc->pointer_table == NULL)
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  for (ii=0; ii<QX_ALLOC_MAX; ii++) 
  {
    doc->pointer_table[ii].address = 0;
    doc->pointer_table[ii].size    = 0L;
    doc->pointer_table[ii].next    = NULL;
    doc->pointer_table[ii].prev    = NULL;
    if (ii==0)
    {
      doc->free_list_head = &(doc->pointer_table[ii]);
    }
    else
    {
      doc->pointer_table[ii].prev = &(doc->pointer_table[ii-1]);
    }

    if (ii < QX_ALLOC_MAX-1)
    {
      doc->pointer_table[ii].next = &(doc->pointer_table[ii+1]);
    }
  }
  doc->free_list_tail = &(doc->pointer_table[QX_ALLOC_MAX-1]);
  doc->do_init_flag = 1;
  doc->allocated_list_head = NULL;
  doc->allocated_list_tail = NULL;
}

/**
 *it is a wrapper of "malloc()" function. 
 */
void*
qs_malloc(Doc* doc, int size, const char* fname, int line) 
{
  Pointer_Table* pt;

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

  pt = s_get_free_pointer_table(doc);
  if (pt == NULL)
  {
    QX_LOGGER_FATAL("Out Of Memory");
  }

  pt->address      = (unsigned int)src;
  pt->size         = size;
  doc->alloc_size += size;

  s_add_to_allocated_list(doc, pt);

  QX_LOGGER_DEBUG_INT("allocated memory size",doc->alloc_size);
  return src;
}



static void
s_add_to_allocated_list(Doc* doc, Pointer_Table* pt)
{
  pt->prev = NULL;
  if (doc->allocated_list_head == NULL)
  {
    doc->allocated_list_head = pt;
    doc->allocated_list_tail = pt;
  }
  else
  {
    doc->allocated_list_tail->next = pt;
    pt->prev = doc->allocated_list_tail;
    doc->allocated_list_tail = pt;
  }
  pt->next = NULL;
}


static Pointer_Table*
s_get_free_pointer_table(Doc* doc)
{
  Pointer_Table* pt = NULL;

  if (doc->free_list_head != NULL)
  {
    pt = doc->free_list_head;
    doc->free_list_head = doc->free_list_head->next;
    if (doc->free_list_head != NULL)
    {
      doc->free_list_head->prev = NULL;
    }
    pt->next = NULL;
    pt->prev = NULL;
  }
  return pt;
}

/**
 *it is a wrapper of "free()" function. 
 */
void
qs_free(Doc* doc, void *s, const char* fname, int line) 
{
  Pointer_Table* pp;
#ifdef MALLOC_TRACE
  char buffer[BUFSIZ];
  sprintf(buffer,"free Address:[0x%x]", s);
  qs_log(doc,QX_LOG_DEBUG,fname,line,buffer);
#endif

  for (pp = doc->allocated_list_tail; pp; pp = pp->prev)
  {
    if (pp->address == (unsigned int)s)
    {
      free((void*)pp->address);
      pp->address = 0;
      doc->alloc_size -= pp->size;
      pp->size = 0;

      break;
    }
  }
  if (pp == NULL)
  {
    return;
  }
  if (pp->next == NULL && pp->prev == NULL)
  {
    /* 先頭で最後の場合 */
    doc->allocated_list_head = NULL;
    doc->allocated_list_tail = NULL;
  }
  else 
  {
    if (pp->prev == NULL)
    {
      /* 先頭の場合 */
      pp->next->prev = pp->prev;
      doc->allocated_list_head = pp->next;
    }
    else
    if (pp->next == NULL)
    {
      /* 最後の場合 */
      pp->prev->next = pp->next;
      doc->allocated_list_tail = pp->prev;
    }
    else
    {
      /* 途中の場合 */
      pp->prev->next = pp->next;
      pp->next->prev = pp->prev;
    }
    pp->next = NULL;
    pp->prev = NULL;
  }
  s_add_to_free_list(doc, pp);

}



static void
s_add_to_free_list(Doc* doc, Pointer_Table* pt)
{
  if (doc->free_list_head == NULL)
  {
    doc->free_list_head = pt;
    doc->free_list_tail = pt;
  }
  else
  {
    doc->free_list_tail->next = pt;
    pt->prev = doc->free_list_tail;
    doc->free_list_tail = pt;
  }
  pt->next = NULL;
} 


void
qs_all_free(Doc* doc, const char* fname, int line) 
{
  Pointer_Table* pp;

  if (doc->do_init_flag) 
  {
    for (pp = doc->allocated_list_tail; pp; pp = pp->prev)
    {
      free((void*)pp->address);
      pp->address = 0;
      pp->size =0;
    }
    free((void*)(doc->pointer_table));
    doc->pointer_table       = NULL;
    doc->allocated_list_head = NULL;
    doc->allocated_list_tail = NULL;
    doc->free_list_head      = NULL;
    doc->free_list_tail      = NULL;
  }
  doc->alloc_size = 0L;

  doc->do_init_flag = 0;
}
/*
 * vim:ts=2 et
 */
