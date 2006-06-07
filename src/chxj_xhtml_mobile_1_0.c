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
#include "mod_chxj.h"
#include "chxj_xhtml_mobile_1_0.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"


static char* s_xhtml_1_0_node_exchange    (xhtml_t* xhtml, Node* node, int indent);
static char* s_xhtml_1_0_start_html_tag   (xhtml_t* xhtml, Node* child);
static char* s_xhtml_1_0_end_html_tag     (xhtml_t* xhtml, Node* child);
static char* s_xhtml_1_0_start_meta_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_meta_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_head_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_head_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_title_tag  (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_title_tag    (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_base_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_base_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_body_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_body_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_a_tag      (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_a_tag        (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_br_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_br_tag       (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_font_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_font_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_form_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_form_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_input_tag  (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_input_tag    (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_center_tag (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_center_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_hr_tag     (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_hr_tag       (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_img_tag    (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_img_tag      (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_select_tag (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_select_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_option_tag (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_option_tag   (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_start_div_tag    (xhtml_t* xhtml, Node* node);
static char* s_xhtml_1_0_end_div_tag      (xhtml_t* xhtml, Node* node);
static void  s_init_xhtml(xhtml_t* xhtml, Doc* doc, request_rec* r, device_table_t* spec);
static int   s_xhtml_search_emoji(xhtml_t* xhtml, char* txt, char** rslt);
static void  s_xhtml_1_0_chxjif_tag(xhtml_t* xhtml, Node* node);
 
/**
 * converts from CHTML to XHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_xhtml_mobile_1_0(
  request_rec* r,
  device_table_t* spec,
  const char* src,
  apr_size_t srclen,
  apr_size_t *dstlen)
{
  char*     dst = NULL;
  char*     ss;
  xhtml_t   xhtml;
  Doc       doc;

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"start chxj_exchange_xhtml_mobile_1_0()");

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst != NULL) {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"i found qrcode xml");
    return dst;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The XHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_xhtml(&xhtml, &doc, r, spec);
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
  chxj_dump_out("[src] CHTML->XHTML", ss, srclen);
#endif
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"start parse");
  qs_parse_string(&doc,ss, strlen(ss));
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"end parse");

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to XHTML.                                      */
  /*--------------------------------------------------------------------------*/
  dst = s_xhtml_1_0_node_exchange(&xhtml, qs_get_root(&doc), 0);
  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL) 
    return apr_pstrdup(r->pool,ss);

  if (strlen(dst) == 0)
    dst = apr_psprintf(r->pool, "\n");

  *dstlen = strlen(dst);
#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML->XHTML", dst, *dstlen);
#endif
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"end chxj_exchange_xhtml_mobile_1_0()");

  return dst;
}

/**
 * The XHTML structure is initialized.
 *
 * @param xhtml [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_xhtml(xhtml_t* xhtml, Doc* doc, request_rec* r, device_table_t* spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(xhtml, 0, sizeof(xhtml_t));

  doc->r      = r;
  xhtml->doc  = doc;
  xhtml->spec = spec;
  xhtml->out  = qs_alloc_zero_byte_string(r);
  xhtml->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  xhtml->doc->parse_mode = PARSE_MODE_CHTML;
}

/**
 * It is main processing of conversion from CHTML to XHTML. 
 *
 * @param xhtml   [i/o] The pointer to the XHTML structure is specified. 
 * @param node    [i]   The pointer to a current node is specified. 
 * @param indent  [i]   The depth of the node processing it now is specified. 
 *
 * @return The character string after it converts it is returned. 
 */
static char*
s_xhtml_1_0_node_exchange(xhtml_t* xhtml, Node* node, int indent) 
{
  Node*         child;
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* It is the main loop of the conversion processing.                        */
  /*--------------------------------------------------------------------------*/
  for (child = qs_get_child_node(doc,node);
       child ;
       child = qs_get_next_node(doc,child)) {
    char* name = qs_get_node_name(doc,child);

    if (*name == 'h' || *name == 'H') {
      /*----------------------------------------------------------------------*/
      /* <HTML>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "html") == 0) {
        s_xhtml_1_0_start_html_tag(xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_html_tag  (xhtml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HEAD>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "head") == 0) {
        s_xhtml_1_0_start_head_tag(xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_head_tag  (xhtml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "hr") == 0) {
        s_xhtml_1_0_start_hr_tag  (xhtml, child);
        s_xhtml_1_0_end_hr_tag    (xhtml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(name, "meta") == 0) {
      s_xhtml_1_0_start_meta_tag(xhtml, child);
      s_xhtml_1_0_end_meta_tag  (xhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "title") == 0) {
      s_xhtml_1_0_start_title_tag (xhtml, child);
      s_xhtml_1_0_node_exchange   (xhtml, child,indent+1);
      s_xhtml_1_0_end_title_tag   (xhtml, child);
    }
    else 
    if (*name == 'b' || *name == 'B') {
      /*----------------------------------------------------------------------*/
      /* <BASE>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "base") == 0) {
        s_xhtml_1_0_start_base_tag(xhtml, child);
        s_xhtml_1_0_end_base_tag  (xhtml, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <BODY>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "body") == 0) {
        s_xhtml_1_0_start_body_tag(xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_body_tag  (xhtml, child);
      }
      else
      /*----------------------------------------------------------------------*/
      /* <BR>                                                                 */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "br") == 0) {
        s_xhtml_1_0_start_br_tag  (xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_br_tag    (xhtml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "a") == 0) {
      s_xhtml_1_0_start_a_tag   (xhtml, child);
      s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
      s_xhtml_1_0_end_a_tag     (xhtml, child);
    }
    else
    if (*name == 'f' || *name == 'F') {
      /*----------------------------------------------------------------------*/
      /* <FONT>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "font") == 0) {
        s_xhtml_1_0_start_font_tag(xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_font_tag  (xhtml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <FORM>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "form") == 0) {
        s_xhtml_1_0_start_form_tag(xhtml, child);
        s_xhtml_1_0_node_exchange (xhtml, child,indent+1);
        s_xhtml_1_0_end_form_tag  (xhtml, child);
      }
    }
    else 
    if (*name == 'i' || *name == 'I') {
      /*----------------------------------------------------------------------*/
      /* <INPUT>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "input") == 0) {
        s_xhtml_1_0_start_input_tag (xhtml, child);
        s_xhtml_1_0_node_exchange   (xhtml, child,indent+1);
        s_xhtml_1_0_end_input_tag   (xhtml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <IMG>                                                                */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "img") == 0) {
        s_xhtml_1_0_start_img_tag (xhtml, child);
        s_xhtml_1_0_end_img_tag   (xhtml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "select") == 0) {
      s_xhtml_1_0_start_select_tag(xhtml, child);
      s_xhtml_1_0_node_exchange   (xhtml, child, indent+1);
      s_xhtml_1_0_end_select_tag  (xhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'o' || *name == 'O') && strcasecmp(name, "option") == 0) {
      s_xhtml_1_0_start_option_tag(xhtml, child);
      s_xhtml_1_0_node_exchange   (xhtml, child, indent+1);
      s_xhtml_1_0_end_option_tag  (xhtml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'd' || *name == 'D') && strcasecmp(name, "div") == 0) {
      s_xhtml_1_0_start_div_tag (xhtml, child);
      s_xhtml_1_0_node_exchange (xhtml, child, indent+1);
      s_xhtml_1_0_end_div_tag   (xhtml, child);
    }
    else
    if (*name == 'c' || *name == 'C') {
      /*----------------------------------------------------------------------*/
      /* <CHXJ:IF>                                                            */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "chxj:if") == 0) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag found");
        if (chxj_chxjif_is_mine(xhtml->spec, doc, child)) {
          char* destlang = qs_get_destlang_attr(doc, child, r);
          if (destlang != NULL && strcasecmp(destlang, "hdml") == 0) {
            ap_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
          }
          ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
  
          char* parse_attr = NULL;
          parse_attr = qs_get_parse_attr(doc, child, r);
          if (parse_attr != NULL && strcasecmp(parse_attr, "true") == 0) {
            s_xhtml_1_0_node_exchange (xhtml, child, indent+1);
          }
          else {
            s_xhtml_1_0_chxjif_tag(xhtml, child);
          }
        }
      }
      /*----------------------------------------------------------------------*/
      /* <CENTER>                                                             */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "center") == 0) {
        s_xhtml_1_0_start_center_tag(xhtml, child);
        s_xhtml_1_0_node_exchange   (xhtml, child,indent+1);
        s_xhtml_1_0_end_center_tag  (xhtml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* NORMAL TEXT                                                            */
    /*------------------------------------------------------------------------*/
    else
    if (strcasecmp(name, "text") == 0) {
      char*   textval;
      char*   tmp;
      char*   tdst;
      char    one_byte[2];
      int     ii;
      int     tdst_len;

      textval = qs_get_node_value(doc,child);
      textval = qs_trim_string(xhtml->doc->r, textval);
      if (strlen(textval) == 0)
        continue;

      tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
      memset(tmp, 0, qs_get_node_size(doc,child)+1);

      tdst     = qs_alloc_zero_byte_string(r);
      memset(one_byte, 0, sizeof(one_byte));
      tdst_len = 0;

      for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
        char* out;
        int rtn = s_xhtml_search_emoji(xhtml, &textval[ii], &out);
        if (rtn != 0) {
          ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0, r,
                          "[%s][%d]", out, rtn);
          tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
          ii+=(rtn - 1);
          continue;
        }
        if (is_sjis_kanji(textval[ii])) {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          one_byte[0] = textval[ii+1];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          ii++;
        }
        else if (textval[ii] != '\r' && textval[ii] != '\n') {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        }
      }
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, tdst, NULL);
    }
  }
  return xhtml->out;
}

/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param xhtml   [i]   The pointer to the XHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_xhtml_search_emoji(xhtml_t* xhtml, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table_t* spec;
  int           len;

  spec = xhtml->spec;

  len = strlen(txt);
  r = xhtml->doc->r;

  if (spec == NULL)
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");

  for (ee = xhtml->conf->emoji;
       ee;
       ee = ee->next) {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (!ee->imode) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }

      if (strcasecmp(xhtml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      } 
      else
      if (strcasecmp(xhtml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeB);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeC);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeD);
        return strlen(ee->imode->string);
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }
      return 0;
    }
    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;
    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeA);
        return 2;
      }

      if (strcasecmp(xhtml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeA);
        return 2;
      } 
      else
      if (strcasecmp(xhtml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeB);
        return 2;
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeC);
        return 2;
      }
      else
      if (strcasecmp(xhtml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeD);
        return 2;
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<img localsrc=\"%s\">",
                        ee->ezweb->typeD);
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
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_html_tag(xhtml_t* xhtml, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Add XML Declare                                                          */
  /*--------------------------------------------------------------------------*/
  xhtml->out = apr_pstrcat(r->pool, 
    xhtml->out, 
    "<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n", 
    NULL);

  /*--------------------------------------------------------------------------*/
  /* Add DocType                                                              */
  /*--------------------------------------------------------------------------*/
  xhtml->out = apr_pstrcat(r->pool, 
          xhtml->out, 
          "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n",NULL);
  xhtml->out = apr_pstrcat(r->pool, 
          xhtml->out, 
          " \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n",NULL);

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<html", NULL);
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, 
          " xmlns=\"http://www.w3.org/1999/xhtml\"", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "lang") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " xml:lang=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "version") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"", 
                      NULL);
    }
  }

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\r\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_html_tag(xhtml_t* xhtml, Node* child) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</html>\r\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_meta_tag(xhtml_t* xhtml, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<meta", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "name") == 0) {
      xhtml->out = apr_pstrcat(r->pool,
                      xhtml->out,
                      " ", 
                      name, 
                      "=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "http-equiv") == 0) {
      xhtml->out = apr_pstrcat(r->pool,
                      xhtml->out,
                      " ", 
                      name,
                      "=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "content") == 0) {
      xhtml->out = apr_pstrcat(r->pool,
                      xhtml->out, 
                      " ", 
                      name, 
                      "=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }

  xhtml->out = apr_pstrcat(r->pool, 
                  xhtml->out, 
                  " />\r\n", 
                  NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_meta_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_head_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<head>\r\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_head_tag(xhtml_t* xhtml, Node* child) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</head>\r\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_title_tag(xhtml_t* xhtml, Node* node) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<title>", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_title_tag(xhtml_t* xhtml, Node* child) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</title>\r\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_base_tag(xhtml_t* xhtml, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<base", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "href") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, " />\r\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_base_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_body_tag(xhtml_t* xhtml, Node* node) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<body", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'b' || *name == 'B') && strcasecmp(name, "bgcolor") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " bgcolor=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " text=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'l' || *name == 'L') && strcasecmp(name, "link") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " link=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "alink") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'v' || *name == 'V') && strcasecmp(name, "vlink") == 0) {
      /* ignore */
    }
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\r\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_body_tag(xhtml_t* xhtml, Node* child) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</body>\r\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_a_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<a", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " id=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "href") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "accesskey") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " accesskey=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if (strcasecmp(name, "cti") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ijam") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "utn") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "telbook") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "kana") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "email") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ista") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "ilet") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "iswf") == 0) {
      /* ignore */
    }
    else
    if (strcasecmp(name, "irst") == 0) {
      /* ignore */
    }
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_a_tag(xhtml_t* xhtml, Node* child) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</a>", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_br_tag(xhtml_t* xhtml, Node* node) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<br />\r\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_br_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_font_tag(xhtml_t* xhtml, Node* node) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<font", NULL);
  /* Get Attributes */
  for (attr = qs_get_attr(doc,node);
       attr; attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "color") == 0) {
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " color=\"",value,"\"", NULL);
    }
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "size") == 0) {
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " size=\"",value,"\"", NULL);
    }
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_font_tag(xhtml_t* xhtml, Node* child) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</font>", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_form_tag(xhtml_t* xhtml, Node* node) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<form", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "action") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " action=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(name, "method") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " method=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'u' || *name == 'U') && strcasecmp(name, "utn") == 0) {
      /* ignore */
    }
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_form_tag(xhtml_t* xhtml, Node* child) 
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</form>", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_input_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc         = xhtml->doc;
  request_rec*  r           = doc->r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<input", NULL);

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

  if (type) {
    xhtml->out = apr_pstrcat(r->pool,
                    xhtml->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
  }
  if (size) {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
  }
  if (name) {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
  }
  if (value) {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
  }
  if (accesskey) {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
  }
  if (istyle) {
    char* fmt = qs_conv_istyle_to_format(r,istyle);
    if (max_length) {
      int ii;
      for (ii=0; ii<strlen(max_length); ii++) {
        if (max_length[ii] < '0' || max_length[ii] > '9') {
          max_length = apr_psprintf(r->pool, "0");
          break;
        }
      }

      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      apr_psprintf(r->pool, " FORMAT=\"%d%s\"", 
                      atoi(max_length), 
                      fmt), 
                      NULL);
    }
    else {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", 
                      "*", 
                      fmt, 
                      "\"", 
                      NULL);
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (type && istyle == NULL 
  && (*type == 'p' || *type == 'P') 
  && strcasecmp(type, "password") == 0) {
    if (max_length) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", 
                      max_length, 
                      "N", 
                      "\"", 
                      NULL);
    }
    else {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", "*", "N", "\"", NULL);
    }
  }
  if (checked) {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, " checked=\"checked\"", NULL);
  }

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, " />", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_input_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_center_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<center>", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_center_tag(xhtml_t* xhtml, Node* child) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</center>", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_hr_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<hr />", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_hr_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_img_tag(xhtml_t* xhtml, Node* node) 
{
  Doc*          doc = xhtml->doc;
  request_rec*  r   = doc->r;
  Attr* attr;

#ifndef IMG_NOT_CONVERT_FILENAME
  device_table_t* spec = xhtml->spec;
#endif

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<img", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "src") == 0) {
#ifdef IMG_NOT_CONVERT_FILENAME
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " src=\"",value,"\"", NULL);
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out,
                      " src=\"", chxj_img_conv(r,spec,value) , NULL);
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, "\"", NULL);
#endif
    }
    else
    if (strcasecmp(name, "align" ) == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " align=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "width" ) == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " width=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "height") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " height=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "hspace") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " hspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "vspace") == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " vspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) {
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " alt=\"",value,"\"", NULL);
    }
  }

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_img_tag(xhtml_t* xhtml, Node* child) 
{
  return xhtml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_select_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* size      = NULL;
  char* name      = NULL;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<select", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "size") == 0)
    {
      /* CHTML version 2.0 */
      size = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "name") == 0)
    {
      /* CHTML version 2.0 */
      name = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "multiple") == 0)
    {
      /* CHTML version 2.0 */
      /* Ignore */
    }
  }

  if (size != NULL)
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " size=\"",size,"\"", NULL);
  }
  if (name != NULL)
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " name=\"",name,"\"", NULL);
  }
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_select_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</select>\n", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_option_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* selected   = NULL;
  char* value      = NULL;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<option", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "selected") == 0)
    {
      /* CHTML version 2.0 */
      selected = apr_pstrdup(r->pool, val);
    }
    else
    if (strcasecmp(nm, "value") == 0)
    {
      /* CHTML version 2.0 */
      value = apr_pstrdup(r->pool, val);
    }
  }

  if (value != NULL)
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " value=\"",value,"\"", NULL);
  }
  else
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " value=\"\"", NULL);
  }

  if (selected != NULL)
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " selected=\"selected\"", NULL);
  }

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_option_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</option>\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_start_div_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* align   = NULL;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<div", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (strcasecmp(nm, "align") == 0)
    {
      /* CHTML version 3.2 */
      align = apr_pstrdup(r->pool, val);
    }
  }

  if (align != NULL)
  {
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, " align=\"", align, "\"", NULL);
  }

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
  return xhtml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_xhtml_1_0_end_div_tag(xhtml_t* xhtml, Node* child)
{
  Doc* doc = xhtml->doc;
  request_rec* r = doc->r;

  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</div>\n", NULL);

  return xhtml->out;
}

/**
 * It is a handler who processes the CHXJ:IF tag.
 *
 * @param xhtml  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CHXJ:IF tag node is specified.
 */
static void
s_xhtml_1_0_chxjif_tag(xhtml_t* xhtml, Node* node)
{
  Doc*         doc   = xhtml->doc;
  Node*        child;
  request_rec* r = xhtml->doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, child->otext, NULL);
    s_xhtml_1_0_chxjif_tag(xhtml, child);
  }
}
/*
 * vim:ts=2 et
 */
