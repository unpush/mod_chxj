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
#include "chxj_chtml10.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"

static char* s_chtml10_node_exchange    (chtml10_t* chtml, Node* node, int indent);
static char* s_chtml10_start_html_tag   (chtml10_t* chtml, Node* child);
static char* s_chtml10_end_html_tag     (chtml10_t* chtml, Node* child);
static char* s_chtml10_start_meta_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_meta_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_head_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_head_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_title_tag  (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_title_tag    (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_base_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_base_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_body_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_body_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_a_tag      (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_a_tag        (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_br_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_br_tag       (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_font_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_font_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_form_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_form_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_input_tag  (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_input_tag    (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_center_tag (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_center_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_hr_tag     (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_hr_tag       (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_img_tag    (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_img_tag      (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_select_tag (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_select_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_option_tag (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_option_tag   (chtml10_t* chtml, Node* node);
static char* s_chtml10_start_div_tag    (chtml10_t* chtml, Node* node);
static char* s_chtml10_end_div_tag      (chtml10_t* chtml, Node* node);
static void  s_init_chtml10(chtml10_t* chtml, Doc* doc, request_rec* r, device_table_t* spec);
static int   s_chtml10_search_emoji(chtml10_t* chtml, char* txt, char** rslt);
static void  s_chtml10_chxjif_tag(chtml10_t* chtml, Node* node);


/**
 * converts from CHTML5.0 to CHTML1.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_chtml10(
  request_rec* r,
  device_table_t* spec,
  const char* src,
  apr_size_t srclen,
  apr_size_t *dstlen)
{
  char*     dst = NULL;
  char*     ss;
  chtml10_t     chtml10;
  Doc       doc;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"i found qrcode xml");
    return dst;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml10(&chtml10, &doc, r, spec);
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
  chxj_dump_out("[src] CHTML -> CHTML1.0", ss, srclen);
#endif

  qs_parse_string(&doc,ss, strlen(ss));

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  dst = s_chtml10_node_exchange(&chtml10, qs_get_root(&doc), 0);
  qs_all_free(&doc,QX_LOGMARK);

  if (!dst)
    return apr_pstrdup(r->pool,ss);

  if (strlen(dst) == 0) {
    dst = apr_psprintf(r->pool, "\n");
  }
  *dstlen = strlen(dst);
#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> CHTML1.0", dst, *dstlen);
#endif

  return dst;
}

/**
 * The CHTML structure is initialized.
 *
 * @param chtml10 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_chtml10(chtml10_t* chtml10, Doc* doc, request_rec* r, device_table_t* spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(chtml10, 0, sizeof(chtml10_t));

  doc->r      = r;
  chtml10->doc  = doc;
  chtml10->spec = spec;
  chtml10->out  = qs_alloc_zero_byte_string(r);
  chtml10->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  chtml10->doc->parse_mode = PARSE_MODE_CHTML;
}

/**
 * It is main processing of conversion from CHTML to CHTML. 
 *
 * @param chtml10   [i/o] The pointer to the CHTML structure is specified. 
 * @param node    [i]   The pointer to a current node is specified. 
 * @param indent  [i]   The depth of the node processing it now is specified. 
 *
 * @return The character string after it converts it is returned. 
 */
static char*
s_chtml10_node_exchange(chtml10_t* chtml10, Node* node, int indent) 
{
  Node*         child;
  Doc*          doc   = chtml10->doc;
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
        s_chtml10_start_html_tag(chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_html_tag  (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HEAD>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "head") == 0) {
        s_chtml10_start_head_tag(chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_head_tag  (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "hr") == 0) {
        s_chtml10_start_hr_tag  (chtml10, child);
        s_chtml10_end_hr_tag    (chtml10, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(name, "meta") == 0) {
      s_chtml10_start_meta_tag(chtml10, child);
      s_chtml10_end_meta_tag  (chtml10, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "title") == 0) {
      s_chtml10_start_title_tag (chtml10, child);
      s_chtml10_node_exchange   (chtml10, child,indent+1);
      s_chtml10_end_title_tag   (chtml10, child);
    }
    else
    if (*name == 'b' || *name == 'B') {
      /*----------------------------------------------------------------------*/
      /* <BASE>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "base") == 0) {
        s_chtml10_start_base_tag(chtml10, child);
        s_chtml10_end_base_tag  (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BODY>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "body") == 0) {
        s_chtml10_start_body_tag(chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_body_tag  (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "br") == 0) {
        s_chtml10_start_br_tag  (chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_br_tag    (chtml10, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "a") == 0) {
      s_chtml10_start_a_tag   (chtml10, child);
      s_chtml10_node_exchange (chtml10, child,indent+1);
      s_chtml10_end_a_tag     (chtml10, child);
    }
    else
    if (*name == 'f' || *name == 'F') {
      /*----------------------------------------------------------------------*/
      /* <FONT>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "font") == 0) {
        s_chtml10_start_font_tag(chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_font_tag  (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <FORM>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "form") == 0) {
        s_chtml10_start_form_tag(chtml10, child);
        s_chtml10_node_exchange (chtml10, child,indent+1);
        s_chtml10_end_form_tag  (chtml10, child);
      }
    }
    else
    if (*name == 'i' || *name == 'I') {
      /*----------------------------------------------------------------------*/
      /* <INPUT>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "input") == 0) {
        s_chtml10_start_input_tag (chtml10, child);
        s_chtml10_node_exchange   (chtml10, child,indent+1);
        s_chtml10_end_input_tag   (chtml10, child);
      }
      /*----------------------------------------------------------------------*/
      /* <IMG>                                                                */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "img") == 0) {
        s_chtml10_start_img_tag (chtml10, child);
        s_chtml10_end_img_tag   (chtml10, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "select") == 0) {
      s_chtml10_start_select_tag(chtml10, child);
      s_chtml10_node_exchange   (chtml10, child, indent+1);
      s_chtml10_end_select_tag  (chtml10, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'o' || *name == 'O') && strcasecmp(name, "option") == 0) {
      s_chtml10_start_option_tag(chtml10, child);
      s_chtml10_node_exchange   (chtml10, child, indent+1);
      s_chtml10_end_option_tag  (chtml10, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'd' || *name == 'D') && strcasecmp(name, "div") == 0) {
      s_chtml10_start_div_tag (chtml10, child);
      s_chtml10_node_exchange (chtml10, child, indent+1);
      s_chtml10_end_div_tag   (chtml10, child);
    }
    else
    if (*name == 'c' || *name == 'C') {
      /*------------------------------------------------------------------------*/
      /* <CENTER>                                                               */
      /*------------------------------------------------------------------------*/
      if (strcasecmp(name, "center") == 0) {
        s_chtml10_start_center_tag(chtml10, child);
        s_chtml10_node_exchange   (chtml10, child,indent+1);
        s_chtml10_end_center_tag  (chtml10, child);
      }
      /*------------------------------------------------------------------------*/
      /* <CHXJ:IF>                                                              */
      /*------------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "chxj:if") == 0) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag found");

        if (chxj_chxjif_is_mine(chtml10->spec, doc, child)) {
          ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
          char* parse_attr = NULL;
          parse_attr = qs_get_parse_attr(doc, child, r);

          if (parse_attr != NULL && strcasecmp(parse_attr, "true") == 0)
            s_chtml10_node_exchange (chtml10, child, indent+1);
          else 
            s_chtml10_chxjif_tag(chtml10, child);
        }
      }
    }
    /*------------------------------------------------------------------------*/
    /* NORMAL TEXT                                                            */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text") == 0) {
      char*   textval;
      char*   tmp;
      char*   tdst;
      char    one_byte[2];
      int     ii;
      int     tdst_len;

      textval = qs_get_node_value(doc,child);
      textval = qs_trim_string(chtml10->doc->r, textval);
      if (strlen(textval) == 0)
        continue;

      tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
      memset(tmp, 0, qs_get_node_size(doc,child)+1);

      tdst     = qs_alloc_zero_byte_string(r);
      memset(one_byte, 0, sizeof(one_byte));
      tdst_len = 0;

      for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
        char* out;
        int rtn = s_chtml10_search_emoji(chtml10, &textval[ii], &out);
        if (rtn) {
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
      chtml10->out = apr_pstrcat(r->pool, chtml10->out, tdst, NULL);
    }
  }
  return chtml10->out;
}

/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml10   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_chtml10_search_emoji(chtml10_t* chtml10, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table_t* spec;
  int           len;

  spec = chtml10->spec;

  len = strlen(txt);
  r = chtml10->doc->r;

  if (!spec) {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");
  }

  for (ee = chtml10->conf->emoji;
       ee;
       ee = ee->next) {

    if (!ee->imode) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string
    &&  txt
    &&  strlen(ee->imode->string) > 0
    &&  *ee->imode->string == *txt
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (!spec || !spec->emoji_type) {
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
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_html_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc   = chtml10->doc;
  request_rec*  r     = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<html>\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the HTML tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_html_tag(chtml10_t* chtml10, Node* child) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</html>\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_meta_tag(chtml10_t* chtml10, Node* node) 
{
  /* ignore */
  return chtml10->out;
}

/**
 * It is a handler who processes the META tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_meta_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_head_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<head>\r\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the HEAD tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_head_tag(chtml10_t* chtml10, Node* child) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</head>\r\n", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_title_tag(chtml10_t* chtml10, Node* node) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<title>", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the TITLE tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_title_tag(chtml10_t* chtml10, Node* child) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</title>\r\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_base_tag(chtml10_t* chtml10, Node* node) 
{
  Attr*         attr;
  Doc*          doc   = chtml10->doc;
  request_rec*  r     = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<base", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "href") == 0) {
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, " >\r\n", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the BASE tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_base_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_body_tag(chtml10_t* chtml10, Node* node) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<body", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);

    if ((*name == 'b' || *name == 'B') && strcasecmp(name, "bgcolor") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'l' || *name == 'L') && strcasecmp(name, "link") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "alink") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'v' || *name == 'V') && strcasecmp(name, "vlink") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">\r\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the BODY tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_body_tag(chtml10_t* chtml10, Node* child) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</body>\r\n", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_a_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc   = chtml10->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<a", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " name=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "href") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "accesskey") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " accesskey=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "cti") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ijam") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'u' || *name == 'U') && strcasecmp(name, "utn") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "telbook") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'k' || *name == 'K') && strcasecmp(name, "kana") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'e' || *name == 'E') && strcasecmp(name, "email") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ista") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ilet") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "iswf") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "irst") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the A tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_a_tag(chtml10_t* chtml10, Node* child) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</a>", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_br_tag(chtml10_t* chtml10, Node* node) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<br>\r\n", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the BR tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_br_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_font_tag(chtml10_t* chtml10, Node* node) 
{
  /* Ignore */

  return chtml10->out;
}

/**
 * It is a handler who processes the FONT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_font_tag(chtml10_t* chtml10, Node* child) 
{
  /* ignore */
  return chtml10->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_form_tag(chtml10_t* chtml10, Node* node) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<form", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "action") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " action=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(name, "method") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " method=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'u' || *name == 'U') && strcasecmp(name, "utn") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the FORM tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_form_tag(chtml10_t* chtml10, Node* child) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</form>", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_input_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc         = chtml10->doc;
  request_rec*  r           = doc->r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<input", NULL);

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

  if (type)
    chtml10->out = apr_pstrcat(r->pool,
                    chtml10->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);

  if (size) 
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);

  if (name)
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);

  if (value)
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);

  if (accesskey)
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);

  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    /* ignore */
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length)
    chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, 
                      " maxlength=\"", 
                      max_length, 
                      "\"", 
                      NULL);

  if (checked)
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, " checked ", NULL);

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, " >", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_input_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_center_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<center>", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_center_tag(chtml10_t* chtml10, Node* child) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</center>", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_hr_tag(chtml10_t* chtml10, Node* node) 
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<hr ", NULL);
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "align") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool,
                        chtml10->out, 
                        " align=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "size") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool,
                        chtml10->out, 
                        " size=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 'w' || *name == 'W') && strcasecmp(name, "width") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool,
                        chtml10->out, 
                        " width=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "noshade") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool,
                        chtml10->out, 
                        " noshade ", NULL);
    }
    else
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "color") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, " >", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_hr_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_img_tag(chtml10_t* chtml10, Node* node) 
{
  Doc*          doc = chtml10->doc;
  request_rec*  r   = doc->r;
  Attr* attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table_t* spec = chtml10->spec;
#endif

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<img", NULL);
 

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 's' || *name == 'S') && strcasecmp(name, "src") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " src=\"",value,"\"", NULL);
#else
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " src=\"", 
                      chxj_img_conv(r, spec, value), NULL);
      chtml10->out = apr_pstrcat(r->pool, chtml10->out, "\"", NULL);
#endif
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "align" ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " align=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'w' || *name == 'W') && strcasecmp(name, "width" ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " width=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "height") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " height=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "hspace") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " hspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "vspace") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " vspace=\"",value,"\"", NULL);
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) 
    {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      chtml10->out = apr_pstrcat(r->pool, 
                      chtml10->out, " alt=\"",value,"\"", NULL);
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

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">", NULL);

  return chtml10->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_img_tag(chtml10_t* chtml10, Node* child) 
{
  return chtml10->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_select_tag(chtml10_t* chtml10, Node* child)
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* size      = NULL;
  char* name      = NULL;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<select", NULL);
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
      /* Ignore */
    }
  }

  if (size != NULL)
  {
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, " size=\"",size,"\"", NULL);
  }
  if (name != NULL)
  {
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, " name=\"",name,"\"", NULL);
  }
  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">\n", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_select_tag(chtml10_t* chtml10, Node* child)
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</select>\n", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_option_tag(chtml10_t* chtml10, Node* child)
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* selected   = NULL;
  char* value      = NULL;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<option", NULL);
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
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, " value=\"",value,"\"", NULL);
  }
  else
  {
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, " value=\"\"", NULL);
  }

  if (selected != NULL)
  {
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, " selected ", NULL);
  }

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_option_tag(chtml10_t* chtml10, Node* child)
{
  /* Don't close */
  return chtml10->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_start_div_tag(chtml10_t* chtml10, Node* child)
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;
  Attr* attr;

  char* align   = NULL;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "<div", NULL);
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
    chtml10->out = apr_pstrcat(r->pool, 
                    chtml10->out, " align=\"", align, "\"", NULL);
  }

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, ">", NULL);
  return chtml10->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param chtml10  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml10_end_div_tag(chtml10_t* chtml10, Node* child)
{
  Doc* doc = chtml10->doc;
  request_rec* r = doc->r;

  chtml10->out = apr_pstrcat(r->pool, chtml10->out, "</div>\n", NULL);

  return chtml10->out;
}

static void
s_chtml10_chxjif_tag(chtml10_t* chtml10, Node* node)
{
  Doc*         doc   = chtml10->doc;
  Node*        child;
  request_rec* r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    chtml10->out = apr_pstrcat(r->pool, chtml10->out, child->otext, NULL);
    s_chtml10_chxjif_tag(chtml10, child);
  }
}
/*
 * vim:ts=2 et
 */
