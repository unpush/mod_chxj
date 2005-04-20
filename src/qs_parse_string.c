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
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

static int s_cut_tag (const char* s, int len) ;
static int s_cut_text(const char* s, int len) ;
static void qs_dump_node(Doc* doc, Node* node, int indent);


Node*
qs_parse_string(Doc* doc, const char* src, int srclen) 
{
  int ii;

  doc->now_parent_node = qs_init_root_node(doc);

  for (ii=0; ii<srclen; ii++) 
  {
    if (doc->parse_mode != PARSE_MODE_NO_PARSE && is_white_space(src[ii])) 
    {
      continue;
    }
    if ((unsigned char)'<' == src[ii]) 
    {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
      node = qs_parse_tag(doc, &src[ii], endpoint);

      ii += endpoint;
      if (node->name[0] == '/' ) 
      {

        if ((doc->parse_mode == PARSE_MODE_CHTML && has_child(&(node->name[1])))
        ||  (doc->parse_mode == PARSE_MODE_NO_PARSE && strcasecmp(&node->name[1], "chxj:if") == 0)) 
        {
          if (doc->now_parent_node->parent != NULL)
          {
            doc->now_parent_node = doc->now_parent_node->parent;
            doc->parse_mode = PARSE_MODE_CHTML;
          }
        }

        if (doc->parse_mode != PARSE_MODE_NO_PARSE)
        {
          qs_free_node(doc,node);
          continue;
        }
      }
      if (strncmp(node->name, "!--", 3) == 0) 
      {
        /* comment tag */
        qs_free_node(doc, node);
        continue;
      }
      qs_add_child_node(doc,node);

      if (doc->parse_mode == PARSE_MODE_NO_PARSE)
      {
        if (node->name[0] == '/')
        {
          continue;
        }
      }
#ifdef DEBUG
  QX_LOGGER_DEBUG("return from qs_add_child_node()");
#endif
      if (doc->parse_mode == PARSE_MODE_CHTML && strcasecmp(node->name, "chxj:if") == 0)
      {
        Attr* parse_attr;

        doc->parse_mode = PARSE_MODE_NO_PARSE;
        doc->now_parent_node = node;
        for(parse_attr = node->attr; parse_attr; parse_attr = parse_attr->next)
        {
          if (strcasecmp(parse_attr->name, "parse") == 0)
          {
            if (strcasecmp(parse_attr->value, "true") == 0)
            {
              doc->parse_mode = PARSE_MODE_CHTML;
            }
          }
        }

      }
      if (doc->parse_mode == PARSE_MODE_CHTML && has_child(node->name)) 
      {
        doc->now_parent_node = node;
      }
    }
    else {
      /* TEXT */
      int endpoint = s_cut_text(&src[ii], srclen - ii);
      Node* node = qs_new_tag(doc);
      node->value = (char*)apr_palloc(doc->pool,endpoint+1);
      node->name  = (char*)apr_palloc(doc->pool,4+1);
      node->otext = (char*)apr_palloc(doc->pool,endpoint+1);
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


Node*
qs_init_root_node(Doc* doc) {
  doc->root_node = (Node*)apr_palloc(doc->pool,sizeof(struct _node));
  if (doc->root_node == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  doc->root_node->next   = NULL;
  doc->root_node->parent = NULL;
  doc->root_node->child  = NULL;
  doc->root_node->attr   = NULL;

  doc->root_node->name   = (char*)apr_palloc(doc->pool,5);
  if (doc->root_node->name == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
  }
  memset(doc->root_node->name, 0, 5);
  strcpy(doc->root_node->name, "ROOT");

  return doc->root_node;
}



void
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



void
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




Node*
qs_get_root(Doc* doc) {
  return doc->root_node;
}




char* 
qs_get_node_value(Doc* doc, Node* node) {
  return node->value;
}




char*
qs_get_node_name(Doc* doc, Node* node) {
  return node->name;
}



Node*
qs_get_child_node(Doc* doc, Node* node) {
  return node->child;
}




Node*
qs_get_next_node(Doc* doc, Node* node) {
  return node->next;
}



Attr*
qs_get_attr(Doc* doc, Node* node) {
  return node->attr;
}




Attr*
qs_get_next_attr(Doc* doc, Attr* attr) {
  return attr->next;
}



char*
qs_get_attr_name(Doc* doc, Attr* attr) {
  return attr->name;
}



char*
qs_get_attr_value(Doc* doc, Attr* attr) {
  return attr->value;
}

int 
qs_get_node_size(Doc* doc, Node* node) {
  return node->size;
}
/*
 * vim:ts=2 et
 */
