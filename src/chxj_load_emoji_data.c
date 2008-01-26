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
#include "mod_chxj.h"
#include "chxj_load_emoji_data.h"
#include "chxj_emoji.h"

static char *s_load_emoji_set_tag(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node);

static char *s_set_emoji_data(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node);

static char *s_load_emoji_emoji_tag(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node);

static void  s_emoji_add_to_tail(
  mod_chxj_config  *conf,
  emoji_t          *emoji);

static char *s_load_emoji_no_tag(
  Doc         *doc,
  apr_pool_t  *p,
  emoji_t     *em,
  Node        *node);

static char *s_load_emoji_imode_tag(
  Doc        *doc,
  apr_pool_t *p,
  emoji_t    *em,
  Node       *node);

static char *s_load_emoji_ezweb_tag(
  Doc        *doc,
  apr_pool_t *p,
  emoji_t    *em,
  Node       *node);

static char *s_load_emoji_softbank_tag(
  Doc        *doc,
  apr_pool_t *p,
  emoji_t    *em,
  Node       *node);

static char  s_hexstring_to_byte(char *s);

static char *
s_load_emoji_ezweb_type_tag(
  Doc          *doc,
  apr_pool_t   *p,
  Node         *node,
  ezweb_type_t *type);


/**
 * The emoji data is loaded on the memory.
 */
char *
chxj_load_emoji_data(
  Doc             *doc,
  apr_pool_t      *p,
  mod_chxj_config *conf) 
{
  char *rtn;

  conf->emoji      = NULL;
  conf->emoji_tail = NULL;
  if ((rtn = s_set_emoji_data(doc, p, conf,qs_get_root(doc))) != NULL) {
    return rtn;
  }
  chxj_emoji_init(conf);
  return NULL;
}


/**
 */
static char *
s_set_emoji_data(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node) 
{
  Node *child;
  char *rtn;

  for (child = qs_get_child_node(doc,node);
       child;
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    if ((*name == 'e' || *name == 'E') 
        && strcasecmp(name, "emoji") == 0) {
      if ((rtn = s_load_emoji_emoji_tag(doc, p, conf, child))) {
        return rtn;
      }
    }
  }
  return NULL;
}


static char *
s_load_emoji_emoji_tag(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node)
{
  Node *child;
  char *rtn;

  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child)) {
    char *name = qs_get_node_name(doc, child);
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "set") == 0) {
      if ((rtn = s_load_emoji_set_tag(doc, p, conf, child)) != NULL) {
        return rtn;
      }
    }
  }

  return NULL;
}


static char *
s_load_emoji_set_tag(
  Doc              *doc,
  apr_pool_t       *p,
  mod_chxj_config  *conf,
  Node             *node)
{
  Node     *child;
  emoji_t  *em;
  char     *rtn;

  em         = apr_palloc(p, sizeof(emoji_t));
  memset(&em->imode, 0, sizeof(imode_emoji_t));
  memset(&em->ezweb, 0, sizeof(ezweb_emoji_t));
  memset(&em->softbank, 0, sizeof(softbank_emoji_t));
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    char *name  = qs_get_node_name(doc, child);
    switch (*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "no") == 0) {
        if ((rtn = s_load_emoji_no_tag(doc, p, em, child)) != NULL) {
          return rtn;
        }
      }
      break;
    
    case 'i':
    case 'I':
      if (strcasecmp(name, "imode") == 0) {
        if ((rtn = s_load_emoji_imode_tag(doc, p, em, child)) != NULL) {
          return rtn;
        }
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "ezweb") == 0) {
        if ((rtn = s_load_emoji_ezweb_tag(doc, p, em, child)) != NULL)
          return rtn;
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "softbank") == 0) {
        if ((rtn = s_load_emoji_softbank_tag(doc, p, em, child)) != NULL)
          return rtn;
      }
      break;
    default:
      break;
    }
  }
  s_emoji_add_to_tail(conf, em);
  return NULL;
}


static char *
s_load_emoji_no_tag(
  Doc         *doc,
  apr_pool_t  *p,
  emoji_t     *em,
  Node        *node)
{
  int len;
  int ii;

  Node *child = qs_get_child_node(doc,node);
  char *name  = qs_get_node_name(doc,  child);
  char *value = qs_get_node_value(doc, child);
  if (STRCASEEQ('t','T',"text",name)) {
    len = strlen(value);
    for (ii=0; ii<len; ii++) {
      if (value[ii] < '0' || value[ii] > '9') {
        return apr_psprintf(p, 
                            "invalid data <no> tag [%s]",
                             value);
      }
    }
    em->no = atoi(value);
  }
  return NULL;
}


static char *
s_load_emoji_imode_tag(
  Doc         *doc,
  apr_pool_t  *p,
  emoji_t     *em,
  Node        *node)
{
  Node *child;
  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child)) {
    char *name  = qs_get_node_name(doc, child);
    switch (*name) {
    case 's':
    case 'S':
      if (strcasecmp(name, "sjis-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            em->imode.sjis.hex_string = apr_pstrdup(p, cvalue);
            em->imode.sjis.hex[0] = s_hexstring_to_byte(cvalue);
            em->imode.sjis.hex[1] = s_hexstring_to_byte(&cvalue[2]);
          }
        }
      }
      else if (strcasecmp(name, "sjis-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            em->imode.sjis.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "eucjp-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[7];
            memset(test, 0, 7);
            strncpy(test, cvalue, 6);
            if (strlen(test) < 6) {
              return apr_pstrdup(p, "invalid imode::eucjp-hex");
            }
            em->imode.euc.hex_string = apr_pstrdup(p, cvalue);
            em->imode.euc.hex[0] = s_hexstring_to_byte(cvalue);
            em->imode.euc.hex[1] = s_hexstring_to_byte(&cvalue[2]);
            em->imode.euc.hex[2] = s_hexstring_to_byte(&cvalue[4]);
          }
        }
      }
      else if (strcasecmp(name, "eucjp-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->imode.euc.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "unicode-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[5];
            memset(test, 0, 5);
            strncpy(test, cvalue, 4);
            if (strlen(test) < 4) {
              return apr_pstrdup(p, "invalid imode::unicode-hex");
            }
            em->imode.unicode.hex_string = apr_pstrdup(p, cvalue);
            em->imode.unicode.hex[0] = s_hexstring_to_byte(cvalue);
            em->imode.unicode.hex[1] = s_hexstring_to_byte(&cvalue[2]);
          }
        }
      }
      else if (strcasecmp(name, "unicode-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->imode.unicode.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      else if (strcasecmp(name, "utf8-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[7];
            memset(test, 0, 7);
            strncpy(test, cvalue, 6);
            if (strlen(test) < 6) {
              return apr_pstrdup(p, "invalid imode::utf8-hex");
            }
            em->imode.utf8.hex_string = apr_pstrdup(p, cvalue);
            em->imode.utf8.hex[0] = s_hexstring_to_byte(cvalue);
            em->imode.utf8.hex[1] = s_hexstring_to_byte(&cvalue[2]);
            em->imode.utf8.hex[2] = s_hexstring_to_byte(&cvalue[4]);
          }
        }
      }
      else if (strcasecmp(name, "utf8-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->imode.utf8.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;
    
    case 'd':
    case 'D':
      if (strcasecmp(name, "description") == 0) {
        Node* description_node = qs_get_child_node(doc, child);
        if (description_node) {
          char* cname = qs_get_node_name(doc, description_node);
          char* cvalue = qs_get_node_value(doc, description_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->imode.description = apr_pstrdup(p, cvalue);
          }
        }
        else {
          em->imode.description    = apr_palloc(p, 1);
          em->imode.description[0] = 0;
        }
      }
      break;

    default:
      break;
    }
  }
  return NULL;
}


static char *
s_load_emoji_ezweb_tag(
  Doc         *doc,
  apr_pool_t  *p,
  emoji_t     *em,
  Node        *node)
{
  Node *child;
  char *rtn;
  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child)) {
    char *name  = qs_get_node_name(doc, child);
    if (STRCASEEQ('a','A',"a", name)) {
      if ((rtn = s_load_emoji_ezweb_type_tag(doc, p, child, &em->ezweb.typeA)) != NULL) {
        return rtn;
      }
    }
    else if (STRCASEEQ('b','B',"b",name)) {
      if ((rtn = s_load_emoji_ezweb_type_tag(doc, p, child, &em->ezweb.typeB)) != NULL) {
        return rtn;
      }
    }
    else if (STRCASEEQ('c','C',"c",name)) {
      if ((rtn = s_load_emoji_ezweb_type_tag(doc, p, child, &em->ezweb.typeC)) != NULL) {
        return rtn;
      }
    }
    else if (STRCASEEQ('d','D',"d",name)) {
      if ((rtn = s_load_emoji_ezweb_type_tag(doc, p, child, &em->ezweb.typeD)) != NULL) {
        return rtn;
      }
    }
  }

  return NULL;
}


static char *
s_load_emoji_ezweb_type_tag(
  Doc          *doc,
  apr_pool_t   *p,
  Node         *node,
  ezweb_type_t *type)
{
  Node *child;
  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child)) {
    char *name  = qs_get_node_name(doc, child);
    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "no") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            int len = strlen(cvalue);
            int ii;
            for (ii=0; ii<len; ii++) {
              if (cvalue[ii] < '0' || cvalue[ii] > '9') {
                return apr_psprintf(p, 
                                    "invalid ezweb::no [%s]",
                                     cvalue);
              }
            }
          }
          type->no = atoi(cvalue);
        }
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "sjis-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            char test[5];
            memset(test, 0, 5);
            strncpy(test, cvalue, 4);
            if (strlen(test) < 4) {
              return apr_psprintf(p, 
                                  "invalid ezweb::sjis-hex [%s]",
                                  test);
            }
            type->sjis.hex_string = apr_pstrdup(p, cvalue);
            type->sjis.hex[0] = s_hexstring_to_byte(cvalue);
            type->sjis.hex[1] = s_hexstring_to_byte(&cvalue[2]);
          }
        }
      }
      else if (strcasecmp(name, "sjis-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            type->sjis.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "unicode-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[5];
            memset(test, 0, 5);
            strncpy(test, cvalue, 4);
            if (strlen(test) < 4) {
              return apr_psprintf(p, 
                                  "invalid ezweb::unicode-hex [%s]",
                                  test);
            }
            type->unicode.hex_string = apr_pstrdup(p, cvalue);
            type->unicode.hex[0] = s_hexstring_to_byte(cvalue);
            type->unicode.hex[1] = s_hexstring_to_byte(&cvalue[2]);
          }
        }
      }
      else if (strcasecmp(name, "unicode-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            type->unicode.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      else if (strcasecmp(name, "utf8-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[7];
            memset(test, 0, 7);
            strncpy(test, cvalue, 6);
            if (strlen(test) < 6) {
              return apr_psprintf(p, 
                                  "invalid ezweb::utf8-hex [%s]",
                                  test);
            }
            type->utf8.hex_string = apr_pstrdup(p, cvalue);
            type->utf8.hex[0] = s_hexstring_to_byte(cvalue);
            type->utf8.hex[1] = s_hexstring_to_byte(&cvalue[2]);
            type->utf8.hex[2] = s_hexstring_to_byte(&cvalue[4]);
          }
        }
      }
      else if (strcasecmp(name, "utf8-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            type->utf8.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    default:
      break;
    }
  }
  return NULL;
}


static char *
s_load_emoji_softbank_tag(
  Doc         *doc,
  apr_pool_t  *p,
  emoji_t     *em,
  Node        *node)
{
  Node *child;
  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child)) {
    char *name  = qs_get_node_name(doc, child);
    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "no") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            int len = strlen(cvalue);
            int ii;
            for (ii=0; ii<len; ii++) {
              if (cvalue[ii] < '0' || cvalue[ii] > '9') {
                return apr_psprintf(p, 
                                    "invalid softbank::no [%s]",
                                     cvalue);
              }
            }
          }
          em->softbank.no = atoi(cvalue);
        }
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "sjis-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            char test[11];
            memset(test, 0, 11);
            strncpy(test, cvalue, 10);
            if (strlen(test) < 10) {
              return apr_psprintf(p,
                                 "invalid softbank::sjis-hex [%s]",
                                 test);
            }
            em->softbank.sjis.hex_string = apr_pstrdup(p, cvalue);
            em->softbank.sjis.hex[0] = s_hexstring_to_byte(cvalue);
            em->softbank.sjis.hex[1] = s_hexstring_to_byte(&cvalue[2]);
            em->softbank.sjis.hex[2] = s_hexstring_to_byte(&cvalue[4]);
            em->softbank.sjis.hex[3] = s_hexstring_to_byte(&cvalue[6]);
            em->softbank.sjis.hex[4] = s_hexstring_to_byte(&cvalue[8]);
          }
        }
      }
      else if (strcasecmp(name, "sjis-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text",cname)) {
            em->softbank.sjis.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "unicode-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[5];
            memset(test, 0, 5);
            strncpy(test, cvalue, 4);
            if (strlen(test) < 4) {
              return apr_psprintf(p, 
                                 "invalid softbank::unicode-hex [%s]",
                                 test);
            }
            em->softbank.unicode.hex_string = apr_pstrdup(p, cvalue);
            em->softbank.unicode.hex[0] = s_hexstring_to_byte(cvalue);
            em->softbank.unicode.hex[1] = s_hexstring_to_byte(&cvalue[2]);
          }
        }
      }
      else if (strcasecmp(name, "unicode-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->softbank.unicode.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      else if (strcasecmp(name, "utf8-hex") == 0) {
        Node *hex_node = qs_get_child_node(doc, child);
        if (hex_node) {
          char *cname = qs_get_node_name(doc, hex_node);
          char *cvalue = qs_get_node_value(doc, hex_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            char test[7];
            memset(test, 0, 7);
            strncpy(test, cvalue, 6);
            if (strlen(test) < 6) {
              return apr_psprintf(p, 
                                 "invalid softbank::utf8-hex [%s]",
                                 test);
            }
            em->softbank.utf8.hex_string = apr_pstrdup(p, cvalue);
            em->softbank.utf8.hex[0] = s_hexstring_to_byte(cvalue);
            em->softbank.utf8.hex[1] = s_hexstring_to_byte(&cvalue[2]);
            em->softbank.utf8.hex[2] = s_hexstring_to_byte(&cvalue[4]);
          }
        }
      }
      else if (strcasecmp(name, "utf8-dec") == 0) {
        Node *dec_node = qs_get_child_node(doc, child);
        if (dec_node) {
          char *cname = qs_get_node_name(doc, dec_node);
          char *cvalue = qs_get_node_value(doc, dec_node);
          if (STRCASEEQ('t','T',"text", cname)) {
            em->softbank.utf8.dec_string = apr_pstrdup(p, cvalue);
          }
        }
      }
      break;

    default:
      break;
    }
  }
  return NULL;
}


static void
s_emoji_add_to_tail(
  mod_chxj_config *conf,
  emoji_t         *emoji)
{
  emoji->next = NULL;
  if (conf->emoji == NULL) {
    conf->emoji      = emoji;
    conf->emoji_tail = emoji;
    return;
  }
  conf->emoji_tail->next = emoji;
  conf->emoji_tail       = emoji;
}


static char 
s_hexstring_to_byte(char *s)
{
  int  len;
  int  ii;
  char one_byte = 0;

  len = strlen(s);

  for (ii=0; ii<len && ii<2; ii++) {
    switch(s[ii]) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
      one_byte |= (0xFF & (((s[ii] - '0') & 0xFF) << ((1-ii)*4)));
      break;
    case 'A':
    case 'a':
      one_byte |= (0xFF & ((0x0A & 0xFF) << ((1-ii)*4)));
      break;
    case 'B':
    case 'b':
      one_byte |= (0xFF & ((0x0B & 0xFF) << ((1-ii)*4)));
      break;
    case 'C':
    case 'c':
      one_byte |= (0xFF & ((0x0C & 0xFF) << ((1-ii)*4)));
      break;
    case 'D':
    case 'd':
      one_byte |= (0xFF & ((0x0d & 0xFF) << ((1-ii)*4)));
      break;
    case 'E':
    case 'e':
      one_byte |= (0xFF & ((0x0e & 0xFF) << ((1-ii)*4)));
      break;
    case 'F':
    case 'f':
      one_byte |= (0xFF & ((0x0f & 0xFF) << ((1-ii)*4)));
      break;
    default:
      break;
    }
  }

  return one_byte;
}
/*
 * vim:ts=2 et
 */
