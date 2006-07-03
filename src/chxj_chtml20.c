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

#define GET_CHTML20(X) ((chtml20_t*)(X))

static char* s_chtml20_start_html_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_html_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_meta_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_meta_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_textarea_tag(void* pdoc, Node* node);
static char* s_chtml20_end_textarea_tag  (void* pdoc, Node* node);
static char* s_chtml20_start_p_tag       (void* pdoc, Node* node);
static char* s_chtml20_end_p_tag         (void* pdoc, Node* node);
static char* s_chtml20_start_pre_tag     (void* pdoc, Node* node);
static char* s_chtml20_end_pre_tag       (void* pdoc, Node* node);
static char* s_chtml20_start_h1_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h1_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_h2_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h2_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_h3_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h3_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_h4_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h4_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_h5_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h5_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_h6_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_h6_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_ul_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_ul_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_ol_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_ol_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_li_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_li_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_head_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_head_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_title_tag   (void* pdoc, Node* node);
static char* s_chtml20_end_title_tag     (void* pdoc, Node* node);
static char* s_chtml20_start_base_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_base_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_body_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_body_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_a_tag       (void* pdoc, Node* node);
static char* s_chtml20_end_a_tag         (void* pdoc, Node* node);
static char* s_chtml20_start_br_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_br_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_tr_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_tr_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_font_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_font_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_form_tag    (void* pdoc, Node* node);
static char* s_chtml20_end_form_tag      (void* pdoc, Node* node);
static char* s_chtml20_start_input_tag   (void* pdoc, Node* node);
static char* s_chtml20_end_input_tag     (void* pdoc, Node* node);
static char* s_chtml20_start_center_tag  (void* pdoc, Node* node);
static char* s_chtml20_end_center_tag    (void* pdoc, Node* node);
static char* s_chtml20_start_hr_tag      (void* pdoc, Node* node);
static char* s_chtml20_end_hr_tag        (void* pdoc, Node* node);
static char* s_chtml20_start_img_tag     (void* pdoc, Node* node);
static char* s_chtml20_end_img_tag       (void* pdoc, Node* node);
static char* s_chtml20_start_select_tag  (void* pdoc, Node* node);
static char* s_chtml20_end_select_tag    (void* pdoc, Node* node);
static char* s_chtml20_start_option_tag  (void* pdoc, Node* node);
static char* s_chtml20_end_option_tag    (void* pdoc, Node* node);
static char* s_chtml20_start_div_tag     (void* pdoc, Node* node);
static char* s_chtml20_end_div_tag       (void* pdoc, Node* node);

static void  s_init_chtml20(chtml20_t* chtml, Doc* doc, request_rec* r, device_table* spec);

static int   s_chtml20_search_emoji(chtml20_t* chtml, char* txt, char** rslt);

static char* s_chtml20_chxjif_tag(void* pdoc, Node* node); 
static char* s_chtml20_text_tag(void* pdoc, Node* node);


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
char*
chxj_exchange_chtml20(
  request_rec*        r,
  device_table*       spec,
  const char*         src,
  apr_size_t          srclen,
  apr_size_t*         dstlen,
  chxjconvrule_entry* entryp,
  cookie_t*           cookie
)
{
  char*     dst = NULL;
  char*     ss;
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
  chxj_node_exchange(spec,r,(void*)&chtml20, &doc, qs_get_root(&doc), 0);
  dst = chtml20.out;

  qs_all_free(&doc,QX_LOGMARK);

  if (dst == NULL) 
    return apr_pstrdup(r->pool,ss);

  if (strlen(dst) == 0)
    dst = apr_psprintf(r->pool, "\n");

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
s_init_chtml20(chtml20_t* chtml20, Doc* doc, request_rec* r, device_table* spec)
{
  memset(doc,     0, sizeof(Doc));
  memset(chtml20, 0, sizeof(chtml20_t));

  doc->r        = r;
  chtml20->doc  = doc;
  chtml20->spec = spec;
  chtml20->out  = qs_alloc_zero_byte_string(r);
  chtml20->conf = ap_get_module_config(r->per_dir_config, &chxj_module);

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
s_chtml20_search_emoji(chtml20_t* chtml20, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
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
static char*
s_chtml20_start_html_tag(void* pdoc, Node* node) 
{
  Doc*          doc;
  request_rec*  r;
  chtml20_t*    chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<html>\n", NULL);

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
static char*
s_chtml20_end_html_tag(void* pdoc, Node* child) 
{
  Doc*          doc;
  request_rec*  r;
  chtml20_t*    chtml20;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</html>\n", NULL);

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
static char*
s_chtml20_start_meta_tag(void* pdoc, Node* node) 
{
  chtml20_t*   chtml20;
  Doc*         doc;
  request_rec* r;
  Attr*        attr;
  int          content_type_flag;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  content_type_flag = 0;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<meta", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name   = qs_get_attr_name(doc,attr);
    value  = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'h':
    case 'H':
      if (strcasecmp(name, "http-equiv") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " http-equiv=\"", 
                        value,
                        "\"",
                        NULL);
  
        if ((*value == 'c' || *value == 'C') 
        && strcasecmp(value, "content-type") == 0)
          content_type_flag = 1;
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0) {
        if (content_type_flag) {
          chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out,
                          " ",
                          name,
                          "=\"",
                          "text/html; charset=Windows-31J",
                          "\"",
                          NULL);
        }
        else {
          chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out,
                          " ",
                          name,
                          "=\"",
                          value,
                          "\"",
                          NULL);
        }
      }
      break;

    default:
      break;
    }
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

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
static char*
s_chtml20_end_meta_tag(void* pdoc, Node* child) 
{
  chtml20_t* chtml20;

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
static char*
s_chtml20_start_head_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<head>\r\n", NULL);

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
static char*
s_chtml20_end_head_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</head>\r\n", NULL);

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
static char*
s_chtml20_start_title_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<title>", NULL);

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
static char*
s_chtml20_end_title_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</title>\r\n", NULL);

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
static char*
s_chtml20_start_base_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<base", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if ((*name == 'h' || *name == 'H') 
    && strcasecmp(name, "href") == 0) {
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
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The BASE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_chtml20_end_base_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_body_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<body", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'b':
    case 'B':
      if (strcasecmp(name, "bgcolor") == 0) {
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
      break;

    case 't':
    case 'T':
      if (strcasecmp(name, "text") == 0) {
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
      break;

    case 'l':
    case 'L':
      if (strcasecmp(name, "link") == 0) {
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">\r\n", NULL);

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
static char*
s_chtml20_end_body_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</body>\r\n", NULL);

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
static char*
s_chtml20_start_a_tag(void* pdoc, Node* node) 
{
  Attr*         attr;
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<a", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'n':
    case 'N':
      if (strcasecmp(name, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " name=\"", 
                        value, 
                        "\"", 
                        NULL);
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "href") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " href=\"", 
                        value, 
                        "\"", 
                        NULL);
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "accesskey") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML1.0                                                           */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " accesskey=\"", 
                        value, 
                        "\"", 
                        NULL);
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "cti") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 2.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " cti=\"", 
                        value, 
                        "\"", 
                        NULL);
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

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
static char*
s_chtml20_end_a_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</a>", NULL);

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
static char*
s_chtml20_start_br_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<br>\r\n", NULL);

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
static char*
s_chtml20_end_br_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_tr_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_end_tr_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<br>\r\n", NULL);

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
static char*
s_chtml20_start_font_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<font", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'c':
    case 'C':
      if (strcasecmp(name, "color") == 0) {
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " color=\"", 
                        value, 
                        "\"", 
                        NULL);
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

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
static char*
s_chtml20_end_font_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</font>", NULL);

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
static char*
s_chtml20_start_form_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<form", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "action") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " action=\"",
                        value,
                        "\"", 
                        NULL);
      }
      break;

    case 'm':
    case 'M':
      if (strcasecmp(name, "method") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                        chtml20->out, 
                        " method=\"",
                        value,
                        "\"", 
                        NULL);
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

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
static char*
s_chtml20_end_form_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</form>", NULL);

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
static char*
s_chtml20_start_input_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


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

  if (type)
    chtml20->out = apr_pstrcat(r->pool,
                    chtml20->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);

  if (size) 
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);

  if (name)
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);

  if (value)
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);

  if (accesskey)
    chtml20->out = apr_pstrcat(r->pool, 
                    chtml20->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);

  if (istyle)
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      " istyle=\"", 
                      istyle, "\" ", 
                      NULL);

  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length) {
    if (chxj_chk_numeric(max_length) != 0)
      max_length = apr_psprintf(r->pool, "0");
    if (istyle != NULL && strcasecmp(istyle, "1") == 0)
      chtml20->out = apr_pstrcat(r->pool, 
                      chtml20->out, 
                      apr_psprintf(r->pool, 
                                   " maxlength=\"%d\"",
                                   chxj_atoi(max_length)*2), 
                                   NULL);
    else
      chtml20->out = apr_pstrcat(r->pool, 
                                 chtml20->out, 
                                 apr_psprintf(r->pool, 
                                              " maxlength=\"%d\"",
                                              chxj_atoi(max_length)),
                                              NULL);
  }

  if (checked)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out,
                               " checked ",
                               NULL);

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, " >", NULL);

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
static char*
s_chtml20_end_input_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_center_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<center>", NULL);

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
static char*
s_chtml20_end_center_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</center>", NULL);

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
static char*
s_chtml20_start_ul_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<ul>", NULL);

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
static char*
s_chtml20_end_ul_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</ul>", NULL);

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
static char*
s_chtml20_start_ol_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<ol>", NULL);

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
static char*
s_chtml20_end_ol_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</ol>", NULL);

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
static char*
s_chtml20_start_li_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<li>", NULL);

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
static char*
s_chtml20_end_li_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</li>", NULL);

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
static char*
s_chtml20_start_hr_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr* attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<hr ", NULL);
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    switch(*name) {
    case 'a':
    case 'A':
      if (strcasecmp(name, "align") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out, 
                          " align=\"", value, "\" ", NULL);
      }
      break;

    case 's':
    case 'S':
      if (strcasecmp(name, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out, 
                          " size=\"", value, "\" ", NULL);
      }
      break;

    case 'w':
    case 'W':
      if (strcasecmp(name, "width") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out, 
                          " width=\"", value, "\" ", NULL);
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(name, "noshade") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool,
                          chtml20->out, 
                          " noshade ", NULL);
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

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, " >", NULL);

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
static char*
s_chtml20_end_hr_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_img_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table* spec;
#endif

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

#ifndef IMG_NOT_CONVERT_FILENAME
  spec = chtml20->spec;
#endif

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<img", NULL);
 

  /*-------------------------------------------------------------------------*/
  /* Get Attributes                                                          */
  /*-------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

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
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out,
                                   " src=\"",
                                   value,
                                   "\"", 
                                   NULL);
#else
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, " src=\"", 
                                   chxj_img_conv(r, spec,value), 
                                   NULL);

        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out,
                                   "\"",
                                   NULL);
#endif
      }
      break;

    case 'a':
    case 'A':
      if (strcasecmp(name, "align" ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " align=\"",
                                   value,
                                   "\"", 
                                   NULL);
      }
      else
      if (strcasecmp(name, "alt"   ) == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " alt=\"",
                                   value,
                                   "\"", 
                                   NULL);
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
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " width=\"",
                                   value,
                                   "\"", 
                                   NULL);
      }
      break;

    case 'h':
    case 'H':
      if (strcasecmp(name, "height") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " height=\"",
                                   value,
                                   "\"", 
                                   NULL);
      }
      else
      if (strcasecmp(name, "hspace") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " hspace=\"",
                                   value,
                                   "\"", 
                                   NULL);
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(name, "vspace") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0                                                          */
        /*--------------------------------------------------------------------*/
        chtml20->out = apr_pstrcat(r->pool, 
                                   chtml20->out, 
                                   " vspace=\"",
                                   value,
                                   "\"", 
                                   NULL);
      }
      break;

    default:
      break;
    }
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">", NULL);

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
static char*
s_chtml20_end_img_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_select_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char* size      = NULL;
  char* name      = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<select", NULL);

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* nm;
    char* val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "size") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        size = apr_pstrdup(r->pool, val);
      }
      break;

    case 'n':
    case 'N':
      if (strcasecmp(nm, "name") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        name = apr_pstrdup(r->pool, val);
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

  if (size)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " size=\"",
                               size,
                               "\"", 
                               NULL);

  if (name)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " name=\"",
                               name,
                               "\"", 
                               NULL);

  chtml20->out = apr_pstrcat(r->pool, 
                             chtml20->out, 
                             ">\n", 
                             NULL);

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
static char*
s_chtml20_end_select_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</select>\n", NULL);

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
static char*
s_chtml20_start_option_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  char* selected   = NULL;
  char* value      = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<option", NULL);

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* nm;
    char* val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    switch(*nm) {
    case 's':
    case 'S':
      if (strcasecmp(nm, "selected") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        selected = apr_pstrdup(r->pool, val);
      }
      break;

    case 'v':
    case 'V':
      if (strcasecmp(nm, "value") == 0) {
        /*--------------------------------------------------------------------*/
        /* CHTML 1.0 version 2.0                                              */
        /*--------------------------------------------------------------------*/
        value = apr_pstrdup(r->pool, val);
      }
      break;

    default:
      break;
    }
  }

  if (value)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " value=\"",
                               value,
                               "\"", 
                               NULL);
  else
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " value=\"\"", 
                               NULL);

  if (selected)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " selected ", 
                               NULL);

  chtml20->out = apr_pstrcat(r->pool, 
                             chtml20->out, 
                             ">", 
                             NULL);

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
static char*
s_chtml20_end_option_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

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
static char*
s_chtml20_start_div_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  char* align   = NULL;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<div", NULL);

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* nm;
    char* val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    if ((*nm == 'a'||*nm == 'A') && strcasecmp(nm, "align") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(r->pool, val);
    }
  }

  if (align)
    chtml20->out = apr_pstrcat(r->pool, 
                               chtml20->out, 
                               " align=\"", 
                               align, 
                               "\"", 
                               NULL);

  chtml20->out = apr_pstrcat(r->pool, 
                             chtml20->out, 
                             ">", 
                             NULL);

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
static char*
s_chtml20_end_div_tag(void* pdoc, Node* child)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</div>\n", NULL);

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
static char*
s_chtml20_start_h1_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h1>\r\n", NULL);

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
static char*
s_chtml20_end_h1_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h1>\r\n", NULL);

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
static char*
s_chtml20_start_h2_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h2>\r\n", NULL);

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
static char*
s_chtml20_end_h2_tag(void* pdoc, Node* child) 
{ 
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h2>\r\n", NULL);

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
static char*
s_chtml20_start_h3_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h3>\r\n", NULL);

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
static char*
s_chtml20_end_h3_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h3>\r\n", NULL);

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
static char*
s_chtml20_start_h4_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h4>\r\n", NULL);

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
static char*
s_chtml20_end_h4_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h4>\r\n", NULL);

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
static char*
s_chtml20_start_h5_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h5>\r\n", NULL);

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
static char*
s_chtml20_end_h5_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h5>\r\n", NULL);

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
static char*
s_chtml20_start_h6_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<h6>\r\n", NULL);

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
static char*
s_chtml20_end_h6_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</h6>\r\n", NULL);

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
static char*
s_chtml20_start_pre_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->pre_flag++;
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<pre>", NULL);

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
static char*
s_chtml20_end_pre_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</pre>", NULL);
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
static char*
s_chtml20_start_p_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<p>", NULL);

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
static char*
s_chtml20_end_p_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</p>", NULL);

  return chtml20->out;
}

static char* 
s_chtml20_chxjif_tag(void* pdoc, Node* node)
{
  chtml20_t*    chtml20;
  Doc*          doc;
  Node*         child;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {

    chtml20->out = apr_pstrcat(r->pool, chtml20->out, child->otext, NULL);

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
static char*
s_chtml20_start_textarea_tag(void* pdoc, Node* node) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;

  chtml20->textarea_flag++;
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "<textarea ", NULL);

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      chtml20->out = apr_pstrcat(r->pool, chtml20->out, " name=\"",value,"\"", NULL);
    }
    else 
    if ((*name == 'r' || *name == 'R') && strcasecmp(name, "rows") == 0) {
      chtml20->out = apr_pstrcat(r->pool, chtml20->out, " rows=\"",value,"\"", NULL);
    }
    else 
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "cols") == 0) {
      chtml20->out = apr_pstrcat(r->pool, chtml20->out, " cols=\"",value,"\"", NULL);
    }
  }

  chtml20->out = apr_pstrcat(r->pool, chtml20->out, ">\r\n", NULL);

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
static char*
s_chtml20_end_textarea_tag(void* pdoc, Node* child) 
{
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;


  chtml20->out = apr_pstrcat(r->pool, chtml20->out, "</textarea>\r\n", NULL);
  chtml20->textarea_flag--;

  return chtml20->out;
}


static char*
s_chtml20_text_tag(void* pdoc, Node* child)
{       
  chtml20_t*    chtml20;
  Doc*          doc;
  request_rec*  r;


  char*   textval;
  char*   tmp;
  char*   tdst;
  char    one_byte[2];
  int     ii;
  int     tdst_len;

  chtml20 = GET_CHTML20(pdoc);
  doc     = chtml20->doc;
  r       = doc->r;
  
  textval = qs_get_node_value(doc,child);
  textval = qs_trim_string(chtml20->doc->r, textval);
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
  chtml20->out = apr_pstrcat(r->pool, chtml20->out, tdst, NULL);

  return chtml20->out;
}
/*
 * vim:ts=2 et
 */
