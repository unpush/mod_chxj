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
#include "chxj_jhtml.h"
#include "chxj_hdml.h"
#include "chxj_dump.h"
#include "chxj_img_conv.h"
#include "chxj_qr_code.h"
#include "chxj_encoding.h"

#define BUFFERED 1

#define GET_JHTML(X) ((jhtml_t*)(X))
#define WJ_L(X)          do { jhtml->out = BUFFERED_WRITE_LITERAL(jhtml->out, &doc->buf, (X)); } while(0)
#define WJ_V(X)          do { jhtml->out = (X) ? BUFFERED_WRITE_VALUE(jhtml->out, &doc->buf, (X))  \
                                               : BUFFERED_WRITE_LITERAL(jhtml->out, &doc->buf, ""); } while(0)

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

  chxj_buffered_write_init(r->pool, &doc.buf);
  /*--------------------------------------------------------------------------*/
  /* It converts it from CHTML to JHTML.                                      */
  /*--------------------------------------------------------------------------*/
  chxj_node_exchange(spec,r,(void*)&jhtml, &doc, qs_get_root(&doc), 0);
  jhtml.out = chxj_buffered_write_flush(jhtml.out, &doc.buf);
  dst = apr_pstrdup(r->pool, jhtml.out);
  chxj_buffered_write_terminate(&doc.buf);


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
s_jhtml_start_html_tag(void* pdoc, Node* UNUSED(node)) 
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
#if BUFFERED
  WJ_L("<html>\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<html>\n", NULL);
#endif

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
s_jhtml_end_html_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc = jhtml->doc;

#if BUFFERED
  WJ_L("</html>\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</html>\n", NULL);
#endif

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

#if BUFFERED
  WJ_L("<meta");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<meta", NULL);
#endif

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
#if BUFFERED
        WJ_L(" http-equiv=\"");
        WJ_V(value);
        WJ_L("\"");
#else
        jhtml->out = apr_pstrcat(r->pool, 
                        jhtml->out, 
                        " http-equiv=\"", 
                        value,
                        "\"",
                        NULL);
#endif
        if (STRCASEEQ('c','C',"content-type",value)) {
          content_type_flag = 1;
        }
        if (STRCASEEQ('r','R',"refresh",value)) {
          refresh_flag = 1;
        }
      }
      break;

    case 'c':
    case 'C':
      if (strcasecmp(name, "content") == 0) {
        /*----------------------------------------------------------------------*/
        /* CHTML 2.0                                                            */
        /*----------------------------------------------------------------------*/
        if (content_type_flag)  {
#if BUFFERED
          WJ_L(" ");
          WJ_V(name);
          WJ_L("=\"");
          WJ_L("text/html; charset=Windows-31J");
          WJ_L("\"");
#else
          jhtml->out = apr_pstrcat(r->pool,
                                  jhtml->out,
                                  " ",
                                  name,
                                  "=\"",
                                  "text/html; charset=Windows-31J",
                                  "\"",
                                  NULL);
#endif
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
#if BUFFERED
            WJ_L(" ");
            WJ_V(name);
            WJ_L("=\"");
            WJ_V(sec);
            WJ_L(";");
            WJ_V(url);
            WJ_L("\"");
#else
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
#endif
          }
        }
        else {
#if BUFFERED
          WJ_L(" ");
          WJ_V(name);
          WJ_L("=\"");
          WJ_V(value);
          WJ_L("\"");
#else
          jhtml->out = apr_pstrcat(r->pool,
                                   jhtml->out,
                                   " ",
                                   name,
                                   "=\"",
                                   value,
                                   "\"",
                                   NULL);
#endif
        }
      }
      break;
    
    default:
      break;
    }
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_meta_tag(void* pdoc, Node* UNUSED(child)) 
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
s_jhtml_start_head_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<head>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<head>\r\n", NULL);
#endif

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
s_jhtml_end_head_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</head>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</head>\r\n", NULL);
#endif

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
s_jhtml_start_title_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<title>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<title>", NULL);
#endif

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
s_jhtml_end_title_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</title>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</title>\r\n", NULL);
#endif

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

#if BUFFERED
  WJ_L("<base");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<base", NULL);
#endif

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

    if (STRCASEEQ('h','H',"href",name)) {
#if BUFFERED
      WJ_L(" href=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " href=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
  }

#if BUFFERED
  WJ_L(" >\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >\r\n", NULL);
#endif

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
s_jhtml_end_base_tag(void* pdoc, Node* UNUSED(child)) 
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

#if BUFFERED
  WJ_L("<body");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<body", NULL);
#endif

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

    if (STRCASEEQ('b','B',"bgcolor",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" bgcolor=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " bgcolor=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('t','T',"text",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" text=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " text=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('l','L',"link",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" link=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                              jhtml->out, 
                              " link=\"", 
                              value, 
                              "\"", 
                              NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"alink",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('v','V',"vlink",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

#if BUFFERED
  WJ_L(">\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\r\n", NULL);
#endif

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
s_jhtml_end_body_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</body>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</body>\r\n", NULL);
#endif

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

#if BUFFERED
  WJ_L("<a");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<a", NULL);
#endif

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

    if (STRCASEEQ('n','N',"name",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" name=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " name=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('h','H',"href",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
#if BUFFERED
      WJ_L(" href=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " href=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"accesskey",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML1.0                                                             */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" accesskey=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " accesskey=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('c','C',"cti",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 2.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" cti=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " cti=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('i','I',"ijam",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" utn ");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " utn ", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('t','T',"telbook",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('k','K',"kana",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('e','E',"email",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /*----------------------------------------------------------------------*/
      /* not support */
    }
    else if (STRCASEEQ('i','I',"ista",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"ilet",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"iswf",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
    else if (STRCASEEQ('i','I',"irst",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_a_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</a>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</a>", NULL);
#endif

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
s_jhtml_start_br_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<br>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<br>\r\n", NULL);
#endif

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
s_jhtml_end_br_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*  jhtml;

  jhtml = GET_JHTML(pdoc);

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
s_jhtml_start_tr_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<br>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<br>\r\n", NULL);
#endif

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
s_jhtml_end_tr_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t* jhtml;

  jhtml = GET_JHTML(pdoc);

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
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<font");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<font", NULL);
#endif

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

    if (STRCASEEQ('c','C',"color",name)) {
#if BUFFERED
      WJ_L(" color=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " color=\"", 
                               value, 
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('s','S',"size",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 5.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_font_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*     jhtml;
  request_rec* r;
  Doc          *doc;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = jhtml->doc->r;

#if BUFFERED
  WJ_L("</font>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</font>", NULL);
#endif

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
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;
  Attr*        attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<form");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<form", NULL);
#endif

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

    if (STRCASEEQ('a','A',"action",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" action=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " action=\"",
                               value,
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('m','M',"method",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" method=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " method=\"",
                               value,
                               "\"", 
                               NULL);
#endif
    }
    else if (STRCASEEQ('u','U',"utn",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 3.0                                                            */
      /* It is special only for CHTML.                                        */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" utn ");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, 
                      " utn ", 
                      NULL);
#endif
    }
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

  if (jhtml->cookie && jhtml->cookie->cookie_id) {
#if BUFFERED
    char *vv = apr_psprintf(doc->buf.pool, "%s<input type='hidden' name='%s' value='%s'>",
                            jhtml->out, 
                            CHXJ_COOKIE_PARAM,
                            jhtml->cookie->cookie_id);
    WJ_V(vv);
#else
    jhtml->out = apr_psprintf(r->pool, "%s<input type='hidden' name='%s' value='%s'>",
                             jhtml->out, 
                             CHXJ_COOKIE_PARAM,
                             jhtml->cookie->cookie_id);
#endif
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
s_jhtml_end_form_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</form>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</form>", NULL);
#endif

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
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;
  char*         max_length;
  char*         type;
  char*         name;
  char*         value;
  char*         istyle;
  char*         size;
  char*         checked;
  char*         accesskey;

  jhtml       = GET_JHTML(pdoc);
  doc         = jhtml->doc;
  r           = doc->r;
  max_length  = NULL;
  type        = NULL;
  name        = NULL;
  value       = NULL;
  istyle      = NULL;
  size        = NULL;
  checked     = NULL;
  accesskey   = NULL;

#if BUFFERED
  WJ_L("<input");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<input", NULL);
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
    WJ_L(" type=\"");
    WJ_V(type);
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool,
                    jhtml->out, 
                    " type=\"", 
                    type, 
                    "\" ", 
                    NULL);
#endif
  }
  if (size) {
#if BUFFERED
    WJ_L(" size=\"");
    WJ_V(size);
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " size=\"", 
                    size, 
                    "\" ", 
                    NULL);
#endif
  }
  if (name) {
#if BUFFERED
    WJ_L(" name=\"");
    WJ_V(name);
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " name=\"", 
                    name, 
                    "\" ", 
                    NULL);
#endif
  }
  if (value) {
#if BUFFERED
    WJ_L(" value=\"");
    WJ_V(value);
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " value=\"", 
                    value, 
                    "\" ", 
                    NULL);
#endif
  }
  if (accesskey) {
#if BUFFERED
    WJ_L(" accesskey=\"");
    WJ_V(accesskey);
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, 
                    " accesskey=\"", 
                    accesskey, "\" ", 
                    NULL);
#endif
  }
  if (istyle) {
    /*------------------------------------------------------------------------*/
    /* CHTML 2.0                                                              */
    /*------------------------------------------------------------------------*/
    if (STRCASEEQ('p','P',"password", type) && ! jhtml->entryp->pc_flag ) {
#if BUFFERED
      WJ_L(" mode=\"");
      WJ_L("numeric");
      WJ_L("\" ");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " mode=\"", 
                               "numeric", 
                               "\" ", 
                               NULL);
#endif
    }
    else {
#if BUFFERED
      char *vv = chxj_istyle_to_mode(r,istyle);
      WJ_L(" mode=\"");
      WJ_V(vv);
      WJ_L("\" ");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                               jhtml->out, 
                               " mode=\"", 
                               chxj_istyle_to_mode(r,istyle), 
                               "\" ", 
                               NULL);
#endif
    }
  }
  else if (istyle == NULL && type != NULL && STRCASEEQ('p','P',"password",type)) {
#if BUFFERED
    WJ_L(" mode=\"");
    WJ_L("numeric");
    WJ_L("\" ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                             jhtml->out, 
                             " mode=\"", 
                             "numeric", 
                             "\" ", 
                             NULL);
#endif
  }
  /*--------------------------------------------------------------------------*/
  /* The figure is default for the password.                                  */
  /*--------------------------------------------------------------------------*/
  if (max_length) {
#if BUFFERED
    WJ_L(" maxlength=\"");
    WJ_V(max_length);
    WJ_L("\"");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                             jhtml->out, 
                             " maxlength=\"", 
                             max_length, 
                             "\"", 
                             NULL);
#endif
  }

  if (checked) {
#if BUFFERED
    WJ_L(" checked ");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                             jhtml->out,
                             " checked ",
                             NULL);
#endif
  }

#if BUFFERED
  WJ_L(" >");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);
#endif

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
s_jhtml_end_input_tag(void* pdoc, Node* UNUSED(child)) 
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
s_jhtml_start_center_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("<center>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<center>", NULL);
#endif

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
s_jhtml_end_center_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</center>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</center>", NULL);
#endif

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
s_jhtml_start_li_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<li>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<li>", NULL);
#endif

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
s_jhtml_end_li_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</li>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</li>", NULL);
#endif

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
s_jhtml_start_ol_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<ol>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<ol>", NULL);
#endif

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
s_jhtml_end_ol_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</ol>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</ol>", NULL);
#endif

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
s_jhtml_start_p_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<p>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<p>", NULL);
#endif

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
s_jhtml_end_p_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</p>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</p>", NULL);
#endif

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
s_jhtml_start_pre_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

  jhtml->pre_flag++;
#if BUFFERED
  WJ_L("<pre>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<pre>", NULL);
#endif

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
s_jhtml_end_pre_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</pre>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</pre>", NULL);
#endif
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
s_jhtml_start_ul_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("<ul>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<ul>", NULL);
#endif

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
s_jhtml_end_ul_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</ul>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</ul>", NULL);
#endif

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
  Attr*        attr;

#if BUFFERED
  WJ_L("<hr ");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<hr ", NULL);
#endif
 
  for (attr = qs_get_attr(doc,node);
       attr; 
       attr = qs_get_next_attr(doc,attr)) {
    char* name = qs_get_attr_name(doc,attr);
    char* value = qs_get_attr_value(doc,attr);
    if (STRCASEEQ('a','A',"align",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" align=\"");
      WJ_V(value);
      WJ_L("\" ");
#else
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " align=\"", value, "\" ", NULL);
#endif
    }
    else if (STRCASEEQ('s','S',"size",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" size=\"");
      WJ_V(value);
      WJ_L("\" ");
#else
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " size=\"", value, "\" ", NULL);
#endif
    }
    else if (STRCASEEQ('w','W',"width",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" width=\"");
      WJ_V(value);
      WJ_L("\" ");
#else
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " width=\"", value, "\" ", NULL);
#endif
    }
    else if (STRCASEEQ('n','N',"noshade",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" noshade ");
#else
      jhtml->out = apr_pstrcat(r->pool,
                        jhtml->out, 
                        " noshade ", NULL);
#endif
    }
    else if (STRCASEEQ('c','C',"color",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

#if BUFFERED
  WJ_L(" >");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, " >", NULL);
#endif

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
s_jhtml_end_hr_tag(void* pdoc, Node* UNUSED(child)) 
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

#if BUFFERED
  WJ_L("<img");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<img", NULL);
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
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#ifdef IMG_NOT_CONVERT_FILENAME
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
#if BUFFERED
      WJ_L(" src=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"",value,"\"", NULL);
#endif

#else
      value = chxj_img_conv(r, spec, value);
      value = chxj_encoding_parameter(r, value);
      value = chxj_add_cookie_parameter(r, value, jhtml->cookie);
      if (value) {
        value = apr_psprintf(r->pool,
                             "%s%c%s=true",
                             value,
                             (strchr(value, '?')) ? '&' : '?',
                             CHXJ_COOKIE_NOUPDATE_PARAM);
      }
#if BUFFERED
      WJ_L(" src=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " src=\"", value, NULL);
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, "\"", NULL);
#endif

#endif
    }
    else if (STRCASEEQ('a','A',"align",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" align=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " align=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('w','W',"width",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" width=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " width=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('h','H',"height",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" height=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " height=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('h','H',"hspace",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" hspace=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " hspace=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('v','V',"vspace",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" vspace=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " vspace=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"alt",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0                                                            */
      /*----------------------------------------------------------------------*/
#if BUFFERED
      WJ_L(" alt=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, 
                      jhtml->out, " alt=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('a','A',"align",name)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 4.0                                                            */
      /*----------------------------------------------------------------------*/
      /* ignore */
    }
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_img_tag(void* pdoc, Node* UNUSED(child)) 
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
  Attr*        attr;

  char* size      = NULL;
  char* name      = NULL;

#if BUFFERED
  WJ_L("<select");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<select", NULL);
#endif

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

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
      /* not support */
    }
  }

  if (size) {
#if BUFFERED
    WJ_L(" size=\"");
    WJ_V(size);
    WJ_L("\"");
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " size=\"",size,"\"", NULL);
#endif
  }

  if (name) {
#if BUFFERED
    WJ_L(" name=\"");
    WJ_V(name);
    WJ_L("\"");
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " name=\"",name,"\"", NULL);
#endif
  }

#if BUFFERED
  WJ_L(">\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\n", NULL);
#endif

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
s_jhtml_end_select_tag(void* pdoc, Node* UNUSED(child))
{
  jhtml_t*     jhtml = GET_JHTML(pdoc);
  Doc*         doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</select>\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</select>\n", NULL);
#endif

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
  Attr*        attr;

  char* selected   = NULL;
  char* value      = NULL;

#if BUFFERED
  WJ_L("<option");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<option", NULL);
#endif

  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {
    char* nm  = qs_get_attr_name(doc,attr);
    char* val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('s','S',"selected",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      selected = apr_pstrdup(doc->buf.pool, val);
    }
    else if (STRCASEEQ('v','V',"value",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 version 2.0                                                */
      /*----------------------------------------------------------------------*/
      value = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (value) {
#if BUFFERED
    WJ_L(" value=\"");
    WJ_V(value);
    WJ_L("\"");
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"",value,"\"", NULL);
#endif
  }
  else {
#if BUFFERED
    WJ_L(" value=\"\"");
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " value=\"\"", NULL);
#endif
  }

  if (selected) {
#if BUFFERED
    WJ_L(" selected ");
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, " selected ", NULL);
#endif
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_option_tag(void* pdoc, Node* UNUSED(child))
{
  jhtml_t*  jhtml;

  jhtml = GET_JHTML(pdoc);

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
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;
  Attr*        attr;

  char*        align;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  align = NULL;

#if BUFFERED
  WJ_L("<div");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<div", NULL);
#endif
  for (attr = qs_get_attr(doc,child);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* nm;
    char* val;

    nm  = qs_get_attr_name(doc,attr);
    val = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('a','A',"align",nm)) {
      /*----------------------------------------------------------------------*/
      /* CHTML 1.0 (W3C version 3.2)                                          */
      /*----------------------------------------------------------------------*/
      align = apr_pstrdup(doc->buf.pool, val);
    }
  }

  if (align) {
#if BUFFERED
    WJ_L(" align=\"");
    WJ_V(align);
    WJ_L("\"");
#else
    jhtml->out = apr_pstrcat(r->pool, 
                    jhtml->out, " align=\"", align, "\"", NULL);
#endif
  }

#if BUFFERED
  WJ_L(">");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">", NULL);
#endif

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
s_jhtml_end_div_tag(void* pdoc, Node* UNUSED(child))
{
  jhtml_t*     jhtml;
  Doc*         doc;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</div>\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</div>\n", NULL);
#endif

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
  jhtml_t*     jhtml;
  Doc*         doc;
  Node*        child;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  for (child = qs_get_child_node(doc, node);
       child;
       child = qs_get_next_node(doc, child)) {
#if BUFFERED
    WJ_V(child->otext);
#else
    jhtml->out = apr_pstrcat(r->pool, jhtml->out, child->otext, NULL);
#endif
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
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;
  Attr*         attr;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

  jhtml->textarea_flag++;
#if BUFFERED
  WJ_L("<textarea ");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<textarea ", NULL);
#endif

  for (attr = qs_get_attr(doc,node);
       attr;
       attr = qs_get_next_attr(doc,attr)) {

    char* name;
    char* value;

    name  = qs_get_attr_name(doc,attr);
    value = qs_get_attr_value(doc,attr);

    if (STRCASEEQ('n','N',"name",name)) {
#if BUFFERED
      WJ_L(" name=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " name=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('r','R',"rows",name)) {
#if BUFFERED
      WJ_L(" rows=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " rows=\"",value,"\"", NULL);
#endif
    }
    else if (STRCASEEQ('c','C',"cols",name)) {
#if BUFFERED
      WJ_L(" cols=\"");
      WJ_V(value);
      WJ_L("\"");
#else
      jhtml->out = apr_pstrcat(r->pool, jhtml->out, " cols=\"",value,"\"", NULL);
#endif
    }
  }

#if BUFFERED
  WJ_L(">\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, ">\r\n", NULL);
#endif

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
s_jhtml_end_textarea_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("</textarea>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</textarea>\r\n", NULL);
#endif
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
s_jhtml_start_b_tag(void* pdoc, Node* UNUSED(node)) 
{
  jhtml_t*      jhtml;
  Doc*          doc;
  request_rec*  r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

#if BUFFERED
  WJ_L("<b>");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "<b>", NULL);
#endif

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
s_jhtml_end_b_tag(void* pdoc, Node* UNUSED(child)) 
{
  jhtml_t*      jhtml = GET_JHTML(pdoc);
  Doc*          doc   = jhtml->doc;

#if BUFFERED
  WJ_L("</b>\r\n");
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, "</b>\r\n", NULL);
#endif

  return jhtml->out;
}

static char*
s_jhtml_text_tag(void* pdoc, Node* child)
{
  jhtml_t*     jhtml;
  Doc*         doc;
  char*        textval;
  char*        tmp;
  char*        tdst;
  char         one_byte[2];
  int          ii;
  int          tdst_len;
  request_rec* r;

  jhtml = GET_JHTML(pdoc);
  doc   = jhtml->doc;
  r     = doc->r;

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
#if BUFFERED
  WJ_V(tdst);
#else
  jhtml->out = apr_pstrcat(r->pool, jhtml->out, tdst, NULL);
#endif

  return jhtml->out;
}
/*
 * vim:ts=2 et
 */
