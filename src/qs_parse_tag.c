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
#include "qs_malloc.h"
#include "qs_parse_string.h"
#include "qs_log.h"
#include "qs_parse_attr.h"
#include "qs_parse_tag.h"

static char* s_get_tag_name(Doc* doc, const char* s, int len) ; 



QS_EXPORT Node*
qs_parse_tag(Doc* doc, const char* s, int len) {
  Node* node;
  char *tag_name;
  char *sp = (char*)s;
  int ll = len;
  int next_point = 0;

  QX_LOGGER_DEBUG("start parse_tag()");


  /* s[0] == '<' && s[len-1] == '>' */
  tag_name = (char *)s_get_tag_name(doc, ++s, --ll);
  QX_LOGGER_DEBUG_INT("ll",ll);
  node = (Node*)qs_new_tag(doc);
  node->name = tag_name;
  node->otext = qs_malloc(doc,len+2, QX_LOGMARK);
  memset(node->otext, 0, len+2);
  memcpy(node->otext, sp, len+1);

  QX_LOGGER_DEBUG(tag_name);

  ll -= (strlen(tag_name));
  QX_LOGGER_DEBUG_INT("ll",ll);
  sp += (strlen(tag_name)+1);
  for (;;) {
    Attr* attr = qs_parse_attr(doc,sp, ll, &next_point);
    if (attr == NULL) {
      QX_LOGGER_DEBUG("End of QS_PARSE_ATTR()");
      break;
    }
    QX_LOGGER_DEBUG(attr->name);
    QX_LOGGER_DEBUG(attr->value);
    sp += next_point;
    ll -= next_point;
    QX_LOGGER_DEBUG_INT(sp, ll);
    node = (Node*)qs_add_attr(doc,node, attr);
  }

  QX_LOGGER_DEBUG("end parse_tag()");
  return node;
}




static char* 
s_get_tag_name(Doc* doc, const char* s, int len)  {
  int ii;
  int sp;
  int size;
  char* return_value = NULL;

  /* ignore space. */
  for (ii = 0; ii < len; ii++) {
    if (is_white_space(s[ii])) {
      continue;
    }
    break;
  }
  sp = ii;
  for (;ii<len; ii++) {
    if (is_white_space(s[ii])) {
      break;
    }
  }

  size = ii-sp;

  return_value = (char*)qs_malloc(doc, size+1,QX_LOGMARK);

  memset(return_value, 0, size+1);
  memcpy(return_value, &s[sp], size);

  QX_LOGGER_DEBUG(return_value);
  return return_value;
}



QS_EXPORT Node*
qs_new_tag(Doc* doc) {
  Node* node      = (Node*)qs_malloc(doc, sizeof(Node), QX_LOGMARK);
  node->next      = NULL;
  node->parent    = NULL;
  node->child     = NULL;
  node->child_tail= NULL;
  node->attr      = NULL;
  node->attr_tail = NULL;
  node->name      = NULL;
  node->value     = NULL;

  return node;
}




QS_EXPORT Node*
qs_add_attr(Doc* doc, Node* node, Attr* attr) {
  if (node == NULL) 
  {
    QX_LOGGER_FATAL("qs_add_attr() node is null");
  }

  attr->parent = node;
  attr->next   = NULL;
  if (node->attr == NULL) 
  {
    node->attr      = attr;
    node->attr_tail = attr;
    return node;
  }

  node->attr_tail->next = attr;
  node->attr_tail       = attr;
  return node;
}
/*
 * vim:ts=2 et
 */
