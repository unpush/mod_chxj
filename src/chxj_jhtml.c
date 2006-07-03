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
#include "chxj_jhtml.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"

#define GET_JHTML(X) ((jhtml_t*)(X))

static char* s_jhtml_start_html_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_html_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_meta_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_meta_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_head_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_head_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_title_tag    (void* pdoc, Node* node);
static char* s_jhtml_end_title_tag      (void* pdoc, Node* node);
static char* s_jhtml_start_base_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_base_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_body_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_body_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_a_tag        (void* pdoc, Node* node);
static char* s_jhtml_end_a_tag          (void* pdoc, Node* node);
static char* s_jhtml_start_pre_tag      (void* pdoc, Node* node);
static char* s_jhtml_end_pre_tag        (void* pdoc, Node* node);
static char* s_jhtml_start_p_tag        (void* pdoc, Node* node);
static char* s_jhtml_end_p_tag          (void* pdoc, Node* node);
static char* s_jhtml_start_ul_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_ul_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_ol_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_ol_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_li_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_li_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_br_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_br_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_tr_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_tr_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_font_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_font_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_form_tag     (void* pdoc, Node* node);
static char* s_jhtml_end_form_tag       (void* pdoc, Node* node);
static char* s_jhtml_start_input_tag    (void* pdoc, Node* node);
static char* s_jhtml_end_input_tag      (void* pdoc, Node* node);
static char* s_jhtml_start_center_tag   (void* pdoc, Node* node);
static char* s_jhtml_end_center_tag     (void* pdoc, Node* node);
static char* s_jhtml_start_hr_tag       (void* pdoc, Node* node);
static char* s_jhtml_end_hr_tag         (void* pdoc, Node* node);
static char* s_jhtml_start_img_tag      (void* pdoc, Node* node);
static char* s_jhtml_end_img_tag        (void* pdoc, Node* node);
static char* s_jhtml_start_select_tag   (void* pdoc, Node* node);
static char* s_jhtml_end_select_tag     (void* pdoc, Node* node);
static char* s_jhtml_start_option_tag   (void* pdoc, Node* node);
static char* s_jhtml_end_option_tag     (void* pdoc, Node* node);
static char* s_jhtml_start_div_tag      (void* pdoc, Node* node);
static char* s_jhtml_end_div_tag        (void* pdoc, Node* node);
static char* s_jhtml_start_textarea_tag (void* pdoc, Node* node);
static char* s_jhtml_end_textarea_tag   (void* pdoc, Node* node);
static char* s_jhtml_start_b_tag        (void* pdoc, Node* node);
static char* s_jhtml_end_b_tag          (void* pdoc, Node* node);
static char* s_jhtml_chxjif_tag         (void* pdoc, Node* node); 
static char* s_jhtml_text_tag           (void* pdoc, Node* node);

static void  s_init_jhtml(jhtml_t* jhtml, Doc* doc, request_rec* r, device_table* spec);

static int   s_jhtml_search_emoji(jhtml_t* jhtml, char* txt, char** rslt);

static char* chxj_istyle_to_mode(request_rec* r, const char* s);



tag_handler jhtml_handler[] = {
  /* tagHTML */
  {
    s_jhtml_start_html_tag,
    s_jhtml_end_html_tag,
  },
  /* tagMETA */
  {
    s_jhtml_start_meta_tag,
    s_jhtml_end_meta_tag,
  },
  /* tagTEXTAREA */
  {
    s_jhtml_start_textarea_tag,
    s_jhtml_end_textarea_tag,
  },
  /* tagP */
  {
    s_jhtml_start_p_tag,
    s_jhtml_end_p_tag,
  },
  /* tagPRE */
  {
    s_jhtml_start_pre_tag,
    s_jhtml_end_pre_tag,
  },
  /* tagUL */
  {
    s_jhtml_start_ul_tag,
    s_jhtml_end_ul_tag,
  },
  /* tagLI */
  {
    s_jhtml_start_li_tag,
    s_jhtml_end_li_tag,
  },
  /* tagOL */
  {
    s_jhtml_start_ol_tag,
    s_jhtml_end_ol_tag,
  },
  /* tagH1 */
  {
    NULL,
    NULL,
  },
  /* tagH2 */
  {
    NULL,
    NULL,
  },
  /* tagH3 */
  {
    NULL,
    NULL,
  },
  /* tagH4 */
  {
    NULL,
    NULL,
  },
  /* tagH5 */
  {
    NULL,
    NULL,
  },
  /* tagH6 */
  {
    NULL,
    NULL,
  },
  /* tagHEAD */
  {
    s_jhtml_start_head_tag,
    s_jhtml_end_head_tag,
  },
  /* tagTITLE */
  {
    s_jhtml_start_title_tag,
    s_jhtml_end_title_tag,
  },
  /* tagBASE */
  {
    s_jhtml_start_base_tag,
    s_jhtml_end_base_tag,
  },
  /* tagBODY */
  {
    s_jhtml_start_body_tag,
    s_jhtml_end_body_tag,
  },
  /* tagA */
  {
    s_jhtml_start_a_tag,
    s_jhtml_end_a_tag,
  },
  /* tagBR */
  {
    s_jhtml_start_br_tag,
    s_jhtml_end_br_tag,
  },
  /* tagTABLE */
  {
    NULL,
    NULL,
  },
  /* tagTR */
  {
    s_jhtml_start_tr_tag,
    s_jhtml_end_tr_tag,
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
    s_jhtml_start_font_tag,
    s_jhtml_end_font_tag,
  },
  /* tagFORM */
  {
    s_jhtml_start_form_tag,
    s_jhtml_end_form_tag,
  },
  /* tagINPUT */
  {
    s_jhtml_start_input_tag,
    s_jhtml_end_input_tag,
  },
  /* tagCENTER */
  {
    s_jhtml_start_center_tag,
    s_jhtml_end_center_tag,
  },
  /* tagHR */
  {
    s_jhtml_start_hr_tag,
    s_jhtml_end_hr_tag,
  },
  /* tagIMG */
  {
    s_jhtml_start_img_tag,
    s_jhtml_end_img_tag,
  },
  /* tagSELECT */
  {
    s_jhtml_start_select_tag,
    s_jhtml_end_select_tag,
  },
  /* tagOPTION */
  {
    s_jhtml_start_option_tag,
    s_jhtml_end_option_tag,
  },
  /* tagDIV */
  {
    s_jhtml_start_div_tag,
    s_jhtml_end_div_tag,
  },
  /* tagCHXJIF */
  {
    s_jhtml_chxjif_tag,
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
    s_jhtml_text_tag,
    NULL,
  },
  /* tagTH */
  {
    NULL,
    NULL,
  },
  /* tagB */
  {
    s_jhtml_start_b_tag,
    s_jhtml_end_b_tag,
  },
  /* tagFIELDSET */
  {
    NULL,
    NULL,
  },
};


/**
 * converts from CHTML5.0 to JHTML.
 *
 * @param r     [i]   Requet_rec is appointed.
 * @param spec  [i]   The result of the device specification processing which 
 *                    was done in advance is appointed.
 * @param src   [i]   The character string before the converting is appointed.
 * @return The character string after the converting is returned.
 */
char*
chxj_exchange_jhtml(
  request_rec*        r,
  device_table*       spec,
  const char*         src,
  apr_size_t          srclen,
  apr_size_t*         dstlen,
  chxjconvrule_entry* entryp,
  cookie_t*           cookie
)
{
  char*     dst;
  char*     ss;
  jhtml_t   jhtml;
  Doc       doc;

  dst = NULL;

  /*--------------------------------------------------------------------------*/
  /* If qrcode xml                                                            */
  /*--------------------------------------------------------------------------*/
  *dstlen = srclen;
  dst = chxj_qr_code_blob_handler(r, src, (size_t*)dstlen);
  if (dst) {
    DBG(r,"I found qrcode xml");
    return dst;
  }
  DBG(r,"not found qrcode xml");

  /*--------------------------------------------------------------------------*/
  /* The CHTML structure is initialized.                                      */
  /*--------------------------------------------------------------------------*/
  s_init_jhtml(&jhtml, &doc, r, spec);

  jhtml.entryp = entryp;
  jhtml.cookie = cookie;

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
  chxj_dump_out("[src] CHTML -> JHTML", ss, srclen);
#endif

  qs_parse_string(&doc,ss,strlen(ss));

  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to JHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_exchange(spec,r,(void*)&jhtml, &doc, qs_get_root(&doc), 0);
  dst = jhtml.out;

  qs_all_free(&doc,QX_LOGMARK);

  if (! dst) 
    return apr_pstrdup(r->pool,ss);

  if (! strlen(dst)) 
    dst = apr_psprintf(r->pool, "\n");

  *dstlen = strlen(dst);

#ifdef DUMP_LOG
  chxj_dump_out("[dst] CHTML -> JHTML", dst, *dstlen);
#endif

  return dst;
}


/**
 * The CHTML structure is initialized.
 *
 * @param jhtml [i/o] The pointer to the JHTML structure that wants to be
 *                   initialized is specified.
 * @param doc   [i]   The Doc structure that should be set to the initialized
 *                   JHTML structure is specified.
 * @param r     [i]   To use POOL, the pointer to request_rec is specified.
 * @param spec  [i]   The pointer to the device_table
 */
static void
s_init_jhtml(jhtml_t* jhtml, Doc* doc, request_rec* r, device_table* spec)
{
  memset(doc,   0, sizeof(Doc));
  memset(jhtml, 0, sizeof(jhtml_t));

  doc->r      = r;
  jhtml->doc  = doc;
  jhtml->spec = spec;
  jhtml->out  = qs_alloc_zero_byte_string(r);
  jhtml->conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  jhtml->doc->parse_mode = PARSE_MODE_CHTML;
}


/**
 * Corresponding EMOJI to a current character-code is retrieved. 
 * The substitution character string is stored in the rslt pointer if agreeing.
 *
 * @param jhtml   [i]   The pointer to the CHTML structure is specified. 
 * @param txt     [i]   The character string to want to examine whether it is 
 *                      EMOJI is specified. 
 * @param rslt    [o]   The pointer to the pointer that stores the result is 
 *                      specified. 
 * @return When corresponding EMOJI exists, it returns it excluding 0. 
 */
static int
s_jhtml_search_emoji(jhtml_t* jhtml, char* txt, char** rslt)
{
  emoji_t*      ee;
  request_rec*  r;
  device_table* spec;
  int           len;

  spec = jhtml->spec;

  len = strlen(txt);
  r = jhtml->doc->r;

  if (! spec) DBG(r,"spec is NULL");

  for (ee = jhtml->conf->emoji;
       ee;
       ee = ee->next) {

    unsigned char hex1byte;
    unsigned char hex2byte;

    if (! ee->imode) { 
      DBG(r,"emoji->imode is NULL");
      continue;
    }

    hex1byte = ee->imode->hex1byte & 0xff;
    hex2byte = ee->imode->hex2byte & 0xff;

    if (ee->imode->string
    &&  strlen(ee->imode->string) > 0
    &&  strncasecmp(ee->imode->string, txt, strlen(ee->imode->string)) == 0) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
        return strlen(ee->imode->string);
      }

      return 0;
    }

    if (len >= 2
    && ((unsigned char)txt[0] & 0xff) == ((unsigned char)hex1byte)
    && ((unsigned char)txt[1] & 0xff) == ((unsigned char)hex2byte)) {
      if (spec == NULL || spec->emoji_type == NULL) {
        *rslt = apr_psprintf(r->pool,"%s", ee->jphone->string);
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
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HTML tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_start_html_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;


  jhtml  = GET_JHTML(pdoc);
  doc    = jhtml->doc;
  r      = doc->r;
  DBG(r, "start s_jhtml_start_html_tag()");

  /*--------------------------------------------------------------------------*/
  /* start HTML tag                                                           */
  /*--------------------------------------------------------------------------*/
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<html>\n", NULL);

  DBG(r, "end s_jhtml_start_html_tag()");

  return jhtml->out;
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
s_jhtml_end_html_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc = jhtml->doc;
  request_rec*  r   = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</html>\n", NULL);

  return jhtml->out;
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
s_jhtml_start_meta_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;
  Attr*        attr;
  int          content_type_flag;
  int          refresh_flag;

  jhtml             = GET_JHTML(pdoc);
  doc               = jhtml->doc;
  r                 = doc->r;
  refresh_flag      = 0;
  content_type_flag = 0;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<meta", NULL);


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

    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "http-equiv") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " http-equiv=\"", 
                      value,
                      "\"",
                      NULL);
      if ((*value == 'c' || *value == 'C') 
      && strcasecmp(value, "content-type") == 0) {
        content_type_flag = 1;
      }
      if ((*value == 'r' || *value == 'R')
      && strcasecmp(value, "refresh") == 0)
        refresh_flag = 1;
    }
    else
    if ((*name == 'c' || *name == 'C') &&strcasecmp(name, "content") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      if (content_type_flag)  {
        jhtml->out = apr_pstrcat(r->pool,
                                jhtml->out,
                                " ",
                                name,
                                "=\"",
                                "text/html; charset=Windows-31J",
                                "\"",
                                NULL);
      }
      else
      if (refresh_flag) {
        char* buf;
        char* sec;
        char* url;

        buf = apr_pstrdup(r->pool, value);

        url = strchr(buf, ';');
        if (url) {
          sec = apr_pstrdup(r->pool, buf);
          sec[url-buf] = 0;
          url++;
          url = chxj_encoding_parameter(r, url);
          url = chxj_add_cookie_parameter(r, url, jhtml->cookie);
          jhtml->out = apr_pstrcat(r->pool,
                                   jhtml->out,
                                   " ",
                                   name,
                                   "=\"",
                                   sec,
                                   ";",
                                   url,
                                   "\"",
                                   NULL);
        }
      }
      else
        jhtml->out = apr_pstrcat(r->pool,
                                 jhtml->out,
                                 " ",
                                 name,
                                 "=\"",
                                 value,
                                 "\"",
                                 NULL);
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_meta_tag(void* pdoc, Node* child) 
{
  jhtml_t* jhtml;

  jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_head_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<head>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_head_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</head>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_start_title_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<title>", NULL);

  return jhtml->out;
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
s_jhtml_end_title_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</title>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_start_base_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml;
  Attr*         attr;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<base", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "href") == 0) {
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " href=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_base_tag(void* pdoc, Node* child) 
{
  jhtml_t* jhtml;

  jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_body_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;
  Attr*        attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<body", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value  = qs_get_attr_value(doc,attr);

    if ((*name == 'b' || *name == 'B') && strcasecmp(name, "bgcolor") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " bgcolor=\"", 
                               value, 
                               "\"", 
                               NULL);
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "text") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " text=\"", 
                               value, 
                               "\"", 
                               NULL);
    }
    else
    if ((*name == 'l' || *name == 'L') && strcasecmp(name, "link") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                              jhtml->out, 
                              " link=\"", 
                              value, 
                              "\"", 
                              NULL);
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

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_body_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</body>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_start_a_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<a", NULL);

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

    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
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
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
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
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
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
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " cti=\"", 
                               value, 
                               "\"", 
                               NULL);
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
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " utn ", 
                               NULL);
    }
    else
    if ((*name == 't' || *name == 'T') && strcasecmp(name, "telbook") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else
    if ((*name == 'k' || *name == 'K') && strcasecmp(name, "kana") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else
    if ((*name == 'e' || *name == 'E') && strcasecmp(name, "email") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
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

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_a_tag(void* pdoc, Node* child) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</a>", NULL);

  return jhtml->out;
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
s_jhtml_start_br_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<br>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_br_tag(void* pdoc, Node* child) 
{
  jhtml_t*  jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_tr_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<br>\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_tr_tag(void* pdoc, Node* child) 
{
  jhtml_t* jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_font_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<font", NULL);

  /*--------------------------------------------------------------------------*/
  /* Get Attributes                                                           */
  /*--------------------------------------------------------------------------*/
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "color") == 0) {
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " color=\"", 
                      value, 
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "size") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_font_tag(void* pdoc, Node* child) 
{
  jhtml_t* jhtml = GET_JHTML(pdoc);
  request_rec* r = jhtml->doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</font>", NULL);

  return jhtml->out;
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
s_jhtml_start_form_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<form", NULL);

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
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
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
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " method=\"",
                      value,
                      "\"", 
                      NULL);
    }
    else
    if ((*name == 'u' || *name == 'U') && strcasecmp(name, "utn") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " utn ", 
                      NULL);
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  if (jhtml->cookie && jhtml->cookie->cookie_id) {
    jhtml->out = apr_psprintf(r->pool, "%s<input type='hidden' name='%s' value='%s'>",
                             jhtml->out, 
                             CHXJ_COOKIE_PARAM,
                             jhtml->cookie->cookie_id);
  }

  return jhtml->out;
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
s_jhtml_end_form_tag(void* pdoc, Node* child) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</form>", NULL);

  return jhtml->out;
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
s_jhtml_start_input_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml       = GET_JHTML(pdoc);
  Doc*          doc         = jhtml->doc;
  request_rec*  r           = doc->r;
  char*         max_length  = NULL;
  char*         type        = NULL;
  char*         name        = NULL;
  char*         value       = NULL;
  char*         istyle      = NULL;
  char*         size        = NULL;
  char*         checked     = NULL;
  char*         accesskey   = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<input", NULL);

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
    jhtml->out = apr_pstrcat(r->pool,
                    jhtml->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
  }
  if (size) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
  }
  if (name) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
  }
  if (value) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
  }
  if (accesskey) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
  }
  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (type && (*type == 'p' || *type == 'P') && strcasecmp(type, "password") == 0
    && ! jhtml->entryp->pc_flag ) {
      jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    "numeric", "\" ", 
                    NULL);
    }
    else {
      jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    chxj_istyle_to_mode(r,istyle), "\" ", 
                    NULL);
    }
  }
  else
  if (istyle == NULL && type != NULL && strcasecmp(type, "password") == 0) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " mode=\"", 
                    "numeric", "\" ", 
                    NULL);
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length) {
    jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " maxlength=\"", 
                      max_length, 
                      "\"", 
                      NULL);
  }

  if (checked) {
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, " checked ", NULL);
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);

  return jhtml->out;
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
s_jhtml_end_input_tag(void* pdoc, Node* child) 
{
  jhtml_t*  jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_center_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<center>", NULL);

  return jhtml->out;
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
s_jhtml_end_center_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</center>", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_start_li_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<li>", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the li tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The li tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_end_li_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</li>", NULL);

  return jhtml->out;
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
s_jhtml_start_ol_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<ol>", NULL);

  return jhtml->out;
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
s_jhtml_end_ol_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</ol>", NULL);

  return jhtml->out;
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
s_jhtml_start_p_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<p>", NULL);

  return jhtml->out;
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
s_jhtml_end_p_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</p>", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_start_pre_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->pre_flag++;
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<pre>", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the PRE tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The PRE tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_end_pre_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</pre>", NULL);
  jhtml->pre_flag--;

  return jhtml->out;
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
s_jhtml_start_ul_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<ul>", NULL);

  return jhtml->out;
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
s_jhtml_end_ul_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</ul>", NULL);

  return jhtml->out;
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
s_jhtml_start_hr_tag(void* pdoc, Node* node) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<hr ", NULL);
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "align") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " align=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 's' || *name == 'S') && strcasecmp(name, "size") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " size=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 'w' || *name == 'W') && strcasecmp(name, "width") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " width=\"", value, "\" ", NULL);
    }
    else
    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "noshade") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
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
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the HR tag.
 *
 * @param jhtml  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The HR tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_end_hr_tag(void* pdoc, Node* child) 
{
  jhtml_t* jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_img_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;
#ifndef IMG_NOT_CONVERT_FILENAME
  device_table* spec = jhtml->spec;
#endif

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<img", NULL);
 

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
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"",value,"\"", NULL);
#else
      value = chxj_img_conv(r, spec, value);
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"", value, NULL);
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, "\"", NULL);
#endif
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "align" ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " align=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'w' || *name == 'W') && strcasecmp(name, "width" ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " width=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "height") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " height=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'h' || *name == 'H') && strcasecmp(name, "hspace") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " hspace=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'v' || *name == 'V') && strcasecmp(name, "vspace") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " vspace=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "alt"   ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " alt=\"",value,"\"", NULL);
    }
    else
    if ((*name == 'a' || *name == 'A') && strcasecmp(name, "align" ) == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_img_tag(void* pdoc, Node* child) 
{
  jhtml_t*  jhtml = GET_JHTML(pdoc);

  return jhtml->out;
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
s_jhtml_start_select_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  char* size      = NULL;
  char* name      = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<select", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if ((*name == 's' || *name == 'S') && strcasecmp(nm, "size") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      size = apr_pstrdup(r->pool, val);
    }
    else
    if ((*name == 'n' || *name == 'N') && strcasecmp(nm, "name") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      name = apr_pstrdup(r->pool, val);
    }
    else
    if ((*name == 'm' || *name == 'M') && strcasecmp(nm, "multiple") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
  }

  if (size)
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " size=\"",size,"\"", NULL);

  if (name)
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " name=\"",name,"\"", NULL);

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\n", NULL);

  return jhtml->out;
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
s_jhtml_end_select_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</select>\n", NULL);

  return jhtml->out;
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
s_jhtml_start_option_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  char* selected   = NULL;
  char* value      = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<option", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if ((*nm == 's' || *nm == 'S') && strcasecmp(nm, "selected") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(r->pool, val);
    }
    else
    if ((*nm == 'v' || *nm == 'V') && strcasecmp(nm, "value") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(r->pool, val);
    }
  }

  if (value) 
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"",value,"\"", NULL);
  else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"\"", NULL);

  if (selected)
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " selected ", NULL);

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_option_tag(void* pdoc, Node* child)
{
  jhtml_t*  jhtml = GET_JHTML(pdoc);

  /* Don't close */

  return jhtml->out;
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
s_jhtml_start_div_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;
  Attr*        attr;

  char* align   = NULL;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<div", NULL);
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if ((*nm == 'a' || *nm == 'A') && strcasecmp(nm, "align") == 0) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(r->pool, val);
    }
  }

  if (align)
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, " align=\"", align, "\"", NULL);

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);

  return jhtml->out;
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
s_jhtml_end_div_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  request_rec* r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</div>\n", NULL);

  return jhtml->out;
}


static char*
chxj_istyle_to_mode(request_rec* r, const char* s)
{
  char* tmp;

  if (s) {
    switch (s[0]) {
    case '1': return apr_psprintf(r->pool, "hiragana");
    case '2': return apr_psprintf(r->pool, "hankakukana");
    case '3': return apr_psprintf(r->pool, "alphabet");
    case '4': return apr_psprintf(r->pool, "numeric");
    default: 
      tmp = apr_palloc(r->pool, 1);
      tmp[0] = '\0';
      return apr_pstrdup(r->pool, tmp);
    }
  }

  tmp = apr_palloc(r->pool, 1);
  tmp[0] = '\0';
  return apr_pstrdup(r->pool,tmp);
}


static char*
s_jhtml_chxjif_tag(void* pdoc, Node* node)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;
  Node*        child;
  request_rec* r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, child->otext, NULL);
    s_jhtml_chxjif_tag(jhtml, child);
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
s_jhtml_start_textarea_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;
  Attr*         attr;

  jhtml->textarea_flag++;
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<textarea ", NULL);

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name  = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);

    if ((*name == 'n' || *name == 'N') && strcasecmp(name, "name") == 0) {
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " name=\"",value,"\"", NULL);
    }
    else 
    if ((*name == 'r' || *name == 'R') && strcasecmp(name, "rows") == 0) {
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " rows=\"",value,"\"", NULL);
    }
    else 
    if ((*name == 'c' || *name == 'C') && strcasecmp(name, "cols") == 0) {
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " cols=\"",value,"\"", NULL);
    }
  }

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\r\n", NULL);

  return jhtml->out;
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
s_jhtml_end_textarea_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</textarea>\r\n", NULL);
  jhtml->textarea_flag--;

  return jhtml->out;
}

/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_start_b_tag(void* pdoc, Node* node) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<b>", NULL);

  return jhtml->out;
}


/**
 * It is a handler who processes the B tag.
 *
 * @param pdoc  [i/o] The pointer to the CHTML structure at the output
 *                     destination is specified.
 * @param node   [i]   The B tag node is specified.
 * @return The conversion result is returned.
 */
static char*
s_jhtml_end_b_tag(void* pdoc, Node* child) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;
  request_rec*  r     = doc->r;

  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</b>\r\n", NULL);

  return jhtml->out;
}

static char*
s_jhtml_text_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc = jhtml->doc;
  char*        textval;
  char*        tmp;
  char*        tdst;
  char         one_byte[2];
  int          ii;
  int          tdst_len;
  request_rec* r = doc->r;

  textval = qs_get_node_value(doc,child);
  textval = qs_trim_string(jhtml->doc->r, textval);
  if (strlen(textval) == 0)
    return jhtml->out;

  tmp = apr_palloc(r->pool, qs_get_node_size(doc,child)+1);
  memset(tmp, 0, qs_get_node_size(doc,child)+1);

  tdst     = qs_alloc_zero_byte_string(r);
  memset(one_byte, 0, sizeof(one_byte));
  tdst_len = 0;

  for (ii=0; ii<qs_get_node_size(doc,child); ii++) {
    char* out;
    int rtn = s_jhtml_search_emoji(jhtml, &textval[ii], &out);
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
    else 
    if (jhtml->pre_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else
    if (jhtml->textarea_flag) {
      one_byte[0] = textval[ii+0];
      tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
    }
    else {
      if (textval[ii] != '\r' && textval[ii] != '\n') {
        one_byte[0] = textval[ii+0];
        tdst = qs_out_apr_pstrcat(r, tdst, one_byte, &tdst_len);
      }
    }
  }
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, tdst, NULL);

  return jhtml->out;
}
/*
 * vim:ts=2 et
 */
