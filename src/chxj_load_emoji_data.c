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
#include "mod_chxj.h"
#include "chxj_load_emoji_data.h"

static char* load_emoji_set_tag( Doc* doc, apr_pool_t* p, mod_chxj_config* conf, Node* node);
static char* set_emoji_data( Doc* doc, apr_pool_t* p, mod_chxj_config* conf, Node* node);
static char* load_emoji_emoji_tag( Doc* doc, apr_pool_t* p, mod_chxj_config* conf, Node* node);
static void emoji_add_to_tail( mod_chxj_config* conf, emoji_t* emoji);
static char* load_emoji_no_tag( Doc* doc, apr_pool_t* p, emoji_t* em, Node* node);
static char* load_emoji_imode_tag( Doc* doc, apr_pool_t* p, emoji_t* em, Node* node);
static char* load_emoji_ezweb_tag( Doc* doc, apr_pool_t* p, emoji_t* em, Node* node);
static char* load_emoji_jphone_tag( Doc* doc, apr_pool_t* p, emoji_t* em, Node* node);
static char hexstring_to_byte(char* s);


/**
 * load emoji.xml
 */
char* 
chxj_load_emoji_data(
  Doc* doc,
  apr_pool_t *p,
  mod_chxj_config* conf) 
{
  char* rtn;

  conf->emoji = NULL;
  conf->emoji_tail = NULL;
  rtn = set_emoji_data(doc, p, conf,qs_get_root(doc));
  if (rtn != NULL)
  {
    return rtn;
  }

  return NULL;
}

/**
 * <emoji>
 */
static char*
set_emoji_data(
  Doc* doc,
  apr_pool_t* p,
  mod_chxj_config* conf,
  Node* node) 
{
  Node* child;
  char* rtn;

  for (child = qs_get_child_node(doc,node);
       child;
       child = qs_get_next_node(doc,child)) 
  {
    char* name = qs_get_node_name(doc,child);
    if (strcasecmp(name, "emoji") == 0) 
    {
      rtn = load_emoji_emoji_tag(doc, p, conf, child);
      if (rtn != NULL)
      {
        return rtn;
      }
    }
  }
  return NULL;
}

static char* 
load_emoji_emoji_tag(
  Doc* doc,
  apr_pool_t* p,
  mod_chxj_config* conf,
  Node* node)
{
  Node* child;
  char* rtn;
  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child))
  {
    char* name = qs_get_node_name(doc, child);
    if (strcasecmp(name, "set") == 0)
    {
      rtn = load_emoji_set_tag(doc, p, conf, child);
      if (rtn != NULL)
      {
        return rtn;
      }
    }
  }
  return NULL;
}


static char*
load_emoji_set_tag(
  Doc* doc,
  apr_pool_t* p,
  mod_chxj_config* conf,
  Node* node)
{
  Node* child;
  emoji_t* em;
  char* rtn;

  em = apr_palloc(p, sizeof(emoji_t));
  em->imode = apr_palloc(p, sizeof(imode_emoji));
  em->ezweb = apr_palloc(p, sizeof(ezweb_emoji));
  em->jphone = apr_palloc(p, sizeof(jphone_emoji));
  
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    char* name  = qs_get_node_name(doc, child);

    if (strcasecmp(name, "no") == 0) 
    {
      rtn = load_emoji_no_tag(doc, p, em, child);
      if (rtn != NULL)
      {
        return rtn;
      }

    }
    else 
    if (strcasecmp(name, "imode") == 0)
    {
      rtn = load_emoji_imode_tag(doc, p, em, child);
      if (rtn != NULL)
      {
        return rtn;
      }
    }
    else
    if (strcasecmp(name, "ezweb") == 0)
    {
      rtn = load_emoji_ezweb_tag(doc, p, em, child);
      if (rtn != NULL)
      {
        return rtn;
      }
    }
    else
    if (strcasecmp(name, "jphone") == 0)
    {
      rtn = load_emoji_jphone_tag(doc, p, em, child);
      if (rtn != NULL)
      {
        return rtn;
      }
    }
  }

  emoji_add_to_tail(conf, em);
  return NULL;
}


static char*
load_emoji_no_tag(
  Doc* doc,
  apr_pool_t* p,
  emoji_t* em,
  Node* node)
{
  int len;
  int ii;
  Node* child = qs_get_child_node(doc,node);
  char* name = qs_get_node_name(doc,  child);
  char* value = qs_get_node_value(doc, child);

  if (strcasecmp(name, "text") == 0)
  {
    len = strlen(value);

    for (ii=0; ii<len; ii++)
    {
      if (value[ii] < '0' || value[ii] > '9')
      {
        return apr_psprintf(p, "invalid data <no> tag [%s]", value);
      }
    }
    em->no = atoi(value);
  }
  return NULL;
}

static char*
load_emoji_imode_tag(
  Doc* doc,
  apr_pool_t* p,
  emoji_t* em,
  Node* node)
{
  Node* child;


  em->imode->hex1byte = 0;
  em->imode->hex2byte = 0;
  em->imode->string = NULL;
  em->imode->description = NULL;

  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child))
  {
    char* name  = qs_get_node_name(doc, child);
    if (strcasecmp(name, "hex1") == 0) 
    {
      Node* hex1node = qs_get_child_node(doc, child);
      if (hex1node != NULL)
      {
        char* cname = qs_get_node_name(doc, hex1node);
        char* cvalue = qs_get_node_value(doc, hex1node);
        if (strcasecmp(cname, "text") == 0)
        {
          em->imode->hex1byte = hexstring_to_byte(cvalue);
        }
      }
      else
      {
        em->imode->hex1byte    = 0;
      }
    }
    else
    if (strcasecmp(name, "hex2") == 0)
    {
      Node* hex2node = qs_get_child_node(doc, child);
      if (hex2node != NULL)
      {
        char* cname = qs_get_node_name(doc, hex2node);
        char* cvalue = qs_get_node_value(doc, hex2node);
        if (strcasecmp(cname, "text") == 0)
        {
          em->imode->hex2byte = hexstring_to_byte(cvalue);
        }
      }
      else
      {
        em->imode->hex2byte    = 0;
      }
    }
    else
    if (strcasecmp(name, "string") == 0)
    {
      Node* string_node = qs_get_child_node(doc, child);
      if (string_node != NULL)
      {
        char* cname = qs_get_node_name(doc, string_node);
        char* cvalue = qs_get_node_value(doc, string_node);
        if (strcasecmp(cname, "text") == 0)
        {
          em->imode->string = apr_pstrdup(p, cvalue);
        }
      }
      else
      {
        em->imode->string    = apr_palloc(p, 1);
        em->imode->string[0] = 0;
      }
    }
    else
    if (strcasecmp(name, "description") == 0)
    {
      Node* description_node = qs_get_child_node(doc, child);
      if (description_node != NULL)
      {
        char* cname = qs_get_node_name(doc, description_node);
        char* cvalue = qs_get_node_value(doc, description_node);
        if (strcasecmp(cname, "text") == 0)
        {
          em->imode->description = apr_pstrdup(p, cvalue);
        }
      }
      else
      {
        em->imode->description    = apr_palloc(p, 1);
        em->imode->description[0] = 0;
      }
    }
  }
  return NULL;
}

static char*
load_emoji_ezweb_tag(
  Doc* doc,
  apr_pool_t* p,
  emoji_t* em,
  Node* node)
{
  Node* child;


  em->ezweb->typeA = NULL;
  em->ezweb->typeB = NULL;
  em->ezweb->typeC = NULL;
  em->ezweb->typeD = NULL;

  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child))
  {
    char* name  = qs_get_node_name(doc, child);
    if (strcasecmp(name, "A") == 0) 
    {
      Node* typeAnode = qs_get_child_node(doc, child);

      if (typeAnode != NULL)
      {
        char* cname = qs_get_node_name(doc, typeAnode);
        char* cvalue = qs_get_node_value(doc, typeAnode);
        if (strcasecmp(cname, "text") == 0)
        {
          em->ezweb->typeA = apr_pstrdup(p,cvalue);
        }
      }
      else
      {
        em->ezweb->typeA    = apr_palloc(p, 1);
        em->ezweb->typeA[0] = 0;
      }
    }
    else
    if (strcasecmp(name, "B") == 0) 
    {
      Node* typeBnode = qs_get_child_node(doc, child);

      if (typeBnode != NULL)
      {
        char* cname = qs_get_node_name(doc, typeBnode);
        char* cvalue = qs_get_node_value(doc, typeBnode);
        if (strcasecmp(cname, "text") == 0)
        {
          em->ezweb->typeB = apr_pstrdup(p,cvalue);
        }
      }
      else
      {
        em->ezweb->typeB    = apr_palloc(p, 1);
        em->ezweb->typeB[0] = 0;
      }
    }
    else
    if (strcasecmp(name, "C") == 0) 
    {
      Node* typeCnode = qs_get_child_node(doc, child);

      if (typeCnode != NULL)
      {
        char* cname = qs_get_node_name(doc, typeCnode);
        char* cvalue = qs_get_node_value(doc, typeCnode);
        if (strcasecmp(cname, "text") == 0)
        {
          em->ezweb->typeC = apr_pstrdup(p,cvalue);
        }
      }
      else
      {
        em->ezweb->typeC = apr_palloc(p, 1);
        em->ezweb->typeC[0] = 0;
      }
    }
    else
    if (strcasecmp(name, "D") == 0) 
    {
      Node* typeDnode = qs_get_child_node(doc, child);

      if (typeDnode != NULL)
      {
        char* cname = qs_get_node_name(doc, typeDnode);
        char* cvalue = qs_get_node_value(doc, typeDnode);
        if (strcasecmp(cname, "text") == 0)
        {
          em->ezweb->typeD = apr_pstrdup(p,cvalue);
        }
      }
      else
      {
        em->ezweb->typeD = apr_palloc(p, 1);
        em->ezweb->typeD[0] = 0;
      }
    }
  }
  return NULL;
}

static char*
load_emoji_jphone_tag(
  Doc* doc,
  apr_pool_t* p,
  emoji_t* em,
  Node* node)
{
  Node* child;


  em->jphone->string = NULL;

  for (child = qs_get_child_node(doc, node);
       child ;
       child = qs_get_next_node(doc, child))
  {
    char* name  = qs_get_node_name(doc, child);
    if (strcasecmp(name, "string") == 0) 
    {
      Node* stringnode = qs_get_child_node(doc, child);
      if (stringnode != NULL)
      {
        char* cname = qs_get_node_name(doc, stringnode);
        char* cvalue = qs_get_node_value(doc, stringnode);
        if (strcasecmp(cname, "text") == 0)
        {
          int ii;
          int jj;
          char* tmp = apr_palloc(p, strlen(cvalue)+1);
          memset(tmp, 0, strlen(cvalue)+1);

          for (jj=0,ii=0; ii< strlen(cvalue); ii++) 
          {
            if (strncasecmp(&cvalue[ii] , ESCAPE_CHAR_LT, strlen(ESCAPE_CHAR_LT)) == 0)
            {
              tmp[jj] = '<';
              jj++;
              ii += (strlen(ESCAPE_CHAR_LT) - 1);
            }
            else
            if (strncasecmp(&cvalue[ii] , ESCAPE_CHAR_GT, strlen(ESCAPE_CHAR_GT)) == 0)
            {
              tmp[jj] = '>';
              jj++;
              ii += (strlen(ESCAPE_CHAR_GT) - 1);
            }
            else
            {
              tmp[jj] = cvalue[ii];
              jj++;
            }
          }
          em->jphone->string = apr_pstrdup(p,tmp);
        }
      }
      else
      {
        em->jphone->string    = apr_palloc(p, 1);
        em->jphone->string[0] = 0;
      }
    }
  }
  return NULL;
}

static void
emoji_add_to_tail(
  mod_chxj_config* conf,
  emoji_t* emoji)
{
  emoji->next = NULL;
  if (conf->emoji == NULL)
  {
    conf->emoji      = emoji;
    conf->emoji_tail = emoji;
    return;
  }

  conf->emoji_tail->next = emoji;
  conf->emoji_tail = emoji;
}

static char 
hexstring_to_byte(char* s)
{
  int len;
  int ii;
  char one_byte = 0;

  len = strlen(s);
  for (ii=0; ii<len; ii++)
  {
    switch(s[ii]) 
    {
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
