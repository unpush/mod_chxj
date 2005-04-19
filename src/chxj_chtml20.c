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
#include "chxj_chtml20.h"
#include "chxj_hdml.h"
#include "chxj_str_util.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"

static char* s_chtml20_node_exchange    (Chtml20* chtml, Node* node, int indent);
static char* s_chtml20_start_html_tag   (Chtml20* chtml, Node* child);
static char* s_chtml20_end_html_tag     (Chtml20* chtml, Node* child);
static char* s_chtml20_start_meta_tag   (Chtml20* chtml, Node* node);
static char* s_chtml20_end_meta_tag     (Chtml20* chtml, Node* node);
static char* s_chtml20_start_head_tag   (Chtml20* chtml, Node* node);
static char* s_chtml20_end_head_tag     (Chtml20* chtml, Node* node);
static char* s_chtml20_start_title_tag  (Chtml20* chtml, Node* node);
static char* s_chtml20_end_title_tag    (Chtml20* chtml, Node* node);
static char* s_chtml20_start_base_tag   (Chtml20* chtml, Node* node);
static char* s_chtml20_end_base_tag     (Chtml20* chtml, Node* node);
static char* s_chtml20_start_body_tag   (Chtml20* chtml, Node* node);
static char* s_chtml20_end_body_tag     (Chtml20* chtml, Node* node);
static char* s_chtml20_start_a_tag      (Chtml20* chtml, Node* node);
static char* chtml20_end_a_tag        (Chtml20* chtml, Node* node);
static char* chtml20_start_br_tag     (Chtml20* chtml, Node* node);
static char* chtml20_end_br_tag       (Chtml20* chtml, Node* node);
static char* chtml20_start_font_tag   (Chtml20* chtml, Node* node);
static char* chtml20_end_font_tag     (Chtml20* chtml, Node* node);
static char* chtml20_start_form_tag   (Chtml20* chtml, Node* node);
static char* chtml20_end_form_tag     (Chtml20* chtml, Node* node);
static char* chtml20_start_input_tag  (Chtml20* chtml, Node* node);
static char* chtml20_end_input_tag    (Chtml20* chtml, Node* node);
static char* chtml20_start_center_tag (Chtml20* chtml, Node* node);
static char* chtml20_end_center_tag   (Chtml20* chtml, Node* node);
static char* chtml20_start_hr_tag     (Chtml20* chtml, Node* node);
static char* chtml20_end_hr_tag       (Chtml20* chtml, Node* node);
static char* chtml20_start_img_tag    (Chtml20* chtml, Node* node);
static char* chtml20_end_img_tag      (Chtml20* chtml, Node* node);
static char* chtml20_start_select_tag (Chtml20* chtml, Node* node);
static char* chtml20_end_select_tag   (Chtml20* chtml, Node* node);
static char* chtml20_start_option_tag (Chtml20* chtml, Node* node);
static char* chtml20_end_option_tag   (Chtml20* chtml, Node* node);
static char* chtml20_start_div_tag    (Chtml20* chtml, Node* node);
static char* chtml20_end_div_tag      (Chtml20* chtml, Node* node);
static void chxj_init_chtml20(Chtml20* chtml, Doc* doc, request_rec* r, device_table* spec);
static int chtml20_search_emoji(Chtml20* chtml, char* txt, char** rslt);
static void chtml20_chxjif_tag(Chtml20* chtml, Node* node); 

/**
 * converts from CHTML5.0 to CHTML2.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_chtml20(
  request_rec* r,
  device_table *spec,
  const char* src,
  apr_size_t srclen,
  apr_size_t *dstlen)
{
  char*     dst = NULL;
  char*     ss;
  Chtml20   chtml20;
  Doc       doc;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst != NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"i found qrcode xml");
    return dst;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_init_chtml20(&chtml20, &doc, r, spec);
  ap_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0, srclen + 1);
  memcpy(ss, src, srclen);
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML2.0", ss, srclen);
#endif

  qs_parse_string(&doc,ss, strlen(ss));

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  dst = s_chtml20_node_exchange(&chtml20, qs_get_root(&doc), 0);
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
  chxj_dump_out("[dst] CHTML -> CHTML2.0", dst, *dstlen);
#endif
  return dst;
}

/**
 * The CHTML structure is initialized.
 *
 * @param chtml20 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
chxj_init_chtml20(Chtml20* chtml20, Doc* doc, request_rec* r, device_table* spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(chtml20, 0, sizeof(Chtml20));

  doc->r      = r;
  chtml20->doc  = doc;
  chtml20->spec = spec;
  chtml20->out  = qs_alloc_zero_byte_string(r);
  chtml20->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  chtml20->doc->parse_mode = PARSE_MODE_CHTML;
}

/**
 * It is main processing of conversion from CHTML to CHTML. 
 *
 * @param chtml20   [i/o] The pointer to the CHTML structure is specified. 
 * @param node    [i]   The pointer to a current node is specified. 
 * @param indent  [i]   The depth of the node processing it now is specified. 
 *
 * @return The character string after it converts it is returned. 
 */
static char*
s_chtml20_node_exchange(Chtml20* chtml20, Node* node, int indent) 
{
  Node*         child;
  Doc*          doc   = chtml20->doc;
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
      s_chtml20_start_html_tag(chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      s_chtml20_end_html_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "meta") == 0) 
    {
      s_chtml20_start_meta_tag(chtml20, child);
      s_chtml20_end_meta_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HEAD>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "head") == 0) 
    {
      s_chtml20_start_head_tag(chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      s_chtml20_end_head_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "title") == 0) 
    {
      s_chtml20_start_title_tag (chtml20, child);
      s_chtml20_node_exchange   (chtml20, child,indent+1);
      s_chtml20_end_title_tag   (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BASE>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "base") == 0) 
    {
      s_chtml20_start_base_tag(chtml20, child);
      s_chtml20_end_base_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BODY>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "body") == 0) 
    {
      s_chtml20_start_body_tag(chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      s_chtml20_end_body_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "a") == 0) 
    {
      s_chtml20_start_a_tag   (chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      chtml20_end_a_tag     (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <BR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "br") == 0) 
    {
      chtml20_start_br_tag  (chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      chtml20_end_br_tag    (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FONT>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "font") == 0) 
    {
      chtml20_start_font_tag(chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      chtml20_end_font_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <FORM>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "form") == 0) 
    {
      chtml20_start_form_tag(chtml20, child);
      s_chtml20_node_exchange (chtml20, child,indent+1);
      chtml20_end_form_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <INPUT>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "input") == 0) 
    {
      chtml20_start_input_tag (chtml20, child);
      s_chtml20_node_exchange   (chtml20, child,indent+1);
      chtml20_end_input_tag   (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <HR>                                                                   */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "hr") == 0) 
    {
      chtml20_start_hr_tag  (chtml20, child);
      chtml20_end_hr_tag    (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <CENTER>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "center") == 0) 
    {
      chtml20_start_center_tag(chtml20, child);
      s_chtml20_node_exchange   (chtml20, child,indent+1);
      chtml20_end_center_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <IMG>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "img") == 0) 
    {
      chtml20_start_img_tag (chtml20, child);
      chtml20_end_img_tag   (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "select") == 0)
    {
      chtml20_start_select_tag(chtml20, child);
      s_chtml20_node_exchange   (chtml20, child, indent+1);
      chtml20_end_select_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "option") == 0)
    {
      chtml20_start_option_tag(chtml20, child);
      s_chtml20_node_exchange   (chtml20, child, indent+1);
      chtml20_end_option_tag  (chtml20, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "div") == 0)
    {
      chtml20_start_div_tag (chtml20, child);
      s_chtml20_node_exchange (chtml20, child, indent+1);
      chtml20_end_div_tag   (chtml20, child);
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
      if (chxj_chxjif_is_mine(chtml20->spec, doc, child))
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
        char* parse_attr = NULL;
        parse_attr = qs_get_parse_attr(doc, child, r);
        if (parse_attr != NULL && strcasecmp(parse_attr, "true") == 0)
        {
          s_chtml20_node_exchange (chtml20, child, indent+1);
        }
        else
        {
          chtml20_chxjif_tag(chtml20, child);
        }
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
      textval = qs_trim_string(chtml20->doc->r, textval);
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
        int rtn = chtml20_search_emoji(chtml20, &textval[ii], &out);
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
      chtml20->out = apr_pstrcat(r->pool, chtml20->out, tdst, NULL);
    }
  }
  return chtml20->out;
}

/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml20   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
chtml20_search_emoji(Chtml20* chtml20, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
  int           len;

  spec = chtml20->spec;

  len = strlen(txt);
  r = chtml20->doc->r;

  if (spec == NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");
  }

  for (ee = chtml20->conf->emoji;
       ee;
       ee = ee->next) 
  {
    if (ee->imode == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0)
    {
      if (spec == NULL || spec->emoji_type == NULL)
      {
        *rslt = apr_palloc(r->pool, 3);
        (*rslt)[0] = ee->imode->hex1byte & 0xff;
        (*rslt)[1] = ee->imode->hex2byte & 0xff;
        (*rslt)[2] = 0;
        return strlen(ee->imode->string);
      }

      return 0;
    }
  }
  return 0;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_html_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc   = chtml20->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<html>\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_html_tag(Chtml20* chtml20, Node* child) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</html>\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_meta_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<meta", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      " content=\"", 
                      value,
                      "\"",
                      NULL);
    }
  }
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_meta_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_head_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<head>\r\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_head_tag(Chtml20* chtml20, Node* child) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</head>\r\n", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_title_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<title>", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_title_tag(Chtml20* chtml20, Node* child) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</title>\r\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_base_tag(Chtml20* chtml20, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = chtml20->doc;
  request_rec*  r     = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<base", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, " >\r\n", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_base_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_body_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<body", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">\r\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_body_tag(Chtml20* chtml20, Node* child) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</body>\r\n", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_start_a_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc   = chtml20->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<a", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "telbook") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "kana") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if (strcasecmp(name, "email") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
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
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_a_tag(Chtml20* chtml20, Node* child) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</a>", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_br_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<br>\r\n", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_br_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_font_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc   = chtml20->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<font", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_font_tag(Chtml20* chtml20, Node* child) 
{
  request_rec* r = chtml20->doc->r;
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</font>", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_form_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<form", NULL);

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
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
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_form_tag(Chtml20* chtml20, Node* child) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</form>", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_input_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc         = chtml20->doc;
  request_rec*  r           = doc->r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<input", NULL);

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
    chtml20->out = apr_pstrcat(r->pool,
                    chtml20->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
  }
  if (size != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
  }
  if (name != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
  }
  if (value != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
  }
  if (accesskey != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
  }
  if (istyle != NULL)
  {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " istyle=\"", 
                    istyle, "\" ", 
                    NULL);
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length != NULL)
  {
    if (chxj_chk_numeric(max_length) != 0)
    {
      max_length = apr_psprintf(r->pool, "0");
    }
    if (istyle != NULL && strcasecmp(istyle, "1") == 0)
    {
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      apr_psprintf(r->pool, " maxlength=\"%d\"", chxj_atoi(max_length)*2) , NULL);
    }
    else
    {
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      apr_psprintf(r->pool, " maxlength=\"%d\"", chxj_atoi(max_length)) , NULL);
    }
  }

  if (checked != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, " checked ", NULL);
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, " >", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_input_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_center_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<center>", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_center_tag(Chtml20* chtml20, Node* child) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</center>", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_hr_tag(Chtml20* chtml20, Node* node) 
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<hr ", NULL);
 
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
      chtml20->out = apr_pstrcat(r->pool,
                        chtml20->out, 
                        " align=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "size") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool,
                        chtml20->out, 
                        " size=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "width") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool,
                        chtml20->out, 
                        " width=\"", value, "\" ", NULL);
    }
    else
    if (strcasecmp(name, "noshade") == 0)
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool,
                        chtml20->out, 
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
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, " >", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_hr_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_img_tag(Chtml20* chtml20, Node* node) 
{
  Doc*          doc = chtml20->doc;
  request_rec*  r   = doc->r;
  Attr* attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table* spec = chtml20->spec;
#endif

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<img", NULL);
 

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
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " src=\"",value,"\"", NULL);
#else
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " src=\"", 
                      chxj_img_conv(r, spec,value), NULL);
      chtml20->out = apr_pstrcat(r->pool, chtml20->out, "\"", NULL);
#endif
    }
    else
    if (strcasecmp(name, "align" ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " align=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "width" ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " width=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "height") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " height=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "hspace") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " hspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "vspace") == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " vspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, " alt=\"",value,"\"", NULL);
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

  return chtml20->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_img_tag(Chtml20* chtml20, Node* child) 
{
  return chtml20->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_select_tag(Chtml20* chtml20, Node* child)
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* size      = NULL;
  char* name      = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<select", NULL);
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
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, " size=\"",size,"\"", NULL);
  }
  if (name != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, " name=\"",name,"\"", NULL);
  }
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">\n", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_select_tag(Chtml20* chtml20, Node* child)
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</select>\n", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_option_tag(Chtml20* chtml20, Node* child)
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* selected   = NULL;
  char* value      = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<option", NULL);
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
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, " value=\"",value,"\"", NULL);
  }
  else
  {
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, " value=\"\"", NULL);
  }

  if (selected != NULL)
  {
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, " selected ", NULL);
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_option_tag(Chtml20* chtml20, Node* child)
{
  /* Don't close */
  return chtml20->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_start_div_tag(Chtml20* chtml20, Node* child)
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* align   = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<div", NULL);
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
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, " align=\"", align, "\"", NULL);
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);
  return chtml20->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
chtml20_end_div_tag(Chtml20* chtml20, Node* child)
{
  Doc* doc = chtml20->doc;
  request_rec* r = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</div>\n", NULL);

  return chtml20->out;
}


static void
chtml20_chxjif_tag(Chtml20* chtml20, Node* node)
{
  Doc*         doc   = chtml20->doc;
  Node*        child;
  request_rec* r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    chtml20->out = apr_pstrcat(r->pool, chtml20->out, child->otext, NULL);
    chtml20_chxjif_tag(chtml20, child);
  }
}
/*
 * vim:ts=2 et
 */
