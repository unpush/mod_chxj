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
#include <apr_time.h>
#include "chxj_chtml40.h"
#include "chxj_hdml.h"
#include "chxj_str_util.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"

#define GET_CHTML40(X) ((chtml40_t *)(X))
#undef W_L
#undef W_V
#define W_L(X)          do { chtml40->out = BUFFERED_WRITE_LITERAL(chtml40->out, &doc->buf, (X)); } while(0)
#define W_V(X)          do { chtml40->out = (X) ? BUFFERED_WRITE_VALUE(chtml40->out, &doc->buf, (X))  \
                                                : BUFFERED_WRITE_LITERAL(chtml40->out, &doc->buf, ""); } while(0)
#undef W_NLCODE
#define W_NLCODE()     do { char *nlcode = TO_NLCODE(chtml40->conf); W_V(nlcode); } while (0)

static char *s_chtml40_start_html_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_html_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_meta_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_meta_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_textarea_tag (void *pdoc, Node *node);
static char *s_chtml40_end_textarea_tag   (void *pdoc, Node *node);
static char *s_chtml40_start_p_tag        (void *pdoc, Node *node);
static char *s_chtml40_end_p_tag          (void *pdoc, Node *node);
static char *s_chtml40_start_pre_tag      (void *pdoc, Node *node);
static char *s_chtml40_end_pre_tag        (void *pdoc, Node *node);
static char *s_chtml40_start_h1_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h1_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_h2_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h2_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_h3_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h3_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_h4_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h4_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_h5_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h5_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_h6_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_h6_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_ul_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_ul_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_ol_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_ol_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_li_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_li_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_head_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_head_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_title_tag    (void *pdoc, Node *node);
static char *s_chtml40_end_title_tag      (void *pdoc, Node *node);
static char *s_chtml40_start_base_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_base_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_body_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_body_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_a_tag        (void *pdoc, Node *node);
static char *s_chtml40_end_a_tag          (void *pdoc, Node *node);
static char *s_chtml40_start_br_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_br_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_tr_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_tr_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_font_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_font_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_form_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_form_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_input_tag    (void *pdoc, Node *node);
static char *s_chtml40_end_input_tag      (void *pdoc, Node *node);
static char *s_chtml40_start_center_tag   (void *pdoc, Node *node);
static char *s_chtml40_end_center_tag     (void *pdoc, Node *node);
static char *s_chtml40_start_hr_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_hr_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_img_tag      (void *pdoc, Node *node);
static char *s_chtml40_end_img_tag        (void *pdoc, Node *node);
static char *s_chtml40_start_select_tag   (void *pdoc, Node *node);
static char *s_chtml40_end_select_tag     (void *pdoc, Node *node);
static char *s_chtml40_start_option_tag   (void *pdoc, Node *node);
static char *s_chtml40_end_option_tag     (void *pdoc, Node *node);
static char *s_chtml40_start_div_tag      (void *pdoc, Node *node);
static char *s_chtml40_end_div_tag        (void *pdoc, Node *node);
static char *s_chtml40_chxjif_tag         (void *pdoc, Node *node); 
static char *s_chtml40_text_tag           (void *pdoc, Node *node);
static char *s_chtml40_start_blockquote_tag(void *pdoc, Node *node);
static char *s_chtml40_end_blockquote_tag  (void *pdoc, Node *node);
static char *s_chtml40_start_dir_tag      (void *pdoc, Node *node);
static char *s_chtml40_end_dir_tag        (void *pdoc, Node *node);
static char *s_chtml40_start_dl_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_dl_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_dt_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_dt_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_dd_tag       (void *pdoc, Node *node);
static char *s_chtml40_end_dd_tag         (void *pdoc, Node *node);
static char *s_chtml40_start_marquee_tag  (void *pdoc, Node *node);
static char *s_chtml40_end_marquee_tag    (void *pdoc, Node *node);
static char *s_chtml40_start_blink_tag    (void *pdoc, Node *node);
static char *s_chtml40_end_blink_tag      (void *pdoc, Node *node);
static char *s_chtml40_start_menu_tag     (void *pdoc, Node *node);
static char *s_chtml40_end_menu_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_plaintext_tag       (void *pdoc, Node *node);
static char *s_chtml40_start_plaintext_tag_inner (void *pdoc, Node *node);
static char *s_chtml40_end_plaintext_tag         (void *pdoc, Node *node);

static void  s_init_chtml40(chtml40_t *chtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_chtml40_search_emoji(chtml40_t *chtml, char *txt, char **rslt);


tag_handler chtml40_handler[] = {
  /* tagHTML */
  {
    s_chtml40_start_html_tag,
    s_chtml40_end_html_tag,
  },
  /* tagMETA */
  {
    s_chtml40_start_meta_tag,
    s_chtml40_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_chtml40_start_textarea_tag,
    s_chtml40_end_textarea_tag,
  },
  /* tagP */
  {
    s_chtml40_start_p_tag,
    s_chtml40_end_p_tag,
  },
  /* tagPRE */
  {
    s_chtml40_start_pre_tag,
    s_chtml40_end_pre_tag,
  },
  /* tagUL */
  {
    s_chtml40_start_ul_tag,
    s_chtml40_end_ul_tag,
  },
  /* tagLI */
  {
    s_chtml40_start_li_tag,
    s_chtml40_end_li_tag,
  },
  /* tagOL */
  {
    s_chtml40_start_ol_tag,
    s_chtml40_end_ol_tag,
  },
  /* tagH1 */
  {
    s_chtml40_start_h1_tag,
    s_chtml40_end_h1_tag,
  },
  /* tagH2 */
  {
    s_chtml40_start_h2_tag,
    s_chtml40_end_h2_tag,
  },
  /* tagH3 */
  {
    s_chtml40_start_h3_tag,
    s_chtml40_end_h3_tag,
  },
  /* tagH4 */
  {
    s_chtml40_start_h4_tag,
    s_chtml40_end_h4_tag,
  },
  /* tagH5 */
  {
    s_chtml40_start_h5_tag,
    s_chtml40_end_h5_tag,
  },
  /* tagH6 */
  {
    s_chtml40_start_h6_tag,
    s_chtml40_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_chtml40_start_head_tag,
    s_chtml40_end_head_tag,
  },
  /* tagTITLE */
  {
    s_chtml40_start_title_tag,
    s_chtml40_end_title_tag,
  },
  /* tagBASE */
  {
    s_chtml40_start_base_tag,
    s_chtml40_end_base_tag,
  },
  /* tagBODY */
  {
    s_chtml40_start_body_tag,
    s_chtml40_end_body_tag,
  },
  /* tagA */
  {
    s_chtml40_start_a_tag,
    s_chtml40_end_a_tag,
  },
  /* tagBR */
  {
    s_chtml40_start_br_tag,
    s_chtml40_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_chtml40_start_tr_tag,
    s_chtml40_end_tr_tag,
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
    s_chtml40_start_font_tag,
    s_chtml40_end_font_tag,
  },
  /* tagFORM */
  {
    s_chtml40_start_form_tag,
    s_chtml40_end_form_tag,
  },
  /* tagINPUT */
  {
    s_chtml40_start_input_tag,
    s_chtml40_end_input_tag,
  },
  /* tagCENTER */
  {
    s_chtml40_start_center_tag,
    s_chtml40_end_center_tag,
  },
  /* tagHR */
  {
    s_chtml40_start_hr_tag,
    s_chtml40_end_hr_tag,
  },
  /* tagIMG */
  {
    s_chtml40_start_img_tag,
    s_chtml40_end_img_tag,
  },
  /* tagSELECT */
  {
    s_chtml40_start_select_tag,
    s_chtml40_end_select_tag,
  },
  /* tagOPTION */
  {
    s_chtml40_start_option_tag,
    s_chtml40_end_option_tag,
  },
  /* tagDIV */
  {
    s_chtml40_start_div_tag,
    s_chtml40_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_chtml40_chxjif_tag,
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
    s_chtml40_text_tag,
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
    s_chtml40_start_dt_tag,
    s_chtml40_end_dt_tag,
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
    s_chtml40_start_blockquote_tag,
    s_chtml40_end_blockquote_tag,
  },
  /* tagDIR */
  {
    s_chtml40_start_dir_tag,
    s_chtml40_end_dir_tag,
  },
  /* tagDL */
  {
    s_chtml40_start_dl_tag,
    s_chtml40_end_dl_tag,
  },
  /* tagDD */
  {
    s_chtml40_start_dd_tag,
    s_chtml40_end_dd_tag,
  },
  /* tagMENU */
  {
    s_chtml40_start_menu_tag,
    s_chtml40_end_menu_tag,
  },
  /* tagPLAINTEXT */
  {
    s_chtml40_start_plaintext_tag,
    s_chtml40_end_plaintext_tag,
  },
  /* tagBLINK */
  {
    s_chtml40_start_blink_tag,
    s_chtml40_end_blink_tag,
  },
  /* tagMARQUEE */
  {
    s_chtml40_start_marquee_tag,
    s_chtml40_end_marquee_tag,
  },
};


/**
 * converts from CHTML5.0 to CHTML3.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_convert_chtml40(
  request_rec         *r,
  device_table        *spec,
  const char          *src,
  apr_size_t          srclen,
  apr_size_t          *dstlen,
  chxjconvrule_entry  *entryp,
  cookie_t            *cookie
)
{
  char      *dst = NULL;
  char      *ss;
  chtml40_t chtml40;
  Doc       doc;

  DBG(r, "start chxj_convert_chtml40()");

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"i found qrcode xml");
    DBG(r, "end chxj_convert_chtml40()");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml40(&chtml40, &doc, r, spec);

  chtml40.entryp = entryp;
  chtml40.cookie = cookie;

  chxj_set_content_type(r, "text/html; charset=Windows-31J");

  /*--------------------------------------------------------------------------*/
  /* The character string of the input is analyzed.                           */
  /*--------------------------------------------------------------------------*/
  qs_init_malloc(&doc);
  qs_init_root_node(&doc);

  ss = apr_pcalloc(r->pool, srclen + 1);
  memset(ss, 0, srclen + 1);
  memcpy(ss, src, srclen);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML4.0", ss, srclen);
#endif
  chxj_buffered_write_init(r->pool, &doc.buf);

  qs_parse_string(&doc,ss, strlen(ss));

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_convert(spec,r,(void*)&chtml40, &doc, qs_get_root(&doc), 0);
  chtml40.out = chxj_buffered_write_flush(chtml40.out, &doc.buf);
  dst = apr_pstrdup(r->pool, chtml40.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL) {
    dst = apr_pstrdup(r->pool,ss);
  }
  if (strlen(dst) == 0) {
    dst = apr_psprintf(r->pool, "\n");
  }

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[src] CHTML -> CHTML4.0", dst, *dstlen);
#endif

  DBG(r, "end chxj_convert_chtml40()");
  return dst;
}


/**
 * The CHTML structure is initialized.
 *
 * @param chtml40 [i/o] The pointer to the HDML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   HDML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_chtml40(chtml40_t *chtml40, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml40, 0, sizeof(chtml40_t));

  doc->r        = r;
  chtml40->doc  = doc;
  chtml40->spec = spec;
  chtml40->out  = qs_alloc_zero_byte_string(r->pool);
  chtml40->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  chtml40->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param chtml40   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_chtml40_search_emoji(chtml40_t *chtml40, char *txt, char **rslt)
{
  emoji_t       *ee;
  request_rec   *r;
  device_table  *spec;
  int           len;

  spec = chtml40->spec;

  len = strlen(txt);
  r   = chtml40->doc->r;

  if (!spec) {
    DBG(r,"spec is NULL");
  }

  for (ee = chtml40->conf->emoji;
       ee;
       ee = ee->next) {
    if (ee->imode == NULL) {
      DBG(r, "emoji->imode is NULL");
      continue;
    }

    if (ee->imode->string != NULL
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {

      if (spec == NULL || spec->emoji_type == NULL) {
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
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W_L("<html>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the HTML tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40 = GET_CHTML40(pdoc);
  Doc         *doc     = chtml40->doc;

  W_L("</html>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_meta_tag(void *pdoc, Node *node) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  int           content_type_flag;
  int           refresh_flag;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  content_type_flag = 0;
  refresh_flag      = 0;

  W_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    switch(*name) {
    case 'h':
    case 'H':
      if (strcasecmp(name, "http-equiv") == 0 && value && *value) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W_L(" http-equiv=\"");
        W_V(value);
        W_L("\"");
        if (STRCASEEQ('c','C',"content-type", value))
          content_type_flag = 1;
  
        if (STRCASEEQ('r','R',"refresh", value))
          refresh_flag = 1;
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0 && value && *value) {
        if (content_type_flag) {
          W_L(" ");
          W_V(name);
          W_L("=\"text/html; charset=Windows-31J\"");
        }
        else
        if (refresh_flag) {
          char *buf;
          char *sec;
          char *url;
          buf = apr_pstrdup(r->pool, value);
          url = strchr(buf, ';');
          if (url) {
            sec = apr_pstrdup(r->pool, buf);
            sec[url-buf] = 0;
            url++;
            url = chxj_encoding_parameter(r, url);
            url = chxj_add_cookie_parameter(r, url, chtml40->cookie);
            W_L(" ");
            W_V(name);
            W_L("=\"");
            W_V(sec);
            W_L(";");
            W_V(url);
            W_L("\"");
          }
        }
        else {
          W_L(" ");
          W_V(name);
          W_L("=\"");
          W_V(value);
          W_L("\"");
        }
      }
      break;

    default:
      break;
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the META tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The META tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml40_end_meta_tag(void* pdoc, Node* UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml40_start_head_tag(void* pdoc, Node* UNUSED(node)) 
{
  chtml40_t   *chtml40 = GET_CHTML40(pdoc);
  Doc         *doc     = chtml40->doc;

  W_L("<head>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the HEAD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HEAD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</head>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t     *chtml40 = GET_CHTML40(pdoc);
  Doc           *doc     = chtml40->doc;

  W_L("<title>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the TITLE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TITLE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t    *chtml40 = GET_CHTML40(pdoc);
  Doc          *doc     = chtml40->doc;

  W_L("</title>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_base_tag(void *pdoc, Node *node) 
{
  Attr        *attr;
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('h','H',"href", name)) {
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the BASE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_body_tag(void *pdoc, Node *node) 
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<body");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('b','B', "bgcolor", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" bgcolor=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('t','T', "text", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" text=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('l','L',"link", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" link=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"alink", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" alink=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"vlink", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" vlink=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the BODY tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BODY tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40;
  Doc         *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</body>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_a_tag(void *pdoc, Node *node) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('n','N',"name", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W_L(" name=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"href", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml40->cookie);
      W_L(" href=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"accesskey", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cti", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" cti=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('i','I',"ijam", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      W_L(" utn ");
    }
    else if (STRCASEEQ('t','T',"telbook", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('k','K',"kana", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('e','E',"email", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('i','I',"ista", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the A tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The A tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40;
  Doc         *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</a>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_br_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;
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
  W_L(">");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the BR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param chtml40  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<br>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_font_tag(void *pdoc, Node *node) 
{
  Attr          *attr;
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  char          *color = NULL;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr && color == NULL; 
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('c','C',"color", name) && value && *value) {
      color = apr_pstrdup(doc->buf.pool, value);
      break;
    }
    else if (STRCASEEQ('s','S',"size", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }
  if (color) {
    W_L("<font color=\"");
    W_V(color);
    W_L("\">");
    chtml40->font_flag++;
  }
  return chtml40->out;
}


/**
 * It is a handler who processes the FONT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FONT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_font_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  if (chtml40->font_flag) {
    W_L("</font>");
    W_NLCODE();
    chtml40->font_flag--;
  }

  return chtml40->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_form_tag(void *pdoc, Node *node) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<form");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"action", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml40->cookie);
      W_L(" action=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('m','M',"method", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" method=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('u','U',"utn", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      W_L(" utn");
    }
  }
  W_L(">");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the FORM tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The FORM tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t    *chtml40;
  Doc          *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</form>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_input_tag(void *pdoc, Node *node) 
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  char        *max_length  = NULL;
  char        *type        = NULL;
  char        *name        = NULL;
  char        *value       = NULL;
  char        *istyle      = NULL;
  char        *size        = NULL;
  char        *checked     = NULL;
  char        *accesskey   = NULL;

  chtml40   = GET_CHTML40(pdoc);
  doc       = chtml40->doc;
  r         = doc->r;
  W_L("<input");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  type       = qs_get_type_attr(doc, node, doc->buf.pool);
  name       = qs_get_name_attr(doc, node, doc->buf.pool);
  value      = qs_get_value_attr(doc,node, doc->buf.pool);
  istyle     = qs_get_istyle_attr(doc,node,r);
  max_length = qs_get_maxlength_attr(doc,node,r);
  checked    = qs_get_checked_attr(doc,node,doc->buf.pool);
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
    W_V(value);
    W_L("\"");
  }
  if (accesskey && *accesskey) {
    W_L(" accesskey=\"");
    W_V(accesskey);
    W_L("\"");
  }
  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (*istyle == '1' || *istyle == '2' || *istyle == '3' || *istyle == '4') {
      W_L(" istyle=\"");
      W_V(istyle);
      W_L("\"");
    }
  }

  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length && *max_length) {
    if (chxj_chk_numeric(max_length) != 0) {
      max_length = apr_psprintf(r->pool, "0");
    }
    if (istyle != NULL && *istyle == '1') {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length) * 2);
      W_V(vv);
    }
    else  {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length));
      W_V(vv);
    }
  }
  if (checked) {
    W_L(" checked");
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the INPUT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The INPUT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_center_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t     *chtml40;
  Doc           *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("<center>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the CENTER tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The CENTER tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t     *chtml40;
  Doc           *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</center>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_hr_tag(void *pdoc, Node *node) 
{
  Attr        *attr;
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

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
      W_L(" noshade");
    }
    else if (STRCASEEQ('c','C',"color", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" color=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_img_tag(void *pdoc, Node *node) 
{
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table *spec;
#endif
  chtml40_t    *chtml40;
  Doc          *doc;
  request_rec  *r;
  Attr         *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;
#ifndef IMG_NOT_CONVERT_FILENAME
  spec    = chtml40->spec;
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
    if (STRCASEEQ('s','S',"src", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml40->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
        W_L(" src=\"");
        W_V(value);
        W_L("\"");
      }

#else
      value = chxj_img_conv(r,spec,value);
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, chtml40->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
      W_L(" src=\"");
      W_V(value);
      W_L("\"");
#endif
    }
    else if (STRCASEEQ('a','A',"align", name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      if (value && (STRCASEEQ('t','T',"top",    value) ||
                    STRCASEEQ('m','M',"middle", value) ||
                    STRCASEEQ('b','B',"bottom", value) ||
                    STRCASEEQ('l','L',"left",   value) ||
                    STRCASEEQ('c','C',"center", value) ||
                    STRCASEEQ('r','R',"right",  value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('w','W',"width", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" width=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H', "height", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" height=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('h','H',"hspace", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" hspace=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('v','V',"vspace", name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" vspace=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('a','A',"alt",name) && value && *value) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      W_L(" alt=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the IMG tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The IMG tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_select_tag(void *pdoc, Node *child)
{
  chtml40_t   *chtml40  = GET_CHTML40(pdoc);
  Doc         *doc      = chtml40->doc;
  char        *size     = NULL;
  char        *name     = NULL;
  char        *multiple = NULL;
  Attr        *attr;

  W_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"size", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('n','N',"name", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('m','M',"multiple", nm)) {
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
    W_L(" multiple");
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the SELECT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The SELECT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t    *chtml40 = GET_CHTML40(pdoc);
  Doc          *doc   = chtml40->doc;

  W_L("</select>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_option_tag(void *pdoc, Node *child)
{
  Attr        *attr;
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  char        *selected;
  char        *value;

  chtml40    = GET_CHTML40(pdoc);
  doc        = chtml40->doc;
  r          = doc->r;
  selected   = NULL;
  value      = NULL;

  W_L("<option");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *nm  = qs_get_attr_name(doc,attr);
    char *val = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('s','S',"selected", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }
  if (value && *value) {
    W_L(" value=\"");
    W_V(value);
    W_L("\"");
  }
  if (selected) {
    W_L(" selected");
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the OPTION tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OPTION tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  /* Don't close */

  return chtml40->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_div_tag(void *pdoc, Node *child)
{
  Attr          *attr;
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  char          *align;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;
  align   = NULL;

  W_L("<div");
  for (attr = qs_get_attr(doc,child);
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
      }
    }
  }
  if (align) {
    W_L(" align=\"");
    W_V(align);
    W_L("\"");
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DIV tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIV tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_div_tag(void *pdoc, Node *UNUSED(node))
{
  chtml40_t    *chtml40 = GET_CHTML40(pdoc);
  Doc          *doc     = chtml40->doc;

  W_L("</div>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_ul_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40 = GET_CHTML40(pdoc);
  Doc         *doc     = chtml40->doc;
  Attr        *attr;
  W_L("<ul");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        W_L(" type=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(">");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the UL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The UL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t     *chtml40 = GET_CHTML40(pdoc);
  Doc           *doc     = chtml40->doc;

  W_L("</ul>");
  W_NLCODE();

  return chtml40->out;
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
s_chtml40_start_pre_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  chtml40->pre_flag++;
  W_L("<pre>");
  W_NLCODE();

  return chtml40->out;
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
s_chtml40_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t     *chtml40 = GET_CHTML40(pdoc);
  Doc           *doc     = chtml40->doc;

  W_L("</pre>");
  W_NLCODE();
  chtml40->pre_flag--;

  return chtml40->out;
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
s_chtml40_start_p_tag(void *pdoc, Node *node) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;
  char          *align = NULL;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

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
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the P tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The P tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40;
  Doc         *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</p>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_ol_tag(void *pdoc, Node *node)
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

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
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the OL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The OL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_ol_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</ol>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_li_tag(void *pdoc, Node *node) 
{
  chtml40_t   *chtml40 = GET_CHTML40(pdoc);
  Doc         *doc     = chtml40->doc;
  Attr        *attr;

  W_L("<li");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (*value == '1' || *value == 'a' || *value == 'A' || STRCASEEQ('d','D',"disc",value) || STRCASEEQ('s','S',"square",value) || STRCASEEQ('c','C',"circle",value))) {
        W_L(" type=\"");
        W_V(value);
        W_L("\"");
      }
    }
    else if (STRCASEEQ('v','V',"value", name) && value && *value) {
      W_L(" value=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the LI tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The LI tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h1_tag(void *pdoc, Node *node)
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h1");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");

  return chtml40->out;
}


/**
 * It is a handler who processes the H1 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H1 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40;
  Doc         *doc;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;

  W_L("</h1>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h2_tag(void *pdoc, Node *node)
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h2");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the H2 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H2 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t   *chtml40 = GET_CHTML40(pdoc);
  Doc         *doc     = chtml40->doc;

  W_L("</h2>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h3_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h3");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");

  return chtml40->out;
}


/**
 * It is a handler who processes the H3 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H3 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h3_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc   = chtml40->doc;

  W_L("</h3>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h4_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h4");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the H4 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H4 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</h4>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h5_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h5");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the H5 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H5 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</h5>");
  W_NLCODE();

  return chtml40->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_h6_tag(void *pdoc, Node *node)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;
  Attr        *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  W_L("<h6");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align", name)) {
      if (value && (STRCASEEQ('l','L',"left",value) || STRCASEEQ('r','R',"right",value) || STRCASEEQ('c','C',"center",value))) {
        W_L(" align=\"");
        W_V(value);
        W_L("\"");
        break;
      }
    }
  }
  W_L(">");
  return chtml40->out;
}


/**
 * It is a handler who processes the H6 tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The H6 tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</h6>");
  W_NLCODE();

  return chtml40->out;
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
s_chtml40_start_textarea_tag(void *pdoc, Node *node) 
{
  chtml40_t     *chtml40;
  Doc           *doc;
  request_rec   *r;
  Attr          *attr;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  chtml40->textarea_flag++;

  W_L("<textarea");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name  = qs_get_attr_name(doc,attr);
    char *value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"accesskey",name) && value && *value != 0) {
      W_L(" accesskey=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('i','I',"istyle", name) && value && (*value == '1' || *value == '2' || *value == '3' || *value == '4')) {
      W_L(" istyle=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('n','N',"name", name) && value && *value) {
      W_L(" name=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('r','R',"rows", name) && value && *value) {
      W_L(" rows=\"");
      W_V(value);
      W_L("\"");
    }
    else if (STRCASEEQ('c','C',"cols", name) && value && *value) {
      W_L(" cols=\"");
      W_V(value);
      W_L("\"");
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
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
s_chtml40_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("</textarea>");
  W_NLCODE();
  chtml40->textarea_flag--;

  return chtml40->out;
}


static char *
s_chtml40_chxjif_tag(void *pdoc, Node *node)
{
  Node        *child;
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_chtml40_chxjif_tag(chtml40, child);
  }

  return NULL;
}


static char *
s_chtml40_text_tag(void *pdoc, Node *child)
{
  chtml40_t   *chtml40;
  Doc         *doc;
  request_rec *r;

  char        *textval;
  char        *tmp;
  char        *tdst;
  char        one_byte[2];
  int         ii;
  int         tdst_len;

  chtml40 = GET_CHTML40(pdoc);
  doc     = chtml40->doc;
  r       = doc->r;
  
  textval = qs_get_node_value(doc,child);
  if (strlen(textval) == 0) {
    return chtml40->out;
  }
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(doc->buf.pool);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_chtml40_search_emoji(chtml40, &textval[ii], &out);
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
    else if (chtml40->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (chtml40->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  W_V(tdst);
  return chtml40->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("<blockquote>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the BLOCKQUOTE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLOCKQUOTE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_blockquote_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("</blockquote>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_dir_tag(void *pdoc, Node *node)
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  Attr      *attr;
  W_L("<dir");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        W_L(" type=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DIR tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DIR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_dir_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  W_L("</dir>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_dl_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("<dl>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DL tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DL tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_dl_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("</dl>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_dt_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("<dt>");
  return chtml40->out;
}


/**
 * It is a handler who processes the DT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_dt_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_dd_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;

  W_L("<dd>");

  return chtml40->out;
}


/**
 * It is a handler who processes the DD tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The DD tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_dd_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);

  return chtml40->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_marquee_tag(void *pdoc, Node *node)
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  Attr      *attr;
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
  return chtml40->out;
}


/**
 * It is a handler who processes the MARQUEE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MARQUEE tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_marquee_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  W_L("</marquee>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_blink_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  W_L("<blink>");
  return chtml40->out;
}


/**
 * It is a handler who processes the BLINK tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BLINK tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_blink_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  W_L("</blink>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_menu_tag(void *pdoc, Node *node)
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  Attr      *attr;
  W_L("<menu");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name   = qs_get_attr_name(doc,attr);
    char *value  = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('t','T',"type",name)) {
      if (value && (STRCASEEQ('d','D',"disc",value) || STRCASEEQ('c','C',"circle",value) || STRCASEEQ('s','S',"square",value))) {
        W_L(" type=\"");
        W_V(value);
        W_L("\"");
      }
    }
  }
  W_L(">");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the MENU tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The MENU tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_menu_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc = chtml40->doc;
  W_L("</menu>");
  W_NLCODE();
  return chtml40->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_start_plaintext_tag(void *pdoc, Node *node)
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  W_L("<plaintext>");
  s_chtml40_start_plaintext_tag_inner(pdoc,node);
  return chtml40->out;
}

static char *
s_chtml40_start_plaintext_tag_inner(void *pdoc, Node *node)
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  Doc       *doc     = chtml40->doc;
  Node      *child;
  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    W_V(child->otext);
    s_chtml40_start_plaintext_tag_inner(pdoc, child);
  }
  return chtml40->out;
}


/**
 * It is a handler who processes the PLAINTEXT tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PLAINTEXT tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml40_end_plaintext_tag(void *pdoc, Node *UNUSED(child))
{
  chtml40_t *chtml40 = GET_CHTML40(pdoc);
  return chtml40->out;
}
/*
 * vim:ts=2 et
 */
