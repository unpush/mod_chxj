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
#include "chxj_chtml20.h"
#include "chxj_hdml.h"
#include "chxj_str_util.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"
#include "chxj_buffered_write.h"


#define GET_CHTML20(X) ((chtml20_t*)(X))
#define W20_L(X)          do { chtml20->out = BUFFERED_WRITE_LITERAL(chtml20->out, &doc->buf, (X)); } while(0)
#define W20_V(X)          do { chtml20->out = (X) ? BUFFERED_WRITE_VALUE(chtml20->out, &doc->buf, (X))  \
                                                  : BUFFERED_WRITE_LITERAL(chtml20->out, &doc->buf, ""); } while(0)

static char *s_chtml20_start_html_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_html_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_meta_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_meta_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_textarea_tag(void *pdoc, Node *node);
static char *s_chtml20_end_textarea_tag  (void *pdoc, Node *node);
static char *s_chtml20_start_p_tag       (void *pdoc, Node *node);
static char *s_chtml20_end_p_tag         (void *pdoc, Node *node);
static char *s_chtml20_start_pre_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_pre_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_h1_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h1_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h2_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h2_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h3_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h3_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h4_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h4_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h5_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h5_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_h6_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_h6_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_ul_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_ul_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_ol_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_ol_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_li_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_li_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_head_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_head_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_title_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_title_tag     (void *pdoc, Node *node);
static char *s_chtml20_start_base_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_base_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_body_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_body_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_a_tag       (void *pdoc, Node *node);
static char *s_chtml20_end_a_tag         (void *pdoc, Node *node);
static char *s_chtml20_start_br_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_br_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_tr_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_tr_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_font_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_font_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_form_tag    (void *pdoc, Node *node);
static char *s_chtml20_end_form_tag      (void *pdoc, Node *node);
static char *s_chtml20_start_input_tag   (void *pdoc, Node *node);
static char *s_chtml20_end_input_tag     (void *pdoc, Node *node);
static char *s_chtml20_start_center_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_center_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_hr_tag      (void *pdoc, Node *node);
static char *s_chtml20_end_hr_tag        (void *pdoc, Node *node);
static char *s_chtml20_start_img_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_img_tag       (void *pdoc, Node *node);
static char *s_chtml20_start_select_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_select_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_option_tag  (void *pdoc, Node *node);
static char *s_chtml20_end_option_tag    (void *pdoc, Node *node);
static char *s_chtml20_start_div_tag     (void *pdoc, Node *node);
static char *s_chtml20_end_div_tag       (void *pdoc, Node *node);

static void  s_init_chtml20(chtml20_t *chtml, Doc *doc, request_rec *r, device_table *spec);

static int   s_chtml20_search_emoji(chtml20_t *chtml, char *txt, char **rslt);

static char *s_chtml20_chxjif_tag(void *pdoc, Node *node); 
static char *s_chtml20_text_tag(void *pdoc, Node *node);


tag_handler chtml20_handler[] = {
  /* tagHTML */
  {
    s_chtml20_start_html_tag,
    s_chtml20_end_html_tag,
  },
  /* tagMETA */
  {
    s_chtml20_start_meta_tag,
    s_chtml20_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_chtml20_start_textarea_tag,
    s_chtml20_end_textarea_tag,
  },
  /* tagP */
  {
    s_chtml20_start_p_tag,
    s_chtml20_end_p_tag,
  },
  /* tagPRE */
  {
    s_chtml20_start_pre_tag,
    s_chtml20_end_pre_tag,
  },
  /* tagUL */
  {
    s_chtml20_start_ul_tag,
    s_chtml20_end_ul_tag,
  },
  /* tagLI */
  {
    s_chtml20_start_li_tag,
    s_chtml20_end_li_tag,
  },
  /* tagOL */
  {
    s_chtml20_start_ol_tag,
    s_chtml20_end_ol_tag,
  },
  /* tagH1 */
  {
    s_chtml20_start_h1_tag,
    s_chtml20_end_h1_tag,
  },
  /* tagH2 */
  {
    s_chtml20_start_h2_tag,
    s_chtml20_end_h2_tag,
  },
  /* tagH3 */
  {
    s_chtml20_start_h3_tag,
    s_chtml20_end_h3_tag,
  },
  /* tagH4 */
  {
    s_chtml20_start_h4_tag,
    s_chtml20_end_h4_tag,
  },
  /* tagH5 */
  {
    s_chtml20_start_h5_tag,
    s_chtml20_end_h5_tag,
  },
  /* tagH6 */
  {
    s_chtml20_start_h6_tag,
    s_chtml20_end_h6_tag,
  },
  /* tagHEAD */
  {
    s_chtml20_start_head_tag,
    s_chtml20_end_head_tag,
  },
  /* tagTITLE */
  {
    s_chtml20_start_title_tag,
    s_chtml20_end_title_tag,
  },
  /* tagBASE */
  {
    s_chtml20_start_base_tag,
    s_chtml20_end_base_tag,
  },
  /* tagBODY */
  {
    s_chtml20_start_body_tag,
    s_chtml20_end_body_tag,
  },
  /* tagA */
  {
    s_chtml20_start_a_tag,
    s_chtml20_end_a_tag,
  },
  /* tagBR */
  {
    s_chtml20_start_br_tag,
    s_chtml20_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_chtml20_start_tr_tag,
    s_chtml20_end_tr_tag,
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
    s_chtml20_start_font_tag,
    s_chtml20_end_font_tag,
  },
  /* tagFORM */
  {
    s_chtml20_start_form_tag,
    s_chtml20_end_form_tag,
  },
  /* tagINPUT */
  {
    s_chtml20_start_input_tag,
    s_chtml20_end_input_tag,
  },
  /* tagCENTER */
  {
    s_chtml20_start_center_tag,
    s_chtml20_end_center_tag,
  },
  /* tagHR */
  {
    s_chtml20_start_hr_tag,
    s_chtml20_end_hr_tag,
  },
  /* tagIMG */
  {
    s_chtml20_start_img_tag,
    s_chtml20_end_img_tag,
  },
  /* tagSELECT */
  {
    s_chtml20_start_select_tag,
    s_chtml20_end_select_tag,
  },
  /* tagOPTION */
  {
    s_chtml20_start_option_tag,
    s_chtml20_end_option_tag,
  },
  /* tagDIV */
  {
    s_chtml20_start_div_tag,
    s_chtml20_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_chtml20_chxjif_tag,
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
    s_chtml20_text_tag,
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
  /* tagPLAINTEXT */
  {
    NULL,
    NULL,
  },
};

/**
 * converts from CHTML5.0 to CHTML2.0.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char *
chxj_exchange_chtml20(
  request_rec         *r,
  device_table *spec,
  const char *src,
  apr_size_t          srclen,
  apr_size_t *dstlen,
  chxjconvrule_entry *entryp,
  cookie_t *cookie
)
{
  char *dst = NULL;
  char *ss;
  chtml20_t chtml20;
  Doc       doc;

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
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_chtml20(&chtml20, &doc, r, spec);
  DBG(r,"init end");

  chtml20.entryp = entryp;
  chtml20.cookie = cookie;

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
  chxj_dump_out("[src] CHTML -> CHTML2.0", ss, srclen);
#endif

  qs_parse_string(&doc,ss, strlen(ss));

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to CHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_exchange(spec,r,(void *)&chtml20, &doc, qs_get_root(&doc), 0);
  chtml20.out = chxj_buffered_write_flush(chtml20.out, &doc.buf);
  dst = apr_pstrdup(r->pool, chtml20.out);
  chxj_buffered_write_terminate(&doc.buf);

  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL)  {
    return apr_pstrdup(r->pool,ss);
  }

  if (strlen(dst) == 0) {
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
s_init_chtml20(chtml20_t *chtml20, Doc *doc, request_rec *r, device_table *spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml20, 0, sizeof(chtml20_t));

  doc->r        = r;
  chtml20->doc  = doc;
  chtml20->spec = spec;
  chtml20->out  = qs_alloc_zero_byte_string(r);
  chtml20->conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  chtml20->doc->parse_mode = PARSE_MODE_CHTML;
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
s_chtml20_search_emoji(chtml20_t *chtml20, char *txt, char **rslt)
{
  emoji_t *ee;
  request_rec *r;
  device_table *spec;
  int           len;

  spec = chtml20->spec;

  len = strlen(txt);
  r   = chtml20->doc->r;

  if (spec == NULL)
    DBG(r,"spec is NULL");

  for (ee = chtml20->conf->emoji;
       ee;
       ee = ee->next) {

    if (ee->imode == NULL) {
      DBG(r,"emoji->imode is NULL");
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
s_chtml20_start_html_tag(void *pdoc, Node *UNUSED(node)) 
{
  Doc *doc;
  request_rec *r;
  chtml20_t *chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  W20_L("<html>\n");
  return chtml20->out;
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
s_chtml20_end_html_tag(void *pdoc, Node *UNUSED(child)) 
{
  Doc *doc;
  request_rec *r;
  chtml20_t *chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</html>\n");
  return chtml20->out;
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
s_chtml20_start_meta_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;
  int content_type_flag;
  int refresh_flag;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  content_type_flag = 0;
  refresh_flag      = 0;

  W20_L("<meta");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name   = qs_get_attr_name(doc,attr);
    value  = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'h':
    case 'H':
      if (strcasecmp(name, "http-equiv") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W20_L(" http-equiv=\"");
        W20_V(value);
        W20_L("\"");
        if (STRCASEEQ('c','C',"content-type", value)) {
          content_type_flag = 1;
        }
        if (STRCASEEQ('r','R',"refresh", value)) {
          refresh_flag = 1;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0) {
        if (content_type_flag) {
          W20_L(" ");
          W20_V(name);
          W20_L("=\"text/html; charset=Windows-31J\"");
        }
        else if (refresh_flag) {
          char *buf = apr_pstrdup(r->pool, value);
          char *sec;
          char *url;

          url = strchr(buf, ';');
          if (url) {
            sec = apr_pstrdup(r->pool, buf);
            sec[url-buf] = 0;
            url++;
            url = chxj_encoding_parameter(r, url);
            url = chxj_add_cookie_parameter(r, url, chtml20->cookie);
            W20_L(" ");
            W20_V(name);
            W20_L("=\"");
            W20_V(sec);
            W20_L(";");
            W20_V(url);
            W20_L("\"");
          }
        }
        else {
          W20_L(" ");
          W20_V(name);
          W20_L("=\"");
          W20_V(value);
          W20_L("\"");
        }
      }
      break;

    default:
      break;
    }
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_meta_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;

  chtml20 = GET_CHTML20(pdoc);

  return chtml20->out;
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
s_chtml20_start_head_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<head>\r\n");
  return chtml20->out;
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
s_chtml20_end_head_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</head>\r\n");
  return chtml20->out;
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
s_chtml20_start_title_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<title>");
  return chtml20->out;
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
s_chtml20_end_title_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</title>\r\n");
  return chtml20->out;
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
s_chtml20_start_base_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<base");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('h','H',"href", name)) {
      W20_L(" href=\"");
      W20_V(value);
      W20_L("\"");
    }
  }
  W20_L(" >\r\n");
  return chtml20->out;
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
s_chtml20_end_base_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_body_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<body");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'b':
    case 'B':
      if (strcasecmp(name, "bgcolor") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W20_L(" bgcolor=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "text") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W20_L(" text=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'l':
    case 'L':
      if (strcasecmp(name, "link") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        W20_L(" link=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "alink") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 4.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(name, "vlink") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 4.0                                                            */
        /*----------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  W20_L(">\r\n");
  return chtml20->out;
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
s_chtml20_end_body_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</body>\r\n");
  return chtml20->out;
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
s_chtml20_start_a_tag(void *pdoc, Node *node) 
{
  Attr *attr;
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<a");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        W20_L(" name=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "href") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        W20_L(" href=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "accesskey") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        W20_L(" accesskey=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cti") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 2.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" cti=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'i':
    case 'I':
      if (strcasecmp(name, "ijam") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "ista") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "ilet") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "iswf") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      else
      if (strcasecmp(name, "irst") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "utn") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "telbook") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'k':
    case 'K':
      if (strcasecmp(name, "kana") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'e':
    case 'E':
      if (strcasecmp(name, "email") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_a_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</a>");
  return chtml20->out;
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
s_chtml20_start_br_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<br>\r\n");
  return chtml20->out;
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
s_chtml20_end_br_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_tr_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
}


/**
 * It is a handler who processes the TR tag.
 *
 * @param chtml20  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The TR tag node is specified.
 * @return The conversion result is returned.
 */
static char *
s_chtml20_end_tr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<br>\r\n");
  return chtml20->out;
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
s_chtml20_start_font_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  W20_L("<font");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        W20_L(" color=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 5.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_font_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</font>");
  return chtml20->out;
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
s_chtml20_start_form_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<form");
  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "action") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);

        W20_L(" action=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'm':
    case 'M':
      if (strcasecmp(name, "method") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" method=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'u':
    case 'U':
      if (strcasecmp(name, "utn") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 3.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_form_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</form>");
  return chtml20->out;
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
s_chtml20_start_input_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  char *max_length  = NULL;
  char *type        = NULL;
  char *name        = NULL;
  char *value       = NULL;
  char *istyle      = NULL;
  char *size        = NULL;
  char *checked     = NULL;
  char *accesskey   = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  W20_L("<input");
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
    W20_L(" type=\"");
    W20_V(type);
    W20_L("\" ");
  }

  if (size) {
    W20_L(" size=\"");
    W20_V(size);
    W20_L("\" ");
  }

  if (name) {
    W20_L(" name=\"");
    W20_V(name);
    W20_L("\" ");
  }

  if (value) {
    W20_L(" value=\"");
    W20_V(value);
    W20_L("\" ");
  }

  if (accesskey) {
    W20_L(" accesskey=\"");
    W20_V(accesskey);
    W20_L("\" ");
  }

  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    W20_L(" istyle=\"");
    W20_V(istyle);
    W20_L("\" ");
  }

  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length) {
    if (chxj_chk_numeric(max_length) != 0)
      max_length = apr_psprintf(doc->buf.pool, "0");
    if (istyle != NULL && strcasecmp(istyle, "1") == 0) {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length) * 2);
      W20_V(vv);
    }
    else {
      char *vv = apr_psprintf(doc->buf.pool, " maxlength=\"%d\"", chxj_atoi(max_length));
      W20_V(vv);
    }
  }

  if (checked) {
    W20_L(" checked");
  }
  W20_L(" >");
  return chtml20->out;
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
s_chtml20_end_input_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_center_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<center>");
  return chtml20->out;
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
s_chtml20_end_center_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</center>");
  return chtml20->out;
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
s_chtml20_start_ul_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<ul>");
  return chtml20->out;
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
s_chtml20_end_ul_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</ul>");
  return chtml20->out;
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
s_chtml20_start_ol_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<ol>");
  return chtml20->out;
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
s_chtml20_end_ol_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</ol>");
  return chtml20->out;
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
s_chtml20_start_li_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<li>");
  return chtml20->out;
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
s_chtml20_end_li_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</li>");
  return chtml20->out;
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
s_chtml20_start_hr_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  W20_L("<hr ");
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "align") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" align=\"");
        W20_V(value);
        W20_L("\" ");
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" size=\"");
        W20_V(value);
        W20_L("\" ");
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" width=\"");
        W20_V(value);
        W20_L("\" ");
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(name, "noshade") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" noshade ");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    default:
      break;
    }
  }
  W20_L(" >");
  return chtml20->out;
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
s_chtml20_end_hr_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_img_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table *spec;
#endif

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

#ifndef IMG_NOT_CONVERT_FILENAME
  spec = chtml20->spec;
#endif

  W20_L("<img");
  /*-------------------------------------------------------------------------*/
  /* Get Attributes                                                          */
  /*-------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 's':
    case 'S':
      if (strcasecmp(name, "src") == 0) {
        /*-------------------------------------------------------------------*/
        /* CHTML 1.0                                                         */
        /*-------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        if (value) {
          value = apr_psprintf(doc->buf.pool,
                               "%s%c%s=true",
                               value,
                               (strchr(value, '?')) ? '&' : '?',
                               CHXJ_COOKIE_NOUPDATE_PARAM);
        }
        W20_L(" src=\"");
        W20_V(value);
        W20_L("\"");
#else
        value = chxj_img_conv(r, spec, value);
        value = chxj_encoding_parameter(r, value);
        value = chxj_add_cookie_parameter(r, value, chtml20->cookie);
        if (value) {
          value = apr_psprintf(doc->buf.pool,
                               "%s%c%s=true",
                               value,
                               (strchr(value, '?')) ? '&' : '?',
                               CHXJ_COOKIE_NOUPDATE_PARAM);
        }
        W20_L(" src=\"");
        W20_V(value);
        W20_L("\"");
#endif
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "align" ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" align=\"");
        W20_V(value);
        W20_L("\"");
      }
      else
      if (strcasecmp(name, "alt"   ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" alt=\"");
        W20_V(value);
        W20_L("\"");
      }
      else
      if (strcasecmp(name, "align" ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 4.0                                                          */
        /*--------------------------------------------------------------------*/
        /* ignore */
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width" ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" width=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "height") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" height=\"");
        W20_V(value);
        W20_L("\"");
      }
      else
      if (strcasecmp(name, "hspace") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" hspace=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(name, "vspace") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        W20_L(" vspace=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    default:
      break;
    }
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_img_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  return chtml20->out;
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
s_chtml20_start_select_tag(void *pdoc, Node *child)
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char* size      = NULL;
  char* name      = NULL;

  W20_L("<select");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *nm;
    char *val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        size = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(nm, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        name = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'm':
    case 'M':
      if (strcasecmp(nm, "multiple") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        /* not support */
      }
      break;

    default:
      break;
    }
  }

  if (size) {
    W20_L(" size=\"");
    W20_V(size);
    W20_L("\"");
  }

  if (name) {
    W20_L(" name=\"");
    W20_V(name);
    W20_L("\"");
  }

  W20_L(">\r\n");
  return chtml20->out;
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
s_chtml20_end_select_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</select>\r\n");
  return chtml20->out;
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
s_chtml20_start_option_tag(void *pdoc, Node *child)
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char *selected   = NULL;
  char *value      = NULL;

  W20_L("<option");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *nm;
    char *val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "selected") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        selected = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(nm, "value") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        value = apr_pstrdup(doc->buf.pool, val);
      }
      break;

    default:
      break;
    }
  }

  if (value) {
    W20_L(" value=\"");
    W20_V(value);
    W20_L("\"");
  }
  else {
    W20_L(" value=\"\"");
  }

  if (selected) {
    W20_L(" selected ");
  }
  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_option_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /* Don't close */

  return chtml20->out;
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
s_chtml20_start_div_tag(void *pdoc, Node *child)
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  char* align   = NULL;

  W20_L("<div");
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *nm;
    char *val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A', "align", nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (align) {
    W20_L(" align=\"");
    W20_V(align);
    W20_L("\"");
  }

  W20_L(">");
  return chtml20->out;
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
s_chtml20_end_div_tag(void *pdoc, Node *UNUSED(child))
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</div>\r\n");
  return chtml20->out;
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
s_chtml20_start_h1_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h1>\r\n");
  return chtml20->out;
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
s_chtml20_end_h1_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h1>\r\n");
  return chtml20->out;
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
s_chtml20_start_h2_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h2>\r\n");
  return chtml20->out;
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
s_chtml20_end_h2_tag(void *pdoc, Node *UNUSED(child)) 
{ 
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h2>\r\n");
  return chtml20->out;
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
s_chtml20_start_h3_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h3>\r\n");
  return chtml20->out;
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
s_chtml20_end_h3_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h3>\r\n");
  return chtml20->out;
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
s_chtml20_start_h4_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h4>\r\n");
  return chtml20->out;
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
s_chtml20_end_h4_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h4>\r\n");
  return chtml20->out;
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
s_chtml20_start_h5_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h5>\r\n");
  return chtml20->out;
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
s_chtml20_end_h5_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h5>\r\n");
  return chtml20->out;
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
s_chtml20_start_h6_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<h6>\r\n");
  return chtml20->out;
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
s_chtml20_end_h6_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</h6>\r\n");
  return chtml20->out;
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
s_chtml20_start_pre_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->pre_flag++;
  W20_L("<pre>");
  return chtml20->out;
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
s_chtml20_end_pre_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</pre>");
  chtml20->pre_flag--;

  return chtml20->out;
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
s_chtml20_start_p_tag(void *pdoc, Node *UNUSED(node)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("<p>");
  return chtml20->out;
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
s_chtml20_end_p_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</p>");
  return chtml20->out;
}


static char *
s_chtml20_chxjif_tag(void *pdoc, Node *node)
{
  chtml20_t *chtml20;
  Doc *doc;
  Node *child;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {

    W20_V(child->otext);
    s_chtml20_chxjif_tag(pdoc, child);
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
s_chtml20_start_textarea_tag(void *pdoc, Node *node) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  Attr *attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->textarea_flag++;
  W20_L("<textarea ");
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char *name;
    char *value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0) {
        W20_L(" name=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'r':
    case 'R':
      if (strcasecmp(name, "rows") == 0) {
        W20_L(" rows=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cols") == 0) {
        W20_L(" cols=\"");
        W20_V(value);
        W20_L("\"");
      }
      break;

    default:
      break;
    }
  }
  W20_L(">\r\n");
  return chtml20->out;
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
s_chtml20_end_textarea_tag(void *pdoc, Node *UNUSED(child)) 
{
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  W20_L("</textarea>\r\n");
  chtml20->textarea_flag--;

  return chtml20->out;
}


static char *
s_chtml20_text_tag(void *pdoc, Node *child)
{       
  chtml20_t *chtml20;
  Doc *doc;
  request_rec *r;
  char *textval;
  char *tmp;
  char *tdst;
  char one_byte[2];
  int           ii;
  int           tdst_len;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;
  
  textval = qs_get_node_value(doc,child);
  textval = qs_trim_string(doc->buf.pool, textval);
  if (strlen(textval) == 0) 
    return chtml20->out;
  
  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);
  
  tdst     = qs_alloc_zero_byte_string(r);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;
  
  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_chtml20_search_emoji(chtml20, &textval[ii], &out);
    if (rtn != 0) {
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
    else 
    if (chtml20->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (chtml20->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else 
    if (textval[ii] != '\r' && textval[ii] != '\n') {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
  }
  W20_V(tdst);
  return chtml20->out;
}
/*
 * vim:ts=2 et
 */
