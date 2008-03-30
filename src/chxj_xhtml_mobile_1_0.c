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

#define BUFFERED 1

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

  ap_set_content_type(r, "text/html; charset=Windows-31J");

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
  xhtml->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
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
#if BUFFERED
  WX_L("<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, 
    xhtml->out, 
    "<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n", 
    NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Add DocType                                                              */
  /*--------------------------------------------------------------------------*/
#if BUFFERED
  WX_L("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n");
  WX_L(" \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, 
          xhtml->out, 
          "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n",NULL);
  xhtml->out = apr_pstrcat(r->pool, 
          xhtml->out, 
          " \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n",NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
#if BUFFERED
  WX_L("<html xmlns=\"http://www.w3.org/1999/xhtml\"");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<html", NULL);
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, 
          " xmlns=\"http://www.w3.org/1999/xhtml\"", NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('l','L',"lang", name)) {
#if BUFFERED
      WX_L(" xml:lang=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " xml:lang=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('v','V',"version", name)) {
#if BUFFERED
      WX_L(" version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " version=\"-//OPENWAVE//DTD XHTML Mobile 1.0//EN\"", 
                      NULL);
#endif
    }
  }

#if BUFFERED
  WX_L(">\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\r\n", NULL);
#endif
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

#if BUFFERED
  WX_L("</html>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</html>\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<meta");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<meta", NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name", name)) {
#if BUFFERED
      WX_L(" ");
      WX_V(name);
      WX_L("=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool,
                      xhtml->out,
                      " ", 
                      name, 
                      "=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('h','H',"http-equiv", name)) {
#if BUFFERED
      WX_L(" ");
      WX_V(name);
      WX_L("=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool,
                      xhtml->out,
                      " ", 
                      name,
                      "=\"",
                      value,
                      "\"", 
                      NULL);
#endif
      if (STRCASEEQ('c','C', "content-type", value)) {
        content_type_flag = 1;
      }
    }
    else if (STRCASEEQ('c','C',"content", name)) {
      if (content_type_flag) {
#if BUFFERED
        WX_L(" ");
        WX_V(name);
        WX_L("=\"");
        WX_L("text/html; charset=Windows-31J");
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool,
                        xhtml->out, 
                        " ", 
                        name, 
                        "=\"", 
                        "text/html; charset=Windows-31J", 
                        "\"", 
                        NULL);
#endif
      }
      else {
#if BUFFERED
        WX_L(" ");
        WX_V(name);
        WX_L("=\"");
        WX_V(value);
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool,
                        xhtml->out, 
                        " ", 
                        name, 
                        "=\"", 
                        value, 
                        "\"", 
                        NULL);
#endif
      }
    }
  }

#if BUFFERED
  WX_L(" />\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, 
                  xhtml->out, 
                  " />\r\n", 
                  NULL);
#endif

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

#if BUFFERED
  WX_L("<head>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<head>\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("</head>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</head>\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<title>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<title>", NULL);
#endif

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

#if BUFFERED
  WX_L("</title>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</title>\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<base");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<base", NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href",name)) {
#if BUFFERED
      WX_L(" href=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
      break;
    }
  }
#if BUFFERED
  WX_L(" />\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, " />\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<body");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<body", NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B',"bgcolor", name)) {
#if BUFFERED
      WX_L(" bgcolor=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " bgcolor=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('t','T',"text",name)) {
#if BUFFERED
      WX_L(" text=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " text=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('l','L',"link", name)) {
#if BUFFERED
      WX_L(" link=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " link=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"alink", name)) {
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /* ignore */
    }
  }
#if BUFFERED
  WX_L(">\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("</body>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</body>\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<a");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<a", NULL);
#endif

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name",name)) {
#if BUFFERED
      WX_L(" id=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " id=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('h','H',"href", name)) {
      value = chxj_encoding_parameter(r, value);
#if BUFFERED
      WX_L(" href=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
#if BUFFERED
      WX_L(" accesskey=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " accesskey=\"", 
                      value, 
                      "\"", 
                      NULL);
#endif
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
#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("</a>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</a>", NULL);
#endif

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

#if BUFFERED
  WX_L("<br />\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<br />\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<br />\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<br />\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<font");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<font", NULL);
#endif

  /* Get Attributes */
  for (attr = qs_get_attr(doc,node);
       attr; attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color",name)) {
#if BUFFERED
      WX_L(" color=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " color=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('s','S',"size",name)) {
#if BUFFERED
      WX_L(" size=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " size=\"",value,"\"", NULL);
#endif
    }
  }
#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("</font>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</font>", NULL);
#endif

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

#if BUFFERED
  WX_L("<form");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<form", NULL);
#endif

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
#if BUFFERED
      WX_L(" action=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " action=\"",
                      value,
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('m','M',"method",name)) {
#if BUFFERED
      WX_L(" method=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " method=\"",
                      value,
                      "\"", 
                      NULL);
#endif
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /* ignore */
    }
  }
#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("</form>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</form>", NULL);
#endif

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

#if BUFFERED
  WX_L("<input");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<input", NULL);
#endif

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
#if BUFFERED
    WX_L(" type=\"");
    WX_V(type);
    WX_L("\" ");
#else
    xhtml->out = apr_pstrcat(r->pool,
                    xhtml->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
#endif
  }
  if (size) {
#if BUFFERED
    WX_L(" size=\"");
    WX_V(size);
    WX_L("\" ");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
#endif
  }
  if (name) {
#if BUFFERED
    WX_L(" name=\"");
    WX_V(name);
    WX_L("\" ");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
#endif
  }
  if (value) {
#if BUFFERED
    WX_L(" value=\"");
    WX_V(value);
    WX_L("\" ");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
#endif
  }
  if (accesskey) {
#if BUFFERED
    WX_L(" accesskey=\"");
    WX_V(accesskey);
    WX_L("\" ");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
#endif
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

#if BUFFERED
      {
        char *vv = apr_psprintf(r->pool, " FORMAT=\"%d%s\"", atoi(max_length), fmt);
        WX_V(vv);
      }
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      apr_psprintf(r->pool, " FORMAT=\"%d%s\"", 
                      atoi(max_length), 
                      fmt), 
                      NULL);
#endif
    }
    else {
#if BUFFERED
      WX_L(" FORMAT=\"");
      WX_L("*");
      WX_V(fmt);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", 
                      "*", 
                      fmt, 
                      "\"", 
                      NULL);
#endif
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
#if BUFFERED
      WX_L(" FORMAT=\"");
      WX_V(max_length);
      WX_L("N\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", 
                      max_length, 
                      "N", 
                      "\"", 
                      NULL);
#endif
    }
    else {
#if BUFFERED
      WX_L(" FORMAT=\"*N\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, 
                      " FORMAT=\"", "*", "N", "\"", NULL);
#endif
    }
  }
  if (checked) {
#if BUFFERED
    WX_L(" checked=\"checked\"");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, " checked=\"checked\"", NULL);
#endif
  }

#if BUFFERED
  WX_L(" />");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, " />", NULL);
#endif

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

#if BUFFERED
  WX_L("<center>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<center>", NULL);
#endif

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

#if BUFFERED
  WX_L("</center>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</center>", NULL);
#endif

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

#if BUFFERED
  WX_L("<hr />");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<hr />", NULL);
#endif

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
#if BUFFERED
  WX_L("<pre>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<pre>", NULL);
#endif

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

#if BUFFERED
  WX_L("</pre>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</pre>", NULL);
#endif
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

#if BUFFERED
  WX_L("<p>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<p>", NULL);
#endif

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

#if BUFFERED
  WX_L("</p>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</p>", NULL);
#endif

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

#if BUFFERED
  WX_L("<ul>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<ul>", NULL);
#endif

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

#if BUFFERED
  WX_L("</ul>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</ul>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h1>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h1>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h1>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h1>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h2>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h2>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h2>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h2>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h3>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h3>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h3>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h3>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h4>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h4>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h4>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h4>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h5>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h5>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h5>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h5>", NULL);
#endif

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

#if BUFFERED
  WX_L("<h6>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<h6>", NULL);
#endif

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

#if BUFFERED
  WX_L("</h6>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</h6>", NULL);
#endif

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

#if BUFFERED
  WX_L("<ol>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<ol>", NULL);
#endif

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

#if BUFFERED
  WX_L("</ol>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</ol>", NULL);
#endif

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

#if BUFFERED
  WX_L("<li>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<li>", NULL);
#endif

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

#if BUFFERED
  WX_L("</li>");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</li>", NULL);
#endif

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

#if BUFFERED
  WX_L("<img");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<img", NULL);
#endif

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

#if BUFFERED
      WX_L(" src=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " src=\"",value,"\"", NULL);
#endif

#else

#if BUFFERED
      WX_L(" src=\"");
      {
        char *vv = chxj_img_conv(r,spec,value);
        WX_V(vv);
      }
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out,
                      " src=\"", chxj_img_conv(r,spec,value) , NULL);
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, "\"", NULL);
#endif

#endif
    }
    else 
    if (*name == 'a' || *name == 'A') {
      if (strcasecmp(name, "align" ) == 0) {
#if BUFFERED
        WX_L(" align=\"");
        WX_V(value);
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool, 
                        xhtml->out, " align=\"",value,"\"", NULL);
#endif
      }
      else
      if (strcasecmp(name, "alt"   ) == 0) {
#if BUFFERED
        WX_L(" alt=\"");
        WX_V(value);
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool, 
                        xhtml->out, " alt=\"",value,"\"", NULL);
#endif
      }
    }
    else if (STRCASEEQ('w','W',"width",name)) {
#if BUFFERED
      WX_L(" width=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " width=\"",value,"\"", NULL);
#endif
    }
    else 
    if (*name == 'h' || *name == 'H') {
      if (strcasecmp(name, "height") == 0) {
#if BUFFERED
        WX_L(" height=\"");
        WX_V(value);
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool, 
                        xhtml->out, " height=\"",value,"\"", NULL);
#endif
      }
      else
      if (strcasecmp(name, "hspace") == 0) {
#if BUFFERED
        WX_L(" hspace=\"");
        WX_V(value);
        WX_L("\"");
#else
        xhtml->out = apr_pstrcat(r->pool, 
                        xhtml->out, " hspace=\"",value,"\"", NULL);
#endif
      }
    }
    else if (STRCASEEQ('v','V',"vspace",name)) {
#if BUFFERED
      WX_L(" vspace=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, 
                      xhtml->out, " vspace=\"",value,"\"", NULL);
#endif
    }
  }

#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("<select");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<select", NULL);
#endif
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
#if BUFFERED
    WX_L(" size=\"");
    WX_V(size);
    WX_L("\"");
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " size=\"",size,"\"", NULL);
#endif
  }

  if (name) {
#if BUFFERED
    WX_L(" name=\"");
    WX_V(name);
    WX_L("\"");
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " name=\"",name,"\"", NULL);
#endif
  }

#if BUFFERED
  WX_L(">\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\n", NULL);
#endif

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

#if BUFFERED
  WX_L("</select>\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</select>\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<option");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<option", NULL);
#endif
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
#if BUFFERED
    WX_L(" value=\"");
    WX_V(value);
    WX_L("\"");
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " value=\"",value,"\"", NULL);
#endif
  }
  else {
#if BUFFERED
    WX_L(" value=\"\"");
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " value=\"\"", NULL);
#endif
  }

  if (selected) {
#if BUFFERED
    WX_L(" selected=\"selected\"");
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, " selected=\"selected\"", NULL);
#endif
  }

#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("</option>\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</option>\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<div");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<div", NULL);
#endif
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
#if BUFFERED
    WX_L(" align=\"");
    WX_V(align);
    WX_L("\"");
#else
    xhtml->out = apr_pstrcat(r->pool, 
                    xhtml->out, " align=\"", align, "\"", NULL);
#endif
  }

#if BUFFERED
  WX_L(">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">", NULL);
#endif

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

#if BUFFERED
  WX_L("</div>\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</div>\n", NULL);
#endif

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

#if BUFFERED
  WX_L("<div style=\"font-weight:bold\">");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<div style=\"font-weight:bold\">", NULL);
#endif

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

#if BUFFERED
  WX_L("</div>\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</div>\n", NULL);
#endif

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
#if BUFFERED
    WX_V(child->otext);
#else
    xhtml->out = apr_pstrcat(r->pool, xhtml->out, child->otext, NULL);
#endif
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
#if BUFFERED
  WX_L("<textarea ");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "<textarea ", NULL);
#endif

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name",name)) {
#if BUFFERED
      WX_L(" name=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " name=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('r','R',"rows",name)) {
#if BUFFERED
      WX_L(" rows=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " rows=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('c','C',"cols",name)) {
#if BUFFERED
      WX_L(" cols=\"");
      WX_V(value);
      WX_L("\"");
#else
      xhtml->out = apr_pstrcat(r->pool, xhtml->out, " cols=\"",value,"\"", NULL);
#endif
    }
  }

#if BUFFERED
  WX_L(">\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, ">\r\n", NULL);
#endif

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

#if BUFFERED
  WX_L("</textarea>\r\n");
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, "</textarea>\r\n", NULL);
#endif
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
  textval = qs_trim_string(xhtml->doc->r, textval);
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
#if BUFFERED
  WX_V(tdst);
#else
  xhtml->out = apr_pstrcat(r->pool, xhtml->out, tdst, NULL);
#endif

  return xhtml->out;
}
/*
 * vim:ts=2 et
 */
