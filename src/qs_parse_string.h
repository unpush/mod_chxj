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
#ifndef __QS_PARSE_STRING_H__
#define __QS_PARSE_STRING_H__
#include <httpd.h>
/**
 * Max of memory allocation times.
 */
#define QX_ALLOC_MAX   (4096)

/**
 * It is judged whether it is the first byte of Japanese Shift_JIS "ZENKAKU KANJI". 
 */
#define is_sjis_kanji(c)  ((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f)  \
                          || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))
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
                        &&  (strcasecmp(c, "li"       ) != 0) \
                        &&  (strcasecmp(c, "p"        ) != 0) \
                        &&  (strcasecmp(c, "plaintext") != 0) \
                        &&  (strcasecmp(c, "?xml"     ) != 0) \
                        &&  (strcasecmp(c, "!--"      ) != 0))

/**
 * The structure of the attribute is defined.
 */
typedef struct _attr {
  struct _attr* next;
  struct _node* parent;
  char *name;
  char *value;
} Attr;

/**
 * The structure of the element is defined. 
 */
typedef struct _node {
  struct _node* next;
  struct _node* parent;
  struct _node* child;
  struct _node* child_tail;
  struct _attr* attr;
  struct _attr* attr_tail;
  char* name;
  char* value;
  int   size;
  char* otext;
} Node;

typedef struct pointer_table_t {
  unsigned int address;
  unsigned int size;
} Pointer_Table;



typedef struct _doc {
  Node*         now_parent_node;
  Node*         root_node;

  int           do_init_flag;
  unsigned int  alloc_size;

  Pointer_Table pointer_table[QX_ALLOC_MAX];

#ifndef __NON_MOD_CHXJ__
  request_rec* r;
#endif
} Doc;

#ifdef __NON_MOD_CHXJ__
#define QS_EXPORT 
#else
/*
#define QS_EXPORT static
*/
#define QS_EXPORT
#endif

/*
 * Prototype Declare
 */
QS_EXPORT Node* qs_init_root_node(Doc* doc);
QS_EXPORT void qs_add_child_node(Doc* doc, Node*);
QS_EXPORT void qs_free_node(Doc* doc, Node*);
QS_EXPORT Node* qs_get_root(Doc* doc) ;
QS_EXPORT Node* qs_parse_string(Doc* doc, const char*);
QS_EXPORT char* qs_get_node_value(Doc* doc,Node* node);
QS_EXPORT char* qs_get_node_name(Doc* doc, Node* node) ;
QS_EXPORT int qs_get_node_size(Doc* doc, Node* node) ;
QS_EXPORT Node* qs_get_child_node(Doc* doc, Node* node) ;
QS_EXPORT Node* qs_get_next_node(Doc* doc, Node* node) ;
QS_EXPORT Attr* qs_get_attr(Doc* doc, Node* node) ;
QS_EXPORT Attr* qs_get_next_attr(Doc* doc, Attr* attr) ;
QS_EXPORT char* qs_get_attr_name(Doc* doc, Attr* attr) ;
QS_EXPORT char* qs_get_attr_value(Doc* doc, Attr* attr) ;
#endif
