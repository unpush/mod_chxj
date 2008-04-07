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

#define GET_XHTML(X) ((xhtml_t*)(X))
#define WX_L(X)          do { xhtml->out = BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, (X)); } while(0)
#define WX_V(X)          do { xhtml->out = (X) ? BUFFERED_WRITE_VALUE(xhtml->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(xhtml->out, &doc->buf, ""); } while(0)

static char* s_xhtml_1_0_start_html_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_html_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_p_tag      (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_p_tag        (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_pre_tag    (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_pre_tag      (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_ul_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_ul_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h1_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h1_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h2_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h2_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h3_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h3_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h4_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h4_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h5_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h5_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_h6_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_h6_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_ol_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_ol_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_li_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_li_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_meta_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_meta_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_head_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_head_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_title_tag  (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_title_tag    (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_base_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_base_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_body_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_body_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_a_tag      (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_a_tag        (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_br_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_br_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_tr_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_tr_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_font_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_font_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_form_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_form_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_input_tag  (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_input_tag    (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_center_tag (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_center_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_hr_tag     (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_hr_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_img_tag    (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_img_tag      (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_select_tag (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_select_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_option_tag (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_option_tag   (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_div_tag    (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_div_tag      (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_textarea_tag(void* pdoc, Node* node);
static char* s_xhtml_1_0_end_textarea_tag  (void* pdoc, Node* node);
static char* s_xhtml_1_0_start_b_tag       (void* pdoc, Node* node);
static char* s_xhtml_1_0_end_b_tag         (void* pdoc, Node* node);
static char* s_xhtml_1_0_chxjif_tag       (void* pdoc, Node* node);

static void  s_init_xhtml(xhtml_t* xhtml, Doc* doc, request_rec* r, device_table* spec);
static int   s_xhtml_search_emoji(xhtml_t* xhtml, char* txt, char** rslt);
static char* s_xhtml_1_0_text_tag(void* pdoc, Node* child);


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
    NULL,
    NULL,
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
    NULL,
    NULL,
  },
  /* tagDIR */
  {
    NULL,
    NULL,
  },
  /* tagDL */
  {
    NULL,
    NULL,
  },
  /* tagDD */
  {
    NULL,
    NULL,
  },
  /* tagMENU */
  {
    NULL,
    NULL,
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
char*
chxj_exchange_xhtml_mobile_1_0(
  request_rec*    r,
  device_table*   spec,
  const char*     src,
  apr_size_t      srclen,
  apr_size_t*     dstlen,
  chxjconvrule_entry* entryp,
  cookie_t*   UNUSED(cookie)
)
{
  char*     dst = NULL;
  char*     ss;
  xhtml_t   xhtml;
  Doc       doc;

  DBG(r,"start chxj_exchange_xhtml_mobile_1_0()");

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst != NULL) {
    DBG(r,"i found qrcode xml");
    return dst;
  }
  DBG(r,"not found qrcode xml");

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

  if (! dst) 
    return apr_pstrdup(r->pool,ss);

  if (! strlen(dst))
    dst = apr_psprintf(r->pool, "\n");

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
s_init_xhtml(xhtml_t* xhtml, Doc* doc, request_rec* r, device_table* spec)
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
s_xhtml_search_emoji(xhtml_t* xhtml, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
  int           len;

  spec = xhtml->spec;

  len = strlen(txt);
  r = xhtml->doc->r;

  if (spec == NULL)
    DBG(r,"spec is NULL");

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
static char*
s_xhtml_1_0_start_html_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Attr*         attr;
  Doc*          doc   = xhtml->doc;

  /*--------------------------------------------------------------------------*/
  /* Add XML Declare                                                          */
  /*--------------------------------------------------------------------------*/
  WX_L("<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n");

  /*--------------------------------------------------------------------------*/
  /* Add DocType                                                              */
  /*--------------------------------------------------------------------------*/
  WX_L("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n");
  WX_L(" \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n");

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  WX_L("<html xmlns=\"http://www.w3.org/1999/xhtml\"");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('l','L',"lang", name)) {
      WX_L(" xml:lang=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('v','V',"version", name)) {
      WX_L(" version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"");
    }
  }

  WX_L(">\r\n");
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
static char*
s_xhtml_1_0_end_html_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</html>\r\n");

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
static char*
s_xhtml_1_0_start_meta_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Attr*         attr;
  Doc*          doc   = xhtml->doc;
  int           content_type_flag = 0;

  WX_L("<meta");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name", name)) {
      WX_L(" ");
      WX_V(name);
      WX_L("=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('h','H',"http-equiv", name)) {
      WX_L(" ");
      WX_V(name);
      WX_L("=\"");
      WX_V(value);
      WX_L("\"");
      if (STRCASEEQ('c','C', "content-type", value)) {
        content_type_flag = 1;
      }
    }
    else if (STRCASEEQ('c','C',"content", name)) {
      if (content_type_flag) {
        WX_L(" ");
        WX_V(name);
        WX_L("=\"");
        WX_L("text/html; charset=Windows-31J");
        WX_L("\"");
      }
      else {
        WX_L(" ");
        WX_V(name);
        WX_L("=\"");
        WX_V(value);
        WX_L("\"");
      }
    }
  }

  WX_L(" />\r\n");
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
static char*
s_xhtml_1_0_end_meta_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*  xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_head_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<head>\r\n");
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
static char*
s_xhtml_1_0_end_head_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</head>\r\n");
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
static char*
s_xhtml_1_0_start_title_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("<title>");
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
static char*
s_xhtml_1_0_end_title_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</title>\r\n");

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
static char*
s_xhtml_1_0_start_base_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Attr*         attr;
  Doc*          doc   = xhtml->doc;

  WX_L("<base");

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href",name)) {
      WX_L(" href=\"");
      WX_V(value);
      WX_L("\"");
      break;
    }
  }
  WX_L(" />\r\n");
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
static char*
s_xhtml_1_0_end_base_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t* xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_body_tag(void* pdoc, Node* node) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Attr*        attr;

  WX_L("<body");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B',"bgcolor", name)) {
      WX_L(" bgcolor=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('t','T',"text",name)) {
      WX_L(" text=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('l','L',"link", name)) {
      WX_L(" link=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('a','A',"alink", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /* ignore */
    }
  }
  WX_L(">\r\n");

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
static char*
s_xhtml_1_0_end_body_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</body>\r\n");
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
static char*
s_xhtml_1_0_start_a_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  WX_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name",name)) {
      WX_L(" id=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('h','H',"href", name)) {
      value = chxj_encoding_parameter(r, value);
      WX_L(" href=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
      WX_L(" accesskey=\"");
      WX_V(value);
      WX_L("\"");
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
  WX_L(">");
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
static char*
s_xhtml_1_0_end_a_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</a>");
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
static char*
s_xhtml_1_0_start_br_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("<br />\r\n");
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
static char*
s_xhtml_1_0_end_br_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t* xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_tr_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("<br />\r\n");
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
static char*
s_xhtml_1_0_end_tr_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t* xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_font_tag(void* pdoc, Node* node) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Attr*        attr;

  WX_L("<font");
  /* Get Attributes */
  for (attr = qs_get_attr(doc,node);
       attr; attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color",name)) {
      WX_L(" color=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('s','S',"size",name)) {
      WX_L(" size=\"");
      WX_V(value);
      WX_L("\"");
    }
  }
  WX_L(">");
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
static char*
s_xhtml_1_0_end_font_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</font>");
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
static char*
s_xhtml_1_0_start_form_tag(void* pdoc, Node* node) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  WX_L("<form");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"action",name)) {
      value = chxj_encoding_parameter(r, value);
      WX_L(" action=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('m','M',"method",name)) {
      WX_L(" method=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /* ignore */
    }
  }
  WX_L(">");
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
static char*
s_xhtml_1_0_end_form_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</form>");
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
static char*
s_xhtml_1_0_start_input_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml       = GET_XHTML(pdoc);
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

  WX_L("<input");
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
    WX_L(" type=\"");
    WX_V(type);
    WX_L("\" ");
  }
  if (size) {
    WX_L(" size=\"");
    WX_V(size);
    WX_L("\" ");
  }
  if (name) {
    WX_L(" name=\"");
    WX_V(name);
    WX_L("\" ");
  }
  if (value) {
    WX_L(" value=\"");
    WX_V(value);
    WX_L("\" ");
  }
  if (accesskey) {
    WX_L(" accesskey=\"");
    WX_V(accesskey);
    WX_L("\" ");
  }
  if (istyle) {
    char* fmt = qs_conv_istyle_to_format(r,istyle);
    if (max_length) {
      int ii;
      for (ii=0; (unsigned int)ii<strlen(max_length); ii++) {
        if (max_length[ii] < '0' || max_length[ii] > '9') {
          max_length = apr_psprintf(r->pool, "0");
          break;
        }
      }

      {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%d%s\"", atoi(max_length), fmt);
        WX_V(vv);
      }
    }
    else {
      WX_L(" FORMAT=\"");
      WX_L("*");
      WX_V(fmt);
      WX_L("\"");
    }
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (type && istyle == NULL 
      && (*type == 'p' || *type == 'P') 
      && strcasecmp(type, "password") == 0
      && ! xhtml->entryp->pc_flag) {
    if (max_length) {
      WX_L(" FORMAT=\"");
      WX_V(max_length);
      WX_L("N\"");
    }
    else {
      WX_L(" FORMAT=\"*N\"");
    }
  }
  if (checked) {
    WX_L(" checked=\"checked\"");
  }
  WX_L(" />");
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
static char*
s_xhtml_1_0_end_input_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t* xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_center_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<center>");
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
static char*
s_xhtml_1_0_end_center_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</center>");
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
static char*
s_xhtml_1_0_start_hr_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<hr />");
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
static char*
s_xhtml_1_0_end_hr_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t* xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_pre_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  xhtml->pre_flag++;
  WX_L("<pre>");
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
static char*
s_xhtml_1_0_end_pre_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</pre>");
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
static char*
s_xhtml_1_0_start_p_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<p>");
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
static char*
s_xhtml_1_0_end_p_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</p>");
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
static char*
s_xhtml_1_0_start_ul_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<ul>");
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
static char*
s_xhtml_1_0_end_ul_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</ul>");
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
static char*
s_xhtml_1_0_start_h1_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<h1>");
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
static char*
s_xhtml_1_0_end_h1_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h1>");
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
static char*
s_xhtml_1_0_start_h2_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc = xhtml->doc;

  WX_L("<h2>");
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
static char*
s_xhtml_1_0_end_h2_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h2>");
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
static char*
s_xhtml_1_0_start_h3_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<h3>");
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
static char*
s_xhtml_1_0_end_h3_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h3>");
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
static char*
s_xhtml_1_0_start_h4_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<h4>");
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
static char*
s_xhtml_1_0_end_h4_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h4>");
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
static char*
s_xhtml_1_0_start_h5_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<h5>");
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
static char*
s_xhtml_1_0_end_h5_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h5>");
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
static char*
s_xhtml_1_0_start_h6_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<h6>");
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
static char*
s_xhtml_1_0_end_h6_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</h6>");
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
static char*
s_xhtml_1_0_start_ol_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<ol>");
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
static char*
s_xhtml_1_0_end_ol_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</ol>");
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
static char*
s_xhtml_1_0_start_li_tag(void* pdoc, Node* UNUSED(node)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("<li>");
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
static char*
s_xhtml_1_0_end_li_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</li>");
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
static char*
s_xhtml_1_0_start_img_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

#ifndef IMG_NOT_CONVERT_FILENAME
  device_table  *spec = xhtml->spec;
#endif

  WX_L("<img");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"src",name)) {
      value = chxj_encoding_parameter(r, value);
#ifdef IMG_NOT_CONVERT_FILENAME

      WX_L(" src=\"");
      WX_V(value);
      WX_L("\"");

#else

      WX_L(" src=\"");
      {
        char *vv = chxj_img_conv(r,spec,value);
        WX_V(vv);
      }
      WX_L("\"");

#endif
    }
    else 
    if (*name == 'a' || *name == 'A') {
      if (strcasecmp(name, "align" ) == 0) {
        WX_L(" align=\"");
        WX_V(value);
        WX_L("\"");
      }
      else
      if (strcasecmp(name, "alt"   ) == 0) {
        WX_L(" alt=\"");
        WX_V(value);
        WX_L("\"");
      }
    }
    else if (STRCASEEQ('w','W',"width",name)) {
      WX_L(" width=\"");
      WX_V(value);
      WX_L("\"");
    }
    else 
    if (*name == 'h' || *name == 'H') {
      if (strcasecmp(name, "height") == 0) {
        WX_L(" height=\"");
        WX_V(value);
        WX_L("\"");
      }
      else
      if (strcasecmp(name, "hspace") == 0) {
        WX_L(" hspace=\"");
        WX_V(value);
        WX_L("\"");
      }
    }
    else if (STRCASEEQ('v','V',"vspace",name)) {
      WX_L(" vspace=\"");
      WX_V(value);
      WX_L("\"");
    }
  }
  WX_L(">");
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
s_xhtml_1_0_end_img_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*  xhtml = GET_XHTML(pdoc);

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
static char*
s_xhtml_1_0_start_select_tag(void* pdoc, Node* child)
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Attr*        attr;

  char* size      = NULL;
  char* name      = NULL;

  WX_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"size",nm)) {
      /* CHTML version 2.0 */
      size = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name",nm)) {
      /* CHTML version 2.0 */
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple",nm)) {
      /* CHTML version 2.0 */
      /* Ignore */
    }
  }

  if (size) {
    WX_L(" size=\"");
    WX_V(size);
    WX_L("\"");
  }

  if (name) {
    WX_L(" name=\"");
    WX_V(name);
    WX_L("\"");
  }
  WX_L(">\n");
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
static char*
s_xhtml_1_0_end_select_tag(void* pdoc, Node* UNUSED(child))
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</select>\n");
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
static char*
s_xhtml_1_0_start_option_tag(void* pdoc, Node* child)
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Attr*        attr;

  char* selected   = NULL;
  char* value      = NULL;

  WX_L("<option");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"selected",nm)) {
      /* CHTML version 2.0 */
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /* CHTML version 2.0 */
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (value) {
    WX_L(" value=\"");
    WX_V(value);
    WX_L("\"");
  }
  else {
    WX_L(" value=\"\"");
  }
  if (selected) {
    WX_L(" selected=\"selected\"");
  }
  WX_L(">");
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
static char*
s_xhtml_1_0_end_option_tag(void* pdoc, Node* UNUSED(child))
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</option>\n");
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
static char*
s_xhtml_1_0_start_div_tag(void* pdoc, Node* child)
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Attr*        attr;

  char* align   = NULL;

  WX_L("<div");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A',"align",nm)) {
      /* CHTML version 3.2 */
      align = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (align) {
    WX_L(" align=\"");
    WX_V(align);
    WX_L("\"");
  }

  WX_L(">");
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
static char*
s_xhtml_1_0_end_div_tag(void* pdoc, Node* UNUSED(child))
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</div>\n");
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
static char*
s_xhtml_1_0_start_b_tag(void* pdoc, Node* UNUSED(child))
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("<div style=\"font-weight:bold\">");
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
static char*
s_xhtml_1_0_end_b_tag(void* pdoc, Node* UNUSED(child))
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;

  WX_L("</div>\n");
  return xhtml->out;
}


/**
 * It is a handler who processes the CHXJ:IF tag.
 *
 * @param pdoc  [i/o] The pointer to the XHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CHXJ:IF tag node is specified.
 */
static char*
s_xhtml_1_0_chxjif_tag(void* pdoc, Node* node)
{
  xhtml_t*     xhtml = GET_XHTML(pdoc);
  Doc*         doc   = xhtml->doc;
  Node*        child;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    WX_V(child->otext);
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
static char*
s_xhtml_1_0_start_textarea_tag(void* pdoc, Node* node) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;
  Attr*         attr;

  xhtml->textarea_flag++;
  WX_L("<textarea ");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name",name)) {
      WX_L(" name=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('r','R',"rows",name)) {
      WX_L(" rows=\"");
      WX_V(value);
      WX_L("\"");
    }
    else if (STRCASEEQ('c','C',"cols",name)) {
      WX_L(" cols=\"");
      WX_V(value);
      WX_L("\"");
    }
  }
  WX_L(">\r\n");
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
static char*
s_xhtml_1_0_end_textarea_tag(void* pdoc, Node* UNUSED(child)) 
{
  xhtml_t*      xhtml = GET_XHTML(pdoc);
  Doc*          doc   = xhtml->doc;

  WX_L("</textarea>\r\n");
  xhtml->textarea_flag--;

  return xhtml->out;
}

static char*
s_xhtml_1_0_text_tag(void* pdoc, Node* child)
{
  xhtml_t*     xhtml   = GET_XHTML(pdoc);
  Doc*         doc     = xhtml->doc;
  request_rec* r       = doc->r;
  char*    textval;
  char*    tmp;
  char*    tdst;
  char     one_byte[2];
  int      ii;
  int      tdst_len;
  
  textval = qs_get_node_value(doc,child);
  textval = qs_trim_string(doc->buf.pool, textval);
  if (strlen(textval) == 0)
    return xhtml->out;
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(r);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
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
  WX_V(tdst);
  return xhtml->out;
}
/*
 * vim:ts=2 et
 */
