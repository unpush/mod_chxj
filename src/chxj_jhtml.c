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
#include "chxj_jhtml.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"

static char* jhtml_node_exchange    (Jhtml* jhtml, Node* node, int indent);
static char* jhtml_start_html_tag   (Jhtml* jhtml, Node* child);
static char* jhtml_end_html_tag     (Jhtml* jhtml, Node* child);
static char* jhtml_start_meta_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_meta_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_head_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_head_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_title_tag  (Jhtml* jhtml, Node* node);
static char* jhtml_end_title_tag    (Jhtml* jhtml, Node* node);
static char* jhtml_start_base_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_base_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_body_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_body_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_a_tag      (Jhtml* jhtml, Node* node);
static char* jhtml_end_a_tag        (Jhtml* jhtml, Node* node);
static char* jhtml_start_br_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_end_br_tag       (Jhtml* jhtml, Node* node);
static char* jhtml_start_font_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_font_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_form_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_end_form_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_start_input_tag  (Jhtml* jhtml, Node* node);
static char* jhtml_end_input_tag    (Jhtml* jhtml, Node* node);
static char* jhtml_start_center_tag (Jhtml* jhtml, Node* node);
static char* jhtml_end_center_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_start_hr_tag     (Jhtml* jhtml, Node* node);
static char* jhtml_end_hr_tag       (Jhtml* jhtml, Node* node);
static char* jhtml_start_img_tag    (Jhtml* jhtml, Node* node);
static char* jhtml_end_img_tag      (Jhtml* jhtml, Node* node);
static char* jhtml_start_select_tag (Jhtml* jhtml, Node* node);
static char* jhtml_end_select_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_start_option_tag (Jhtml* jhtml, Node* node);
static char* jhtml_end_option_tag   (Jhtml* jhtml, Node* node);
static char* jhtml_start_div_tag    (Jhtml* jhtml, Node* node);
static char* jhtml_end_div_tag      (Jhtml* jhtml, Node* node);
static void chxj_init_jhtml(Jhtml* jhtml, Doc* doc, request_rec* r, device_table* spec);
static int jhtml_search_emoji(Jhtml* jhtml, char* txt, char** rslt);
static char* chxj_istyle_to_mode(request_rec* r, const char* s);
static void jhtml_1_0_chxjif_tag(Jhtml* jhtml, Node* node); 

/**
 * converts from CHTML5.0 to JHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_jhtml(
  request_rec* r,
  device_table *spec,
  const char* src,
  apr_size_t srclen,
  apr_size_t *dstlen)
{
  char*     dst = NULL;
  char*     ss;
  Jhtml   jhtml;
  Doc       doc;

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_init_jhtml(&jhtml, &doc, r, spec);

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0, srclen + 1);
  memcpy(ss, src, srclen);
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> JHTML", ss, srclen);
#endif
  qs_parse_string(&doc,ss, srclen);

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to JHTML.                                      */
  /*--------------------------------------------------------------------------*/
  dst = jhtml_node_exchange(&jhtml, qs_get_root(&doc), 0);
  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL) 
  {
    return apr_pstrdup(r->pool,ss);
  }

  if (strlen(dst) == 0)
  {
    dst = apr_psprintf(r->pool, "\n");
  }
  *dstlen = strlen(dst);
#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> JHTML", dst, *dstlen);
#endif
  return dst;
}

/**
 * The CHTML structure is initialized.
 *
 * @param jhtml [i/o] The pointer to the JHTML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   JHTML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
chxj_init_jhtml(Jhtml* jhtml, Doc* doc, request_rec* r, device_table* spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(jhtml, 0, sizeof(Jhtml));

  doc->r      = r;
  jhtml->doc  = doc;
  jhtml->spec = spec;
  jhtml->out  = qs_alloc_zero_byte_string(r);
  jhtml->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  jhtml->doc->parse_mode = PARSE_MODE_CHTML;
}

/**
 * It is main processing of conversion from CHTML to JHTML. 
 *
 * @param jhtml   [i/o] The pointer to the CHTML structure is specified. 
 * @param node    [i]   The pointer to a current node is specified. 
 * @param indent  [i]   The depth of the node processing it now is specified. 
 *
 * @return The character string after it converts it is returned. 
 */
static char*
jhtml_node_exchange(Jhtml* jhtml, Node* node, int indent) 
{
  Node*         child;
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* It is the main loop of the conversion processing.                        */
  /*--------------------------------------------------------------------------*/
  for (child = qs_get_child_node(doc,node);
       child ;
       child = qs_get_next_node(doc,child)) 
  {
    char* name = qs_get_node_name(doc,child);

    /*------------------------------------------------------------------------*/
    /* <HTML>                                                                 */
    /*------------------------------------------------------------------------*/
    if (strcasecmp(name, "html") == 0) 
    {
      jhtml_start_html_tag(jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_html_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "meta") == 0) 
    {
      jhtml_start_meta_tag(jhtml, child);
      jhtml_end_meta_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HEAD>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "head") == 0) 
    {
      jhtml_start_head_tag(jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_head_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "title") == 0) 
    {
      jhtml_start_title_tag (jhtml, child);
      jhtml_node_exchange   (jhtml, child,indent+1);
      jhtml_end_title_tag   (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BASE>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "base") == 0) 
    {
      jhtml_start_base_tag(jhtml, child);
      jhtml_end_base_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BODY>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "body") == 0) 
    {
      jhtml_start_body_tag(jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_body_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "a") == 0) 
    {
      jhtml_start_a_tag   (jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_a_tag     (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "br") == 0) 
    {
      jhtml_start_br_tag  (jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_br_tag    (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FONT>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "font") == 0) 
    {
      jhtml_start_font_tag(jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_font_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FORM>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "form") == 0) 
    {
      jhtml_start_form_tag(jhtml, child);
      jhtml_node_exchange (jhtml, child,indent+1);
      jhtml_end_form_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <INPUT>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "input") == 0) 
    {
      jhtml_start_input_tag (jhtml, child);
      jhtml_node_exchange   (jhtml, child,indent+1);
      jhtml_end_input_tag   (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "hr") == 0) 
    {
      jhtml_start_hr_tag  (jhtml, child);
      jhtml_end_hr_tag    (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <CENTER>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "center") == 0) 
    {
      jhtml_start_center_tag(jhtml, child);
      jhtml_node_exchange   (jhtml, child,indent+1);
      jhtml_end_center_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <IMG>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "img") == 0) 
    {
      jhtml_start_img_tag (jhtml, child);
      jhtml_end_img_tag   (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "select") == 0)
    {
      jhtml_start_select_tag(jhtml, child);
      jhtml_node_exchange   (jhtml, child, indent+1);
      jhtml_end_select_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "option") == 0)
    {
      jhtml_start_option_tag(jhtml, child);
      jhtml_node_exchange   (jhtml, child, indent+1);
      jhtml_end_option_tag  (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "div") == 0)
    {
      jhtml_start_div_tag (jhtml, child);
      jhtml_node_exchange (jhtml, child, indent+1);
      jhtml_end_div_tag   (jhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BLINK>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "blink") == 0)
    {
      /* ignore */
    }
    /*------------------------------------------------------------------------*/
    /* <CHXJ:IF>                                                              */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "chxj:if") == 0)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag found");
      if (chxj_chxjif_is_mine(jhtml->spec, doc, child))
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
        jhtml_1_0_chxjif_tag(jhtml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* NORMAL TEXT                                                            */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "text") == 0) 
    {
      char*   textval;
      char*   tmp;
      char*   tdst;
      char    one_byte[2];
      int     ii;
      int     tdst_len;

      textval = qs_get_node_value(doc,child);
      textval = qs_trim_string(jhtml->doc->r, textval);
      if (strlen(textval) == 0)
      {
        continue;
      }

      tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
      memset(tmp, 0, qs_get_node_size(doc,child)+1);

      tdst     = qs_alloc_zero_byte_string(r);
      memset(one_byte, 0, sizeof(one_byte));
      tdst_len = 0;

      for (ii=0; ii<qs_get_node_size(doc,child); ii++)
      {
        char* out;
        int rtn = jhtml_search_emoji(jhtml, &textval[ii], &out);
        if (rtn != 0)
        {
          tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
          ii+=(rtn - 1);
          continue;
        }
        if (is_sjis_kanji(textval[ii]))
        {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          one_byte[0] = textval[ii+1];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          ii++;
        }
        else if (textval[ii] != '\r' && textval[ii] != '\n')
        {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        }
      }
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, tdst, NULL);
    }
  }
  return jhtml->out;
}

/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param jhtml   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
jhtml_search_emoji(Jhtml* jhtml, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
  int           len;

  spec = jhtml->spec;

  len = strlen(txt);
  r = jhtml->doc->r;

  if (spec == NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");
  }

  for (ee = jhtml->conf->emoji;
       ee;
       ee = ee->next) 
  {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (ee->imode == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }
    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0)
    {
      if (spec == NULL || spec->emoji_type == NULL)
      {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
        return strlen(ee->imode->string);
      }

      return 0;
    }
    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte))
    {
      if (spec == NULL || spec->emoji_type == NULL)
      {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
        return 2;
      }

      return 0;
    }
  }
  return 0;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_html_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<html>\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_html_tag(Jhtml* jhtml, Node* child) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</html>\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_meta_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<meta", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name   = qs_get_attr_name(doc,attr);
    char* value  = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "http-equiv") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " http-equiv=\"", 
                      value,
                      "\"",
                      NULL);
    }
    else
    if (strcasecmp(name, "content") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " content=\"", 
                      value,
                      "\"",
                      NULL);
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_meta_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_head_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<head>\r\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_head_tag(Jhtml* jhtml, Node* child) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</head>\r\n", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_title_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<title>", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_title_tag(Jhtml* jhtml, Node* child) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</title>\r\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_base_tag(Jhtml* jhtml, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<base", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "href") == 0) 
    {
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >\r\n", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_base_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_body_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<body", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value  = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "bgcolor") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " bgcolor=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "text") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " text=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "link") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " link=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "alink") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "vlink") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\r\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_body_tag(Jhtml* jhtml, Node* child) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</body>\r\n", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_a_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<a", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "name") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " name=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "href") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "accesskey") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " accesskey=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "cti") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " cti=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "ijam") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "utn") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " utn ", 
                      NULL);
    }
    else
    if (strcasecmp(name, "telbook") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else
    if (strcasecmp(name, "kana") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else
    if (strcasecmp(name, "email") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else
    if (strcasecmp(name, "ista") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "ilet") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "iswf") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "irst") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_a_tag(Jhtml* jhtml, Node* child) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</a>", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_br_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<br>\r\n", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_br_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_font_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<font", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "color") == 0) 
    {
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " color=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "size") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_font_tag(Jhtml* jhtml, Node* child) 
{
  request_rec* r = jhtml->doc->r;
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</font>", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_form_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<form", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "action") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " action=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "method") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " method=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "utn") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " utn ", 
                      NULL);
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_form_tag(Jhtml* jhtml, Node* child) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</form>", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_input_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc         = jhtml->doc;
  request_rec*  r           = doc->r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<input", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/

  type       = qs_get_type_attr(doc, node, r);
  name       = qs_get_name_attr(doc, node, r);
  value      = qs_get_value_attr(doc,node,r);
  istyle     = qs_get_istyle_attr(doc,node,r);
  max_length = qs_get_maxlength_attr(doc,node,r);
  checked    = qs_get_checked_attr(doc,node,r);
  accesskey  = qs_get_accesskey_attr(doc, node, r);
  size       = qs_get_size_attr(doc, node, r);

  if (type != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool,
                    jhtml->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
  }
  if (size != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
  }
  if (name != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
  }
  if (value != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
  }
  if (accesskey != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
  }
  if (istyle != NULL)
  {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (type != NULL && strcasecmp(type, "password") == 0)
    {
      jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    "numeric", "\" ", 
                    NULL);
    }
    else
    {
      jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    chxj_istyle_to_mode(r,istyle), "\" ", 
                    NULL);
    }
  }
  else
  if (istyle == NULL && type != NULL && strcasecmp(type, "password") == 0)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    "numeric", "\" ", 
                    NULL);
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " maxlength=\"", 
                      max_length, 
                      "\"", 
                      NULL);
  }

  if (checked != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, " checked ", NULL);
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_input_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_center_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<center>", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_center_tag(Jhtml* jhtml, Node* child) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</center>", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_hr_tag(Jhtml* jhtml, Node* node) 
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<hr ", NULL);
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "align") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " align=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "size") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " size=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "width") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " width=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "noshade") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " noshade ", NULL);
    }
    else
    if (strcasecmp(name, "color") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_hr_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_img_tag(Jhtml* jhtml, Node* node) 
{
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;
  Attr* attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table* spec = jhtml->spec;
#endif

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<img", NULL);
 

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "src") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"",value,"\"", NULL);
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"", chxj_img_conv(r,spec,value), NULL);
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, "\"", NULL);
#endif
    }
    else
    if (strcasecmp(name, "align" ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " align=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "width" ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " width=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "height") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " height=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "hspace") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " hspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "vspace") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " vspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " alt=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "align" ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_img_tag(Jhtml* jhtml, Node* child) 
{
  return jhtml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_select_tag(Jhtml* jhtml, Node* child)
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* size      = NULL;
  char* name      = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<select", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "size") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "name") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "multiple") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
  }

  if (size != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " size=\"",size,"\"", NULL);
  }
  if (name != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " name=\"",name,"\"", NULL);
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\n", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_select_tag(Jhtml* jhtml, Node* child)
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</select>\n", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_option_tag(Jhtml* jhtml, Node* child)
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* selected   = NULL;
  char* value      = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<option", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "selected") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "value") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(r->pool, val);
    }
  }

  if (value != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"",value,"\"", NULL);
  }
  else
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"\"", NULL);
  }

  if (selected != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " selected ", NULL);
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_option_tag(Jhtml* jhtml, Node* child)
{
  /* Don't close */
  return jhtml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_start_div_tag(Jhtml* jhtml, Node* child)
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* align   = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<div", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "align") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(r->pool, val);
    }
  }

  if (align != NULL)
  {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, " align=\"", align, "\"", NULL);
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
  return jhtml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
jhtml_end_div_tag(Jhtml* jhtml, Node* child)
{
  Doc* doc = jhtml->doc;
  request_rec* r = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</div>\n", NULL);

  return jhtml->out;
}

static char*
chxj_istyle_to_mode(request_rec* r, const char* s)
{
  char* tmp;
  if (strcasecmp(s, "1") == 0)
  {
    return apr_psprintf(r->pool, "hiragana");
  }
  else
  if (strcasecmp(s, "2") == 0)
  {
    return apr_psprintf(r->pool, "hankakukana");
  }
  else
  if (strcasecmp(s, "3") == 0)
  {
    return apr_psprintf(r->pool, "alphabet");
  }
  else
  if (strcasecmp(s, "4") == 0)
  {
    return apr_psprintf(r->pool, "numeric");
  }

  tmp = apr_palloc(r->pool, 1);
  tmp[0] = '\0';
  return apr_pstrdup(r->pool,tmp);
}

static void
jhtml_1_0_chxjif_tag(Jhtml* jhtml, Node* node)
{
  Doc*         doc   = jhtml->doc;
  Node*        child;
  request_rec* r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, child->otext, NULL);
    jhtml_1_0_chxjif_tag(jhtml, child);
  }
}
/*
 * vim:ts=2 et
 */
