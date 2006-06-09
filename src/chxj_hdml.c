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
#include "chxj_hdml.h"
#include "chxj_tag_util.h"
#include "chxj_str_util.h"
#include <unistd.h>
#include "chxj_img_conv.h"
#include "chxj_dump.h"
#include "chxj_qr_code.h"

#define CHECK_BOX_PREFIX     "_chxj_c_"
#define RADIO_BUTTON_PREFIX  "_chxj_r_"
#define SUBMIT_BUTTON_PREFIX "_chxj_s_"

static void  s_init_hdml            (hdml_t* hdml, Doc* doc, request_rec* r, device_table_t* spec);
static char* s_hdml_node_exchange   (hdml_t* doc,  Node* node, int indent);
static char* s_hdml_start_html_tag  (hdml_t* doc,  Node* child);
static char* s_hdml_end_html_tag    (hdml_t* doc,  Node* child);
static char* s_hdml_start_meta_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_meta_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_head_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_head_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_title_tag (hdml_t* doc,  Node* node);
static char* s_hdml_end_title_tag   (hdml_t* doc,  Node* node);
static char* s_hdml_start_base_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_base_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_body_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_body_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_a_tag     (hdml_t* doc,  Node* node);
static char* s_hdml_end_a_tag       (hdml_t* doc,  Node* node);
static char* s_hdml_start_br_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_end_br_tag      (hdml_t* doc,  Node* node);
static char* s_hdml_start_font_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_font_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_form_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_end_form_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_start_input_tag (hdml_t* doc,  Node* node);
static char* s_hdml_end_input_tag   (hdml_t* doc,  Node* node);
static char* s_hdml_start_select_tag(hdml_t* doc,  Node* node);
static char* s_hdml_end_select_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_start_option_tag(hdml_t* doc,  Node* node);
static char* s_hdml_end_option_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_start_center_tag(hdml_t* doc,  Node* node);
static char* s_hdml_end_center_tag  (hdml_t* doc,  Node* node);
static char* s_hdml_start_hr_tag    (hdml_t* doc,  Node* node);
static char* s_hdml_end_hr_tag      (hdml_t* doc,  Node* node);
static char* s_hdml_start_img_tag   (hdml_t* doc,  Node* node);
static char* s_hdml_end_img_tag     (hdml_t* doc,  Node* node);
static char* s_hdml_start_div_tag   (hdml_t* doc,  Node* node);
static char* s_hdml_end_div_tag     (hdml_t* doc,  Node* node);
static char* s_get_form_no          (request_rec* r, hdml_t* hdml);
static void  s_hdml_count_radio_tag (hdml_t* hdml, Node* node) ;
static void  s_hdml_do_input_text_tag    (hdml_t* hdml, Node* tag);
static void  s_hdml_do_input_password_tag(hdml_t* hdml, Node* tag);
static void  s_hdml_do_input_submit_tag  (hdml_t* hdml, Node* tag);
static void  s_hdml_do_input_hidden_tag  (hdml_t* hdml, Node* tag);
static void  s_hdml_do_input_radio_tag   (hdml_t* hdml, Node* tag);
static void  s_hdml_do_input_checkbox_tag(hdml_t* hdml, Node* tag);
static void  s_hdml_tag_output_upper_half(hdml_t* hdml, Node* node);
static hdml_t* s_output_to_hdml_out       (hdml_t* hdml, char* s);
static hdml_t* s_output_to_hdml_card      (hdml_t* hdml, char* s);
static void  s_output_to_postdata       (hdml_t* hdml, char* s);
static void  s_output_to_init_vars      (hdml_t* hdml, char* s);
static int   s_hdml_search_emoji         (hdml_t* hdml, char* txt, char** rslt);
static void  s_hdml_chxjif_tag           (hdml_t* hdml, Node* node);



/**
 * converts from CHTML to hdml.
 *
 * @param r    [i] Requet_rec is appointed.
 * @param spec [i] The result of the device specification processing which 
 *                 was done in advance is appointed.
 * @param src  [i] The character string before the converting is appointed.
 * @return         The character string after the converting is returned.
 */
char*
chxj_exchange_hdml(request_rec* r, 
                   device_table_t* spec, 
                   const char* src, 
                   apr_size_t srclen, 
                   apr_size_t* dstlen)
{
  char*     dst = NULL;
  char*     buf = NULL;
  Doc       doc;
  hdml_t      hdml;

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
  /* initialize hdml structure                                                */
  /*--------------------------------------------------------------------------*/
  s_init_hdml(&hdml,&doc,r, spec);
  ap_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
  /*--------------------------------------------------------------------------*/
  /* DEBUG                                                                    */
  /*--------------------------------------------------------------------------*/
#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML->HDML", src, srclen);
#endif


  /*--------------------------------------------------------------------------*/
  /* It is examined whether there is a location header. It ends without doing */
  /* anything when is.                                                        */
  /*--------------------------------------------------------------------------*/
  buf = (char*)apr_table_get(r->headers_out, (const char*)"Location");
  if (buf) {
    /*------------------------------------------------------------------------*/
    /* The Location header generates tag in an initial HDML machine for the   */
    /* uncorrespon dence.                                                     */
    /*------------------------------------------------------------------------*/
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "Location is not null[Location:%s]", buf);
    s_output_to_hdml_out(&hdml, 
        "<HDML VERSION=3.0 MARKABLE=TRUE PUBLIC=TRUE>\n"
        "<NODISPLAY MARKABLE=TRUE PUBLIC=TRUE TITLE=\" \">\n"
        "<ACTION TYPE=ACCEPT TASK=GO DEST=\""
        );
    s_output_to_hdml_out(&hdml, buf);
    s_output_to_hdml_out(&hdml,
        "\">\n"
        "</NODISPLAY>\n"
        "</HDML>\n"
        );
    dst = apr_pstrdup(r->pool, hdml.out);
  }
  else {
    /*------------------------------------------------------------------------*/
    /* Here, the parsing of the received character string is done             */
    /*------------------------------------------------------------------------*/
    char *ss = apr_pstrdup(r->pool, src);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                    "input srclen=[%d]\n", srclen);
    qs_init_malloc(&doc); 
    qs_init_root_node(&doc);
    ss[srclen] = '\0';
    qs_parse_string(&doc, ss, srclen);

    /*------------------------------------------------------------------------*/
    /* The number of radiobuttons is counted.                                 */
    /*------------------------------------------------------------------------*/
    s_hdml_count_radio_tag(&hdml, qs_get_root(&doc));

    dst = s_hdml_node_exchange(&hdml, qs_get_root(&doc), 0);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "tmp=[%s]", dst);
    qs_all_free(&doc,QX_LOGMARK);
  }

  /*--------------------------------------------------------------------------*/
  /* DEBUG                                                                    */
  /*--------------------------------------------------------------------------*/
#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML->HDML", hdml.out, hdml.out_len);
#endif

  /*--------------------------------------------------------------------------*/
  /* When there is no processing result, former character string is copied    */
  /* and it returns it.                                                       */
  /*--------------------------------------------------------------------------*/
  if (!dst) {
    *dstlen = srclen;
    return apr_pstrdup(r->pool,src);
  }

  *dstlen = hdml.out_len;

  /*--------------------------------------------------------------------------*/
  /* Null is set at the end of the character string to make sure.             */
  /*--------------------------------------------------------------------------*/
  dst[hdml.out_len] = 0;

  return dst;
}

/**
 * The HDML structure is initialized. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure that wants to be 
 *                   initialized is specified. 
 * @param doc  [i]   The Doc structure that should be set to the initialized 
 *                   HDML structure is specified. 
 * @param r    [i]   To use POOL, the pointer to request_rec is specified. 
 * @param spec [i]   The pointer to the device_table
 */
static void 
s_init_hdml(hdml_t* hdml, Doc* doc, request_rec* r, device_table_t* spec)
{
  int     ii;
  int     jj;

  /*--------------------------------------------------------------------------*/
  /* init hdml structure value                                                */
  /*--------------------------------------------------------------------------*/
  memset(hdml, 0, sizeof(hdml_t));
  hdml->doc      = doc;
  hdml->card     = qs_alloc_zero_byte_string(r);
  hdml->spec     = spec;
  hdml->conf     = ap_get_module_config(r->per_dir_config, &chxj_module);
  hdml->doc->parse_mode = PARSE_MODE_CHTML;

  for (ii=0; ii<MAX_FORM_COUNT; ii++) {
    hdml->var_cnt[ii]     = 0;
    hdml->postdata[ii]    = qs_alloc_zero_byte_string(r);
  }

  for (ii=0; ii<MAX_RADIO_COUNT; ii++) {
    for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) 
      hdml->radio_value_list[ii][jj] = NULL;

    hdml->radio_name_list[ii]     = NULL;
    hdml->radio_out_cnt[ii]       = 0;
    hdml->radio_checked_value[ii] = NULL;
  }

  for (ii=0; ii<MAX_SUBMIT_BUTTON_COUNT; ii++) 
    hdml->submit_button[ii] = NULL;

  hdml->init_vars      = qs_alloc_zero_byte_string(r);

  doc->r               = r;

  hdml->form_cnt = apr_time_now();
  hdml->out = qs_alloc_zero_byte_string(r);
}

/**
 * It is main processing converted from CHTML to HDML. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The route node is specified. 
 * @param indent [i]   Depth from the route node is specified. At present, 
 *                     other use doesn't exist. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_node_exchange(hdml_t* hdml, Node* node,  int indent) 
{
  Node*         child;
  Doc*          doc   = hdml->doc; 
  request_rec*  r     = doc->r;

  for (child = qs_get_child_node(doc,node); 
       child ; 
       child = qs_get_next_node(doc,child)) {
    char* name = qs_get_node_name(doc,child);

    /*----------------------------------------------------------------------*/
    /* When </ HDML > tag has already been output, it doesn't process it.   */
    /*----------------------------------------------------------------------*/
    if (hdml->hdml_end_flag == 1) 
      continue;

    if (*name == 'h' || *name == 'H') {
      /*----------------------------------------------------------------------*/
      /* <HTML>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "html") == 0) {
        hdml->out = s_hdml_start_html_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_html_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HEAD>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "head") == 0) {
        hdml->out = s_hdml_start_head_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_head_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <HR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "hr") == 0) {
        hdml->out = s_hdml_start_hr_tag(hdml, child);
        hdml->out = s_hdml_end_hr_tag(hdml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <META>                                                                 */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(name, "meta") == 0) {
      hdml->out = s_hdml_start_meta_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_end_meta_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <TITLE>                                                                */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "title") == 0) {
      hdml->out = s_hdml_start_title_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
      hdml->out = s_hdml_end_title_tag(hdml, child);
    }
    else
    if (*name == 'b' || *name == 'B') {
      /*----------------------------------------------------------------------*/
      /* <BASE>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "base") == 0) {
        hdml->out = s_hdml_start_base_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_end_base_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BODY>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "body") == 0) {
        hdml->out = s_hdml_start_body_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_body_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <BR>                                                                 */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "br") == 0) {
        hdml->out = s_hdml_start_br_tag(hdml, child);
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_br_tag(hdml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <A>                                                                    */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "a") == 0) {
      hdml->out = s_hdml_start_a_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
      hdml->out = s_hdml_end_a_tag(hdml, child);
    }
    else
    if (*name == 'f' || *name == 'F') {
      /*----------------------------------------------------------------------*/
      /* <FONT>                                                               */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "font") == 0) {
        hdml->out = s_hdml_start_font_tag(hdml, child);
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_font_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <FORM>                                                               */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "form") == 0) {
        hdml->out = s_hdml_start_form_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_form_tag(hdml, child);
      }
    }
    else
    if (*name == 'i' || *name == 'I') {
      /*----------------------------------------------------------------------*/
      /* <INPUT>                                                              */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "input") == 0) {
        hdml->out = s_hdml_start_input_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_input_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <IMG>                                                                */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "img") == 0) {
        hdml->out = s_hdml_start_img_tag(hdml, child);
        hdml->hdml_br_flag = 0;
        hdml->out = s_hdml_end_img_tag  (hdml, child);
      }
    }
    /*------------------------------------------------------------------------*/
    /* <SELECT>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "select") == 0) {
      hdml->out = s_hdml_start_select_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
      hdml->out = s_hdml_end_select_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <OPTION>                                                               */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'o' || *name == 'O') && strcasecmp(name, "option") == 0) {
      hdml->out = s_hdml_start_option_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
      hdml->out = s_hdml_end_option_tag(hdml, child);
    }
    /*------------------------------------------------------------------------*/
    /* <DIV>                                                                  */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 'd' || *name == 'D') && strcasecmp(name, "div") == 0) {
      hdml->out = s_hdml_start_div_tag(hdml, child);
      hdml->hdml_br_flag = 0;
      hdml->out = s_hdml_node_exchange(hdml, child, indent+1);
      hdml->out = s_hdml_end_div_tag(hdml, child);
    }
    else
    if (*name == 'c' || *name == 'C') {
      /*----------------------------------------------------------------------*/
      /* <CENTER>                                                             */
      /*----------------------------------------------------------------------*/
      if (strcasecmp(name, "center") == 0) {
        hdml->out = s_hdml_start_center_tag(hdml, child);
        hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
        hdml->out = s_hdml_end_center_tag(hdml, child);
      }
      /*----------------------------------------------------------------------*/
      /* <CHXJ:IF>                                                            */
      /*----------------------------------------------------------------------*/
      else
      if (strcasecmp(name, "chxj:if") == 0) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag found");
        if (chxj_chxjif_is_mine(hdml->spec, doc, child)) {
          ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "chxj:if tag is mine");
          char* parse_attr = NULL;
  
          parse_attr = qs_get_parse_attr(doc, child, r);
          if (parse_attr && strcasecmp(parse_attr, "true") == 0) 
            hdml->out = s_hdml_node_exchange(hdml, child,indent+1);
          else
            s_hdml_chxjif_tag(hdml, child);
        }
      }
    }
    /*------------------------------------------------------------------------*/
    /* NORMAL TEXT                                                            */
    /*------------------------------------------------------------------------*/
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text") == 0) {
      char* textval;
      char* tmp;
      char* tdst;
      char one_byte[2];
      int ii;
      int tdst_len = 0;

      textval = qs_get_node_value(doc,child);
      textval = qs_trim_string(r, textval);
      if (strlen(textval) == 0)
        continue;

      if (hdml->option_flag == 1) 
        continue;

      tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
      memset(tmp, 0, qs_get_node_size(doc,child)+1);
      tdst = apr_palloc(r->pool, 1);
      tdst[0] = '\0';
      one_byte[0] = '\0';
      one_byte[1] = '\0';

      for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
        char* out;
        int rtn = s_hdml_search_emoji(hdml, &textval[ii], &out);
        if (rtn) {
          tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
          ii += (rtn-1);
          continue;
        }
        if (is_sjis_kanji(textval[ii])) {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          one_byte[0] = textval[ii+1];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
          ii++;
        }
        else 
        if (textval[ii] != '\r' && textval[ii] != '\n') {
          one_byte[0] = textval[ii+0];
          tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        }
      }
      memcpy(tmp, textval, strlen(textval)-1);

      s_hdml_tag_output_upper_half(hdml, node);
      s_output_to_hdml_out(hdml, tdst);
      hdml->hdml_br_flag = 0;
    }
  }
  hdml->out[hdml->out_len] = 0;

  return hdml->out;
}

static int
s_hdml_search_emoji(hdml_t* hdml, char* txt, char** rslt)
{
  emoji_t* ee;
  request_rec* r;
  device_table_t* spec;
  int len;

  spec = hdml->spec;

  len = strlen(txt);
  r = hdml->doc->r;

  if (!spec)
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG,0,r, "spec is NULL");

  for (ee = hdml->conf->emoji;
       ee;
       ee = ee->next) {
    unsigned char hex1byte;
    unsigned char hex2byte;

    if (!ee->imode) {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      "emoji->imode is NULL");
      continue;
    }

    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;

    if (ee->imode->string
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }

      if (strcasecmp(hdml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      } 
      else
      if (strcasecmp(hdml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeB);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeC);
        return strlen(ee->imode->string);
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeD);
        return strlen(ee->imode->string);
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return strlen(ee->imode->string);
      }
      return 0;
    }
    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return 2;
      }

      if (strcasecmp(hdml->spec->emoji_type, "a") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
        return 2;
      } 
      else
      if (strcasecmp(hdml->spec->emoji_type, "b") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeB);
        return 2;
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "c") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeC);
        return 2;
      }
      else
      if (strcasecmp(hdml->spec->emoji_type, "d") == 0) {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeD);
        return 2;
      }
      else {
        *rslt = apr_psprintf(r->pool,
                        "<IMG ICON=%s>",
                        ee->ezweb->typeA);
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
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_html_tag(hdml_t* hdml, Node* node) 
{
  s_output_to_hdml_out(hdml, 
    "<HDML VERSION=3.0 TTL=0 MARKABLE=TRUE>\n"
    "<NODISPLAY NAME=D0>\n"
    "<ACTION TYPE=ACCEPT TASK=GOSUB DEST=#D1 NEXT=#D2 CLEAR=TRUE>\n"
    "</NODISPLAY>\n"
    );

  hdml->card_cnt = 2;

  return hdml->out;
}

/**
 * It is a handler who processes the HTML tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_html_tag(hdml_t* hdml, Node* child) 
{
  s_output_to_hdml_card(hdml, 
                  "<NODISPLAY NAME=D1>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN VARS=\""
                  );

  if (strlen(hdml->init_vars)) 
    s_output_to_hdml_card(hdml, hdml->init_vars   );
  else 
    s_output_to_hdml_card(hdml, "_chxj_dmy="            );

  s_output_to_hdml_card(hdml,   
                  "\" CLEAR=TRUE>\n"
                  "</NODISPLAY>\n"
                  );

  s_output_to_hdml_out(hdml, hdml->card );
  s_output_to_hdml_out(hdml, "</HDML>\n");

  hdml->hdml_end_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the META tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_meta_tag(hdml_t* hdml, Node* node) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the META tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_meta_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_head_tag(hdml_t* hdml, Node* node) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_head_tag(hdml_t* hdml, Node* child) 
{
  /* ignore */
  return hdml->out;
}

/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_title_tag(hdml_t* hdml, Node* node) 
{
  s_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"");
  hdml->found_title = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_title_tag(hdml_t* hdml, Node* child) 
{
  s_output_to_hdml_out(hdml, "\">\n");

  return hdml->out;
}

/**
 * It is a handler who processes the BASE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_base_tag(hdml_t* hdml, Node* node) 
{
  return hdml->out;
}


/**
 * It is a handler who processes the BASE tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_base_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the BODY tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_body_tag(hdml_t* hdml, Node* node) 
{
  Doc*         doc  = hdml->doc;
  Attr*        attr;

  if (hdml->found_title == 0)
    s_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"NO TITLE\">\n");

  s_output_to_hdml_out(hdml, "<ACTION TYPE=ACCEPT TASK=NOOP LABEL=\" \"");

  /*--------------------------------*/
  /* Get Attributes                 */
  /*--------------------------------*/
  for (attr = qs_get_attr(doc,node); 
    attr; 
    attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);

    if ((*name == 'b' || *name == 'B') && strcasecmp(name, "bgcolor")     == 0) {
      /* ignore */
    }
    else 
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text")   == 0) {
      /* ignore */
    }
    else 
    if ((*name == 'l' || *name == 'L') && strcasecmp(name, "link")   == 0) {
      /* ignore */
    }
    else 
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "alink")  == 0) {
      /* ignore */
    }
    else 
    if ((*name == 'v' || *name == 'V') && strcasecmp(name, "vlink")  == 0) {
      /* ignore */
    }
  }

  s_output_to_hdml_out(hdml, ">\n");

  return hdml->out;
}

/**
 * It is a handler who processes the BODY tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_body_tag(hdml_t* hdml, Node* child) 
{
  s_output_to_hdml_out(hdml, "\n</DISPLAY>\n");

  return hdml->out;
}

/**
 * It is a handler who processes the A tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_a_tag(hdml_t* hdml, Node* node) 
{
  Doc*         doc  = hdml->doc;
  Attr*        attr;

  s_hdml_tag_output_upper_half(hdml,node);

  s_output_to_hdml_out(hdml, "<A");


  /*------------------------------------*/
  /* Get Attributes                     */
  /*------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      /* IGNORE */
    }
    else 
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "href") == 0) {
      if ((*value == 'm' || *value == 'M') && strncasecmp(value, "mailto:", 7) == 0) {
        s_output_to_hdml_out(hdml, " TASK=GO DEST=\""     );
        s_output_to_hdml_out(hdml, value                  );
        s_output_to_hdml_out(hdml, "\" "                  );
      }
      else 
      if ((*value == 't' || *value == 'T') && strncasecmp(value, "tel:", 4) == 0) {

        s_output_to_hdml_out(hdml,  " TASK=CALL NUMBER=\"");
        s_output_to_hdml_out(hdml, &value[4]              );
        s_output_to_hdml_out(hdml, "\" "                  );
      }
      else {
        s_output_to_hdml_out(hdml, " TASK=GO DEST=\""     );
        s_output_to_hdml_out(hdml, value                  );
        s_output_to_hdml_out(hdml, "\""                   );
      }
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "accesskey") == 0) {
      if (strcasecmp(value, "0") != 0) {
        s_output_to_hdml_out(hdml, " ACCESSKEY="          );
        s_output_to_hdml_out(hdml, value                  );
        s_output_to_hdml_out(hdml, ""                     );
      }
    }
    else
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "cti") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ijam") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'u' || *name == 'U') && strcasecmp(name, "utn") == 0) {
      /* ignore */
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "telbook") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'k' || *name == 'K') && strcasecmp(name, "kana") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'e' || *name == 'E') && strcasecmp(name, "email") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ista") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "ilet") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "iswf") == 0) {
      /* ignore */
    }
    else
    if ((*name == 'i' || *name == 'I') && strcasecmp(name, "irst") == 0) {
      /* ignore */
    }
  }
  s_output_to_hdml_out(hdml, ">"  );

  hdml->hdml_a_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the A tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_a_tag(hdml_t* hdml, Node* child) 
{
  s_output_to_hdml_out(hdml, "</A>\n");

  hdml->hdml_a_flag = 0;

  return hdml->out;
}

/**
 * It is a handler who processes the BR tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_br_tag(hdml_t* hdml, Node* node) 
{
  if (hdml->in_center > 0) 
    hdml->in_center = 0;

  if (hdml->div_in_center > 0) 
    hdml->div_in_center = 0;

  s_output_to_hdml_out(hdml, "<BR>\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the BR tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_br_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FONT tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_font_tag(hdml_t* hdml, Node* node) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FONT tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_end_font_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the FORM tag. 
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified. 
 * @return The conversion result is returned. 
 */
static char*
s_hdml_start_form_tag(hdml_t* hdml, Node* node) 
{
  request_rec* r = hdml->doc->r;
  Attr* attr;
  Doc* doc = hdml->doc;
  char* act = NULL;

  hdml->form_tmp = apr_psprintf(r->pool,
                  "<NODISPLAY NAME=F%d>\n",
                  hdml->pure_form_cnt);
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "<ACTION TYPE=ACCEPT TASK=GO METHOD=POST DEST=\"",NULL);
  /* Get Attributes */
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "action") == 0) {
      act = apr_psprintf(r->pool, "%s", value);
      break;
    }
  }
  if (act) {
    hdml->form_tmp = apr_pstrcat(r->pool,
                             hdml->form_tmp,
                             act,
                             NULL);
  }
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "\" ",NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                           hdml->form_tmp,
                           "POSTDATA=\"", NULL);

  return hdml->out;
}

/**
 * handler of the form end tag. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param child [i]  unused.
 * @return The HDML output result after it edits it is returned. 
 */
static char*
s_hdml_end_form_tag(hdml_t* hdml, Node* child) 
{
  request_rec* r = hdml->doc->r;

  s_output_to_postdata(hdml, "_chxj_dmy=");

  hdml->form_tmp = apr_pstrcat(r->pool, 
                             hdml->form_tmp,
                             hdml->postdata[hdml->pure_form_cnt],
                             NULL);

  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "\" CLEAR=TRUE >\n", 
                               NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "</NODISPLAY>\n",
                               NULL);

  s_output_to_hdml_card(hdml, hdml->form_tmp);

  hdml->form_tmp = NULL;
  hdml->pure_form_cnt++;

  return hdml->out;
}

/**
 * It is a handler that takes charge of the processing of the input tag. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param node [i]   The tag node to be processed is specified. 
 */
static char*
s_hdml_start_input_tag(hdml_t* hdml, Node* node) 
{
  Doc* doc = hdml->doc;
  Attr* attr;

  /*--------------------------------------------------------------------------*/
  /* The attribute of the input tag is acquired.                              */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "type") == 0) 
    {
      if (strcasecmp(value, "text") == 0)
      {
        /*--------------------------------------------------------------------*/
        /* "input type ='text'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_text_tag(hdml, node);
      }
      if (strcasecmp(value, "password") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='password'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_password_tag(hdml, node);
      }
      else
      if (strcasecmp(value, "submit") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='submit'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_submit_tag(hdml, node);
      }
      else 
      if (strcasecmp(value, "hidden") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='hidden'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_hidden_tag(hdml, node);
      }
      else
      if (strcasecmp(value, "radio") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='radio'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_radio_tag(hdml, node);
      }
      else 
      if (strcasecmp(value, "checkbox") == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* "input type='checkbox'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_checkbox_tag(hdml, node);
      }
    }
    else 
    if (strcasecmp(name, "name")      == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "value")     == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "size")      == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "maxlength") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "checked")   == 0) 
    {
      /* ignore */
    }
    else 
    if (strcasecmp(name, "accesskey") == 0) 
    {
      /* ignore */
    }
    else
    if (strcasecmp(name, "istyle")    == 0) 
    {
      /* ignore */
    }
  }
  return hdml->out;
}

/**
 * The substitution processing of tag "input type = text" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=text is specified. 
 */
static void
s_hdml_do_input_text_tag(hdml_t* hdml, Node* tag)
{
  Doc*          doc   = hdml->doc;
  request_rec*  r     = doc->r;
  char*         mlen;
  char*         val;
  char*         is;
  char*         nm;
  char*         fmt;
  int           ii;
  s_hdml_tag_output_upper_half(hdml, tag);

  hdml->card_cnt++;
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool,
                          "<A TASK=GOSUB LABEL=\x93\xfc\x97\xcd DEST=#D%d "
                          "VARS=\"V=$%s%02d\" RECEIVE=%s%02d>",
                          hdml->card_cnt,
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt],
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]
                  ));

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "[$%s%02d]</A>\n"  , 
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  /*--------------------------------------------------------------------------*/
  /* ENTRY CARD is output here.                                               */
  /*--------------------------------------------------------------------------*/
  s_output_to_hdml_card(hdml, "<ENTRY NAME="                               );
  s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "D%d ", hdml->card_cnt));
  s_output_to_hdml_card(hdml, " KEY=V DEFAULT=$V "                         );

  mlen = NULL;
  is   = NULL;
  val  = NULL;
  fmt  = NULL;
  nm = qs_get_name_attr(doc, tag, r);
  if (nm == NULL)
  {
    nm = qs_alloc_zero_byte_string(r);
  }

  s_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, 
                          "%s=$%s%02d", 
                          nm,
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r);
  is   = qs_get_istyle_attr     (doc, tag, r);
  val  = qs_get_value_attr      (doc, tag, r);

  fmt  = qs_conv_istyle_to_format(r, is);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "qs_conv_istyle_to_format end");
        
  if (fmt != NULL) 
  {
    if (mlen != NULL) 
    {
      for (ii=0; ii<strlen(mlen); ii++)
      {
        if (mlen[ii] < '0' || mlen[ii] > '9')
        {
          mlen = apr_psprintf(r->pool, "0");
          break;
        }
      }
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=%d%s", atoi(mlen), fmt));
    }
    else 
    {
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );
    }
  }

  s_output_to_hdml_card(hdml, 
                  " MARKABLE=FALSE>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\n"
                  "</ENTRY>\n");

  if (val != NULL) 
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=%s", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt],
                        ap_escape_uri(r->pool,val)));
  }
  else 
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt]));
  }
  hdml->var_cnt[hdml->pure_form_cnt]++;
}

/**
 * The substitution processing of tag "input type = password" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=password is specified. 
 */
static void
s_hdml_do_input_password_tag(hdml_t* hdml, Node* tag)
{
  Doc*            doc = hdml->doc;
  request_rec*    r   = doc->r;
  char*           mlen;
  char*           val;
  char*           is;
  char*           nm;
  char*           fmt;

  s_hdml_tag_output_upper_half(hdml, tag);

  hdml->card_cnt++;
  s_output_to_hdml_out(hdml, "<A TASK=GOSUB LABEL=\"\x93\xfc\x97\xcd\" DEST=");
  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "#D%d ", hdml->card_cnt));
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "VARS=\"V=$%s%02d\" ", 
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "RECEIVE=%s%02d>"  , 
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "[$%s%02d]</A>\n"  , 
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  s_output_to_hdml_card(hdml, "<ENTRY NAME="                               );
  s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "D%d ", hdml->card_cnt));
  s_output_to_hdml_card(hdml, " KEY=V DEFAULT=$V "                         );

  mlen = NULL;
  is   = NULL;
  val  = NULL;
  fmt  = NULL;

  nm = qs_get_name_attr(doc, tag, r);
  if (nm == NULL)
  {
    nm = qs_alloc_zero_byte_string(r);
  }
  s_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, 
                          "%s=$%s%02d", 
                          nm,
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r);
  val  = qs_get_value_attr      (doc, tag, r);
  /*--------------------------------------------------------------------------*/
  /* Default is a figure input.                                               */
  /*--------------------------------------------------------------------------*/
  fmt = apr_psprintf(r->pool, "N");
  if (mlen != NULL) 
  {
    if (chxj_chk_numeric(mlen) != 0)
    {
      mlen = apr_psprintf(r->pool, "0");
    }
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=%d%s", chxj_atoi(mlen), fmt));
  }
  else 
  {
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );
  }
  s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " NOECHO=TRUE "));

  s_output_to_hdml_card(hdml, 
                  " MARKABLE=FALSE>\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\n"
                  "</ENTRY>\n");

  if (val != NULL) 
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=%s", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt], 
                        ap_escape_uri(r->pool,val)));
  }
  else 
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt]));
  }
  hdml->var_cnt[hdml->pure_form_cnt]++;
}

/**
 * The substitution processing of tag "input type = submit" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=submit is specified. 
 */
static void
s_hdml_do_input_submit_tag(hdml_t* hdml, Node* tag)
{
  Doc*          doc = hdml->doc;
  request_rec*  r   = doc->r;
  char*         nm  = NULL;
  char*         val = NULL;

  s_hdml_tag_output_upper_half(hdml, tag);

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GO LABEL=OK DEST=#F%d ",
                          hdml->pure_form_cnt));

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  if (nm != NULL && val != NULL) 
  {
    s_output_to_hdml_out(hdml, 
                    apr_psprintf(r->pool, "VARS=\"%s=%s\" ", 
                            nm, 
                            ap_escape_uri(r->pool,val)));
    if (strstr(hdml->postdata[hdml->pure_form_cnt], nm) == NULL) 
    {
      s_output_to_postdata(hdml, 
                      apr_psprintf(r->pool,"%s%s=$%s", 
                              SUBMIT_BUTTON_PREFIX, nm, nm));
    }
  }
  s_output_to_hdml_out(hdml, ">"         );
  s_output_to_hdml_out(hdml, val);
  s_output_to_hdml_out(hdml, "</A>\n"    );
}

/**
 * The substitution processing of tag "input type = hidden" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=hidden is specified. 
 */
static void
s_hdml_do_input_hidden_tag(hdml_t* hdml, Node* tag)
{
  Doc*          doc = hdml->doc;
  request_rec*  r   = doc->r;
  char*         nm  = NULL;
  char*         val = NULL;

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  if (nm != NULL && val != NULL) 
  {
    s_output_to_postdata(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            ap_escape_uri(r->pool, val)));
  }
}

/**
 * The substitution processing of tag "input type = radio" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=radio is specified. 
 */
static void
s_hdml_do_input_radio_tag(hdml_t* hdml, Node* tag)
{
  Doc*          doc       = hdml->doc;
  request_rec*  r         = doc->r;
  char*         nm        = NULL;
  char*         val       = NULL;
  int           ii;
  int           jj;
  int           kk;
  int           r_cnt;

  s_hdml_tag_output_upper_half(hdml, tag);

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r);
  val = qs_get_value_attr (doc, tag, r);

  /*--------------------------------------------------------------------------*/
  /* The same name is searched out from the list made beforehand.             */
  /*--------------------------------------------------------------------------*/
  for (ii=0; ii<MAX_RADIO_COUNT; ii++) 
  {
    if (hdml->radio_name_list[ii] == NULL) 
    {
      /* @todo Oops..  */
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, 
                      "%s:%d Oops... radio list is null", 
                      APLOG_MARK);
      /*----------------------------------------------------------------------*/
      /* Processing is ended because it doesn't happen off the fly.           */
      /*----------------------------------------------------------------------*/
      return;
    }

    if (strcasecmp(hdml->radio_name_list[ii], nm) == 0) 
    {
      break;
    }
  }
  if (ii == MAX_RADIO_COUNT) 
  {
    /* @todo Oops.. */
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, 
                    "%s:%d Oops... The same name was not in the list. ", 
                    APLOG_MARK);
    /*------------------------------------------------------------------------*/
    /* Processing is ended because it doesn't happen off the fly.             */
    /*------------------------------------------------------------------------*/
    return;
  }

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GOSUB "
                          "LABEL=\"\x93\xfc\x97\xcd\" "
                          "DEST=#R%d VARS=\"VAL=%s\" "
                          "RECEIVE=\"%s;", 
                          ii, 
                          val, 
                          nm));

  if (hdml->radio_out_cnt[ii] == 0) 
  {
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool,
                            "<NODISPLAY NAME=R%d>\n",
                            ii));
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<ACTION "
                            "TYPE=ACCEPT TASK=RETURN RETVALS=\"$VAL;"));
  }

  kk = hdml->radio_out_cnt[ii];
  for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) 
  {
    if (hdml->radio_value_list[ii][jj] == NULL) 
    {
      break;
    }
  }
  r_cnt = jj;

  for (jj=0; jj<r_cnt; jj++) 
  {
    if (hdml->radio_value_list[ii][jj] == NULL)
    {
      break;
    }
    if (jj != 0) 
    {
      s_output_to_hdml_out(hdml, apr_psprintf(r->pool, ";"));
      if (hdml->radio_out_cnt[ii] == 0) 
      {
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, ";"));
      }
    }

    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "%s_%02d", nm, kk));
    if (hdml->radio_out_cnt[ii] == 0) 
    {
      if (jj == 0) 
      {
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "X"));
      }
      else 
      {
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "_"));
      }
    }
    kk++;
    if (kk >= r_cnt) 
    {
      kk=0;
    }
  }
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "\" >$%s_%02d</A>", 
                          nm, 
                          hdml->radio_out_cnt[ii]));
  if (hdml->radio_out_cnt[ii] == 0) 
  {
    s_output_to_hdml_card(hdml, "\">\n"         );
    s_output_to_hdml_card(hdml, "</NODISPLAY>\n");

    s_output_to_postdata(hdml,  apr_psprintf(r->pool, "%s%s=$%s", RADIO_BUTTON_PREFIX, nm, nm));

    for (jj=0; jj<r_cnt; jj++) 
    {
      if (hdml->radio_value_list[ii][jj] != NULL 
      &&  hdml->radio_checked_value[ii] != NULL) 
      {
        if (strcasecmp(hdml->radio_value_list[ii][jj], 
                       hdml->radio_checked_value[ii]) == 0)
        {
          s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=X", 
                                  nm, 
                                  jj));
        }
        else 
        {
          s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=_", 
                                  nm, 
                                  jj));
        }
      }
      else 
      {
        s_output_to_init_vars(hdml, 
                        apr_psprintf(r->pool, 
                                "%s_%02d=_", 
                                nm, 
                                jj));
      }
    }

    if (hdml->radio_checked_value[ii] != NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "radio button is checked. checked value is [%s]", 
                    hdml->radio_checked_value[ii]);
      s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            hdml->radio_checked_value[ii]));
    }
    else
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                    "radio button is not checked. checked value is []");
      s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=", 
                            nm));
    }
  }
  hdml->radio_out_cnt[ii]++;
}

/**
 * The substitution processing of tag "input type = checkbox" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=checkbox is specified. 
 */
static void
s_hdml_do_input_checkbox_tag(hdml_t* hdml, Node* tag)
{
  Doc*          doc       = hdml->doc;
  request_rec*  r         = doc->r;
  char*         nm        = NULL;
  char*         val       = NULL;
  int           chk;

  /*--------------------------------------------------------------------------*/
  /* It is posted to the one without the checked attribute.                   */
  /* However, they were able to be removed with INPUT FILTER.                 */
  /*--------------------------------------------------------------------------*/
  if (hdml->has_checkbox == 0) 
  {
    hdml->has_checkbox++;
    s_output_to_hdml_card(hdml, 
                    "<NODISPLAY NAME=\"_chk\">\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_uchk;$V;X\" >\n"
                    "</NODISPLAY>\n"
                    "<NODISPLAY NAME=\"_uchk\">\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_chk;;_\" >\n"
                    "</NODISPLAY>\n"
                    );
  }
        
  /*--------------------------------------------------------------------------*/
  /* It is examined whether it is CHECKED.                                    */
  /*--------------------------------------------------------------------------*/
  chk = qs_is_checked_checkbox_attr(doc, tag, r);

  /*--------------------------------------------------------------------------*/
  /* The value of the name attribute and the value attribute is acquired      */
  /* respectively.                                                            */
  /*--------------------------------------------------------------------------*/
  val = qs_get_value_attr(doc, tag, r);
  nm  = qs_get_name_attr(doc, tag, r);

  if (val == NULL) 
  {
    val    = qs_alloc_zero_byte_string(r);
  }
  if (nm == NULL)
  {
    nm   = qs_alloc_zero_byte_string(r);
  }
  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                "<A TASK=GOSUB LABEL=\"a\xaf\xb8\" "
                                   "DEST=\"#$%s%02d\" "
                                   "VARS=\"V=%s\" "
                                   "RECEIVE=\"%s%02d;%s%02d;%s%02d\">"
                                   "$%s%02d</A>\n",
                                s_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 0,
                                val,
                                s_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 0,
                                s_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 1,
                                s_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 2,
                                s_get_form_no(r, hdml),
                                hdml->var_cnt[hdml->pure_form_cnt] + 2));
  if (chk) 
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s%02d=_uchk&%s%02d=%s&%s%02d=X",
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 0,
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 1,
                            ap_escape_uri(r->pool,val),
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 2
                            ));
  }
  else
  {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s%02d=_chk&%s%02d=&%s%02d=_",
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 0,
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 1,
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 2
                            ));
  }

  s_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, "%s%s=$%s%02d",
                                  CHECK_BOX_PREFIX,
                                  nm,
                                  s_get_form_no(r, hdml),
                                  hdml->var_cnt[hdml->pure_form_cnt] + 1));
  hdml->var_cnt[hdml->pure_form_cnt] += 3;
}

/**
 * The ISTYLE attribute is converted into the HDML form.
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param is   [i]   The value of the ISTYLE attribute is specified. 
 * @return The ISTYLE attribute converted into the HDML form is returned. 
 */
char*
qs_conv_istyle_to_format(request_rec* r, char* is)
{
  char* fmt;
  if (is == NULL)
  {
    return NULL;
  }

  if (strcasecmp(is, "1") == 0)
  {
    fmt = apr_psprintf(r->pool, "M");
  }
  else
  if (strcasecmp(is, "2") == 0)
  {
    fmt = apr_psprintf(r->pool, "M");
  }
  else
  if (strcasecmp(is, "3") == 0)
  {
    fmt = apr_psprintf(r->pool, "m");
  }
  else
  if (strcasecmp(is, "4") == 0)
  {
    fmt = apr_psprintf(r->pool, "N");
  }
  else 
  {
    return NULL;
  }
  return fmt;
}

/**
 * It is a handler who processes the INPUT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_end_input_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_start_center_tag(hdml_t* hdml, Node* node) 
{
  hdml->center++;
  hdml->in_center++;

  if (hdml->hdml_br_flag == 0) 
  {
    hdml = s_output_to_hdml_out(hdml, "<BR>\n");
  }
  hdml = s_output_to_hdml_out(hdml, "<CENTER>");

  return hdml->out;
}

/**
 * It is a handler who processes the CENTER tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_end_center_tag(hdml_t* hdml, Node* child) 
{
  hdml->center = 0;
  hdml->in_center = 0;

  hdml = s_output_to_hdml_out(hdml, "<BR>\n");
  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_start_hr_tag(hdml_t* hdml, Node* node) 
{
  if (hdml->hdml_br_flag == 0) 
  {
    s_output_to_hdml_out(hdml, "<BR>\n");
    if (hdml->in_center > 0) 
    {
      hdml->in_center--;
    }
    else
    if (hdml->div_in_center > 0)
    {
      hdml->div_in_center--;
    }
  }

  s_output_to_hdml_out(hdml, "<CENTER>\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c<BR>\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}

/**
 * It is a handler who processes the HR tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_end_hr_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_start_img_tag(hdml_t* hdml, Node* node) 
{
  Doc*          doc  = hdml->doc;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table_t* spec = hdml->spec;
#endif
  Attr*         attr;

  s_hdml_tag_output_upper_half(hdml, node);

  s_output_to_hdml_out(hdml, "<img");

  /* Get Attributes */
  for (attr = qs_get_attr(doc,node); attr != NULL; attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (strcasecmp(name, "src") == 0) 
    {
      s_output_to_hdml_out(hdml, " src=\"");
#ifdef IMG_NOT_CONVERT_FILENAME
      s_output_to_hdml_out(hdml, value    );
#else
      s_output_to_hdml_out(hdml, chxj_img_conv(doc->r, spec,value));
#endif
      s_output_to_hdml_out(hdml, "\""     );
    }
    else 
    if (strcasecmp(name, "align" ) == 0) 
    {
      s_output_to_hdml_out(hdml, " align=\"" );
      s_output_to_hdml_out(hdml, value       );
      s_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "width" ) == 0) 
    {
      s_output_to_hdml_out(hdml, " width=\"");
      s_output_to_hdml_out(hdml, value      );
      s_output_to_hdml_out(hdml, "\""       );
    }
    else
    if (strcasecmp(name, "height") == 0) 
    {
      s_output_to_hdml_out(hdml, " height=\"");
      s_output_to_hdml_out(hdml, value       );
      s_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "hspace") == 0) 
    {
      s_output_to_hdml_out(hdml, " hspace=\"");
      s_output_to_hdml_out(hdml, value       );
      s_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "vspace") == 0) 
    {
      s_output_to_hdml_out(hdml, " vspace=\"");
      s_output_to_hdml_out(hdml, value       );
      s_output_to_hdml_out(hdml, "\""        );
    }
    else
    if (strcasecmp(name, "alt"   ) == 0) 
    {
      s_output_to_hdml_out(hdml, " alt=\""   );
      s_output_to_hdml_out(hdml, value       );
      s_output_to_hdml_out(hdml, "\""        );
    }
  }
  s_output_to_hdml_out(hdml, ">"             );

  return hdml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_hdml_end_img_tag(hdml_t* hdml, Node* child) 
{
  return hdml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_start_select_tag(hdml_t* hdml, Node* node)  
{
  Doc*         doc = hdml->doc;
  request_rec* r   = doc->r;
  Attr*        attr;

  s_hdml_tag_output_upper_half(hdml, node);

  hdml->card_cnt++;

  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                   "<A TASK=GOSUB LABEL=\x91\x49\x91\xf0 "
                                   "VARS=\"V=$%s%02d\" DEST=#D%d "
                                   "RECEIVE=\"%s%02d;%s%02d\">"
                                   "$%s%02d</A>\n",
                                   s_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+0, 
                                   hdml->card_cnt,
                                   s_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+0, 
                                   s_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+1, 
                                   s_get_form_no(r, hdml),
                                   hdml->var_cnt[hdml->pure_form_cnt]+1));

  s_output_to_hdml_card(hdml, 
                  apr_psprintf(r->pool, 
                          "<CHOICE KEY=V NAME=D%d ", hdml->card_cnt));
  s_output_to_hdml_card(hdml, 
                  apr_psprintf(r->pool, 
                          "DEFAULT=$V METHOD=ALPHA MARKABLE=FALSE>\n"));
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr != NULL; 
       attr=qs_get_next_attr(doc,attr)) 
  {
    char* name      = qs_get_attr_name(doc,attr);
    char* value     = qs_get_attr_value(doc,attr);
    char* selval    = NULL;
    char* selvaltxt = NULL;
    if (strcasecmp(name, "name") == 0) 
    {

      s_output_to_postdata(hdml, 
                      apr_psprintf(r->pool, "%s=$%s%02d", 
                              value,
                              s_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt]));
      selval = qs_get_selected_value(doc, node, r);
      if (selval == NULL) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "selected value not found");
        selval = qs_alloc_zero_byte_string(r);
      }
      else
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "selected value found[%s]" , selval);
      }
      selvaltxt = qs_get_selected_value_text(doc, node, r);
      if (selvaltxt == NULL)
      {
        selvaltxt = qs_alloc_zero_byte_string(r);
      }
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "selvaltxt:[%s]" ,selvaltxt);

      s_output_to_init_vars(hdml, 
                      apr_psprintf(r->pool, 
                              "%s%02d=%s&%s%02d=%s",
                              s_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt] + 0,
                              selval, 
                              s_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt] + 1,
                              selvaltxt));

      hdml->var_cnt[hdml->pure_form_cnt] += 2;
      break;
    }
  }
  return hdml->out;
}

/**
 * It is a handler who processes the SELECT tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_end_select_tag(hdml_t* hdml,  Node* node)  
{
  s_output_to_hdml_card(hdml, "</CHOICE>\n");

  return hdml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_start_option_tag(hdml_t* hdml, Node* node) 
{
  request_rec* r     = hdml->doc->r;
  Doc*         doc   = hdml->doc;
  Node*        child;
  char*        val;
  char*        txtval;

  hdml->card_cnt++;

  hdml->option_flag = 1;
  val = qs_get_value_attr(doc, node, r);

  /*--------------------------------------------------------------------------*/
  /* The child node of the object tag node acquires the value in assumption   */
  /* that is the TEXT node.                                                   */
  /*--------------------------------------------------------------------------*/
  child = qs_get_child_node(doc, node);
  if (child == NULL) 
  {
    txtval    = apr_palloc(r->pool, 1);
    txtval[0] = 0;
  }
  else 
  {
    txtval = qs_get_node_value(doc, child);
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "txtval:[%s]" , txtval);

  if (val != NULL && txtval != NULL) 
  {
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<CE TASK=RETURN VALUE=\"%s\" "
                            "RETVALS=\"$V;%s\">%s</CE>\n", 
                            val, 
                            qs_trim_string(r,txtval), 
                            qs_trim_string(r,txtval)));
  }
 
  return hdml->out;
}

/**
 * It is a handler who processes the OPTION tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_end_option_tag(hdml_t* hdml,  Node* node)  
{
  hdml->option_flag = 0;
  return hdml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_start_div_tag(hdml_t* hdml, Node* node) 
{
  Doc*    doc   = hdml->doc;
  Attr*   attr;

  /*--------------------------------------------------------------------------*/
  /* If the br tag is not output immediately before the div tag appears, the  */
  /* br tag is output.                                                        */
  /*--------------------------------------------------------------------------*/
  if (hdml->hdml_br_flag == 0)
  {
    hdml->hdml_br_flag = 1;
    s_output_to_hdml_out(hdml, "<BR>\n");
  }

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr != NULL; 
       attr = qs_get_next_attr(doc,attr)) 
  {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (strcasecmp(name, "align") == 0) 
    {
      if (strcasecmp(value, "right") == 0) 
      {
        hdml->div_right_flag = 1;
        s_output_to_hdml_out(hdml, "<RIGHT>");
        hdml->hdml_br_flag = 0;
        break;
      }
      else if (strcasecmp(value, "center") == 0)
      {
        hdml->div_center_flag = 1;
        s_output_to_hdml_out(hdml, "<CENTER>");
        hdml->hdml_br_flag = 0;
        break;
      }
    }
  }
 
  return hdml->out;
}

/**
 * It is a handler who processes the DIV tag.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char* 
s_hdml_end_div_tag(hdml_t* hdml,  Node* node)  
{
  request_rec* r = hdml->doc->r;


  if (hdml->div_right_flag == 1) 
  {
    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\n"));
    hdml->div_right_flag = 0;
  }
  if (hdml->div_center_flag == 1) 
  {
    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\n"));
    hdml->div_center_flag = 0;
    hdml->div_in_center   = 0;
  }
  return hdml->out;
}


/**
 * It is a wrapper of the apr_pstrcat function. 
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param o    [i]   The character string of connected origin is specified.
 * @param s    [i]   The character string connected with 'o' parameter is 
 *                   specified.
 * @param len  [i/o] The pointer to the area where the character string 
 *                   length is stored is specified. 
 * @return The character string after it connects it is returned. 
 */
char*
qs_out_apr_pstrcat(request_rec* r, char* o, char* s, int* len)
{
  *len = (strlen(s) + *len);
  return apr_pstrcat(r->pool, o, s, NULL);
}

/**
 * The prepositive character string used to generate the variable for HDML is 
 * generated. 
 *
 * @param r    [i]   To use POOL, the pointer to request_rec is specified.
 * @param hdml [i]   The pointer to the HDML structure that maintains the seed 
 *                   to generate it is specified.
 * @return The character string after it generates it is returned. 
 */
static char*
s_get_form_no(request_rec* r, hdml_t* hdml) 
{
  char *result;
  apr_time_exp_t tm;
  unsigned long fc = hdml->form_cnt;

  apr_time_exp_tz(&tm, hdml->form_cnt, 0);

  result = apr_psprintf(r->pool, "%c",(int)('A' + ((fc / 100) % 26)));
  result = apr_pstrcat(r->pool, result, 
                  apr_psprintf(r->pool, "%02d%02d%02d", 
                          tm.tm_hour,
                          tm.tm_min,
                          tm.tm_sec), NULL);
  return result;
}

/**
 * The number of tag nodes .."Input type =' radio '".. is counted. 
 *
 * @param hdml [i] The pointer to the HDML structure is specified.
 * @param node [i] The first parents node that counts the radio tag node is 
 *                 specified. 
 */
static void
s_hdml_count_radio_tag(hdml_t* hdml, Node* node) 
{
  Node*         child;
  Doc*          doc       = hdml->doc; 
  request_rec*  r         = doc->r;

  /*--------------------------------------------------------------------------*/
  /* All the child nodes of the specified node are scanned.                   */
  /*--------------------------------------------------------------------------*/
  for (child =  qs_get_child_node(doc,node); 
       child != NULL; 
       child =  qs_get_next_node(doc,child)) 
  {
    char*     type;
    char*     rname;
    char*     rvalue;
    char*     chkd;
    char*     name;
    int       ii;
    int       jj;

    name = qs_get_node_name(doc,child);
    if (strcasecmp(name, "input") != 0)
    {
      s_hdml_count_radio_tag(hdml, child);
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found input tag");
    type = qs_get_type_attr(doc, child, r);
    if (type == NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR,0,r,
        "Oops! The input tag without the type attribute has been found."
        "Please give a type.");
      continue;
    }
    if (strcasecmp(type, "radio") != 0)
    {
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found type=radio");
    rname  = qs_get_name_attr (doc, child, r);
    rvalue = qs_get_value_attr(doc, child, r);
    if (rname == NULL)
    {
      /*----------------------------------------------------------------------*/
      /* Oops!. The input tag without the name attribute has been found.      */
      /*----------------------------------------------------------------------*/
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
        "Oops!. The input tag without the name attribute has been found."
        "Please give a name.");
      continue;
    }

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "found name attribute");
    /*------------------------------------------------------------------------*/
    /* It scans in radio_name_list. When the same value exists, the           */
    /* value is overwrited.                                                   */
    /* The value is written in a new area when not is.                        */
    /*------------------------------------------------------------------------*/
    for (ii=0; ii<MAX_RADIO_COUNT; ii++) 
    {
      if (hdml->radio_name_list[ii] == NULL) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "new name:[%s]", rname);
        break;
      }
      if (strcasecmp(hdml->radio_name_list[ii], rname) == 0) 
      {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                        "already registered name:[%s]", rname);
        break;
      }
    }
    if (ii == MAX_RADIO_COUNT)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                      apr_psprintf(r->pool,
                        "I do not understand the name of the radiobutton "
                        "of %d piece or more. Please decrease "
                        "the number of radiobuttons.",
                        MAX_RADIO_COUNT));
      continue;
    }
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "add radio name:[%s]" ,rname);
    hdml->radio_name_list[ii] = apr_pstrdup(r->pool, rname);

    /*------------------------------------------------------------------------*/
    /* It adds it to radio_value_list.                                        */
    /*------------------------------------------------------------------------*/
    for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++)
    {
      if (hdml->radio_value_list[ii][jj] == NULL) 
      {
        break;
      }
    }
    if (jj == MAX_RADIO_VALUE_COUNT)
    {
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                      apr_psprintf(r->pool,
                        "I do not understand the value of the radiobutton "
                        "of %d piece or more. Please decrease "
                        "the number of radiobuttons.",
                        MAX_RADIO_VALUE_COUNT));
      continue;
    }
    hdml->radio_value_list[ii][jj] = apr_pstrdup(r->pool, rvalue);

    /*------------------------------------------------------------------------*/
    /* Now let's be the checked attribute or scan.                            */
    /*------------------------------------------------------------------------*/
    chkd = qs_get_checked_attr(hdml->doc, child, hdml->doc->r);
    if (chkd != NULL)
    {
      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                      apr_psprintf(r->pool,
                              "The tag scanned now had the checked "
                              "attribute. The value is [%s].",
                              rvalue));
      hdml->radio_checked_value[ii] = apr_pstrdup(r->pool, rvalue);
    }
  }
}

/**
 * The character string is added, and output to the out member of the HDML 
 * structure.
 *
 * @param hdml [i/o] The pointer to the HDML structure that maintains the out 
 *                   member at the output destination is specified. 
 * @param s    [i]   The character string that should be output is specified. 
 * @return The pointer to the HDML structure after it processes it is returned.
 */
static hdml_t* 
s_output_to_hdml_out(hdml_t* hdml, char* s)
{
  hdml->out = qs_out_apr_pstrcat(hdml->doc->r, hdml->out, s, &hdml->out_len);
  return hdml;
}

/**
 * The character string is added, and output to the card member of the HDML 
 * structure.
 *
 * @param hdml [i/o] The pointer to the HDML structure that maintains the 
 *                   card member at the output destination is specified. 
 * @param s    [i]   The character string that should be output is specified. 
 * @return The pointer to the HDML structure after it processes it is returned.
 */
static hdml_t*
s_output_to_hdml_card(hdml_t* hdml, char* s)
{
  hdml->card = qs_out_apr_pstrcat(hdml->doc->r, hdml->card, s, &hdml->card_len);
  return hdml;
}

/**
 * The data for the post is added, and output. 
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                    destination is specified.
 * @param s    [i]   The character string that wants to output to postdata is 
 *                   specified.
 */
static void
s_output_to_postdata(hdml_t* hdml, char* s)
{
  request_rec*          r = hdml->doc->r;

  if (strlen(hdml->postdata[hdml->pure_form_cnt]) != 0)
  {
    hdml->postdata[hdml->pure_form_cnt] =
                  apr_pstrcat(r->pool,
                              hdml->postdata[hdml->pure_form_cnt],
                              "&",
                              NULL);
  }
  hdml->postdata[hdml->pure_form_cnt] =
          apr_pstrcat(r->pool, 
                          hdml->postdata[hdml->pure_form_cnt],
                          qs_trim_string(r, s),
                          NULL);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, 
                  "POSTDATA:[%s]", 
                  hdml->postdata[hdml->pure_form_cnt] );
}

/**
 * The tag output upper half.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 */
static void
s_hdml_tag_output_upper_half(hdml_t* hdml, Node* node)
{
  if (hdml->hdml_br_flag   == 1 
  &&  hdml->div_right_flag == 1) 
  {
    s_output_to_hdml_out(hdml, "<RIGHT>");
    hdml->hdml_br_flag = 0;
  }

  if (hdml->hdml_br_flag == 1 
  &&  hdml->center > 0 
  &&  hdml->in_center == 0) 
  {
    s_output_to_hdml_out(hdml, "<CENTER>");
    hdml->in_center++;
    hdml->hdml_br_flag = 0;
  }
  else
  if (hdml->hdml_br_flag == 1 
  &&  hdml->div_center_flag > 0 
  &&  hdml->div_in_center == 0) 
  {
    s_output_to_hdml_out(hdml, "<CENTER>");
    hdml->div_in_center++;
    hdml->hdml_br_flag = 0;
  }
}

/**
 * The data for the init_vars is added, and output.
 *
 * @param hdml [i/o] The pointer to the HDML structure at the output
 *                    destination is specified.
 * @param s    [i]   The character string that wants to output to postdata is
 *                   specified.
 */
static void
s_output_to_init_vars(hdml_t* hdml, char* s)
{
  request_rec*    r = hdml->doc->r;

  if (strlen(hdml->init_vars) > 0)
  {
    hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, "&", NULL);
  }
  hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, qs_trim_string(r,s), NULL);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,
                  "INIT_VARS:[%s]",
                  hdml->init_vars);
}

static void
s_hdml_chxjif_tag(hdml_t* hdml, Node* node)
{
  Doc*         doc   = hdml->doc;
  Node*        child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child))
  {
    s_output_to_hdml_out(hdml, child->otext);
    s_hdml_chxjif_tag(hdml, child);
  }
}
/*
 * vim:ts=2 et
 */
