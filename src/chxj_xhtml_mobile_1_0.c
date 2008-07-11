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
#include "chxj_encoding.h"
#include "chxj_xhtml_mobile_1_0.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_buffered_write.h"
#include "chxj_str_util.h"

#define GET_XHTML(X) ((xhtml_t*)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { xhtml->out = BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { xhtml->out = (X) ? BUFFERED_WRITE_VALUE(xhtml->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, ""); } while(0)

static char *s_xhtml_1_0_start_html_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_html_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_p_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_p_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_pre_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_pre_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_ul_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_ul_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h1_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h1_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h2_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h2_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h3_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h3_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h4_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h4_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h5_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h5_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_h6_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_h6_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_ol_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_ol_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_li_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_li_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_meta_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_meta_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_head_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_head_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_title_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_title_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_base_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_base_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_body_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_body_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_a_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_a_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_br_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_br_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_tr_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_tr_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_font_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_font_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_form_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_form_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_input_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_input_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_center_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_center_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_hr_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_hr_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_img_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_img_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_select_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_select_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_option_tag (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_option_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_div_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_div_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_textarea_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_textarea_tag    (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_b_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_b_tag           (void *pdoc, Node *node);
static char *s_xhtml_1_0_chxjif_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_blockquote_tag(void *pdoc, Node *node);
static char *s_xhtml_1_0_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dir_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dir_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dl_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dl_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dt_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dt_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_dd_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_dd_tag          (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_menu_tag      (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_menu_tag        (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_plaintext_tag         (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_blink_tag     (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_blink_tag       (void *pdoc, Node *node);
static char *s_xhtml_1_0_start_marquee_tag   (void *pdoc, Node *node);
static char *s_xhtml_1_0_end_marquee_tag     (void *pdoc, Node *node);

static void  s_init_xhtml(xhtml_t *xhtml, Doc *doc, request_rec *r, device_table *spec);
static int   s_xhtml_search_emoji(xhtml_t *xhtml, char *txt, char **rslt);
static char *s_xhtml_1_0_text_tag(void *pdoc, Node *child);


tag_handler xhtml_handler[] = {
  /* tagHTML */
  {
    s_xhtml_1_0_start_html_tag,
    s_xhtml_1_0_end_html_tag,
  },
  /* tagMETA */
  {
    s_xhtml_1_0_start_meta_tag,
    s_xhtml_1_0_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_xhtml_1_0_start_textarea_tag,
    s_xhtml_1_0_end_textarea_tag,
  },
  /* tagP */
  {
    s_xhtml_1_0_start_p_tag,
    s_xhtml_1_0_end_p_tag,
  },
  /* tagPRE */
  {
    s_xhtml_1_0_start_pre_tag,
    s_xhtml_1_0_end_pre_tag,
  },
  /* tagUL */
  {
    s_xhtml_1_0_start_ul_tag,
    s_xhtml_1_0_end_ul_tag,
  },
  /* tagLI */
  {
    s_xhtml_1_0_start_li_tag,
    s_xhtml_1_0_end_li_tag,
  },
  /* tagOL */
  {
    s_xhtml_1_0_start_ol_tag,
    s_xhtml_1_0_end_ol_tag,
  },
  /* tagH1 */
  {
    s_xhtml_1_0_start_h1_tag,
    s_xhtml_1_0_end_h1_tag,
  },
  /* tagH2 */
  {
    s_xhtml_1_0_start_h2_tag,
    s_xhtml_1_0_end_h2_tag,
  },
  /* tagH3 */
  {
    s_xhtml_1_0_start_h3_tag,
    s_xhtml_1_0_end_h3_tag,
  },
  /* tagH4 */
  {
    s_xhtml_1_0_start_h4_tag,
    s_xhtml_1_0_end_h4_tag,
  },
  /* tagH5 */
  {
    s_xhtml_1_0_start_h5_tag,
    s_xhtml_1_0_end_h5_tag,
  },
  /* tagH6 */
  {
    s_xhtml_1_0_start_h6_tag,
    s_xhtml_1_0_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_xhtml_1_0_start_head_tag,
    s_xhtml_1_0_end_head_tag,
  },
  /* tagTITLE */
  {
    s_xhtml_1_0_start_title_tag,
    s_xhtml_1_0_end_title_tag,
  },
  /* tagBASE */
  {
    s_xhtml_1_0_start_base_tag,
    s_xhtml_1_0_end_base_tag,
  },
  /* tagBODY */
  {
    s_xhtml_1_0_start_body_tag,
    s_xhtml_1_0_end_body_tag,
  },
  /* tagA */
  {
    s_xhtml_1_0_start_a_tag,
    s_xhtml_1_0_end_a_tag,
  },
  /* tagBR */
  {
    s_xhtml_1_0_start_br_tag,
    s_xhtml_1_0_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_xhtml_1_0_start_tr_tag,
    s_xhtml_1_0_end_tr_tag,
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
    s_xhtml_1_0_start_font_tag,
    s_xhtml_1_0_end_font_tag,
  },
  /* tagFORM */
  {
    s_xhtml_1_0_start_form_tag,
    s_xhtml_1_0_end_form_tag,
  },
  /* tagINPUT */
  {
    s_xhtml_1_0_start_input_tag,
    s_xhtml_1_0_end_input_tag,
  },
  /* tagCENTER */
  {
    s_xhtml_1_0_start_center_tag,
    s_xhtml_1_0_end_center_tag,
  },
  /* tagHR */
  {
    s_xhtml_1_0_start_hr_tag,
    s_xhtml_1_0_end_hr_tag,
  },
  /* tagIMG */
  {
    s_xhtml_1_0_start_img_tag,
    s_xhtml_1_0_end_img_tag,
  },
  /* tagSELECT */
  {
    s_xhtml_1_0_start_select_tag,
    s_xhtml_1_0_end_select_tag,
  },
  /* tagOPTION */
  {
    s_xhtml_1_0_start_option_tag,
    s_xhtml_1_0_end_option_tag,
  },
  /* tagDIV */
  {
    s_xhtml_1_0_start_div_tag,
    s_xhtml_1_0_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_xhtml_1_0_chxjif_tag,
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
    s_xhtml_1_0_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_xhtml_1_0_start_b_tag,
    s_xhtml_1_0_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
  /* tagDT */
  {
    s_xhtml_1_0_start_dt_tag,
    s_xhtml_1_0_end_dt_tag,
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
    s_xhtml_1_0_start_blockquote_tag,
    s_xhtml_1_0_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_xhtml_1_0_start_dir_tag,
    s_xhtml_1_0_end_dir_tag,
  },
  /* tagDL */
  {
    s_xhtml_1_0_start_dl_tag,
    s_xhtml_1_0_end_dl_tag,
  },
  /* tagDD */
  {
    s_xhtml_1_0_start_dd_tag,
    s_xhtml_1_0_end_dd_tag,
  },
  /* tagMENU */
  {
    s_xhtml_1_0_start_menu_tag,
    s_xhtml_1_0_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_xhtml_1_0_start_plaintext_tag,
    s_xhtml_1_0_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_xhtml_1_0_start_blink_tag,
    s_xhtml_1_0_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_xhtml_1_0_start_marquee_tag,
    s_xhtml_1_0_end_marquee_tag,
  },
};
 
/**
 * converts from CHTML to XHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_exchange_xhtml_mobile_1_0(
  request_rec        *r,
  device_table       *spec,
  const char         *src,
  apr_size_t         srclen,
  apr_size_t         *dstlen,
  chxjconvrule_entry *entryp,
  cookie_t           *UNUSED(cookie)
)
{
  char      *dst = NULL;
  char      *ss;
  xhtml_t   xhtml;
  Doc       doc;

  DBG(r,"start chxj_exchange_xhtml_mobile_1_0()");
  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst != NULL) {
    DBG(r,"end chxj_exchange_xhtml_mobile_1_0() (found qrcode.xml)");
    return dst;
  }

  /*--------------------------------------------------------------------------*/
  /* The XHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_xhtml(&xhtml, &doc, r, spec);

  xhtml.entryp = entryp;

  chxj_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss,   0, srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML->XHTML", ss, srclen);
#endif
  qs_parse_string(&doc,ss, strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to XHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_exchange(spec,r,(void*)&xhtml, &doc, qs_get_root(&doc), 0);
  xhtml.out = chxj_buffered_write_flush(xhtml.out, &doc.buf);
  dst = apr_pstrdup(r->pool, xhtml.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (! dst) {
    return apr_pstrdup(r->pool,ss);
  }

  if (! *dst) {
    dst = apr_psprintf(r->pool, "\n");
  }
  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML->XHTML", dst, *dstlen);
#endif

  DBG(r,"end chxj_exchange_xhtml_mobile_1_0()");
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
s_init_xhtml(xhtml_t *xhtml, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(xhtml, 0, sizeof(xhtml_t));

  doc->r      = r;
  xhtml->doc  = doc;
  xhtml->spec = spec;
  xhtml->out  = qs_alloc_zero_byte_string(r);
  xhtml->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  xhtml->doc->parse_mode = PARSE_MODE_CHTML;
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
s_xhtml_search_emoji(xhtml_t *xhtml, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = xhtml->spec;

  len = strlen(txt);
  r = xhtml->doc->r;

  if (spec == NULL) {
    DBG(r,"spec is NULL");
  }

  for (ee = xhtml->conf->emoji;
       ee;
       ee = ee->next) {
    unsigned char hex1byte;
    unsigned char hex2byte;
    if (!ee->imode) {
      DBG(r,"emoji->imode is NULL");
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
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_html_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;

  /*--------------------------------------------------------------------------*/
  /* Add XML Declare                                                          */
  /*--------------------------------------------------------------------------*/
  W_L("<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n");
  /*--------------------------------------------------------------------------*/
  /* Add DocType                                                              */
  /*--------------------------------------------------------------------------*/
  W_L("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n");
  W_L(" \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n");
  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html xmlns=\"http://www.w3.org/1999/xhtml\"");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('l','L',"lang", name)) {
      W_L(" xml:lang=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"version", name)) {
      W_L(" version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"");
    }
  }
  W_L(">\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;
  W_L("</html>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_meta_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;
  int           content_type_flag = 0;

  W_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name", name) && value && *value) {
      W_L(" ");
      W_V(name);
      W_L("=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"http-equiv", name) && value && *value) {
      W_L(" ");
      W_V(name);
      W_L("=\"");
      W_V(value);
      W_L("\"");
      if (STRCASEEQ('c','C', "content-type", value)) {
        content_type_flag = 1;
      }
    }
    else if (STRCASEEQ('c','C',"content", name) && value && *value) {
      if (content_type_flag) {
        W_L(" ");
        W_V(name);
        W_L("=\"");
        W_L("text/html; charset=Windows-31J");
        W_L("\"");
      }
      else {
        W_L(" ");
        W_V(name);
        W_L("=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(" />\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("<head>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</head>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t      *xhtml = GET_XHTML(pdoc);
  Doc          *doc   = xhtml->doc;

  W_L("<title>");
  return xhtml->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</title>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_base_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Attr          *attr;
  Doc           *doc   = xhtml->doc;

  W_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href",name)) {
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
      break;
    }
  }
  W_L(" />\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_body_tag(void *pdoc, Node *node) 
{
  xhtml_t      *xhtml = GET_XHTML(pdoc);
  Doc          *doc   = xhtml->doc;
  Attr         *attr;

  W_L("<body");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B',"bgcolor", name) && value && *value) {
      W_L(" bgcolor=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('t','T',"text",name) && value && *value) {
      W_L(" text=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('l','L',"link", name) && value && *value) {
      W_L(" link=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"alink", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /* ignore */
    }
  }
  W_L(">\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;

  W_L("</body>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_a_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;
  request_rec   *r     = doc->r;
  Attr          *attr;

  W_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name) && value && *value) {
      W_L(" id=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"href", name) && value && *value) {
      value = chxj_encoding_parameter(r, value);
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cti",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ijam", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
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
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</a>");
  return xhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_br_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<br");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"clear",name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('a','A',"all",value))) {
        W_L(" clear=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(" />\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<br />\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_font_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<font");
  /*=========================================================================*/
  /* Get Attributes                                                          */
  /*=========================================================================*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color",name) && value && *value) {
      W_L(" color=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('s','S',"size",name) && value && *value) {
      W_L(" size=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_font_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</font>");
  return xhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_form_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml = GET_XHTML(pdoc);
  Doc         *doc   = xhtml->doc;
  request_rec *r     = doc->r;
  Attr        *attr;

  W_L("<form");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"action",name)) {
      value = chxj_encoding_parameter(r, value);
      W_L(" action=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('m','M',"method",name)) {
      W_L(" method=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /* ignore */
    }
    else if (STRCASEEQ('n','N',"name",name)) {
      W_L(" name=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</form>");
  return xhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_input_tag(void *pdoc, Node *node) 
{
  xhtml_t     *xhtml       = GET_XHTML(pdoc);
  Doc         *doc         = xhtml->doc;
  request_rec *r           = doc->r;
  char        *max_length  = NULL;
  char        *type        = NULL;
  char        *name        = NULL;
  char        *value       = NULL;
  char        *istyle      = NULL;
  char        *size        = NULL;
  char        *checked     = NULL;
  char        *accesskey   = NULL;

  W_L("<input");
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
    type = qs_trim_string(doc->buf.pool, type);
    if (type && (STRCASEEQ('t','T',"text",    type) ||
                 STRCASEEQ('p','P',"password",type) ||
                 STRCASEEQ('c','C',"checkbox",type) ||
                 STRCASEEQ('r','R',"radio",   type) ||
                 STRCASEEQ('h','H',"hidden",  type) ||
                 STRCASEEQ('s','S',"submit",  type) ||
                 STRCASEEQ('r','R',"reset",   type))) {
      W_L(" type=\"");
      W_V(type);
      W_L("\"");
    }
  }
  if (size && *size) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }
  if (name && *name) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(chxj_add_slash_to_doublequote(doc->pool,value));
    W_L("\"");
  }
  if (accesskey && *accesskey) {
    W_L(" accesskey=\"");
    W_V(accesskey);
    W_L("\"");
  }
  if (istyle && *istyle && (*istyle == '1' || *istyle == '2' || *istyle == '3' || *istyle == '4')) {
    char *fmt = qs_conv_istyle_to_format(r,istyle);
    if (max_length && *max_length) {
      int ii;
      for (ii=0; (unsigned int)ii<strlen(max_length); ii++) {
        if (max_length[ii] < '0' || max_length[ii] > '9') {
          max_length = apr_psprintf(r->pool, "0");
          break;
        }
      }

      if (strcmp(max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%d%s\"", atoi(max_length), fmt);
        W_V(vv);
      }
    }
    else {
      W_L(" FORMAT=\"");
      W_L("*");
      W_V(fmt);
      W_L("\"");
    }
  }
  else {
    if (max_length && *max_length) {
      if (chxj_chk_numeric(max_length) != 0) {
        max_length = apr_psprintf(r->pool, "0");
      }
      if (strcmp(max_length, "0")) {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%dm\"", atoi(max_length));
        W_V(vv);
      }
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (type && (istyle == NULL || *istyle == 0) && STRCASEEQ('p','P',"password", type) && ! xhtml->entryp->pc_flag) {
    if (max_length) {
      W_L(" FORMAT=\"");
      W_V(max_length);
      W_L("N\"");
    }
    else {
      W_L(" FORMAT=\"*N\"");
    }
  }
  if (checked) {
    W_L(" checked=\"checked\"");
  }
  W_L(" />");
  return xhtml->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_center_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<center>");
  return xhtml->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</center>");
  return xhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_hr_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<hr");
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('s','S',"size", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value && *value) {
        W_L(" size=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('w','W',"width", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value && *value) {
        W_L(" width=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('n','N',"noshade", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('c','C',"color", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  W_L(" />");
  return xhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_pre_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  xhtml->pre_flag++;
  W_L("<pre>");
  return xhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</pre>");
  xhtml->pre_flag--;

  return xhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_p_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *align = NULL;

  W_L("<p");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        align = apr_pstrdup(doc->buf.pool, val);
        break;
      }
    }
  }
  if (align) {
    W_L(" align=\"");
    W_V(align);
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</p>");
  return xhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_ul_tag(void *pdoc, Node *UNUSED(node)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<ul>");
  return xhtml->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</ul>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h1_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<h1");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h1>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h2_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr    *attr;

  W_L("<h2");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h2>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h3_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<h3");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h3>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h4_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<h4");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h4>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h5_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<h5");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h5>");
  return xhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_h6_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<h6");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;
    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value) {
        if (STRCASEEQ('l','L',"left",value)) {
          W_L(" style=\"");
          W_L("text-align:left");
          W_L("\"");
        }
        else if (STRCASEEQ('r','R',"right",value)) {
          W_L(" style=\"");
          W_L("text-align:right");
          W_L("\"");
        }
        else if (STRCASEEQ('c','C',"center",value)) {
          W_L(" style=\"");
          W_L("text-align:center");
          W_L("\"");
        }
        break;
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</h6>");
  return xhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_ol_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<ol");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value && (*value == '1' || *value == 'a' || *value == 'A')) {
      W_L(" type=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('s','S',"start",name) && value && *value) {
      W_L(" start=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</ol>");
  return xhtml->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_li_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  W_L("<li");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name) && value && (*value == '1' || *value == 'a' || *value == 'A')) {
      W_L(" type=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"value", name) && value && *value) {
      W_L(" value=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 ** It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</li>");
  return xhtml->out;
}

/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_img_tag(void *pdoc, Node *node) 
{
  xhtml_t       *xhtml = GET_XHTML(pdoc);
  Doc           *doc   = xhtml->doc;
  request_rec   *r     = doc->r;
  Attr          *attr;

#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = xhtml->spec;
#endif

  W_L("<img");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"src",name)) {
      value = chxj_encoding_parameter(r, value);
#ifdef IMG_NOT_CONVERT_FILENAME

      W_L(" src=\"");
      W_V(value);
      W_L("\"");

#else

      W_L(" src=\"");
      {
        char *vv = chxj_img_conv(r,spec,value);
        W_V(vv);
      }
      W_L("\"");

#endif
    }
    else 
    if (STRCASEEQ('a','A',"align",name)) {
      if (value && (STRCASEEQ('t','T',"top",   value) ||
                    STRCASEEQ('m','M',"middle",value) ||
                    STRCASEEQ('b','B',"bottom",value) ||
                    STRCASEEQ('l','L',"left",  value) ||
                    STRCASEEQ('r','R',"right", value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      W_L(" alt=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('w','W',"width",name) && value && *value) {
      W_L(" width=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"height",name) && value && *value) {
      W_L(" height=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"hspace",name) && value && *value) {
      W_L(" hspace=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"vspace",name) && value && *value) {
      W_L(" vspace=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(" />");
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
static char *
s_xhtml_1_0_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);

  return xhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_select_tag(void *pdoc, Node *child)
{
  xhtml_t *xhtml    = GET_XHTML(pdoc);
  Doc     *doc      = xhtml->doc;
  char    *size     = NULL;
  char    *name     = NULL;
  char    *multiple = NULL;
  Attr    *attr;


  W_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"size",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      multiple = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (size && *size) {
    W_L(" size=\"");
    W_V(size);
    W_L("\"");
  }
  if (name && *name) {
    W_L(" name=\"");
    W_V(name);
    W_L("\"");
  }
  if (multiple) {
    /* "true" is *NOT* W3C. it is specification of WAP2.0 for EZWEB */
    W_L(" multiple=\"true\"");
  }
  W_L(">\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</select>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_option_tag(void *pdoc, Node *child)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  char *selected   = NULL;
  char *value      = NULL;

  W_L("<option");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"selected",nm)) {
      /* CHTML version 2.0 */
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /* CHTML version 2.0 */
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected=\"selected\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</option>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_div_tag(void *pdoc, Node *child)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;
  char    *align   = NULL;

  W_L("<div");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",nm)) {
      /*=====================================================================*/
      /* CHTML version 3.2                                                   */
      /*=====================================================================*/
      if (val && (STRCASEEQ('l','L',"left",val) || STRCASEEQ('r','R',"right",val) || STRCASEEQ('c','C',"center",val))) {
        align = apr_pstrdup(doc->buf.pool, val);
      }
    }
  }
  if (align) {
    W_L(" align=\"");
    W_V(align);
    W_L("\"");
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</div>\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_b_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("<div style=\"font-weight:bold\">");
  return xhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_b_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</div>\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the CHXJ:IF tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CHXJ:IF tag node is specified.
 */
static char *
s_xhtml_1_0_chxjif_tag(void *pdoc, Node *node)
{
  xhtml_t      *xhtml = GET_XHTML(pdoc);
  Doc          *doc   = xhtml->doc;
  Node         *child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_xhtml_1_0_chxjif_tag(xhtml, child);
  }

  return NULL;
}


/**
 * It is a handler who processes the TEXTARE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_textarea_tag(void *pdoc, Node *node) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  Attr    *attr;

  xhtml->textarea_flag++;
  W_L("<textarea");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name",name) && value && *value) {
      W_L(" name=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('r','R',"rows",name) && value && *value) {
      W_L(" rows=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cols",name) && value && *value) {
      W_L(" cols=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('i','I',"istyle", name) && value && (*value == '1' || *value == '2' || *value == '3' || *value == '4')) {
      char *fmt = qs_conv_istyle_to_format(doc->r,value);
      W_L(" FORMAT=\"*");
      W_V(fmt);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey",name) && value && *value != 0) {
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">\r\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the TEXTAREA tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TEXTAREA tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;

  W_L("</textarea>\r\n");
  xhtml->textarea_flag--;

  return xhtml->out;
}

static char *
s_xhtml_1_0_text_tag(void *pdoc, Node *child)
{
  xhtml_t     *xhtml   = GET_XHTML(pdoc);
  Doc         *doc     = xhtml->doc;
  request_rec *r       = doc->r;
  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[2];
  int         ii;
  int         tdst_len;
  
  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return xhtml->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(r);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char *out;
    int rtn = s_xhtml_search_emoji(xhtml, &textval[ii], &out);
    if (rtn != 0) {
      DBG(r,"[%s][%d]", out, rtn);
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
    else if (xhtml->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (xhtml->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  W_V(tdst);
  return xhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("<blockquote>");
  return xhtml->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</blockquote>");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dir_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("<dir>");
  return xhtml->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</dir>");
  return xhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dl_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  W_L("<dl>");
  return xhtml->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  W_L("</dl>");
  return xhtml->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dt_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("<dt>");
  return xhtml->out;
}


/**
 * It is a handter who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</dt>");
  return xhtml->out;
}


/**
 * It is a handder who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_dd_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("<dd>");
  return xhtml->out;
}


/**
 * It is a handder who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc   = xhtml->doc;
  W_L("</dd>");
  return xhtml->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_menu_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml;
  Doc *doc;
  xhtml = GET_XHTML(pdoc);
  doc     = xhtml->doc;
  W_L("<menu>");
  return xhtml->out;
}


/**
 * It is a hanmenuer who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc *doc = xhtml->doc;
  W_L("</menu>");
  return xhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_plaintext_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  W_L("<plaintext>");
  s_xhtml_1_0_start_plaintext_tag_inner(pdoc,node);
  return xhtml->out;
}

static char *
s_xhtml_1_0_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  Node    *child;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_xhtml_1_0_start_plaintext_tag_inner(pdoc, child);
  }
  return xhtml->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc     = xhtml->doc;
  W_L("</plaintext>");
  return xhtml->out;
}


/**
 * It is a hanblinker who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_blink_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("<blink>");
  return xhtml->out;
}


/**
 * It is a hanblinker who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_blink_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</blink>");
  return xhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_start_marquee_tag(void *pdoc, Node *node)
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  Attr *attr;
  W_L("<marquee");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('d','D',"direction", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value))) {
        W_L(" direction=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('b','B',"behavior",name)) {
      if (value && (STRCASEEQ('s','S',"scroll",value) || STRCASEEQ('s','S',"slide",value) || STRCASEEQ('a','A',"alternate",value))) {
        W_L(" behavior=\""); 
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('l','L',"loop",name)) {
      if (value && *value) {
        W_L(" loop=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(">");
  return xhtml->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_xhtml_1_0_end_marquee_tag(void *pdoc, Node *UNUSED(child))
{
  xhtml_t *xhtml = GET_XHTML(pdoc);
  Doc     *doc = xhtml->doc;
  W_L("</marquee>");
  return xhtml->out;
}
/*
 * vim:ts=2 et
 */
