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
#include "chxj_hdml.h"
#include "chxj_tag_util.h"
#include "chxj_str_util.h"
#include <unistd.h>
#include "chxj_img_conv.h"
#include "chxj_dump.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"

#define CHECK_BOX_PREFIX     "_chxj_c_"
#define RADIO_BUTTON_PREFIX  "_chxj_r_"
#define SUBMIT_BUTTON_PREFIX "_chxj_s_"

#define GET_HDML(X) ((hdml_t *)(X))

static void  s_init_hdml            (hdml_t *hdml, Doc *doc, request_rec *r, device_table *spec);
static char *s_s_get_form_no(request_rec *r, hdml_t *hdml);


static char *s_hdml_start_html_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_html_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_meta_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_meta_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_li_tag      (void *pdoc,  Node *node);
static char *s_hdml_end_li_tag        (void *pdoc,  Node *node);
static char *s_hdml_start_head_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_head_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_title_tag   (void *pdoc,  Node *node);
static char *s_hdml_end_title_tag     (void *pdoc,  Node *node);
static char *s_hdml_start_base_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_base_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_body_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_body_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_a_tag       (void *pdoc,  Node *node);
static char *s_hdml_end_a_tag         (void *pdoc,  Node *node);
static char *s_hdml_start_br_tag      (void *pdoc,  Node *node);
static char *s_hdml_end_br_tag        (void *pdoc,  Node *node);
static char *s_hdml_start_tr_tag      (void *pdoc,  Node *node);
static char *s_hdml_end_tr_tag        (void *pdoc,  Node *node);
static char *s_hdml_start_font_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_font_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_form_tag    (void *pdoc,  Node *node);
static char *s_hdml_end_form_tag      (void *pdoc,  Node *node);
static char *s_hdml_start_input_tag   (void *pdoc,  Node *node);
static char *s_hdml_end_input_tag     (void *pdoc,  Node *node);
static char *s_hdml_start_select_tag  (void *pdoc,  Node *node);
static char *s_hdml_end_select_tag    (void *pdoc,  Node *node);
static char *s_hdml_start_option_tag  (void *pdoc,  Node *node);
static char *s_hdml_end_option_tag    (void *pdoc,  Node *node);
static char *s_hdml_start_center_tag  (void *pdoc,  Node *node);
static char *s_hdml_end_center_tag    (void *pdoc,  Node *node);
static char *s_hdml_start_hr_tag      (void *pdoc,  Node *node);
static char *s_hdml_end_hr_tag        (void *pdoc,  Node *node);
static char *s_hdml_start_img_tag     (void *pdoc,  Node *node);
static char *s_hdml_end_img_tag       (void *pdoc,  Node *node);
static char *s_hdml_start_div_tag     (void *pdoc,  Node *node);
static char *s_hdml_end_div_tag       (void *pdoc,  Node *node);
static char *s_hdml_start_blockquote_tag(void *pdoc, Node *node);
static char *s_hdml_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_hdml_start_dir_tag       (void *pdoc, Node *node);
static char *s_hdml_end_dir_tag         (void *pdoc, Node *node);
static char *s_hdml_start_dt_tag        (void *pdoc, Node *node);
static char *s_hdml_end_dt_tag          (void *pdoc, Node *node);
static char *s_hdml_start_dd_tag      (void *pdoc, Node *node);
static char *s_hdml_end_dd_tag        (void *pdoc, Node *node);
static char *s_hdml_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_hdml_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_hdml_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_hdml_start_pre_tag      (void *pdoc, Node *node);
static char *s_hdml_end_pre_tag        (void *pdoc, Node *node);
static char *s_hdml_start_textarea_tag (void *pdoc, Node *node);
static char *s_hdml_inner_textarea_tag_get_value(hdml_t *hdml, Node *node);

static char *((*s_get_form_no)(request_rec* r, hdml_t* hdml)) = s_s_get_form_no;

static void  s_hdml_count_radio_tag (hdml_t* hdml, Node* node) ;

static void  s_hdml_do_input_text_tag    (hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_password_tag(hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_submit_tag  (hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_reset_tag   (hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_hidden_tag  (hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_radio_tag   (hdml_t *hdml, Node *tag);
static void  s_hdml_do_input_checkbox_tag(hdml_t *hdml, Node *tag);
static void  s_hdml_tag_output_upper_half(hdml_t *hdml, Node* node);

static hdml_t *s_output_to_hdml_out       (hdml_t *hdml, char *s);
static hdml_t *s_output_to_hdml_card      (hdml_t *hdml, char *s);
static void  s_output_to_postdata         (hdml_t *hdml, char *s);
static void  s_output_to_init_vars        (hdml_t *hdml, char *s);
static int   s_hdml_search_emoji          (hdml_t *hdml, char *txt, char **rslt);

static char *s_hdml_chxjif_tag            (void *pdoc, Node *node);
static char *s_hdml_text_tag              (void *pdoc, Node *node);


tag_handler hdml_handler[] = {
  /* tagHTML */
  {
    s_hdml_start_html_tag,
    s_hdml_end_html_tag,
  },
  /* tagMETA */
  {
    s_hdml_start_meta_tag,
    s_hdml_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_hdml_start_textarea_tag,
    NULL,
  },
  /* tagP */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagPRE */
  {
    s_hdml_start_pre_tag,
    s_hdml_end_pre_tag,
  },
  /* tagUL */
  {
    s_hdml_start_dir_tag,
    s_hdml_end_dir_tag,
  },
  /* tagLI */
  {
    s_hdml_start_li_tag,
    s_hdml_end_li_tag,
  },
  /* tagOL */
  {
    s_hdml_start_dir_tag,
    s_hdml_end_dir_tag,
  },
  /* tagH1 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagH2 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagH3 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagH4 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagH5 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagH6 */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagHEAD */
  {
    s_hdml_start_head_tag,
    s_hdml_end_head_tag,
  },
  /* tagTITLE */
  {
    s_hdml_start_title_tag,
    s_hdml_end_title_tag,
  },
  /* tagBASE */
  {
    s_hdml_start_base_tag,
    s_hdml_end_base_tag,
  },
  /* tagBODY */
  {
    s_hdml_start_body_tag,
    s_hdml_end_body_tag,
  },
  /* tagA */
  {
    s_hdml_start_a_tag,
    s_hdml_end_a_tag,
  },
  /* tagBR */
  {
    s_hdml_start_br_tag,
    s_hdml_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_hdml_start_tr_tag,
    s_hdml_end_tr_tag,
  },
  /* tagTD */
  {
    NULL,
    NULL,
  },
  /* tagTBODY */
  {
    NULL,
    NULL,
  },
  /* tagFONT */
  {
    s_hdml_start_font_tag,
    s_hdml_end_font_tag,
  },
  /* tagFORM */
  {
    s_hdml_start_form_tag,
    s_hdml_end_form_tag,
  },
  /* tagINPUT */
  {
    s_hdml_start_input_tag,
    s_hdml_end_input_tag,
  },
  /* tagCENTER */
  {
    s_hdml_start_center_tag,
    s_hdml_end_center_tag,
  },
  /* tagHR */
  {
    s_hdml_start_hr_tag,
    s_hdml_end_hr_tag,
  },
  /* tagIMG */
  {
    s_hdml_start_img_tag,
    s_hdml_end_img_tag,
  },
  /* tagSELECT */
  {
    s_hdml_start_select_tag,
    s_hdml_end_select_tag,
  },
  /* tagOPTION */
  {
    s_hdml_start_option_tag,
    s_hdml_end_option_tag,
  },
  /* tagDIV */
  {
    s_hdml_start_div_tag,
    s_hdml_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_hdml_chxjif_tag,
    NULL,
  },
  /* tagNOBR */
  {
    NULL,
    NULL,
  },
  /* tagSMALL */
  {
    NULL,
    NULL,
  },
  /* tagSTYLE */
  {
    NULL,
    NULL,
  },
  /* tagSPAN */
  {
    NULL,
    NULL,
  },
  /* tagTEXT */
  {
    s_hdml_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    NULL,
    NULL,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_hdml_start_dt_tag,
    s_hdml_end_dt_tag,
  },
  /* tagLEGEND */
  {
    NULL,
    NULL,
  },
  /* tagLABEL */
  {
    NULL,
    NULL,
  },
  /* tagBLOCKQUOTE */
  {
    s_hdml_start_blockquote_tag,
    s_hdml_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_hdml_start_dir_tag,
    s_hdml_end_dir_tag,
  },
  /* tagDL */
  {
    NULL,
    NULL,
  },
  /* tagDD */
  {
    s_hdml_start_dd_tag,
    s_hdml_end_dd_tag,
  },
  /* tagMENU */
  {
    s_hdml_start_dir_tag,
    s_hdml_end_dir_tag,
  },
  /* tagPLAINTEXT */
  {
    s_hdml_start_plaintext_tag,
    s_hdml_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    NULL,
    NULL,
  },
  /* tagMARQUEE */
  {
    NULL,
    NULL,
  },
};

/**
 * converts from CHTML to hdml.
 *
 * @param r    [i] Requet_rec is appointed.
 * @param spec [i] The result of the device specification processing which 
 *                 was done in advance is appointed.
 * @param src  [i] The character string before the converting is appointed.
 * @return         The character string after the converting is returned.
 */
char *
chxj_convert_hdml(
  request_rec        *r, 
  device_table       *spec, 
  const char         *src, 
  apr_size_t         srclen, 
  apr_size_t         *dstlen,
  chxjconvrule_entry *entryp,
  cookie_t           *UNUSED(cookie)
)
{
  char      *dst;
  char      *buf;
  Doc       doc;
  hdml_t    hdml;

  dst = NULL;
  buf = NULL;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"i found qrcode xml");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* initialize hdml structure                                                */
  /*--------------------------------------------------------------------------*/
  s_init_hdml(&hdml,&doc,r, spec);

  hdml.entryp = entryp;

  chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
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
  buf = (char *)apr_table_get(r->headers_out, (const char *)"Location");
  if (buf) {
    /*------------------------------------------------------------------------*/
    /* The Location header generates tag in an initial HDML machine for the   */
    /* uncorrespon dence.                                                     */
    /*------------------------------------------------------------------------*/
    DBG(r, "Location is not null[Location:%s]", buf);
    s_output_to_hdml_out(&hdml, 
        "<HDML VERSION=3.0 MARKABLE=TRUE PUBLIC=TRUE>\r\n"
        "<NODISPLAY MARKABLE=TRUE PUBLIC=TRUE TITLE=\" \">\r\n"
        "<ACTION TYPE=ACCEPT TASK=GO DEST=\""
        );
    s_output_to_hdml_out(&hdml, buf);
    s_output_to_hdml_out(&hdml,
        "\">\r\n"
        "</NODISPLAY>\r\n"
        "</HDML>\r\n"
        );
    dst = apr_pstrdup(r->pool, hdml.out);
  }
  else {
    /*------------------------------------------------------------------------*/
    /* Here, the parsing of the received character string is done             */
    /*------------------------------------------------------------------------*/
    char *ss = apr_palloc(r->pool, srclen + 1);
    memset(ss, 0, srclen + 1);
    memcpy(ss, src, srclen);
    
    DBG(r, "input strlen(src)=[%d]", (int)srclen);
    DBG(r, "[[[[%s]]]", src);

    qs_init_malloc(&doc); 
    qs_init_root_node(&doc);
    ss[srclen] = '\0';
    qs_parse_string(&doc, ss, srclen);

    /*------------------------------------------------------------------------*/
    /* The number of radiobuttons is counted.                                 */
    /*------------------------------------------------------------------------*/
    s_hdml_count_radio_tag(&hdml, qs_get_root(&doc));

    chxj_node_convert(spec,r,(void*)&hdml, &doc, qs_get_root(&doc), 0);
    dst = hdml.out;

    DBG(r,"tmp=[%s]", dst);
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
s_init_hdml(hdml_t *hdml, Doc *doc, request_rec *r, device_table *spec)
{
  int     ii;
  int     jj;

  /*--------------------------------------------------------------------------*/
  /* init hdml structure value                                                */
  /*--------------------------------------------------------------------------*/
  memset(hdml, 0, sizeof(hdml_t));
  hdml->doc      = doc;
  hdml->card     = qs_alloc_zero_byte_string(r->pool);
  hdml->spec     = spec;
  hdml->conf     = chxj_get_module_config(r->per_dir_config, &chxj_module);
  hdml->doc->parse_mode = PARSE_MODE_CHTML;

  for (ii=0; ii<MAX_FORM_COUNT; ii++) {
    hdml->var_cnt[ii]     = 0;
    hdml->postdata[ii]    = qs_alloc_zero_byte_string(r->pool);
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

  hdml->init_vars      = qs_alloc_zero_byte_string(r->pool);

  doc->r               = r;

  hdml->form_cnt = apr_time_now();
  hdml->out = qs_alloc_zero_byte_string(r->pool);
}


static int
s_hdml_search_emoji(hdml_t *hdml, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = hdml->spec;

  len = strlen(txt);
  r = hdml->doc->r;

  if (!spec) {
    DBG(r,"spec is NULL");
  }

  for (ee = hdml->conf->emoji;
       ee;
       ee = ee->next) {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (! ee->imode) {
      DBG(r, "emoji->imode is NULL");
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
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_out(hdml, 
    "<HDML VERSION=3.0 TTL=0 MARKABLE=TRUE>\r\n"
    "<NODISPLAY NAME=D0>\r\n"
    "<ACTION TYPE=ACCEPT TASK=GOSUB DEST=#D1 NEXT=#D2 CLEAR=TRUE>\r\n"
    "</NODISPLAY>\r\n"
    );

  hdml->card_cnt = 2;
  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the HTML tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_card(hdml, 
                  "<NODISPLAY NAME=D1>\r\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN VARS=\""
                  );

  if (strlen(hdml->init_vars)) {
    s_output_to_hdml_card(hdml, hdml->init_vars   );
  }
  else {
    s_output_to_hdml_card(hdml, "_chxj_dmy="            );
  }

  s_output_to_hdml_card(hdml,   
                  "\" CLEAR=TRUE>\r\n"
                  "</NODISPLAY>\r\n"
                  );

  s_output_to_hdml_out(hdml, hdml->card );
  s_output_to_hdml_out(hdml, "</HDML>\r\n");

  hdml->hdml_end_flag = 1;

  return hdml->out;
}


/**
 * It is a handler who processes the META tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_meta_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  hdml->hdml_br_flag = 0;

  /* ignore */

  return hdml->out;
}


/**
 * It is a handler who processes the META tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  /* ignore */

  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the HEAD tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  /* ignore */

  return hdml->out;
}


/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"");

  hdml->found_title = 1;
  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the TITLE tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_out(hdml, "\">\r\n");

  return hdml->out;
}


/**
 * It is a handler who processes the BASE tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_base_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the BASE tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the BODY tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_body_tag(void *pdoc, Node *node) 
{
  hdml_t      *hdml;
  Doc         *doc;
  Attr        *attr;

  hdml = GET_HDML(pdoc);

  doc  = hdml->doc;

  if (hdml->found_title == 0) {
    s_output_to_hdml_out(hdml, "<DISPLAY NAME=D2 TITLE=\"NO TITLE\">\r\n");
  }

  s_output_to_hdml_out(hdml, "<ACTION TYPE=ACCEPT TASK=NOOP LABEL=\" \"");
  /*--------------------------------*/
  /* Get Attributes                 */
  /*--------------------------------*/
  for (attr = qs_get_attr(doc,node); 
    attr; 
    attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);

    if (STRCASEEQ('b','B',"bgcolor",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('t','T',"text",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('l','L',"link",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('a','A',"alink",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /* ignore */
    }
  }
  s_output_to_hdml_out(hdml, ">\r\n");
  hdml->hdml_br_flag = 0;
  return hdml->out;
}


/**
 * It is a handler who processes the BODY tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_out(hdml, "\r\n</DISPLAY>\r\n");

  return hdml->out;
}


/**
 * It is a handler who processes the A tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_a_tag(void *pdoc, Node *node) 
{
  hdml_t       *hdml;
  Doc          *doc;
  Attr         *attr;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;

  s_hdml_tag_output_upper_half(hdml,node);

  s_output_to_hdml_out(hdml, "<A");


  /*------------------------------------*/
  /* Get Attributes                     */
  /*------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name)) {
      /* IGNORE */
    }
    else if (STRCASEEQ('h','H',"href",name)) {
      if (STRNCASEEQ('m','M',"mailto:",value,sizeof("mailto:")-1)) {
        value = chxj_encoding_parameter(hdml->doc->r, value);
        s_output_to_hdml_out(hdml, " TASK=GO DEST=\""     );
        s_output_to_hdml_out(hdml, value                  );
        s_output_to_hdml_out(hdml, "\" "                  );
      }
      else if (STRNCASEEQ('t','T',"tel:",value,sizeof("tel:")-1)) {
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
    else if (STRCASEEQ('a','A',"accesskey",name) && value && *value) {
      if (strcasecmp(value, "0") != 0) {
        s_output_to_hdml_out(hdml, " ACCESSKEY="          );
        s_output_to_hdml_out(hdml, value                  );
        s_output_to_hdml_out(hdml, ""                     );
      }
    }
    else if (STRCASEEQ('c','C',"cti",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ijam",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('t','T',"telbook",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('k','K',"kana",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('e','E',"email",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ista",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst",name)) {
      /* ignore */
    }
  }
  s_output_to_hdml_out(hdml, ">"  );

  hdml->hdml_a_flag = 1;

  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the A tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_out(hdml, "</A>\r\n");

  hdml->hdml_a_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the BR tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_br_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  if (hdml->in_center > 0) {
    hdml->in_center = 0;
  }

  if (hdml->div_in_center > 0) {
    hdml->div_in_center = 0;
  }

  s_output_to_hdml_out(hdml, "<BR>\r\n");
  hdml->hdml_br_flag = 1;

  return hdml->out;
}


/**
 * It is a handler who processes the BR tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the TR tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  if (hdml->in_center > 0) {
    hdml->in_center = 0;
  }

  if (hdml->div_in_center > 0) {
    hdml->div_in_center = 0;
  }

  s_output_to_hdml_out(hdml, "<BR>\r\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}


/**
 * It is a handler who processes the TR tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the FONT tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_font_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the FONT tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_end_font_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the FORM tag. 
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified. 
 * @return The conversion result is returned. 
 */
static char *
s_hdml_start_form_tag(void *pdoc, Node *node) 
{
  hdml_t       *hdml;
  request_rec  *r;
  Attr         *attr;
  Doc          *doc;
  char         *act;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;
  r    = hdml->doc->r;
  act  = NULL;

  hdml->form_tmp = apr_psprintf(r->pool,
                  "<NODISPLAY NAME=F%d>\r\n",
                  hdml->pure_form_cnt);
  hdml->form_tmp = apr_pstrcat(r->pool,
                              hdml->form_tmp,
                             "<ACTION TYPE=ACCEPT TASK=GO METHOD=POST DEST=\"",
                              NULL);
  /* Get Attributes */
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"action",name)) {
      value = chxj_encoding_parameter(hdml->doc->r, value);
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
                               "\" ",
                               NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "POSTDATA=\"", 
                               NULL);

  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * handler of the form end tag. 
 *
 * @param pdoc [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param child [i]  unused.
 * @return The HDML output result after it edits it is returned. 
 */
static char *
s_hdml_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t      *hdml = GET_HDML(pdoc);
  request_rec *r    = hdml->doc->r;

  s_output_to_postdata(hdml, "_chxj_dmy=");

  hdml->form_tmp = apr_pstrcat(r->pool, 
                               hdml->form_tmp,
                               hdml->postdata[hdml->pure_form_cnt],
                               NULL);

  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "\" CLEAR=TRUE >\r\n", 
                               NULL);
  hdml->form_tmp = apr_pstrcat(r->pool,
                               hdml->form_tmp,
                               "</NODISPLAY>\r\n",
                               NULL);

  s_output_to_hdml_card(hdml, hdml->form_tmp);

  hdml->form_tmp = NULL;
  hdml->pure_form_cnt++;

  return hdml->out;
}


/**
 * It is a handler that takes charge of the processing of the input tag. 
 *
 * @param pdoc [i/o] The pointer to the HDML structure at the output
 *                   destination is specified.
 * @param node [i]   The tag node to be processed is specified. 
 */
static char *
s_hdml_start_input_tag(void *pdoc, Node *node) 
{
  hdml_t *hdml = GET_HDML(pdoc);
  Doc    *doc  = hdml->doc;
  Attr   *attr;

  /*--------------------------------------------------------------------------*/
  /* The attribute of the input tag is acquired.                              */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (STRCASEEQ('t','T',"text",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type ='text'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_text_tag(hdml, node);
      }
      else if (STRCASEEQ('p','P',"password",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='password'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_password_tag(hdml, node);
      }
      else if (STRCASEEQ('s','S',"submit",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='submit'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_submit_tag(hdml, node);
      }
      else if (STRCASEEQ('h','H',"hidden",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='hidden'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_hidden_tag(hdml, node);
      }
      else if (STRCASEEQ('r','R',"radio",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='radio'" tag is processed.                             */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_radio_tag(hdml, node);
      }
      else if (STRCASEEQ('c','C',"checkbox",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='checkbox'" tag is processed.                          */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_checkbox_tag(hdml, node);
      }
      else if (STRCASEEQ('r','R',"reset",value)) {
        /*--------------------------------------------------------------------*/
        /* "input type='reset'" tag is processed.                            */
        /*--------------------------------------------------------------------*/
        s_hdml_do_input_reset_tag(hdml, node);
      }
    }
    else if (STRCASEEQ('n','N',"name",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"value",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('s','S',"size",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('m','M',"maxlength",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('c','C',"checked",name)) {
      /* ignore */
    }
    else  if (STRCASEEQ('a','A',"accesskey",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"istyle",name)) {
      /* ignore */
    }
  }
  hdml->hdml_br_flag = 0;

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
s_hdml_do_input_text_tag(hdml_t *hdml, Node *tag)
{
  Doc           *doc;
  request_rec   *r;
  char          *mlen;
  char          *val;
  char          *is;
  char          *nm;
  char          *fmt;
  size_t        ii;

  doc   = hdml->doc;
  r     = doc->r;

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
                       apr_psprintf(r->pool, 
                                    "[$%s%02d]</A>\r\n"  , 
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
  nm = qs_get_name_attr(doc, tag, r->pool);
  if (! nm) {
    nm = qs_alloc_zero_byte_string(r->pool);
  }

  s_output_to_postdata(hdml, 
                       apr_psprintf(r->pool, 
                                    "%s=$%s%02d", 
                                    nm,
                                    s_get_form_no(r, hdml),
                                    hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r->pool);
  is   = qs_get_istyle_attr     (doc, tag, r->pool);
  val  = qs_get_value_attr      (doc, tag, r->pool);

  fmt  = qs_conv_istyle_to_format(r, is);
  DBG(r,"qs_conv_istyle_to_format end");
        
  if (fmt) {
    if (mlen) {
      for (ii=0; ii<strlen(mlen); ii++) {
        if (mlen[ii] < '0' || mlen[ii] > '9') {
          mlen = apr_psprintf(r->pool, "0");
          break;
        }
      }
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=%d%s", atoi(mlen), fmt));
    }
    else 
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );
  }

  s_output_to_hdml_card(hdml, 
                        " MARKABLE=FALSE>\r\n"
                        "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\r\n"
                        "</ENTRY>\r\n");

  if (val) {
    s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                       "%s%02d=%s", 
                                       s_get_form_no(r, hdml),
                                       hdml->var_cnt[hdml->pure_form_cnt],
                                       chxj_escape_uri(r->pool,val)));
  }
  else {
    s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                       "%s%02d=", 
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
s_hdml_do_input_password_tag(hdml_t *hdml, Node *tag)
{
  Doc         *doc;
  request_rec *r;
  char        *mlen;
  char        *val;
  char        *is;
  char        *nm;
  char        *fmt;

  doc = hdml->doc;
  r   = doc->r;

  s_hdml_tag_output_upper_half(hdml, tag);

  hdml->card_cnt++;
  s_output_to_hdml_out(hdml, "<A TASK=GOSUB LABEL=\"\x93\xfc\x97\xcd\" DEST=");
  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "#D%d ", hdml->card_cnt));
  s_output_to_hdml_out(hdml, 
                       apr_psprintf(r->pool, 
                                    "VARS=\"V=$%s%02d\" ", 
                                    s_get_form_no(r, hdml),
                                    hdml->var_cnt[hdml->pure_form_cnt]));

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "RECEIVE=%s%02d>"  , 
                               s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "[$%s%02d]</A>\r\n"  , 
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  s_output_to_hdml_card(hdml, "<ENTRY NAME="                               );
  s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "D%d ", hdml->card_cnt));
  s_output_to_hdml_card(hdml, " KEY=V DEFAULT=$V "                         );

  mlen = NULL;
  is   = NULL;
  val  = NULL;
  fmt  = NULL;

  nm = qs_get_name_attr(doc, tag, r->pool);
  if (! nm) {
    nm = qs_alloc_zero_byte_string(r->pool);
  }

  s_output_to_postdata(hdml, 
                  apr_psprintf(r->pool, 
                          "%s=$%s%02d", 
                          nm,
                          s_get_form_no(r, hdml),
                          hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, tag, r->pool);
  val  = qs_get_value_attr      (doc, tag, r->pool);
  /*--------------------------------------------------------------------------*/
  /* Default is a figure input.                                               */
  /*--------------------------------------------------------------------------*/
  fmt = apr_psprintf(r->pool, "N");
  if (mlen) {
    if (chxj_chk_numeric(mlen) != 0)
      mlen = apr_psprintf(r->pool, "0");
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=%d%s", chxj_atoi(mlen), fmt));
  }
  else 
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, " FORMAT=*%s", fmt)       );

  s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " NOECHO=TRUE "));

  s_output_to_hdml_card(hdml, 
                  " MARKABLE=FALSE>\r\n"
                  "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\r\n"
                  "</ENTRY>\r\n");

  if (val) 
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=%s", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt], 
                        chxj_escape_uri(r->pool,val)));
  else 
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, "%s%02d=", 
                        s_get_form_no(r, hdml),
                        hdml->var_cnt[hdml->pure_form_cnt]));

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
s_hdml_do_input_submit_tag(hdml_t *hdml, Node *tag)
{
  Doc         *doc = hdml->doc;
  request_rec *r   = doc->r;
  char        *nm  = NULL;
  char        *val = NULL;

  s_hdml_tag_output_upper_half(hdml, tag);

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GO LABEL=OK DEST=#F%d ",
                          hdml->pure_form_cnt));

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r->pool);
  val = qs_get_value_attr (doc, tag, r->pool);

  if (nm && val) {
    s_output_to_hdml_out(hdml, 
                    apr_psprintf(r->pool, "VARS=\"%s=%s\" ", 
                            nm, 
                            chxj_escape_uri(r->pool,val)));
    if (strstr(hdml->postdata[hdml->pure_form_cnt], nm) == NULL) {
      s_output_to_postdata(hdml, 
                      apr_psprintf(r->pool,"%s%s=$%s", 
                              SUBMIT_BUTTON_PREFIX, nm, nm));
    }
  }
  s_output_to_hdml_out(hdml, ">"         );
  if (val) {
    s_output_to_hdml_out(hdml, val);
  }
  s_output_to_hdml_out(hdml, "</A>\r\n"    );
}


/**
 * The substitution processing of tag "input type = reset" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=submit is specified. 
 */
static void
s_hdml_do_input_reset_tag(hdml_t *hdml, Node *tag)
{
  Doc           *doc = hdml->doc;
  request_rec   *r   = doc->r;
  char          *nm  = NULL;
  char          *val = NULL;

  s_hdml_tag_output_upper_half(hdml, tag);

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GO LABEL=OK DEST=#D0>"));

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r->pool);
  val = qs_get_value_attr (doc, tag, r->pool);
  if (val) {
    s_output_to_hdml_out(hdml, val);
  }
  s_output_to_hdml_out(hdml, "</A>\r\n"    );
}


/**
 * The substitution processing of tag "input type = hidden" is done. 
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   The tag node of input type=hidden is specified. 
 */
static void
s_hdml_do_input_hidden_tag(hdml_t *hdml, Node *tag)
{
  Doc         *doc = hdml->doc;
  request_rec *r   = doc->r;
  char        *nm  = NULL;
  char        *val = NULL;

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r->pool);
  val = qs_get_value_attr (doc, tag, r->pool);
  if (nm && val) {
    s_output_to_postdata(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            chxj_escape_uri(r->pool, val)));
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
s_hdml_do_input_radio_tag(hdml_t *hdml, Node *tag)
{
  Doc         *doc       = hdml->doc;
  request_rec *r         = doc->r;
  char        *nm        = NULL;
  char        *val       = NULL;
  int         ii;
  int         jj;
  int         kk;
  int         r_cnt;

  s_hdml_tag_output_upper_half(hdml, tag);

  /*--------------------------------------------------------------------------*/
  /* get name and value attribute                                             */
  /*--------------------------------------------------------------------------*/
  nm  = qs_get_name_attr  (doc, tag, r->pool);
  val = qs_get_value_attr (doc, tag, r->pool);
  /*--------------------------------------------------------------------------*/
  /* The same name is searched out from the list made beforehand.             */
  /*--------------------------------------------------------------------------*/
  for (ii=0; ii<MAX_RADIO_COUNT; ii++) {
    if (! hdml->radio_name_list[ii]) {
      /* @todo Oops..  */
      DBG(r, "Oops... radio list is null[%d]", ii);
      /*----------------------------------------------------------------------*/
      /* Processing is ended because it doesn't happen off the fly.           */
      /*----------------------------------------------------------------------*/
      return;
    }

    if (strcasecmp(hdml->radio_name_list[ii], nm) == 0) 
      break;
  }
  if (ii == MAX_RADIO_COUNT) {
    /* @todo Oops.. */
    DBG(r,"Oops... The same name was not in the list. ");
    /*------------------------------------------------------------------------*/
    /* Processing is ended because it doesn't happen off the fly.             */
    /*------------------------------------------------------------------------*/
    return;
  }

  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, 
                          "<A TASK=GOSUB "
                          "LABEL=\"check\" "
                          "DEST=#R%d VARS=\"VAL=%s\" "
                          "RECEIVE=\"%s;", 
                          ii, 
                          val, 
                          nm));

  if (hdml->radio_out_cnt[ii] == 0) {
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool,
                            "<NODISPLAY NAME=R%d>\r\n",
                            ii));
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<ACTION "
                            "TYPE=ACCEPT TASK=RETURN RETVALS=\"$VAL;"));
  }

  kk = hdml->radio_out_cnt[ii];
  for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) 
    if (! hdml->radio_value_list[ii][jj]) 
      break;

  r_cnt = jj;

  for (jj=0; jj<r_cnt; jj++) {
    if (! hdml->radio_value_list[ii][jj])
      break;

    if (jj != 0) {
      s_output_to_hdml_out(hdml, apr_psprintf(r->pool, ";"));
      if (hdml->radio_out_cnt[ii] == 0) 
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, ";"));
    }

    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "%s_%02d", nm, kk));
    if (hdml->radio_out_cnt[ii] == 0)  {
      if (jj == 0) 
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "X"));
      else 
        s_output_to_hdml_card(hdml, apr_psprintf(r->pool, "_"));
    }

    kk++;
    if (kk >= r_cnt) 
      kk=0;
  }
  s_output_to_hdml_out(hdml, 
                  apr_psprintf(r->pool, "\" >$%s_%02d</A>", 
                          nm, 
                          hdml->radio_out_cnt[ii]));
  if (! hdml->radio_out_cnt[ii]) {
    s_output_to_hdml_card(hdml, "\">\r\n"         );
    s_output_to_hdml_card(hdml, "</NODISPLAY>\r\n");

    s_output_to_postdata(hdml,  apr_psprintf(r->pool, "%s%s=$%s", RADIO_BUTTON_PREFIX, nm, nm));

    for (jj=0; jj<r_cnt; jj++) {
      if (hdml->radio_value_list[ii][jj] &&  hdml->radio_checked_value[ii]) {
        if (strcasecmp(hdml->radio_value_list[ii][jj], 
                       hdml->radio_checked_value[ii]) == 0) {
          s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=X", 
                                  nm, 
                                  jj));
        }
        else {
          s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                  "%s_%02d=_", 
                                  nm, 
                                  jj));
        }
      }
      else {
        s_output_to_init_vars(hdml, 
                        apr_psprintf(r->pool, 
                                "%s_%02d=_", 
                                nm, 
                                jj));
      }
    }

    if (hdml->radio_checked_value[ii]) {
      DBG(r,"radio button is checked. checked value is [%s]", 
        hdml->radio_checked_value[ii]);
      s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s=%s", 
                            nm, 
                            hdml->radio_checked_value[ii]));
    }
    else {
      DBG(r,"radio button is not checked. checked value is []");
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
s_hdml_do_input_checkbox_tag(hdml_t *hdml, Node *tag)
{
  Doc         *doc       = hdml->doc;
  request_rec *r         = doc->r;
  char        *nm        = NULL;
  char        *val       = NULL;
  int         chk;

  /*--------------------------------------------------------------------------*/
  /* It is posted to the one without the checked attribute.                   */
  /* However, they were able to be removed with INPUT FILTER.                 */
  /*--------------------------------------------------------------------------*/
  if (! hdml->has_checkbox) {
    hdml->has_checkbox++;
    s_output_to_hdml_card(hdml, 
                    "<NODISPLAY NAME=\"_chk\">\r\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_uchk;$V;X\" >\r\n"
                    "</NODISPLAY>\r\n"
                    "<NODISPLAY NAME=\"_uchk\">\r\n"
                    "<ACTION TYPE=\"ACCEPT\" TASK=\"RETURN\" "
                                             "RETVALS=\"_chk;;_\" >\r\n"
                    "</NODISPLAY>\r\n"
                    );
    DBG(r, "wrote checkbox hdml card.");
  }
        
  /*--------------------------------------------------------------------------*/
  /* It is examined whether it is CHECKED.                                    */
  /*--------------------------------------------------------------------------*/
  chk = qs_is_checked_checkbox_attr(doc, tag, r->pool);

  /*--------------------------------------------------------------------------*/
  /* The value of the name attribute and the value attribute is acquired      */
  /* respectively.                                                            */
  /*--------------------------------------------------------------------------*/
  val = qs_get_value_attr(doc, tag, r->pool);
  nm  = qs_get_name_attr(doc, tag, r->pool);

  if (! val) {
    val    = qs_alloc_zero_byte_string(r->pool);
  }

  if (! nm) {
    nm   = qs_alloc_zero_byte_string(r->pool);
  }

  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                "<A TASK=GOSUB LABEL=\"check\" "
                                   "DEST=\"#$%s%02d\" "
                                   "VARS=\"V=%s\" "
                                   "RECEIVE=\"%s%02d;%s%02d;%s%02d\">"
                                   "$%s%02d</A>\r\n",
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
  if (chk) {
    s_output_to_init_vars(hdml, 
                    apr_psprintf(r->pool, 
                            "%s%02d=_uchk&%s%02d=%s&%s%02d=X",
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 0,
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 1,
                            chxj_escape_uri(r->pool,val),
                            s_get_form_no(r, hdml),
                            hdml->var_cnt[hdml->pure_form_cnt] + 2
                            ));
  }
  else {
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
char *
qs_conv_istyle_to_format(request_rec *r, char *is)
{
  char *fmt;

  if (!is)
    return NULL;
  
  switch(*is) {
  case '1':
    fmt = apr_psprintf(r->pool, "M");
    break;
  case '2':
    fmt = apr_psprintf(r->pool, "M");
    break;
  case '3':
    fmt = apr_psprintf(r->pool, "m");
    break;
  case '4':
    fmt = apr_psprintf(r->pool, "N");
    break;
  default:
    return NULL;
  }

  return fmt;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_center_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  hdml->center++;
  hdml->in_center++;

  if (hdml->hdml_br_flag == 0) {
    hdml = s_output_to_hdml_out(hdml, "<BR>\r\n");
  }

  hdml = s_output_to_hdml_out(hdml, "<CENTER>");

  return hdml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  hdml->center = 0;
  hdml->in_center = 0;

  hdml = s_output_to_hdml_out(hdml, "<BR>\r\n");
  hdml->hdml_br_flag = 1;

  return hdml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_hr_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  if (hdml->hdml_br_flag == 0) {
    s_output_to_hdml_out(hdml, "<BR>\r\n");
    if (hdml->in_center)
      hdml->in_center--;
    else
    if (hdml->div_in_center) 
      hdml->div_in_center--;
  }

  s_output_to_hdml_out(hdml, "<CENTER>\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c\x81\x7c<BR>\r\n");

  hdml->hdml_br_flag = 1;

  return hdml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  return hdml->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_li_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);
  int i;

  if (hdml->hdml_br_flag == 0) {
    s_output_to_hdml_out(hdml, "<BR>\r\n");
    if (hdml->in_center)
      hdml->in_center--;
    else
    if (hdml->div_in_center) 
      hdml->div_in_center--;
  }
  s_output_to_hdml_out(hdml, "<WRAP>");
  for (i=0; i<hdml->dir_level; i++) {
    s_output_to_hdml_out(hdml, "&nbsp;");
  } 
  hdml->hdml_br_flag = 1;
  return hdml->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);
  if (hdml->hdml_br_flag == 0) {
    s_output_to_hdml_out(hdml, "<BR>\r\n");
    if (hdml->in_center)
      hdml->in_center--;
    else
    if (hdml->div_in_center) 
      hdml->div_in_center--;
  }
  hdml->hdml_br_flag = 1;
  return hdml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_img_tag(void *pdoc, Node *node) 
{
  hdml_t        *hdml;
  Doc           *doc;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec;
#endif
  Attr          *attr;
  char          *out;
  int           align_flag = 0;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;
#ifndef IMG_NOT_CONVERT_FILENAME
  spec = hdml->spec;
#endif

  s_hdml_tag_output_upper_half(hdml, node);

  out = apr_palloc(doc->r->pool, 1);
  out[0] = 0;
  out = apr_pstrcat(doc->r->pool, out, "<img", NULL);

  /* Get Attributes */
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"src",name) && value && *value) {
      value = chxj_encoding_parameter(hdml->doc->r, value);
      out = apr_pstrcat(doc->r->pool, out, " src=\"", NULL);
#ifdef IMG_NOT_CONVERT_FILENAME
      out = apr_pstrcat(doc->r->pool, out, value, NULL);
#else
      out = apr_pstrcat(doc->r->pool, out, chxj_img_conv(doc->r, spec,value), NULL);
#endif
      out = apr_pstrcat(doc->r->pool, out, "\"", NULL);
    }
    else if (STRCASEEQ('a','A',"align",name)) {
      if (value) {
        if (STRCASEEQ('r','R',"right", value)) {
          s_output_to_hdml_out(hdml, "<RIGHT>" );
          align_flag = 1;
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          s_output_to_hdml_out(hdml, "<CENTER>" );
          align_flag = 1;
        }
      }
    }
    else if (STRCASEEQ('w','W',"width",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('h','H',"height",name) && value && *value) {
      /* ignore */
    }
    else if (STRCASEEQ('h','H',"hspace", name) && value && *value) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vspace",name) && value && *value) {
      /* ignore */
    }
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      out = apr_pstrcat(doc->r->pool, out, " alt=\"", NULL);
      out = apr_pstrcat(doc->r->pool, out, value, NULL);
      out = apr_pstrcat(doc->r->pool, out, "\"", NULL);
    }
  }
  out = apr_pstrcat(doc->r->pool, out, ">", NULL);
  s_output_to_hdml_out(hdml, out);
  if (align_flag) {
    s_output_to_hdml_out(hdml, "<BR>");
  }

  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t *hdml = GET_HDML(pdoc);
  return hdml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_select_tag(void *pdoc, Node *node)  
{
  Doc         *doc;
  request_rec *r;
  Attr        *attr;
  hdml_t      *hdml;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;
  r    = doc->r;

  s_hdml_tag_output_upper_half(hdml, node);

  hdml->card_cnt++;

  s_output_to_hdml_out(hdml, apr_psprintf(r->pool, 
                                   "<A TASK=GOSUB LABEL=\x91\x49\x91\xf0 "
                                   "VARS=\"V=$%s%02d\" DEST=#D%d "
                                   "RECEIVE=\"%s%02d;%s%02d\">"
                                   "$%s%02d</A>\r\n",
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
                          "DEFAULT=$V METHOD=ALPHA MARKABLE=FALSE>\r\n"));
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr=qs_get_next_attr(doc,attr)) {
    char *name      = qs_get_attr_name(doc,attr);
    char *value     = qs_get_attr_value(doc,attr);
    char *selval    = NULL;
    char *selvaltxt = NULL;

    if (STRCASEEQ('n','N',"name",name)) {
      s_output_to_postdata(hdml, 
                      apr_psprintf(r->pool, "%s=$%s%02d", 
                              value,
                              s_get_form_no(r, hdml),
                              hdml->var_cnt[hdml->pure_form_cnt]));
      selval = qs_get_selected_value(doc, node, r->pool);
      if (! selval) {
        DBG(r, "selected value not found");
        selval = qs_alloc_zero_byte_string(r->pool);
      }
      else {
        DBG(r, "selected value found[%s]" , selval);
      }
      selvaltxt = qs_get_selected_value_text(doc, node, r->pool);
      if (!selvaltxt)
        selvaltxt = qs_alloc_zero_byte_string(r->pool);

      DBG(r, "selvaltxt:[%s]" ,selvaltxt);

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
  hdml->hdml_br_flag = 0;
  return hdml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_select_tag(void *pdoc,  Node *UNUSED(node))
{
  hdml_t *hdml = GET_HDML(pdoc);

  s_output_to_hdml_card(hdml, "</CHOICE>\r\n");

  return hdml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char * 
s_hdml_start_option_tag(void *pdoc, Node *node) 
{
  request_rec *r;
  Doc         *doc;
  Node        *child;
  char        *val;
  char        *txtval;
  hdml_t      *hdml;

  hdml  = GET_HDML(pdoc);
  r     = hdml->doc->r;
  doc   = hdml->doc;

  hdml->card_cnt++;

  hdml->option_flag = 1;
  val = qs_get_value_attr(doc, node, r->pool);

  /*--------------------------------------------------------------------------*/
  /* The child node of the object tag node acquires the value in assumption   */
  /* that is the TEXT node.                                                   */
  /*--------------------------------------------------------------------------*/
  child = qs_get_child_node(doc, node);
  if (!child) {
    txtval    = apr_palloc(r->pool, 1);
    txtval[0] = 0;
  }
  else {
    txtval = qs_get_node_value(doc, child);
  }

  DBG(r, "txtval:[%s]" , txtval);

  if (val && txtval) {
    s_output_to_hdml_card(hdml, 
                    apr_psprintf(r->pool, 
                            "<CE TASK=RETURN VALUE=\"%s\" "
                            "RETVALS=\"$V;%s\">%s</CE>\r\n", 
                            val, 
                            qs_trim_string(r->pool,txtval), 
                            qs_trim_string(r->pool,txtval)));
  }
 
  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_option_tag(void *pdoc,  Node *UNUSED(node)) 
{
  hdml_t *hdml = GET_HDML(pdoc);

  hdml->option_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_div_tag(void *pdoc, Node *node) 
{
  hdml_t *hdml;
  Doc    *doc;
  Attr   *attr;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;

  /*--------------------------------------------------------------------------*/
  /* If the br tag is not output immediately before the div tag appears, the  */
  /* br tag is output.                                                        */
  /*--------------------------------------------------------------------------*/
  if (hdml->hdml_br_flag == 0) {
    hdml->hdml_br_flag = 1;
    s_output_to_hdml_out(hdml, "<BR>\r\n");
  }

  /*--------------------------------------------------------------------------*/
  /* The object tag node is scanned.                                          */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node); 
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      if (STRCASEEQ('r','R',"right",value)) {
        hdml->div_right_flag = 1;
        s_output_to_hdml_out(hdml, "<RIGHT>");
        hdml->hdml_br_flag = 0;
        break;
      }
      else if (STRCASEEQ('c','C',"center",value)) {
        hdml->div_center_flag = 1;
        s_output_to_hdml_out(hdml, "<CENTER>");
        hdml->hdml_br_flag = 0;
        break;
      }
    }
  }
  hdml->hdml_br_flag = 0;
 
  return hdml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_div_tag(void *pdoc,  Node *UNUSED(node))
{
  hdml_t       *hdml = GET_HDML(pdoc);
  request_rec  *r    = hdml->doc->r;

  if (hdml->div_right_flag == 1) {
    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\r\n"));
    hdml->div_right_flag = 0;
  }
  if (hdml->div_center_flag == 1) {
    s_output_to_hdml_out(hdml, apr_psprintf(r->pool, "<BR>\r\n"));
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
char *
qs_out_apr_pstrcat(request_rec *r, char *o, char *s, int *len)
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
static char *
s_s_get_form_no(request_rec *r, hdml_t *hdml) 
{
  char           *result;
  apr_time_exp_t tm;
  unsigned long  fc;

  fc = hdml->form_cnt;

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
s_hdml_count_radio_tag(hdml_t *hdml, Node *node) 
{
  Node         *child;
  Doc          *doc; 
  request_rec  *r;

  doc       = hdml->doc; 
  r         = doc->r;

  /*--------------------------------------------------------------------------*/
  /* All the child nodes of the specified node are scanned.                   */
  /*--------------------------------------------------------------------------*/
  for (child =  qs_get_child_node(doc,node); 
       child; 
       child =  qs_get_next_node(doc,child)) {
    char *type;
    char *rname;
    char *rvalue;
    char *chkd;
    char *name;
    int       ii;
    int       jj;

    name = qs_get_node_name(doc,child);
    if (strcasecmp(name, "input") != 0) {
      s_hdml_count_radio_tag(hdml, child);
      continue;
    }

    DBG(r,"found input tag");

    type = qs_get_type_attr(doc, child, r->pool);
    if (!type) {
      ERR(r, "Oops! The input tag without the type attribute has been found.Please give a type.");
      continue;
    }

    if (strcasecmp(type, "radio") != 0) 
      continue;

    DBG(r, "found type=radio");

    rname  = qs_get_name_attr (doc, child, r->pool);
    rvalue = qs_get_value_attr(doc, child, r->pool);

    if (!rname) {
      /*----------------------------------------------------------------------*/
      /* Oops!. The input tag without the name attribute has been found.      */
      /*----------------------------------------------------------------------*/
      DBG(r, "Oops!. The input tag without the name attribute has been found. Please give a name.");
      continue;
    }

    DBG(r, "found name attribute");

    /*------------------------------------------------------------------------*/
    /* It scans in radio_name_list. When the same value exists, the           */
    /* value is overwrited.                                                   */
    /* The value is written in a new area when not is.                        */
    /*------------------------------------------------------------------------*/
    for (ii=0; ii<MAX_RADIO_COUNT; ii++) {
      if (! hdml->radio_name_list[ii]) {
        DBG(r, "new name:[%s]", rname);
        break;
      }
      if (strcasecmp(hdml->radio_name_list[ii], rname) == 0) {
        DBG(r, "already registered name:[%s]", rname);
        break;
      }
    }
    if (ii == MAX_RADIO_COUNT) {
      DBG(r, apr_psprintf(r->pool,
                        "I do not understand the name of the radiobutton "
                        "of %d piece or more. Please decrease "
                        "the number of radiobuttons.",
                        MAX_RADIO_COUNT));
      continue;
    }

    DBG(r,"add radio name:[%s]" ,rname);

    hdml->radio_name_list[ii] = apr_pstrdup(r->pool, rname);

    /*------------------------------------------------------------------------*/
    /* It adds it to radio_value_list.                                        */
    /*------------------------------------------------------------------------*/
    for (jj=0; jj<MAX_RADIO_VALUE_COUNT; jj++) {
      if (!hdml->radio_value_list[ii][jj]) 
        break;
    }
    if (jj == MAX_RADIO_VALUE_COUNT) {
      DBG(r, apr_psprintf(r->pool,
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
    chkd = qs_get_checked_attr(hdml->doc, child, r->pool);
    if (chkd) {
      DBG(r,apr_psprintf(r->pool,
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
static hdml_t *
s_output_to_hdml_out(hdml_t *hdml, char *s)
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
static hdml_t *
s_output_to_hdml_card(hdml_t *hdml, char *s)
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
s_output_to_postdata(hdml_t *hdml, char *s)
{
  request_rec *r = hdml->doc->r;

  if (strlen(hdml->postdata[hdml->pure_form_cnt])) {
    hdml->postdata[hdml->pure_form_cnt] =
                  apr_pstrcat(r->pool,
                              hdml->postdata[hdml->pure_form_cnt],
                              "&",
                              NULL);
  }
  hdml->postdata[hdml->pure_form_cnt] =
          apr_pstrcat(r->pool, 
                          hdml->postdata[hdml->pure_form_cnt],
                          qs_trim_string(r->pool, s),
                          NULL);

  DBG(r, "POSTDATA:[%s]", hdml->postdata[hdml->pure_form_cnt] );
}



/**
 * The tag output upper half.
 *
 * @param hdml   [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 */
static void
s_hdml_tag_output_upper_half(hdml_t *hdml, Node *UNUSED(node))
{
  if (hdml->hdml_br_flag   == 1 &&  hdml->div_right_flag == 1) {
    s_output_to_hdml_out(hdml, "<RIGHT>");
    hdml->hdml_br_flag = 0;
  }

  if (hdml->hdml_br_flag == 1 
      &&  hdml->center > 0 
      &&  hdml->in_center == 0) {
    s_output_to_hdml_out(hdml, "<CENTER>");
    hdml->in_center++;
    hdml->hdml_br_flag = 0;
  }
  else if (hdml->hdml_br_flag == 1 
      &&  hdml->div_center_flag > 0 
      &&  hdml->div_in_center == 0)  {
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
s_output_to_init_vars(hdml_t *hdml, char *s)
{
  request_rec *r = hdml->doc->r;

  if (strlen(hdml->init_vars)) {
    hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, "&", NULL);
  }

  hdml->init_vars = apr_pstrcat(r->pool, hdml->init_vars, qs_trim_string(r->pool,s), NULL);

  DBG(r, "INIT_VARS:[%s]", hdml->init_vars);
}



static char *
s_hdml_chxjif_tag(void *pdoc, Node *node)
{
  hdml_t *hdml;
  Doc    *doc;
  Node   *child;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    s_output_to_hdml_out(hdml, child->otext);
    s_hdml_chxjif_tag(hdml, child);
  }
  return NULL;
}


static char *
s_hdml_text_tag(void *pdoc, Node *child) 
{
  hdml_t      *hdml;
  Doc         *doc;
  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[3];
  int         ii;
  int         tdst_len = 0;
  int         one_line_count = 0;
  request_rec *r;

  hdml = GET_HDML(pdoc);
  doc  = hdml->doc;
  r    = doc->r;
  
  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return hdml->out;
  }
  
  if (hdml->option_flag == 1) {
    return hdml->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  tdst = apr_palloc(r->pool, 1);
  tdst[0] = '\0';
  one_byte[0] = '\0';
  one_byte[1] = '\0';
  one_byte[2] = '\0';
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_hdml_search_emoji(hdml, &textval[ii], &out);
    if (rtn) {
      tdst = qs_out_apr_pstrcat(r, tdst, out, &tdst_len);
      ii += (rtn-1);
      one_line_count+=(rtn-1);
      continue;
    }
    if (hdml->hdml_blockquote_flag) {
      if (one_line_count == 0) {
        tdst = qs_out_apr_pstrcat(r, tdst, "<WRAP>&nbsp;<TAB>", &tdst_len);
      }
      if (is_sjis_kanji(textval[ii])) {
        one_byte[0] = textval[ii+0];
        one_byte[1] = textval[ii+1];
        one_byte[2] = 0;
        tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        ii++;
        one_line_count+=2;
      }
      else {
        one_byte[0] = textval[ii+0];
        one_byte[1] = 0;
        tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        one_line_count++;
        if (one_byte[0] == '\n') {
          one_line_count = 0;
        }
      }
      continue;
    }
    if (hdml->pre_flag) {
      if (one_line_count == 0) {
        tdst = qs_out_apr_pstrcat(r, tdst, "<LINE>", &tdst_len);
      }
      if (is_sjis_kanji(textval[ii])) {
        one_byte[0] = textval[ii+0];
        one_byte[1] = textval[ii+1];
        one_byte[2] = 0;
        tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
        ii++;
        one_line_count+=2;
      }
      else {
        one_byte[0] = textval[ii+0];
        one_byte[1] = 0;
        if (one_byte[0] == ' ') {
          tdst = qs_out_apr_pstrcat(r, tdst, "&nbsp;", &tdst_len);
        }
        else {
          tdst = qs_out_apr_pstrcat(r, tdst, chxj_ap_escape_html(r->pool, one_byte), &tdst_len);
        }
        one_line_count++;
        if (one_byte[0] == '\n') {
          one_line_count = 0;
        }
      }
      continue;
    }
    if (is_sjis_kanji(textval[ii])) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      one_byte[0] = textval[ii+1];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      ii++;
      one_line_count+=2;
    }
    else if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  memcpy(tmp, textval, strlen(textval)-1);
  
  s_hdml_tag_output_upper_half(hdml, child->parent);
  s_output_to_hdml_out(hdml, tdst);
  hdml->hdml_br_flag = 0;

  return hdml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  hdml->hdml_blockquote_flag++;
  return hdml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  hdml->hdml_blockquote_flag--;
  s_output_to_hdml_out(hdml, "\r\n");
  return hdml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_dir_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  hdml->dir_level++;
  return hdml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  hdml->dir_level--;
  return hdml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_dt_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  s_output_to_hdml_out(hdml, "<LINE>");
  return hdml->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  s_output_to_hdml_out(hdml, "\r\n");
  return hdml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_dd_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  s_output_to_hdml_out(hdml, "<WRAP>&nbsp;<TAB>");
  return hdml->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  s_output_to_hdml_out(hdml, "\r\n");
  return hdml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_plaintext_tag(void *pdoc, Node *node)
{
  hdml_t *hdml = GET_HDML(pdoc);
  hdml->plaintext_value_len = 0;
  s_hdml_start_plaintext_tag_inner(pdoc,node);
  return hdml->out;
}

static char *
s_hdml_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  hdml_t *hdml = GET_HDML(pdoc);
  Doc    *doc = hdml->doc;
  Node   *child;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    int  i;
    char *str = chxj_ap_escape_html(doc->r->pool, child->otext);
    int len = strlen(str);
    char oneChar[3];
    for (i=0; i<len; i++) {
      if (hdml->plaintext_value_len == 0) {
        s_output_to_hdml_out(hdml, "<LINE>");
      }
      if (is_sjis_kanji(child->otext[i]) && i + 1 < len) {
        oneChar[0] = child->otext[i+0];
        oneChar[1] = child->otext[i+1];
        oneChar[2] = 0; 
        s_output_to_hdml_out(hdml, oneChar);
        hdml->plaintext_value_len +=2;
        i++;
        continue;
      }
      if (child->otext[i] == '\n') {
        hdml->plaintext_value_len = 0;
        oneChar[0] = '\n';
        oneChar[1] = 0;
        s_output_to_hdml_out(hdml, oneChar);
        continue;
      }
      oneChar[0] = child->otext[i];
      oneChar[1] = 0;
      s_output_to_hdml_out(hdml, oneChar);
      hdml->plaintext_value_len++;
    }
    s_hdml_start_plaintext_tag_inner(pdoc, child);
  }
  return hdml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  hdml_t *hdml = GET_HDML(pdoc);
  return hdml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_start_pre_tag(void *pdoc, Node *UNUSED(node)) 
{
  hdml_t        *hdml;
  Doc           *doc;
  request_rec   *r;

  hdml    = GET_HDML(pdoc);
  doc     = hdml->doc;
  r       = doc->r;

  hdml->pre_flag++;
  return hdml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the HDML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_hdml_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  hdml_t        *hdml;
  Doc           *doc;
  request_rec   *r;

  hdml = GET_HDML(pdoc);
  doc     = hdml->doc;
  r       = doc->r;

  hdml->pre_flag--;

  return hdml->out;
}


/**
 * handler of the TEXTAREA tag.
 * 
 * @param hdml [i/o] The pointer to the HDML structure at the output 
 *                   destination is specified. 
 * @param tag  [i]   Specified The TEXTAREA tag node.
 */
static char *
s_hdml_start_textarea_tag(void *pdoc, Node *node)
{
  Doc           *doc;
  request_rec   *r;
  char          *mlen;
  char          *val;
  char          *is;
  char          *nm;
  char          *fmt;
  size_t        ii;
  hdml_t        *hdml = GET_HDML(pdoc);

  doc   = hdml->doc;
  r     = doc->r;

  s_hdml_tag_output_upper_half(hdml, node);

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
                       apr_psprintf(r->pool, 
                                    "[$%s%02d]</A>\r\n"  , 
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
  nm = qs_get_name_attr(doc, node, r->pool);
  if (! nm) {
    nm = qs_alloc_zero_byte_string(r->pool);
  }

  s_output_to_postdata(hdml, 
                       apr_psprintf(r->pool, 
                                    "%s=$%s%02d", 
                                    nm,
                                    s_get_form_no(r, hdml),
                                    hdml->var_cnt[hdml->pure_form_cnt]));

  mlen = qs_get_maxlength_attr  (doc, node, r->pool);
  is   = qs_get_istyle_attr     (doc, node, r->pool);
  val  = s_hdml_inner_textarea_tag_get_value(hdml, node);

  fmt  = qs_conv_istyle_to_format(r, is);
  if (fmt) {
    if (mlen) {
      for (ii=0; ii<strlen(mlen); ii++) {
        if (mlen[ii] < '0' || mlen[ii] > '9') {
          mlen = apr_psprintf(r->pool, "0");
          break;
        }
      }
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=%d%s", atoi(mlen), fmt));
    }
    else {
      s_output_to_hdml_card(hdml, apr_psprintf(r->pool, " FORMAT=*%s", fmt));
    }
  }

  s_output_to_hdml_card(hdml, 
                        " MARKABLE=FALSE>\r\n"
                        "<ACTION TYPE=ACCEPT TASK=RETURN RETVALS=$V>\r\n"
                        "</ENTRY>\r\n");
  if (val) {
    s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                       "%s%02d=%s", 
                                       s_get_form_no(r, hdml),
                                       hdml->var_cnt[hdml->pure_form_cnt],
                                       chxj_escape_uri(r->pool,val)));
  }
  else {
    s_output_to_init_vars(hdml, 
                          apr_psprintf(r->pool, 
                                       "%s%02d=", 
                                       s_get_form_no(r, hdml),
                                       hdml->var_cnt[hdml->pure_form_cnt]));
  }
  hdml->var_cnt[hdml->pure_form_cnt]++;
  return hdml->out;
}

static char *
s_hdml_inner_textarea_tag_get_value(hdml_t *hdml, Node *node)
{
  Doc    *doc = hdml->doc;
  Node   *child;
  char   *result = apr_pstrdup(doc->r->pool, "\0");
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    char *textval = qs_get_node_value(doc,child);
    if (textval && *textval) {
      result = apr_pstrcat(doc->r->pool, result, textval, NULL);
    }
  }
  return result;
}
/*
 * vim:ts=2 et
 */
