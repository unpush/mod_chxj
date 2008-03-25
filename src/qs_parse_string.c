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
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "httpd.h"
#include "http_log.h"
#include "qs_parse_string.h"
#include "qs_parse_tag.h"
#include "qs_log.h"

#include "mod_chxj.h"
#include <iconv.h>


#define NL_COUNT_MAX  (10)

typedef struct node_stack_element {
  Node *node;
  struct node_stack_element *next;
  struct node_stack_element **ref;
} *NodeStackElement;

typedef struct node_stack {
  NodeStackElement head;
  NodeStackElement tail;
} *NodeStack;

static int s_cut_tag (const char* s, int len);
static int s_cut_text(const char* s, int len, int script);
static void qs_dump_node(Doc* doc, Node* node, int indent);
static void qs_push_node(Doc* doc, Node *node, NodeStack stack);
static Node *qs_pop_node(Doc* doc, NodeStack stack);
#ifdef DUMP_NODE_STACK
static void qs_dump_node_stack(Doc *doc, NodeStack stack);
#endif
static void qs_free_node_stack(Doc *doc, NodeStack stack);
static void s_error_check(Doc *doc, Node *node, NodeStack node_stack, NodeStack err_stack);


Node*
qs_parse_string(Doc* doc, const char* src, int srclen) 
{
  int     ii;
  int     nl_cnt = 0;
  char    encoding[256];
  char*   osrc;
  char*   ibuf;
  size_t  olen;
  size_t  ilen;
  int     script_flag = 0;
  iconv_t cd;

  osrc = NULL;
  ibuf = NULL;
  NodeStack node_stack;
  NodeStack err_stack;

  memset(encoding, 0, 256);

  doc->now_parent_node = qs_init_root_node(doc);
  if (doc->r != NULL) {
    node_stack = apr_palloc(doc->r->pool, sizeof(struct node_stack));
    memset(node_stack, 0, sizeof(struct node_stack));
    err_stack = apr_palloc(doc->r->pool, sizeof(struct node_stack));
    memset(err_stack, 0, sizeof(struct node_stack));
  }
  else {
    node_stack = calloc(sizeof(struct node_stack), 1);
    err_stack  = calloc(sizeof(struct node_stack), 1);
  }

  /* 
   * It is the pre reading. 
   * Because I want to specify encoding.
   */
  for (ii=0; ii<srclen; ii++) {
    if (src[ii] == '\n')  nl_cnt++;
    if (nl_cnt >= NL_COUNT_MAX) break; /* not found <?xml ...> */

    if (is_white_space(src[ii]))
      continue;

    if ((unsigned char)'<' == src[ii]) {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
      node = qs_parse_tag(doc, &src[ii], endpoint);
      ii += endpoint;

      if (node->name[0] != '?') break; 

      if (strcasecmp(node->name, "?xml") == 0) {
        Attr* parse_attr;
        for(parse_attr = node->attr;
            parse_attr && *encoding == '\0'; 
            parse_attr = parse_attr->next) {
          if ((*parse_attr->name == 'e' || *parse_attr->name == 'E')
          &&   strcasecmp(parse_attr->name, "encoding") == 0) {
            switch (*parse_attr->value) {
            case 'X':
            case 'x':
              if (strcasecmp(parse_attr->value, "x-sjis"   ) == 0) {
                strcpy((char*)encoding, (char*)"NONE");
              }
              break;

            case 'S':
            case 's':
              if ((strcasecmp(parse_attr->value, "Shift_JIS") == 0)
              ||  (strcasecmp(parse_attr->value, "SJIS"     ) == 0)
              ||  (strcasecmp(parse_attr->value, "Shift-JIS") == 0)) {
                strcpy((char*)encoding, (char*)"NONE");
              }
              break;

            case 'e':
            case 'E':
              if ((strcasecmp(parse_attr->value, "EUC_JP") == 0)
              ||  (strcasecmp(parse_attr->value, "EUC-JP") == 0)
              ||  (strcasecmp(parse_attr->value, "EUCJP" ) == 0)) {
                strcpy((char*)encoding, "EUC-JP");
              }
              break;

            case 'u':
            case 'U':
              if ((strcasecmp(parse_attr->value, "UTF-8") == 0)
              ||  (strcasecmp(parse_attr->value, "UTF8") == 0)) {
                strcpy((char*)encoding, "UTF-8");
              }
              break;

            default:
              strcpy((char*)encoding, "NONE");
              break;
            }
          }
        }
        break;
      }
      break;
    }
  }

  if (strcasecmp(encoding, "NONE") != 0 && strlen(encoding) != 0) {
    char* sv_osrc;
    olen = srclen * 4 + 1;
    sv_osrc = osrc =(char*)apr_palloc(doc->pool, olen);
    memset((char*)osrc, 0, olen);
    if ((cd = iconv_open("CP932", encoding)) != (iconv_t) -1) {
      ilen = srclen;
      ibuf = apr_palloc(doc->pool, ilen+1);
      memset(ibuf, 0, ilen+1);
      memcpy(ibuf, src, ilen);
      while (ilen > 0) {
        size_t result = iconv(cd, &ibuf, &ilen, &osrc, &olen);
        if (result == (size_t)(-1)) {
          break;
        }
      }
      srclen = olen;
      src = sv_osrc;
      iconv_close(cd);
    }
  }

  /*
   * Now, true parsing is done here. 
   */
  nl_cnt = 1;
  for (ii=0; ii<srclen; ii++) {
    if (src[ii] == '\n') nl_cnt++;
    if (doc->parse_mode != PARSE_MODE_NO_PARSE 
        && is_white_space(src[ii])) {

      continue;
    }
    if ((unsigned char)'<' == src[ii]) {
      int endpoint = s_cut_tag(&src[ii], srclen - ii);
      Node* node   = NULL;
      node = qs_parse_tag(doc, &src[ii], endpoint);
      node->line = nl_cnt;

      ii += endpoint;
      if (node->name[0] == '/' ) {
        if (doc->parse_mode == PARSE_MODE_CHTML) {
          if (has_child(&(node->name[1]))) {
            if (doc->now_parent_node->parent != NULL) {
              doc->now_parent_node = doc->now_parent_node->parent;
              doc->parse_mode = PARSE_MODE_CHTML;
            }
            if (STRCASEEQ('s','S',"script",&node->name[1])) {
              script_flag = 0;
            }
            s_error_check(doc, node, node_stack, err_stack);
          }
          else {
            continue;
          }
        }
        else
        if (doc->parse_mode == PARSE_MODE_NO_PARSE) {
          if ((node->name[1] == 'c' || node->name[1] == 'C')
              &&  strcasecmp(&node->name[1], "chxj:if") == 0) {
            if (doc->now_parent_node->parent != NULL) {
              doc->now_parent_node = doc->now_parent_node->parent;
              doc->parse_mode = PARSE_MODE_CHTML;
            }
          }
        }

        if (doc->parse_mode != PARSE_MODE_NO_PARSE)
          continue;

      }
      if (*node->name == '!' && strncmp(node->name, "!--", 3) == 0) {
        /* comment tag */
        continue;
      }
      qs_add_child_node(doc,node);
      if (has_child(node->name)) {
        qs_push_node(doc, node, node_stack);
      }

      if (doc->parse_mode == PARSE_MODE_NO_PARSE) {
        if (node->name[0] == '/')
          continue;
      }

      if (doc->parse_mode == PARSE_MODE_CHTML 
          && (*node->name == 'c' || *node->name == 'C') 
          &&  strcasecmp(node->name, "chxj:if") == 0) {
        Attr* parse_attr;

        doc->parse_mode = PARSE_MODE_NO_PARSE;
        doc->now_parent_node = node;
        for(parse_attr = node->attr;
            parse_attr; 
            parse_attr = parse_attr->next) {
          if ((*parse_attr->name == 'p' || *parse_attr->name == 'P') 
          &&   strcasecmp(parse_attr->name, "parse") == 0) {
            if ((*parse_attr->value == 't' || *parse_attr->value == 'T')
            &&   strcasecmp(parse_attr->value, "true") == 0) {
              doc->parse_mode = PARSE_MODE_CHTML;
            }
          }
        }

      }
      if (doc->parse_mode == PARSE_MODE_CHTML && has_child(node->name)) {
        doc->now_parent_node = node;
      }
      if (STRCASEEQ('s','S',"script", node->name)) {
        script_flag = 1;
      }
    }
    else {
      /* TEXT */
      int endpoint = s_cut_text(&src[ii], srclen - ii, script_flag);
      Node* node = qs_new_tag(doc);
      node->value = (char*)apr_palloc(doc->pool,endpoint+1);
      node->name  = (char*)apr_palloc(doc->pool,4+1);
      node->otext = (char*)apr_palloc(doc->pool,endpoint+1);
      node->size  = endpoint;
      node->line  = nl_cnt;
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
  if (doc->r != NULL) {
    qs_dump_node(doc, doc->root_node, 0);
  }
#endif
#ifdef DUMP_NODE_STACK
  qs_dump_node_stack(doc, node_stack);
#endif
  {
    Node* prevNode;
    for (prevNode = qs_pop_node(doc,node_stack);
         prevNode;
         prevNode = qs_pop_node(doc, node_stack)) {
      if (has_child(prevNode->name)) {
        if (doc->r)
          ERR(doc->r, "tag parse error (perhaps, not close). tag_name:[%s] line:[%d]", prevNode->name, prevNode->line);
        else
          fprintf(stderr, "error :tag parse error (perhaps, not close). tag_name:[%s] line:[%d]\n", prevNode->name, prevNode->line);
      }
    }
  }
  qs_free_node_stack(doc, node_stack); node_stack = NULL;
  qs_free_node_stack(doc, err_stack);  err_stack = NULL;
  return doc->root_node;
}


static void
s_error_check(Doc *doc, Node *node, NodeStack node_stack, NodeStack err_stack) 
{
  Node *prevNode;
  int err = 0;
  for (prevNode = qs_pop_node(doc,node_stack);
       prevNode;
       prevNode = qs_pop_node(doc, node_stack)) {
    if (prevNode && strcasecmp(prevNode->name, &node->name[1]) != 0) {
      qs_push_node(doc, prevNode, err_stack);
      err++;
      continue;
    }
    break;
  }
  if (err) {
    Node *tmpNode = qs_pop_node(doc,node_stack);
    if (tmpNode == NULL && err != 1) {
      if (doc->r) 
        ERR(doc->r, "tag parse error (perhaps, miss spell). tag_name:[%s] line:[%d]", &node->name[1], node->line);
      else
        fprintf(stderr, "error :tag parse error (perhaps, miss spell). tag_name:[%s] line:[%d]\n", &node->name[1], node->line);
      for (prevNode = qs_pop_node(doc,err_stack);
           prevNode;
           prevNode = qs_pop_node(doc, err_stack)) {
        qs_push_node(doc, prevNode, node_stack);
      }
    }
    else {
      for (prevNode = qs_pop_node(doc,err_stack);
           prevNode;
           prevNode = qs_pop_node(doc, err_stack)) {
        if (doc->r)
          ERR(doc->r, "tag parse error (perhaps, not close). tag_name:[%s] line:[%d]", prevNode->name, prevNode->line);
        else
          fprintf(stderr, "error :tag parse error (perhaps, not close). tag_name:[%s] line:[%d]\n", prevNode->name, prevNode->line);
      }
      qs_push_node(doc, tmpNode, node_stack);
    }
    err = 0;
  }
}


static void
qs_dump_node(Doc* doc, Node* node, int indent) 
{
  Node* p = (Node*)qs_get_child_node(doc,node);

  for (;p;p = (Node*)qs_get_next_node(doc,p)) {
    Attr* attr;
    if ((char*)qs_get_node_value(doc,p) != NULL) {
      DBG(doc->r,"%*.*sNode:[%s][%s]\n", indent,indent," ",
                      (char*)qs_get_node_name(doc,p),
                      (char*)qs_get_node_value(doc,p));
    }
    else {
      DBG(doc->r,"%*.*sNode:[%s]\n", indent,indent," ", qs_get_node_name(doc,p));
    }
    for (attr = (Attr*)qs_get_attr(doc,p); attr; attr = (Attr*)qs_get_next_attr(doc,attr)) {
      DBG(doc->r,"%*.*s  ATTR:[%s]\n", indent,indent," ", (char *)qs_get_attr_name(doc,attr));
      DBG(doc->r,"%*.*s  VAL :[%s]\n", indent,indent," ", (char *)qs_get_attr_value(doc,attr));
    }
    qs_dump_node(doc,p, indent+4);
  }
}



static int
s_cut_tag(const char* s, int len) 
{
  int lv = 0;
  int ii;
  int comment = 0;

  if (strncmp("<!--", s, 4) == 0) {
    comment = 1;
  }

  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) 
      continue;

    if (is_white_space(s[ii])) 
      continue;

    if (s[ii] == '<') {
      lv++;
      continue;
    }
    if (comment && s[ii] == '-') {
      if (strncmp(&s[ii], "-->", 3) == 0) {
        ii += 2;
        break;
      }
    }

    if (!comment && s[ii] == '>') {
      lv--;
      if (lv == 0) 
        break;
      continue;
    }
  }
  return ii;
}

static int
s_cut_text(const char* s, int len, int script)
{
  register int ii;
  register int sq = 0;
  register int dq = 0;

  for (ii=0;ii<len; ii++) {
    if (is_sjis_kanji(s[ii])) {
      ii++;
      continue;
    }
    if (is_sjis_kana(s[ii])) 
      continue;

    if (is_white_space(s[ii])) 
      continue;

    if (script) {
      if (s[ii] == '\\') {
        ii++;
        continue;
      }
      /* single quote */
      if (s[ii] == '\'' && !dq) {
        if (sq) sq--;
        else    sq++;
      }
      /* double quote */
      if (s[ii] == '"' && !sq) {
        if (dq) dq--;
        else    dq++;
      }
    }

    if (!sq && !dq && s[ii] == '<') 
      break;

  }

  return ii;
}


Node*
qs_init_root_node(Doc* doc) 
{
  doc->root_node = (Node*)apr_palloc(doc->pool,sizeof(struct Node));
  if (doc->root_node == NULL) 
    QX_LOGGER_FATAL("Out Of Memory");

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
qs_add_child_node(Doc *doc,Node *node) 
{
  node->next       = NULL;
  node->child      = NULL;
  node->child_tail = NULL;
  node->parent = doc->now_parent_node;
  if (doc->now_parent_node->child == NULL) {
    doc->now_parent_node->child      = node;
    doc->now_parent_node->child_tail = node;
  }
  else {
#ifdef DEBUG
    QX_LOGGER_DEBUG("search child free node");
#endif
    doc->now_parent_node->child_tail->next = node;
    doc->now_parent_node->child_tail       = node;
  }
}




Node *
qs_get_root(Doc *doc) {
  return doc->root_node;
}




char * 
qs_get_node_value(Doc *UNUSED(doc), Node *node) {
  return node->value;
}




char *
qs_get_node_name(Doc *UNUSED(doc), Node *node) {
  return node->name;
}



Node *
qs_get_child_node(Doc *UNUSED(doc), Node *node) {
  return node->child;
}




Node *
qs_get_next_node(Doc* UNUSED(doc), Node *node) {
  return node->next;
}



Attr *
qs_get_attr(Doc *UNUSED(doc), Node *node) {
  return node->attr;
}




Attr *
qs_get_next_attr(Doc *UNUSED(doc), Attr *attr) {
  return attr->next;
}



char *
qs_get_attr_name(Doc *UNUSED(doc), Attr *attr) {
  return attr->name;
}



char *
qs_get_attr_value(Doc *UNUSED(doc), Attr *attr) {
  return attr->value;
}

int 
qs_get_node_size(Doc *UNUSED(doc), Node *node) {
  return node->size;
}


#define list_insert(node, point) do {           \
    node->ref = point->ref;                     \
    *node->ref = node;                          \
    node->next = point;                         \
    point->ref = &node->next;                   \
} while (0)

#define list_remove(node) do {                  \
    *node->ref = node->next;                    \
    node->next->ref = node->ref;                \
} while (0)


static void 
qs_push_node(Doc* doc, Node *node, NodeStack stack)
{
  NodeStackElement elem;
  if (doc->r != NULL) {
    elem = apr_palloc(doc->r->pool, sizeof(struct node_stack_element));
    memset(elem, 0, sizeof(struct node_stack_element));
  }
  else {
    elem = malloc(sizeof(struct node_stack_element));
    memset(elem, 0, sizeof(struct node_stack_element));
  }
  elem->node = node;
  if (stack->head == NULL) {
    /* add dummy head */
    if (doc->r != NULL) {
      stack->head = apr_palloc(doc->r->pool, sizeof(struct node_stack_element));
      memset(stack->head, 0, sizeof(struct node_stack_element));
    }
    else {
      stack->head = malloc(sizeof(struct node_stack_element));
      memset(stack->head, 0, sizeof(struct node_stack_element));
    }
    stack->head->next = stack->head;
    stack->head->ref = &stack->head->next;
  }
  list_insert(elem, stack->head);
  stack->tail = elem;
}

#include "apr_ring.h"

static Node *
qs_pop_node(Doc *doc, NodeStack stack)
{
  NodeStackElement tail = stack->tail;
  Node *result;

  if (tail == NULL) return NULL;
  if (tail == stack->head) return NULL;
  result = tail->node;


  list_remove(tail);
  stack->tail = (NodeStackElement)((apr_size_t)stack->head->ref - (apr_size_t)APR_OFFSETOF(struct node_stack_element, next));
  if (doc->r == NULL)
    free(tail);

  return result;
}

#ifdef DUMP_NODE_STACK
static void
qs_dump_node_stack(Doc *doc, NodeStack stack)
{
  NodeStackElement elm;
  for (elm = stack->head->next;elm != stack->head; elm = elm->next) {
    if (doc->r) DBG(doc->r, "name:[%s]", elm->node->name);
     else       fprintf(stderr, "[%x] name:[%s] next:[%x]\n", (apr_size_t)elm, elm->node->name, (apr_size_t)elm->next);
  }
}
#endif

static void
qs_free_node_stack(Doc *doc, NodeStack stack)
{
  if (doc->r == NULL && stack != NULL) {
    Node* elm;
    for (elm = qs_pop_node(doc, stack);elm; elm = qs_pop_node(doc,stack))
      ;
    if (stack->head) 
      free(stack->head);
    free(stack);
  }
}
/*
 * vim:ts=2 et
 */
