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
#ifndef __QS_PARSE_STRING_H__
#define __QS_PARSE_STRING_H__

#include <httpd.h>

/*
#define DEBUG
#define USE_LOG
*/

/**
 * Max of memory allocation times.
 */
#define QX_ALLOC_MAX   (100*1024)

/**
 * It is judged whether it is the first byte of Japanese Shift_JIS 
 * "ZENKAKU KANJI". 
 */
#define is_sjis_kanji(c)  ((0x81 <= (unsigned char)(c&0xff) && \
                           (unsigned char)(c&0xff) <= 0x9f)  \
                          || (0xe0 <= (unsigned char)(c&0xff) && \
                           (unsigned char)(c&0xff) <= 0xfc))
/**
 * It is judged whether it is a byte of Japanese Shift_JIS "HANKAKU KANA". 
 */
#define is_sjis_kana(c)   ((0xa1 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xdf))

/**
 * It is judged whether it is a Japanese code part of EUC. 
 */
#define is_euc_kanji(c)   (0xa1 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfe)

/**
 * It is judged whether it is a Japanese code part of EUC "KANA". 
 */
#define is_euc_kana(c)    (0x8e == (unsigned char)(c&0xff))

/**
 * True is returned for whitespace. 
 */
#define is_white_space(c)     (' ' == (unsigned char)(c&0xff)  \
                           || '\t' == (unsigned char)(c&0xff) \
                           || '\n' == (unsigned char)(c&0xff) \
                           || '\r' == (unsigned char)(c&0xff))

/**
 * True is returned for the quotation mark. 
 */
#define is_quote(c)       ('\'' == (unsigned char)(c&0xff) \
                         ||'"'  == (unsigned char)(c&0xff))

/**
 * It is judged whether the tag of the object has the child element. 
 * The "<option>" tag has the child. Please write </ option >. 
 */
#define has_child(c)       ((strcasecmp(c, "base"     ) != 0) \
                        &&  (strcasecmp(c, "meta"     ) != 0) \
                        &&  (strcasecmp(c, "br"       ) != 0) \
                        &&  (strcasecmp(c, "dt"       ) != 0) \
                        &&  (strcasecmp(c, "dd"       ) != 0) \
                        &&  (strcasecmp(c, "hr"       ) != 0) \
                        &&  (strcasecmp(c, "img"      ) != 0) \
                        &&  (strcasecmp(c, "input"    ) != 0) \
                        &&  (strcasecmp(c, "p"        ) != 0) \
                        &&  (strcasecmp(c, "plaintext") != 0) \
                        &&  (strcasecmp(c, "?xml"     ) != 0) \
                        &&  (strcasecmp(c, "!doctype" ) != 0) \
                        &&  (strcasecmp(c, "link"     ) != 0) \
                        &&  (strcasecmp(c, "!--"      ) != 0) \
                        &&  (strncasecmp(c, "![CDATA[",8) != 0))

/**
 * It is judged whether the tag of the object has the child element. 
 * The "<option>" tag has the child. Please write </ option >. 
 */
#define has_child_hdml(c)  ((strcasecmp(c, "center"     ) != 0) \
                        &&  (strcasecmp(c, "br"         ) != 0) \
                        &&  (strcasecmp(c, "action"     ) != 0) \
                        &&  (strcasecmp(c, "!--"      ) != 0))

/**
 * The structure of the attribute is defined.
 */
typedef struct Attr Attr;

struct Attr {
  struct Attr* next;
  struct Node* parent;
  char *name;
  char *value;
};

/**
 * The structure of the element is defined. 
 */
typedef struct Node Node;

struct Node {
  struct Node*   next;
  struct Node*   parent;
  struct Node*   child;
  struct Node*   child_tail;
  struct Attr*   attr;
  struct Attr*   attr_tail;
  char*          name;
  char*          value;
  int            size;
  char*          otext;
};

typedef struct pointer_table_t {
  unsigned int            address;
  unsigned long           size;
  struct pointer_table_t* next;
  struct pointer_table_t* prev;
} Pointer_Table;


typedef enum chxj_parse_mode_t {
  PARSE_MODE_CHTML=0,
  PARSE_MODE_NO_PARSE,
} ParseMode_t;

typedef struct _doc {
  Node*         now_parent_node;
  Node*         root_node;

  int           do_init_flag;
  unsigned long alloc_size;

  Pointer_Table* pointer_table;
  Pointer_Table* free_list_head;
  Pointer_Table* free_list_tail;
  Pointer_Table* allocated_list_head;
  Pointer_Table* allocated_list_tail;

  ParseMode_t    parse_mode;

  apr_allocator_t* allocator;
  apr_pool_t*      pool;

#ifndef __NON_MOD_CHXJ__
  request_rec* r;
#endif
} Doc;

/*
 * Prototype Declare
 */
extern Node* qs_init_root_node(
  Doc* doc);

extern void qs_add_child_node(
  Doc* doc, 
  Node*);

extern void qs_free_node(
  Doc* doc,
  Node*);

extern Node* qs_get_root(
  Doc* doc) ;

extern Node* qs_parse_string(
  Doc*        doc, 
  const char* ss, 
  int         len);

extern char* qs_get_node_value(
  Doc*  doc,
  Node* node);

extern char* qs_get_node_name(
  Doc*  doc, 
  Node* node);

extern int qs_get_node_size(Doc* doc, Node* node) ;
extern Node *qs_get_child_node(Doc *doc, Node *node) ;
extern Node *qs_get_next_node(Doc *doc, Node *node) ;

extern void qs_dump_node_to_file(
  FILE *fp, 
  Doc *doc, 
  Node *node, 
  int indent);

Attr* qs_get_attr(Doc* doc, Node* node) ;
Attr* qs_get_next_attr(Doc* doc, Attr* attr) ;
char* qs_get_attr_name(Doc* doc, Attr* attr) ;
char* qs_get_attr_value(Doc* doc, Attr* attr) ;
#endif
