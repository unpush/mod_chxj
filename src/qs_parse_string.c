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
#include "httpd.h"
#include "http_log.h"
#include "qs_malloc.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

static int s_cut_tag (const char* s, int len) ;
static int s_cut_text(const char* s, int len) ;
static void qs_dump_node(Doc* doc, Node* node, int indent);


QS_EXPORT Node*
qs_parse_string(Doc* doc, const char* src, int srclen) {
  int ii;

#ifdef DEBUG
  QX_LOGGER_DEBUG("parse_string start");
#endif
  doc->now_parent_node = qs_init_root_node(doc);

#ifdef DEBUG
  QX_LOGGER_DEBUG("root_node init end");
#endif

  for (ii=0; ii<srclen; ii++) {
    if (is_white_space(src[ii])) {
#ifdef DEBUG
      QX_LOGGER_DEBUG("white_space");
#endif
      continue;
    }
    if ((unsigned char)'<' == src[ii]) {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
#ifdef DEBUG
  QX_LOGGER_DEBUG("call qs_parse_tag()");
#endif
      node = qs_parse_tag(doc, &src[ii], endpoint);
#ifdef DEBUG
  QX_LOGGER_DEBUG("return from qs_parse_tag()");
#endif

      ii += endpoint;
      if (node->name[0] == '/' ) 
      {
        if (has_child(node->name)) 
        {
          if (doc->now_parent_node->parent != NULL)
          {
#ifdef DEBUG
         {char buf[256]; sprintf(buf, "[%s]->[%s]", 
                         doc->now_parent_node->name, 
                         doc->now_parent_node->parent->name); QX_LOGGER_DEBUG(buf);}
#endif
            doc->now_parent_node = doc->now_parent_node->parent;
          }
        }
        qs_free_node(doc,node);
        continue;
      }
      if (strncmp(node->name, "!--", 3) == 0) {
        /* comment tag */
        qs_free_node(doc, node);
        continue;
      }
#ifdef DEBUG
  QX_LOGGER_DEBUG("call qs_add_child_node()");
#endif
      qs_add_child_node(doc,node);
#ifdef DEBUG
  QX_LOGGER_DEBUG("return from qs_add_child_node()");
#endif
      if (has_child(node->name)) {
#ifdef DEBUG
        {char buf[256]; sprintf(buf, "[%s]->[%s]", doc->now_parent_node->name, node->name); QX_LOGGER_DEBUG(buf);}
#endif
        doc->now_parent_node = node;
      }
    }
    else {
      /* TEXT */
      int endpoint = s_cut_text(&src[ii], srclen - ii);
      Node* node = qs_new_tag(doc);
      node->value = (char*)qs_malloc(doc,endpoint+1, QX_LOGMARK);
      node->name  = (char*)qs_malloc(doc,4+1,        QX_LOGMARK);
      node->otext = (char*)qs_malloc(doc,endpoint+1, QX_LOGMARK);
      node->size  = endpoint;
      memset(node->value, 0, endpoint+1);
      memset(node->otext, 0, endpoint+1);
      memset(node->name,  0, 4+1       );
      memcpy(node->value, &src[ii], endpoint);
      memcpy(node->name,  "text",   4);
      memcpy(node->otext,node->value, endpoint);

      qs_add_child_node(doc,node);
      ii += (endpoint - 1);
    }
  }
#ifdef DEBUG
  QX_LOGGER_DEBUG("parse_string end");
#endif

#ifdef DEBUG
  if (doc->r != NULL)
  {
    qs_dump_node(doc, doc->root_node, 0);
  }
#endif

  return doc->root_node;
}

static void
qs_dump_node(Doc* doc, Node* node, int indent) {
  Node* p = (Node*)qs_get_child_node(doc,node);

  for (;p;p = (Node*)qs_get_next_node(doc,p)) {
    Attr* attr;
    if ((char*)qs_get_node_value(doc,p) != NULL) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*sNode:[%s][%s]\n", indent,indent," ",
                      (char*)qs_get_node_name(doc,p),
                      (char*)qs_get_node_value(doc,p));
    }
    else {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*sNode:[%s]\n", indent,indent," ", qs_get_node_name(doc,p));
    }
    for (attr = (Attr*)qs_get_attr(doc,p); attr; attr = (Attr*)qs_get_next_attr(doc,attr)) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*s  ATTR:[%s]\n", indent,indent," ", (char *)qs_get_attr_name(doc,attr));
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, doc->r,
        "%*.*s  VAL :[%s]\n", indent,indent," ", (char *)qs_get_attr_value(doc,attr));
    }
    qs_dump_node(doc,p, indent+4);
  }
}



static int
s_cut_tag(const char* s, int len) {
  int lv = 0;
  int ii;

  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) {
      continue;
    }

    if (is_white_space(s[ii])) {
      continue;
    }

    if (s[ii] == '<') {
      lv++;
      continue;
    }

    if (s[ii] == '>') {
      lv--;
      if (lv == 0) {
        break;
      }
      continue;
    }
  }
  return ii;
}

static int
s_cut_text(const char* s, int len) {
  int ii;
  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) {
      continue;
    }

    if (is_white_space(s[ii])) {
      continue;
    }

    if (s[ii] == '<') {
      break;
    }
  }

  return ii;
}


QS_EXPORT Node*
qs_init_root_node(Doc* doc) {
  doc->root_node = (Node*)qs_malloc(doc,sizeof(struct _node),QX_LOGMARK);
  if (doc->root_node == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  doc->root_node->next   = NULL;
  doc->root_node->parent = NULL;
  doc->root_node->child  = NULL;
  doc->root_node->attr   = NULL;

  doc->root_node->name   = (char*)qs_malloc(doc,5,QX_LOGMARK);
  if (doc->root_node->name == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  memset(doc->root_node->name, 0, 5);
  strcpy(doc->root_node->name, "ROOT");

  return doc->root_node;
}



QS_EXPORT void
qs_add_child_node(Doc* doc,Node* node) 
{
  node->next       = NULL;
  node->child      = NULL;
  node->child_tail = NULL;
  node->parent = doc->now_parent_node;
  if (doc->now_parent_node->child == NULL) 
  {
    doc->now_parent_node->child      = node;
    doc->now_parent_node->child_tail = node;
  }
  else 
  {
#ifdef DEBUG
    QX_LOGGER_DEBUG("search child free node");
#endif
    doc->now_parent_node->child_tail->next = node;
    doc->now_parent_node->child_tail       = node;
  }
}



QS_EXPORT void
qs_free_node(Doc* doc, Node* node) 
{
  QX_LOGGER_DEBUG("start qs_free_node()");

  qs_free(doc,node->name, QX_LOGMARK);
  Attr* attr;
  Attr* nattr;

  for (attr = node->attr; attr ; attr = nattr) {
    nattr = attr->next;
    qs_free(doc,attr->name, QX_LOGMARK);
    qs_free(doc,attr->value, QX_LOGMARK);
    qs_free(doc,attr, QX_LOGMARK);
  }

  qs_free(doc,node, QX_LOGMARK);
  QX_LOGGER_DEBUG("end qs_free_node()");
}




QS_EXPORT Node*
qs_get_root(Doc* doc) {
  return doc->root_node;
}




QS_EXPORT char* 
qs_get_node_value(Doc* doc, Node* node) {
  return node->value;
}




QS_EXPORT char*
qs_get_node_name(Doc* doc, Node* node) {
  return node->name;
}



QS_EXPORT Node*
qs_get_child_node(Doc* doc, Node* node) {
  return node->child;
}




QS_EXPORT Node*
qs_get_next_node(Doc* doc, Node* node) {
  return node->next;
}



QS_EXPORT Attr*
qs_get_attr(Doc* doc, Node* node) {
  return node->attr;
}




QS_EXPORT Attr*
qs_get_next_attr(Doc* doc, Attr* attr) {
  return attr->next;
}



QS_EXPORT char*
qs_get_attr_name(Doc* doc, Attr* attr) {
  return attr->name;
}



QS_EXPORT char*
qs_get_attr_value(Doc* doc, Attr* attr) {
  return attr->value;
}

QS_EXPORT int 
qs_get_node_size(Doc* doc, Node* node) {
  return node->size;
}
/*
 * vim:ts=2 et
 */
