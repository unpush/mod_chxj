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
#include "mod_chxj.h"
#include <qs_ignore_sp.h>
#include <qs_log.h>
#include <qs_malloc.h>
#include <qs_parse_attr.h>
#include <qs_parse_file.h>
#include <qs_parse_string.h>
#include <qs_parse_tag.h>
#include "chxj_load_device_data.h"
#include "chxj_str_util.h"


static void s_set_devices_data(
  Doc              *doc, 
  apr_pool_t       *p, 
  mod_chxj_config  *conf, 
  Node             *node) ;

static void s_set_user_agent_data(
  Doc              *doc, 
  apr_pool_t       *p, 
  mod_chxj_config  *conf, 
  Node             *node);

static void s_set_device_data(
  Doc               *doc, 
  apr_pool_t        *p, 
  device_table_list *dtl, 
  Node              *node) ;


/**
 * load device_data.xml
 */
void
chxj_load_device_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf) 
{
  conf->devices = NULL;
  s_set_devices_data(doc, p, conf,qs_get_root(doc));
}


/**
 * <devices>
 */
static void
s_set_devices_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf, Node *node) 
{
  Node *child;

  for (child = qs_get_child_node(doc,node); 
       child ; 
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    if (STRCASEEQ('d','D',"devices",name)) {
      s_set_user_agent_data(doc, p, conf, child);
    }
  }
}

/**
 * <user_agent>
 */
static void
s_set_user_agent_data(Doc *doc, apr_pool_t *p, mod_chxj_config *conf, Node *node) 
{
  Node              *child;
  device_table_list *t;

  for (child = qs_get_child_node(doc,node);
       child ;
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    if (STRCASEEQ('u','U',"user_agent",name)) {
      Attr *attr;
      device_table_list *dtl;

      if (! conf->devices) {
        conf->devices = apr_pcalloc(p, sizeof(device_table_list));
        conf->devices->next    = NULL;
        conf->devices->pattern = NULL;
        conf->devices->table   = NULL;
        conf->devices->tail    = NULL;
        dtl = conf->devices;
      }
      else {
        for (t = conf->devices; t ; t = t->next) {
          if (! t->next)
            break;
        }
        t->next = apr_pcalloc(p, sizeof(device_table_list));
        t->next->next    = NULL;
        t->next->pattern = NULL;
        t->next->table   = NULL;
        t->next->tail    = NULL;
        dtl = t->next;
      }

      for (attr = qs_get_attr(doc,child); 
           attr ; 
           attr = qs_get_next_attr(doc,attr)) {
        char *attr_name = qs_get_attr_name(doc,attr);

        if (STRCASEEQ('p','P',"pattern",attr_name)) {
            dtl->pattern = apr_pstrdup(p, qs_get_attr_value(doc,attr));
            dtl->regexp = ap_pregcomp(p, (const char *)dtl->pattern, AP_REG_EXTENDED|AP_REG_ICASE);
        }
      }
      s_set_device_data(doc, p, dtl, child);
    }
  }
}


static void
s_set_device_data(Doc *doc, apr_pool_t *p, device_table_list *dtl, Node *node) 
{
  Node         *child;
  device_table *dt;

  dt = apr_pcalloc(p, sizeof(device_table));
  dt->next           = NULL;
  dt->device_id      = NULL;
  dt->device_name    = NULL;
  dt->html_spec_type = CHXJ_SPEC_Chtml_3_0;
  dt->width          = 0;
  dt->heigh          = 0;
  dt->wp_width       = 0;
  dt->wp_heigh       = 0;
  dt->cache          = 5;
  dt->emoji_type     = NULL;
  dt->color          = 256;
  dt->dpi_width      = 96;
  dt->dpi_heigh      = 96;

  for (child = qs_get_child_node(doc,node); 
       child ;
       child = qs_get_next_node(doc,child)) {
    char *name = qs_get_node_name(doc,child);
    switch (*name) {
    case 'd':
    case 'D':
      if (strcasecmp(name, "device") == 0) {
        s_set_device_data(doc,p, dtl, child);
      }
      else
      if (strcasecmp(name, "device_id") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          dt->device_id = apr_pstrdup(p, qs_get_node_value(doc, ch));
        }
      }
      else
      if (strcasecmp(name, "device_name") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          dt->device_name = apr_pstrdup(p, qs_get_node_value(doc, ch));
        }
      }
      else
      if (strcasecmp(name, "dpi_width") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
          if (ii == strlen(vv)) 
            dt->dpi_width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->dpi_width = 0;
        }
      }
      else
      if (strcasecmp(name, "dpi_heigh") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->dpi_heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->dpi_heigh = 0;
        }
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "html_spec_type") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch &&  strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc, ch);
          if (STRCASEEQ('x','X',"xhtml_mobile_1_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_XHtml_Mobile_1_0;
          }
          else if (STRCASEEQ('c','C',"chtml_1_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_1_0;
          }
          else if (STRCASEEQ('c','C',"chtml_2_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_2_0;
          }
          else if (STRCASEEQ('c','C',"chtml_3_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_3_0;
          }
          else if (STRCASEEQ('c','C',"chtml_4_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_4_0;
          }
          else if (STRCASEEQ('c','C',"chtml_5_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_5_0;
          }
          else if (STRCASEEQ('c','C',"chtml_6_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_6_0;
          }
          else if (STRCASEEQ('c','C',"chtml_7_0",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Chtml_7_0;
          }
          else if (STRCASEEQ('h','H',"hdml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Hdml;
          }
          else if (STRCASEEQ('j','J',"jhtml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Jhtml;
          }
          else if (STRCASEEQ('j','J',"jxhtml",vv)) {
            dt->html_spec_type = CHXJ_SPEC_Jxhtml;
          }
        }
      }
      else 
      if (strcasecmp(name, "heigh") == 0) {
        Node* ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->heigh = 0;
        }
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0')
              continue;
            break;
          }
          if (ii == strlen(vv))
            dt->width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->width = 0;
        }
      }
      else
      if (strcasecmp(name, "wp_width") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->wp_width = atoi(qs_get_node_value(doc,ch));
          else 
            dt->wp_width = 0;
        }
      }
      else
      if (strcasecmp(name, "wp_heigh") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->wp_heigh = atoi(qs_get_node_value(doc,ch));
          else 
            dt->wp_heigh = 0;
        }
      }
      break;

    case 'g':
    case 'G':
      if (strcasecmp(name, "gif") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0)
            dt->available_gif = 1;
          else
            dt->available_gif = 0;
        }
      }
      break;

    case 'j':
    case 'J':
      if (strcasecmp(name, "jpeg") == 0 || strcasecmp(name, "jpg") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch != NULL && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_jpeg = 1;
          else 
            dt->available_jpeg = 0;
        }
      }
      break;
  
    case 'p':
    case 'P':
      if (strcasecmp(name, "png") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_png = 1;
          else
            dt->available_png = 0;
        }
      }
      break;

    case 'b':
    case 'B':
      if (strcasecmp(name, "bmp2") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_bmp2 = 1;
          else
            dt->available_bmp2 = 0;
        }
      }
      else
      if (strcasecmp(name, "bmp4") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (strcasecmp(vv, "true") == 0) 
            dt->available_bmp4 = 1;
          else
            dt->available_bmp4 = 0;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
  
          if (chxj_chk_numeric(vv) != 0)
            dt->color = 0;
          else 
            dt->color = chxj_atoi(vv);
        }
      }
      else
      if (strcasecmp(name, "cache") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) {
          char *vv = qs_get_node_value(doc,ch);
          unsigned int ii;
          for (ii=0; ii<strlen(vv); ii++) {
            if ((vv[ii] >= '1' && vv[ii] <= '9') || vv[ii] == '0') 
              continue;
            break;
          }
  
          if (ii == strlen(vv)) 
            dt->cache = atoi(qs_get_node_value(doc,ch));
          else 
            dt->cache = 0;
        }
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "emoji_type") == 0) {
        Node *ch = qs_get_child_node(doc, child);
        if (ch && strcasecmp(qs_get_node_name(doc,ch), "text") == 0) 
          dt->emoji_type = apr_pstrdup(p, qs_get_node_value(doc, ch));
      }
      break;
    default:
      break;
    }
  }

  if (dt->device_id) {
    if (! dtl->table) {
      dtl->table = dt;
      dtl->tail = dt;
    }
    else {
      dtl->tail->next = dt;
      dtl->tail = dt;
    }
  }
}
/*
 * vim:ts=2 et
 */
