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
#include <libgen.h>
#include <limits.h>
#include "mod_chxj.h"
#include "chxj_img_conv_format.h"
#include "chxj_specified_device.h"
#include "chxj_str_util.h"
#include "chxj_qr_code.h"
#include "chxj_apply_convrule.h"
#include "chxj_url_encode.h"
#include "qs_parse_string.h"
#include "chxj_preg_replace.h"

#include "http_core.h"

#include <wand/magick_wand.h>


#define EXIT_MAGICK_ERROR() \
  do { \
    char *description; ExceptionType severity; \
    description=MagickGetException(magick_wand,&severity); \
    ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r,"%s %s %d %s\n",__FILE__,(__func__),__LINE__,description); \
    description=(char *) MagickRelinquishMemory(description); \
    DestroyMagickWand(magick_wand); \
  } while(0) 

typedef enum img_conv_mode_t {
  IMG_CONV_MODE_NORMAL = 0,
  IMG_CONV_MODE_THUMBNAIL,
  IMG_CONV_MODE_WALLPAPER,
  IMG_CONV_MODE_EZGET,
} img_conv_mode_t;

/*----------------------------------------------------------------------------*/
/* User-Agent use flag                                                        */
/*----------------------------------------------------------------------------*/
typedef enum _ua_use_flag_t {
  UA_USE=0,               /* User-Agent is used.                              */
  UA_IGN,                 /* User-Agent is disregarded.                       */
} ua_use_flag_t;

/*----------------------------------------------------------------------------*/
/* Request parameter maintenance structure                                    */
/*----------------------------------------------------------------------------*/
typedef struct query_string_param_t query_string_param_t;

struct query_string_param_t {
  img_conv_mode_t   mode;
  char              *user_agent;
  ua_use_flag_t     ua_flag;

  char              *name;      /* for EZGET */
  long              offset;    /* for EZGET */
  long              count;     /* for EZGET */
  int               width;
  int               height;
};

/*----------------------------------------------------------------------------*/
/* Device_spec when User-Agent is disregarded                                 */
/*----------------------------------------------------------------------------*/
static device_table v_ignore_spec = {
  NULL,
  "IGN",
  "IGN",
  CHXJ_SPEC_HTML,
  640,
  480,
  640,
  480,
  1024*1024,
  1,
  1,
  1,
  0,
  0,
  96,
  96,
  65536,
  NULL,
};

/*----------------------------------------------------------------------------*/
/* CRC calculation table for AU                                               */
/*----------------------------------------------------------------------------*/
static unsigned short  AU_CRC_TBL[256] = {
  0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
  0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
  0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
  0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
  0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
  0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
  0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
  0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
  0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
  0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
  0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
  0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
  0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
  0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
  0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
  0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
  0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
  0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
  0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
  0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
  0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
  0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
  0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
  0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
  0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
  0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
  0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
  0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
  0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
  0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
  0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
  0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0 
};

/*----------------------------------------------------------------------------*/
/* Download page for AU                                                       */
/*----------------------------------------------------------------------------*/
static const char *HDML_FIRST_PAGE = 
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <NODISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=GOSUB DEST=\"device:data/dnld?url=%s&name=%s%s&size=%ld&disposition=%s&title=%s\">\r\n"
  "  </NODISPLAY>\r\n"
  "</HDML>\r\n";

static const char *HDML_SUCCESS_PAGE =
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <DISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=RETURN>\r\n"
  "    \x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc9\x90\xac\x8c\xf7\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n"
  "  </DISPLAY>\r\n"
  "<HDML>\r\n";

static const char *HDML_FAIL_PAGE =
  "<HDML VERSION=3.0 TTL=0 PUBLIC=TRUE>\r\n"
  "  <DISPLAY>\r\n"
  "    <ACTION TYPE=ACCEPT TASK=RETURN>\r\n"
  "    \x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n"
  "  </DISPLAY>\r\n"
  "<HDML>\r\n";

static ap_regex_t *v_docomo_serial_pattern1 = NULL;
static ap_regex_t *v_docomo_serial_pattern2 = NULL;
static ap_regex_t *v_docomo_serial_pattern3 = NULL;
static ap_regex_t *v_softbank_serial_pattern1 = NULL;

/*----------------------------------------------------------------------------*/
/* Prototype declaration                                                      */
/*----------------------------------------------------------------------------*/
static char *s_create_workfile_name(request_rec *, 
                                    mod_chxj_config *, 
                                    const char *,
                                    query_string_param_t *);

static apr_status_t s_create_cache_file(request_rec          *r, 
                                        const char           *tmpfile, 
                                        device_table         *spec,
                                        apr_finfo_t          *st,
                                        query_string_param_t *qsp,
                                        mod_chxj_config      *conf);

static apr_status_t s_send_cache_file(device_table          *spec,
                                      query_string_param_t  *query_string,
                                      request_rec           *r,
                                      const char            *tmpfile);

static apr_status_t s_send_original_file(request_rec *r, 
                                         const char  *originalfile);

static apr_status_t s_header_only_cache_file(device_table         *spec, 
                                             query_string_param_t *query_string, 
                                             request_rec          *r, 
                                             const char           *tmpfile);

static query_string_param_t *s_get_query_string_param(request_rec *r);

static unsigned short s_add_crc(const char *writedata, apr_size_t witebyte);

static MagickWand *s_fixup_size(MagickWand   *, 
                                request_rec  *r, 
                                device_table *spec, 
                                query_string_param_t *qsp);

static MagickWand *s_fixup_color(MagickWand *magick_wand, 
                                 request_rec *r, 
                                 device_table *spec, 
                                 img_conv_mode_t mode);
static MagickWand *s_fixup_depth(MagickWand* magick_wand, 
                                 request_rec* r, device_table* spec);
static MagickWand *s_img_down_sizing(MagickWand *magick_wand, 
                                request_rec *r, device_table *spec);

static MagickWand *s_add_copyright(MagickWand *magick_wand,
                                   request_rec *r,
                                   device_table *spec);

static char *s_create_blob_data(request_rec *r,
                                device_table *spec,
                                query_string_param_t *qsp,
                                char *indata,
                                apr_size_t *len);

static int s_img_conv_format_from_file(request_rec          *r, 
                                       mod_chxj_config      *conf, 
                                       const char           *user_agent,
                                       query_string_param_t *qsp,
                                       device_table         *spec);



int 
chxj_img_conv_format_handler(request_rec *r)
{
  mod_chxj_config       *conf;
  query_string_param_t  *qsp;
  char                  *user_agent;
  device_table          *spec;
  chxjconvrule_entry    *entryp;

  DBG(r, "start chxj_img_conv_format_handler()");
  
  if (r->handler && !STRCASEEQ('c','C',"chxj-picture",r->handler) && !STRCASEEQ('c','C',"chxj-qrcode",r->handler)) {
    DBG(r, "end chxj_img_conv_format_handler()");
    return DECLINED;
  }

  qsp = s_get_query_string_param(r);
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (conf == NULL) {
    DBG(r, "end chxj_img_conv_format_handler() conf is null");
    return DECLINED;
  }

  if (STRCASEEQ('c','C',"chxj-qrcode",r->handler) && conf->image == CHXJ_IMG_OFF) {
    return DECLINED;
  }

  /*------------------------------------------------------------------------*/
  /* get UserAgent from http header                                         */
  /*------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* User-Agent to spec                                                       */
  /*--------------------------------------------------------------------------*/
  if (qsp->user_agent) {
    user_agent = apr_pstrdup(r->pool, qsp->user_agent);
  }
  else {
    entryp = chxj_apply_convrule(r, conf->convrules);
    if (entryp && entryp->user_agent) {
      user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
    }
    else {
      user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
    }
  }



  if (qsp->ua_flag == UA_IGN)
    spec = &v_ignore_spec;
  else
    spec = chxj_specified_device(r, user_agent);

  DBG(r,"found device_name=[%s]", spec->device_name);
  DBG(r,"User-Agent=[%s]", user_agent);


  return s_img_conv_format_from_file(r, conf, user_agent, qsp, spec);
}



/**
 * It converts it from ImageData corresponding to each model.
 *
 * @param r   [i]
 * @param src [i]   It is former image binary data.
 * @param len [i/o] It is length of former image binary data.
 */
char *
chxj_convert_image(request_rec *r, const char **src, apr_size_t *len)
{
  mod_chxj_config       *conf;
  query_string_param_t  *qsp;
  char                  *user_agent;
  device_table          *spec;
  char                  *dst;
  char                  *conv_check;
  chxjconvrule_entry    *entryp;

  DBG(r, "start chxj_convert_image()");

  conv_check = (char*)apr_table_get(r->headers_in, "CHXJ_IMG_CONV");
  if (conv_check) {
    DBG(r, "end chxj_exchnage_image() already convert.");
    return NULL;
  }


  qsp = s_get_query_string_param(r);
  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);
  if (conf == NULL) {
    DBG(r, "end chxj_convert_image()");
    return NULL;
  }

  /*--------------------------------------------------------------------------*/
  /* User-Agent to spec                                                       */
  /*--------------------------------------------------------------------------*/
  if (qsp->user_agent) {
    user_agent = apr_pstrdup(r->pool, qsp->user_agent);
  }
  else {
    entryp = chxj_apply_convrule(r, conf->convrules);
    if (entryp && entryp->user_agent) {
      user_agent = (char*)apr_table_get(r->headers_in, CHXJ_HTTP_USER_AGENT);
    }
    else {
      user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
    }
  }

  if (qsp->ua_flag == UA_IGN)
    spec = &v_ignore_spec;
  else
    spec = chxj_specified_device(r, user_agent);

  DBG(r,"found device_name=[%s]", spec->device_name);
  DBG(r, "User-Agent=[%s]", user_agent);

  if (spec->width == 0 || spec->heigh == 0) 
    return NULL;

  dst = s_create_blob_data(r, spec, qsp, (char*)*src, len);
  if (dst == NULL) 
    *len = 0;

  DBG(r, "end chxj_convert_image()");

  return dst;
}


static int
s_img_conv_format_from_file(
                request_rec          *r, 
                mod_chxj_config      *conf, 
                const char           *user_agent,
                query_string_param_t *qsp,
                device_table         *spec)
{
  apr_status_t   rv;
  apr_finfo_t    st;
  apr_finfo_t    cache_st;
  char           *tmpfile;
  int            try_count;

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    /* 
     * If ``ua'' parameter is specified, it must be CHXJ_SPEC_HTML. 
     */
    return s_send_original_file(r, r->filename);
  }

  /*--------------------------------------------------------------------------*/
  /* Create Workfile Name                                                     */
  /*--------------------------------------------------------------------------*/
  tmpfile = s_create_workfile_name(r, conf, user_agent, qsp);
  DBG(r,"workfile=[%s]", tmpfile);

  rv = apr_stat(&st, r->filename, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  try_count = CACHE_RETRY_COUNT;
  do {
    rv = apr_stat(&cache_st, tmpfile, APR_FINFO_MIN, r->pool);
  
    if (rv != APR_SUCCESS || cache_st.ctime < st.mtime) {
      /*------------------------------------------------------------------------*/
      /* It tries to make the cash file when it doesn't exist or there is       */
      /* change time later since the making time of the cash file.              */
      /*------------------------------------------------------------------------*/
      rv = s_create_cache_file(r,tmpfile, spec, &st, qsp, conf);
      if (rv != OK)
        return rv;
    }
  
    DBG(r,"color=[%d]", spec->color);
    if (! r->header_only)  {
      rv = s_send_cache_file(spec, qsp,r, tmpfile);
    }
    else {
      rv = s_header_only_cache_file(spec, qsp, r, tmpfile);
    }
    if (rv == OK) break;
    if (rv == HTTP_NOT_FOUND) {
      DBG(r, "recheck wait... try_count[%d]", try_count);
      apr_sleep(CACHE_RECHECK_WAIT);
    }
  } while (try_count--); 
  if (try_count <= 0) {
    WRN(r, "cache retry failure....");
    WRN(r, "cache file was deleted...");
  }
  apr_table_setn(r->headers_in, "CHXJ_IMG_CONV", "done");

  DBG(r,"end chxj_img_conv_format");

  return rv;
}


static apr_status_t
s_create_cache_file(request_rec          *r, 
                    const char           *tmpfile, 
                    device_table         *spec, 
                    apr_finfo_t          *st, 
                    query_string_param_t *qsp,
                    mod_chxj_config      *conf)
{
  apr_status_t       rv;
  apr_size_t         readbyte;
  apr_size_t         writebyte;
  unsigned short     crc;
  img_conv_mode_t    mode = qsp->mode;

  char *writedata = NULL;
  char *readdata  = NULL;

  apr_file_t  *fout;
  apr_file_t  *fin;
  apr_finfo_t cache_dir_st;

  MagickWand *magick_wand;

  if (STRCASEEQ('c','C',"chxj-qrcode",r->handler)) {
    /*------------------------------------------------------------------------*/
    /* QRCODE用のファイルの場合                                               */
    /*------------------------------------------------------------------------*/
    Doc       doc;
    Node      *root;
    qr_code_t qrcode;
    int       sts;

    memset(&doc,    0, sizeof(Doc));
    memset(&qrcode, 0, sizeof(qr_code_t));
    doc.r = r;
    doc.parse_mode  = PARSE_MODE_CHTML;
    qrcode.doc      = &doc;
    qrcode.r        = r;

    qs_init_malloc(&doc);

    root = qs_parse_file(&doc, r->filename);

    chxj_qrcode_node_to_qrcode(&qrcode, root);

    qs_all_free(&doc,QX_LOGMARK);

    sts = chxj_qrcode_create_image_data(&qrcode, &readdata, &readbyte);
    if (sts != OK) {
      ERR(r, "qrcode create failed.");
      return sts;
    }
  }
  else {
    /*------------------------------------------------------------------------*/
    /* 通常のイメージファイルの場合                                           */
    /*------------------------------------------------------------------------*/
    rv = apr_file_open(&fin, 
                    r->filename, 
                    APR_READ|APR_BINARY ,
                    APR_OS_DEFAULT, 
                    r->pool);
    if (rv != APR_SUCCESS) {
      DBG(r,"file open failed.[%s]", r->filename);
      return HTTP_NOT_FOUND;
    }
  
    readdata = apr_palloc(r->pool, st->size);
    rv = apr_file_read_full(fin, (void*)readdata, st->size, &readbyte);
    if (rv != APR_SUCCESS || readbyte != st->size) {
      DBG(r,"file read failed.[%s]", r->filename);
      apr_file_close(fin);
  
      return HTTP_NOT_FOUND;
    }
  }
  DBG(r,"start img convert");


  magick_wand = NewMagickWand();
  if (MagickReadImageBlob(magick_wand,readdata, readbyte) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return HTTP_NOT_FOUND;
  }

  if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
    /*
     * The size of the image is changed.
     */
    DBG(r,"call s_fixup_size()");
  
    if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL) 
      return HTTP_NOT_FOUND;
  
    /*
     * The colors of the image is changed.
     */
    DBG(r,"call s_fixup_color()");
  
    if ((magick_wand = s_fixup_color(magick_wand, r,spec, mode)) == NULL) 
      return HTTP_NOT_FOUND;
  
    /*
     * DEPTH of the image is changed.
     */
    DBG(r,"call s_fixup_depth()");
  
    if ((magick_wand = s_fixup_depth(magick_wand, r, spec)) == NULL) 
      return HTTP_NOT_FOUND;
  
  
  
    DBG(r,"start convert and compression");
  
    if (spec->available_jpeg) {
      if (MagickSetImageCompression(magick_wand,JPEGCompression) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickSetImageFormat(magick_wand, "jpg") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickStripImage(magick_wand) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
        return HTTP_NOT_FOUND;
  
      r->content_type = apr_psprintf(r->pool, "image/jpeg");
      DBG(r,"convert to jpg");
    }
    else
    if (spec->available_png) {
  
      if (MagickSetImageCompression(magick_wand,ZipCompression) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickSetImageFormat(magick_wand, "png") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickStripImage(magick_wand) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
        return HTTP_NOT_FOUND;
  
      r->content_type = apr_psprintf(r->pool, "image/png");
      DBG(r, "convert to png");
    }
    else
    if (spec->available_gif) {
  
      if (MagickSetImageCompression(magick_wand,LZWCompression) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickSetImageFormat(magick_wand, "gif") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickStripImage(magick_wand) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
        return HTTP_NOT_FOUND;
  
      r->content_type = apr_psprintf(r->pool, "image/gif");
  
      DBG(r,"convert to gif");
    }
    else
    if (spec->available_bmp2 || spec->available_bmp4) {
  
      if (MagickSetImageCompression(magick_wand,NoCompression) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickSetImageFormat(magick_wand, "bmp") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if (MagickStripImage(magick_wand) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL) 
        return HTTP_NOT_FOUND;
  
      r->content_type = apr_psprintf(r->pool, "image/bmp");
  
      DBG(r, "convert to bmp(unsupported)");
    }
  
    /*
     * Add Comment (Copyright and so on.)
     */
    DBG(r, "call s_add_copyright()");
  
    if ((magick_wand = s_add_copyright(magick_wand, r, spec)) == NULL) 
      return HTTP_NOT_FOUND;
  }
  else {
    char* fmt;
    fmt = MagickGetImageFormat(magick_wand);
    if (fmt == NULL) {
      if (MagickSetImageFormat(magick_wand, "jpg") == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return HTTP_NOT_FOUND;
      }
  
      r->content_type = apr_psprintf(r->pool, "image/jpeg");
    }
    else {
      if (strcasecmp(fmt, "jpg") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/jpeg");
      }
      else
      if (strcasecmp(fmt, "jpeg") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/jpeg");
      }
      else
      if (strcasecmp(fmt, "gif") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/gif");
      }
      else
      if (strcasecmp(fmt, "png") == 0) {
        r->content_type = apr_psprintf(r->pool, "image/png");
      }
    }
  }

  writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

  if (! writebyte) {
    DestroyMagickWand(magick_wand);
    ERR(r,"convert failure to Jpeg [%s]", tmpfile);

    return HTTP_INTERNAL_SERVER_ERROR;
  }

  DBG(r, "end convert and compression");
  
  /* check limit */
  rv = apr_stat(&cache_dir_st, conf->image_cache_dir, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    ERR(r,"dir stat error.[%s]", conf->image_cache_dir);
    if (writedata) free(writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  
  for (;;) {
    /* delete candidate */
    apr_finfo_t dcf;   
    /* get dir files size */
    apr_dir_t *dir;
    unsigned long total_size = 0;
    int found_file = 0;
    unsigned long max_size = (! conf->image_cache_limit) ? DEFAULT_IMAGE_CACHE_LIMIT : conf->image_cache_limit;
    char *delete_file_name;

    rv = apr_dir_open(&dir, conf->image_cache_dir, r->pool);
    if (rv != APR_SUCCESS) { 
      DestroyMagickWand(magick_wand);
      ERR(r,"dir open error.[%s]", conf->image_cache_dir);
      if (writedata) free(writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    memset(&dcf, 0, sizeof(apr_finfo_t));
    dcf.atime = (apr_time_t)LONG_LONG_MAX;
    for (;;) {
      apr_finfo_t dirf;
      rv = apr_dir_read(&dirf, APR_FINFO_SIZE|APR_FINFO_NAME|APR_FINFO_DIRENT|APR_FINFO_ATIME , dir);
      if (rv != APR_SUCCESS) {
        break;
      }
      if (dirf.name && strcmp(dirf.name, ".") != 0 && strcmp(dirf.name, "..") != 0) {
        total_size += (unsigned long)dirf.size;
        DBG(r, "dirf.name=[%s] dirf.size=[%ld] dirf.atime=[%lld]", dirf.name, (long)dirf.size, (long long int)dirf.atime);
        if (dcf.atime >= dirf.atime) {
          memcpy(&dcf, &dirf, sizeof(apr_finfo_t));
        }
        found_file++;
      }
    }
    apr_dir_close(dir);
    if (total_size + writebyte < max_size) {
      DBG(r, "There is an enough size in cache. total_size:[%lu] max_size:[%lu] found_file=[%d] max_size=[%lu]", total_size, max_size, found_file, max_size);
      break;
    }
    if (found_file == 0 && writebyte >= max_size) {
      ERR(r, "==========================================");
      ERR(r, "cache space is too small...");
      ERR(r, "At least the same size as %luByte is necessary for me.", (unsigned long)writebyte); 
      ERR(r, "Please specify the ChxjImageCacheLimit that is larger than now value. ");
      ERR(r, "==========================================");
      if (writedata) free(writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    DBG(r, "Image Cache dir is full. total_size:[%lu] max_size:[%lu]", total_size + writebyte, max_size);
    /* search delete candidate */
    delete_file_name = apr_psprintf(r->pool, "%s/%s", conf->image_cache_dir, dcf.name);
    DBG(r, "delete image cache target:[%s] atime:[%lld]", delete_file_name, (long long int)dcf.atime);
    rv = apr_file_remove(delete_file_name, r->pool);
    if (rv != APR_SUCCESS) {
      ERR(r, "cache file delete failure.[%s]", delete_file_name);
      if (writedata) free(writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    DBG(r, "deleted image cache target:[%s]", delete_file_name);
    if (total_size + writebyte - dcf.size < max_size) {
      DBG(r, "OK, there is an enough size in cache.");
      break;
    }
  }

  /* to cache */
  rv = apr_file_open(&fout, tmpfile,
                  APR_WRITE| APR_CREATE | APR_BINARY | APR_SHARELOCK ,APR_OS_DEFAULT,
                  r->pool);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    ERR(r,"file open error.[%s]", tmpfile);
    if (writedata) free(writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  rv = apr_file_write(fout, (void*)writedata, &writebyte);
  if (rv != APR_SUCCESS) {
    DestroyMagickWand(magick_wand);
    apr_file_close(fout);
    if (writedata) free(writedata);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  /*
   * CRC is added for AU for EzGET.
   */
  if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
  ||  spec->html_spec_type == CHXJ_SPEC_Hdml            ) {

    crc = s_add_crc(writedata, writebyte);

    rv = apr_file_putc((crc >> 8)  & 0xff, fout);
    if (rv != APR_SUCCESS) {
      DestroyMagickWand(magick_wand);
      if (writedata) free(writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }

    rv = apr_file_putc( crc        & 0xff, fout);
    if (rv != APR_SUCCESS) {
      DestroyMagickWand(magick_wand);
      if (writedata) free(writedata);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
  }

  DestroyMagickWand(magick_wand);
  if (writedata) free(writedata);

  rv = apr_file_close(fout);
  if (rv != APR_SUCCESS) {
    DBG(r,"file write error.[%s]", tmpfile);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  return OK;
}


static char *
s_create_blob_data(request_rec          *r, 
                   device_table         *spec, 
                   query_string_param_t *qsp,
                   char                 *indata,
                   apr_size_t           *len)
{
  apr_size_t         writebyte;
  unsigned short     crc;
  img_conv_mode_t    mode = qsp->mode;

  char *writedata = NULL;
  char *dst       = NULL;
  MagickWand *magick_wand;

  magick_wand = NewMagickWand();

  if (MagickReadImageBlob(magick_wand,indata, *len) == MagickFalse) {
    EXIT_MAGICK_ERROR();
    return NULL;
  }

  /*
   * The size of the image is changed.
   */
  DBG(r, "call s_fixup_size()");

  if ((magick_wand = s_fixup_size(magick_wand, r, spec, qsp)) == NULL)
    return NULL;

  /*
   * The colors of the image is changed.
   */
  DBG(r, "call s_fixup_color()");

  if ((magick_wand = s_fixup_color(magick_wand, r,spec, mode)) == NULL)
    return NULL;

  /*
   * DEPTH of the image is changed.
   */

  DBG(r,"call s_fixup_depth()");

  if ((magick_wand = s_fixup_depth(magick_wand, r, spec)) == NULL)
    return NULL;



  DBG(r,"start convert and compression");

  if (spec->available_jpeg) {
    if (MagickSetImageCompression(magick_wand,JPEGCompression) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickSetImageFormat(magick_wand, "jpg") == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickStripImage(magick_wand) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
      return NULL;

    r->content_type = apr_psprintf(r->pool, "image/jpeg");

    DBG(r, "convert to jpg");
  }
  else
  if (spec->available_png) {
    if (MagickSetImageCompression(magick_wand,ZipCompression) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickSetImageFormat(magick_wand, "png") == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickStripImage(magick_wand) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
      return NULL;

    r->content_type = apr_psprintf(r->pool, "image/png");

    DBG(r,"convert to png");
  }
  else
  if (spec->available_gif) {

    if (MagickSetImageCompression(magick_wand,LZWCompression) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickSetImageFormat(magick_wand, "gif") == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickStripImage(magick_wand) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
      return NULL;

    r->content_type = apr_psprintf(r->pool, "image/gif");

    DBG(r,"convert to gif");
  }
  else
  if (spec->available_bmp2 || spec->available_bmp4) {
    if (MagickSetImageCompression(magick_wand,NoCompression) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickSetImageFormat(magick_wand, "bmp") == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if (MagickStripImage(magick_wand) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    if ((magick_wand = s_img_down_sizing(magick_wand, r, spec)) == NULL)
      return NULL;

    r->content_type = apr_psprintf(r->pool, "image/bmp");

    DBG(r,"convert to bmp(unsupported)");
  }
  /*--------------------------------------------------------------------------*/
  /* Add Comment (Copyright and so on.)                                       */
  /*--------------------------------------------------------------------------*/
  DBG(r,"call s_add_copyright()");

  if ((magick_wand = s_add_copyright(magick_wand, r, spec)) == NULL)
    return NULL;

  writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

  if (! writebyte) {
    DestroyMagickWand(magick_wand);
    DBG(r,"convert failure to Jpeg ");
    return NULL;
  }

  DBG(r,"end convert and compression");


  
  dst = apr_palloc(r->pool, writebyte+2);

  memcpy(dst, writedata, writebyte);
  /*--------------------------------------------------------------------------*/
  /* CRC is added for AU for EzGET.                                           */
  /*--------------------------------------------------------------------------*/
  if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
  ||  spec->html_spec_type == CHXJ_SPEC_Hdml) {
    crc = s_add_crc(writedata, writebyte);
    dst[writebyte + 0] = (crc >> 8) & 0xff;
    dst[writebyte + 1] = (crc     ) & 0xff;
    writebyte += 2;
  }

  DestroyMagickWand(magick_wand);

  *len = writebyte;
  return dst;
}


static MagickWand *
s_fixup_size(MagickWand           *magick_wand, 
             request_rec          *r, 
             device_table         *spec, 
             query_string_param_t *qsp)
{
  img_conv_mode_t mode = qsp->mode;
  int oldw;
  int oldh;
  int neww;
  int newh;
  int c_width;
  int c_heigh;

  oldw = MagickGetImageWidth(magick_wand);
  oldh = MagickGetImageHeight(magick_wand);

  DBG(r,"detect width=[%d]", oldw);
  DBG(r,"detect heigh=[%d]", oldh);

  neww = oldw;
  newh = oldh;

  DBG(r,"detect spec width=[%d]", spec->width);
  DBG(r,"detect spec heigh=[%d]", spec->heigh);

  c_width = spec->width;
  c_heigh = spec->heigh;

  switch(mode) {
  case IMG_CONV_MODE_THUMBNAIL:

    DBG(r,"**** detect thumbnail mode ****");

    if (neww > c_width) {
      newh = (int)((double)newh * (double)((double)c_width / (double)neww));
      neww = (int)((double)neww * (double)((double)c_width / (double)neww));
    }
    if (newh > c_heigh) {
      neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
      newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
    }

    neww = (int)((double)(neww / 3) * 0.8);
    newh = (int)((double)(newh / 3) * 0.8);
    break;

  case IMG_CONV_MODE_WALLPAPER:
  case IMG_CONV_MODE_EZGET:

    DBG(r,"**** detect wallpaper mode ****");

    if (spec->wp_width && spec->wp_heigh) {
      c_width = spec->wp_width;
      c_heigh = spec->wp_heigh;
    }

    DBG(r,"calc new width and height");

    neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
    newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));

    DBG(r,"newh = [%d] neww = [%d]", newh, neww);
    break;

  default:

    DBG(r,"**** detect normal mode ****");

    if (qsp->ua_flag != UA_IGN && spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
      if (neww > c_width) {
        newh = (int)((double)newh * (double)((double)c_width / (double)neww));
        neww = (int)((double)neww * (double)((double)c_width / (double)neww));
      }

      if (newh > c_heigh) {
        neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
        newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
      }
    }
    break;
  }

  if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
    DBG(r,"convert width=[%d --> %d]", oldw, neww);
    DBG(r,"convert heigh=[%d --> %d]", oldh, newh);
  
    MagickResetIterator(magick_wand);
  
    while (MagickNextImage(magick_wand) != MagickFalse) {
      switch (mode) {
      case IMG_CONV_MODE_WALLPAPER:
      case IMG_CONV_MODE_EZGET:
  
        if (MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
  
        if (MagickCropImage(magick_wand, 
                        (unsigned long)c_width, 
                        (unsigned long)c_heigh,
                        (long)((neww - c_width) / 2),
                        (long)((newh - c_heigh) / 2)) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
  
      case IMG_CONV_MODE_NORMAL:
        if (qsp->width) {
          DBG(r,"convert width=[%d --> %d]", neww, qsp->width);
          neww = qsp->width;
        }
        if (qsp->height) {
          DBG(r,"convert heigh=[%d --> %d]", newh, qsp->height);
          newh = qsp->height;
        }
  
      default:
        if (MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
      }
  
      if (spec->html_spec_type != CHXJ_SPEC_UNKNOWN) {
        if (MagickSetImageUnits(magick_wand, PixelsPerInchResolution) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
    
        if (MagickSetImageResolution(magick_wand,
                                     (double)spec->dpi_width,
                                     (double)spec->dpi_heigh) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
    
        if (MagickSetImageDispose(magick_wand, BackgroundDispose) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
      }
    }
  }
  return magick_wand;
}


static MagickWand *
s_fixup_color(MagickWand *magick_wand, request_rec *r, device_table *spec, img_conv_mode_t UNUSED(mode))
{
  DBG(r,"start chxj_fixup_clor()");

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r, "Pass s_fixup_color proc");
    return magick_wand;
  }

  if (spec->color >= 256) {

    DBG(r,"call MagickQuantizeImage() spec->color=[%d]",spec->color);

    if (MagickQuantizeImage(magick_wand,
                           spec->color,
                           RGBColorspace,
                           0,
                           1,
                           0) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    DBG(r,"call end MagickQuantizeImage() spec->color=[%d]",spec->color);

  }
  else {
    DBG(r,"call MagickQuantizeImage() spec->color=[%d]",spec->color);

    if (MagickQuantizeImage(magick_wand,
                           spec->color,
                           GRAYColorspace,
                           0,
                           1,
                           0) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    DBG(r,"call end MagickQuantizeImage() spec->color=[%d]",spec->color);
  }


  DBG(r,"end chxj_fixup_clor()");

  return magick_wand;
}



static MagickWand *
s_fixup_depth(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r, "Pass s_fixup_depth proc");
    return magick_wand;
  }

  if (spec->color == 15680000) {
    if (MagickSetImageDepth(magick_wand, 24) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else 
  if (spec->color == 262144) {
    if (MagickSetImageDepth(magick_wand, 18) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 65536) {
    if (MagickSetImageDepth(magick_wand, 16) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4096) {
    if (MagickSetImageDepth(magick_wand, 12) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 256) {
    if (MagickSetImageDepth(magick_wand, 8) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4) {
    if (MagickSetImageDepth(magick_wand, 2) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 2) {
    if (MagickSetImageDepth(magick_wand, 1) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }

  return magick_wand;
}


static MagickWand *
s_add_copyright(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  mod_chxj_config *conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (spec->html_spec_type == CHXJ_SPEC_UNKNOWN) {
    DBG(r, "Pass add_copiright proc");
    return magick_wand;
  }

  if (conf->image_copyright) {

    DBG(r, "Add COPYRIGHT [%s]", conf->image_copyright);

    if (spec->html_spec_type == CHXJ_SPEC_Jhtml) {
      apr_table_setn(r->headers_out, "x-jphone-copyright", "no-transfer");
      if (MagickCommentImage(magick_wand, 
                             apr_psprintf(r->pool, 
                                          "Copyright(C) %s", 
                                          conf->image_copyright)) == MagickFalse) 
        goto on_error;
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
    ||  spec->html_spec_type == CHXJ_SPEC_Hdml) {
      if (MagickCommentImage(magick_wand, 
                             apr_psprintf(r->pool, 
                                         "kddi_copyright=on,%s", 
                                          conf->image_copyright)) == MagickFalse) 
        goto on_error;
    }
    else {
      if (MagickCommentImage(magick_wand, 
                            apr_psprintf(r->pool, 
                                         "copy=\"NO\",%s",
                                         conf->image_copyright)) == MagickFalse)
        goto on_error;
    }
  }
  else {
    if (MagickCommentImage(magick_wand, "mod_chxj") == MagickFalse)
      goto on_error;
  }
  return magick_wand;

on_error:
  EXIT_MAGICK_ERROR();
  return NULL;
}

static MagickWand *
s_img_down_sizing(MagickWand *magick_wand, request_rec *r, device_table *spec)
{
  MagickBooleanType  status;
  unsigned long      quality = 70;
  apr_size_t         writebyte = 0;
  char               *writedata;
  apr_size_t         prev_size = 0;
  int                revers_flag = 0;

  writedata = (char *)MagickGetImageBlob(magick_wand, &writebyte);
  prev_size = writebyte;

  do {
    if (MagickSetImageCompressionQuality(magick_wand, quality) == MagickFalse) {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

    writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);
    if (writebyte >= prev_size || revers_flag) {
      DBG(r, "quality=[%ld] size=[%d]", (long)quality, (int)writebyte);
      revers_flag = 1;
      quality += 10;
      if (quality > 100) {
        if (MagickSetImageCompression(magick_wand,NoCompression) == MagickFalse) {
          EXIT_MAGICK_ERROR();
          return NULL;
        }
        break;
      }
      prev_size = writebyte;
      continue;
    }

    DBG(r, "quality=[%ld] size=[%d]", (long)quality, (int)writebyte);

    if (spec->cache == 0)
      break;

    if (writebyte <= (unsigned int)spec->cache)
      break;

    quality -= 10;

    if (quality == 0 || quality > 100)
      break;

  }
  while (1);


  if (spec->cache > 0 
  &&  writebyte   > (unsigned int)spec->cache) {
    unsigned long now_color = spec->color;
    unsigned long depth     = 0;
    do {
      switch(now_color) {
      case 2:      depth = 1; break;
      case 4:      now_color = 2;        depth = 1;  break;
      case 8:      now_color = 4;        depth = 2;  break;
      case 16:     now_color = 8;        depth = 3;  break;
      case 256:    now_color = 16;       depth = 4;  break;
      case 4096:   now_color = 256;      depth = 8;  break;
      case 65536:  now_color = 4096;     depth = 12; break;
      case 262144: now_color = 65536;    depth = 16; break;
      case 15680000: now_color = 262144; depth = 18; break;
      default:
        now_color = 2;
        break;
      }

      if (now_color <= 2) break;

      if (now_color >= 8) {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             RGBColorspace,
                             0,
                             1,
                             0);
      }
      else {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             GRAYColorspace,
                             0,
                             1,
                             0);
        MagickSetImageType(magick_wand, GrayscaleType);
      }
      if (status == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }

      if (MagickSetImageDepth(magick_wand, depth) == MagickFalse) {
        EXIT_MAGICK_ERROR();
        return NULL;
      }

      writedata = (char*)MagickGetImageBlob(magick_wand, &writebyte);

      DBG(r,"now_color=[%ld] size=[%d]", (long)now_color, (int)writebyte);

      /* Once per request */
      break;
    }
    while(now_color > 2);
  }

  return magick_wand;
}


static apr_status_t 
s_send_cache_file(device_table *spec, query_string_param_t *query_string, request_rec *r, const char *tmpfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  apr_file_t   *fout;
  apr_size_t   sendbyte;
  char         *contentLength;

  rv = apr_stat(&st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  DBG(r, "mode:[%d]",    query_string->mode);
  DBG(r, "name:[%s]",    query_string->name);
  DBG(r, "offset:[%ld]", query_string->offset);
  DBG(r, "count:[%ld]",  query_string->count);

  if (spec->available_jpeg) {
    r->content_type = apr_psprintf(r->pool, "image/jpeg");
  }
  else
  if (spec->available_png) {
    r->content_type = apr_psprintf(r->pool, "image/png");
  }
  else
  if (spec->available_gif) {
    r->content_type = apr_psprintf(r->pool, "image/gif");
  }
  else
  if (spec->available_bmp2 || spec->available_bmp4) {
    r->content_type = apr_psprintf(r->pool, "image/bmp");
  }

  if (query_string->mode != IMG_CONV_MODE_EZGET && query_string->name == NULL) {
    contentLength = apr_psprintf(r->pool, "%d", (int)st.size);
    apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
    DBG(r,"Content-Length:[%d]", (int)st.size);

    rv = apr_file_open(&fout, tmpfile, 
      APR_READ | APR_BINARY, APR_OS_DEFAULT, r->pool);
    if (rv != APR_SUCCESS) {
      DBG(r, "cache file open failed[%s]", tmpfile);
      return HTTP_NOT_FOUND;
    }

    ap_send_fd(fout, r, 0, st.size, &sendbyte);
    apr_file_close(fout);
    ap_rflush(r);
    DBG(r, "send file data[%d]byte", (int)sendbyte);
  }
  else
  if (query_string->mode == IMG_CONV_MODE_EZGET) {
    char *name = apr_pstrdup(r->pool, basename(r->filename));
    name[strlen(name)-4] = 0;
    if (strcasecmp(r->content_type, "image/jpeg") == 0) {

      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".jpg", (long)st.size, "devjaww", name);
    }
    else
    if (strcasecmp(r->content_type, "image/bmp") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".bmp", (long)st.size, "devabm", name);
    }
    else
    if (strcasecmp(r->content_type, "image/png") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".png", (long)st.size, "dev8aww", name);
    }
    else
    if (strcasecmp(r->content_type, "image/gif") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FIRST_PAGE, r->uri, name, ".gif", (long)st.size, "devgi0z", name);
    }
  }
  else
  if (query_string->mode == IMG_CONV_MODE_WALLPAPER && query_string->name != NULL) {
    if (query_string->count == -1 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_SUCCESS_PAGE);
      ap_rflush(r);
    }
    else
    if (query_string->count == -2 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
      ap_rprintf(r, HDML_FAIL_PAGE);
      ap_rflush(r);
    }
    else { 
      chxj_set_content_type(r, "application/x-up-download");
      contentLength = apr_psprintf(r->pool, "%ld", query_string->count);
      apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
      DBG(r, "Content-Length:[%d]", (int)st.size);

      rv = apr_file_open(&fout, tmpfile, 
        APR_READ | APR_BINARY, APR_OS_DEFAULT, r->pool);

      if (rv != APR_SUCCESS) {
        DBG(r,"tmpfile open failed[%s]", tmpfile);
        return HTTP_NOT_FOUND;
      }

      ap_send_fd(fout, r, query_string->offset, query_string->count, &sendbyte);
      apr_file_close(fout);
      ap_rflush(r);
      DBG(r, "send file data[%d]byte", (int)sendbyte);
    }
  }
  
  return OK;
}


static apr_status_t 
s_send_original_file(request_rec *r, const char *originalfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  apr_file_t   *fout;
  apr_size_t   sendbyte = 0;

  rv = apr_stat(&st, originalfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  rv = apr_file_open(&fout, originalfile, 
    APR_READ | APR_BINARY, APR_OS_DEFAULT, r->pool);
  if (rv != APR_SUCCESS) {
    DBG(r, "originalfile open failed[%s]", originalfile);
    return HTTP_NOT_FOUND;
  }

  ap_send_fd(fout, r, 0, st.size, &sendbyte);
  apr_file_close(fout);
  ap_rflush(r);
  DBG(r, "send file data[%d]byte", (int)sendbyte);
  
  return OK;
}


static apr_status_t 
s_header_only_cache_file(device_table *spec, query_string_param_t *query_string, request_rec *r, const char *tmpfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  char         *contentLength;

  rv = apr_stat(&st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
    return HTTP_NOT_FOUND;

  DBG(r, "mode:[%d]",    query_string->mode);
  DBG(r, "name:[%s]",    query_string->name);
  DBG(r, "offset:[%ld]", query_string->offset);
  DBG(r, "count:[%ld]",  query_string->count);

  if (spec->available_jpeg) {
    r->content_type = apr_psprintf(r->pool, "image/jpeg");
  }
  else
  if (spec->available_png) {
    r->content_type = apr_psprintf(r->pool, "image/png");
  }
  else
  if (spec->available_gif) {
    r->content_type = apr_psprintf(r->pool, "image/gif");
  }
  else
  if (spec->available_bmp2 || spec->available_bmp4) {
    r->content_type = apr_psprintf(r->pool, "image/bmp");
  }

  if (query_string->mode != IMG_CONV_MODE_EZGET && query_string->name == NULL) {
    contentLength = apr_psprintf(r->pool, "%d", (int)st.size);
    apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
    DBG(r,"Content-Length:[%d]", (int)st.size);
  }
  else
  if (query_string->mode == IMG_CONV_MODE_EZGET) {
    char *name = apr_pstrdup(r->pool, basename(r->filename));
    name[strlen(name)-4] = 0;
    if (strcasecmp(r->content_type, "image/jpeg") == 0) {

      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/bmp") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/png") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (strcasecmp(r->content_type, "image/gif") == 0) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
  }
  else
  if (query_string->mode == IMG_CONV_MODE_WALLPAPER && query_string->name != NULL) {
    if (query_string->count == -1 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else
    if (query_string->count == -2 && query_string->offset == -1) {
      chxj_set_content_type(r, "text/x-hdml; charset=Shift_JIS");
    }
    else { 
      chxj_set_content_type(r, "application/x-up-download");
      contentLength = apr_psprintf(r->pool, "%ld", query_string->count);
      apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);
  
      DBG(r, "Content-Length:[%d]", (int)st.size);
    }
  }
  
  return OK;
}


static void 
s_init_serial_pattern(apr_pool_t *p)
{
  if (!v_docomo_serial_pattern1) {
    v_docomo_serial_pattern1 = chxj_compile_for_preg_replace(p, "/ser[^;\\)]+");
  }  
  if (!v_docomo_serial_pattern2) {
    v_docomo_serial_pattern2 = chxj_compile_for_preg_replace(p, ";ser[^;\\)]+");
  }  
  if (!v_docomo_serial_pattern3) {
    v_docomo_serial_pattern3 = chxj_compile_for_preg_replace(p, ";icc[^;\\)]+");
  }  
  if (!v_softbank_serial_pattern1) {
    v_softbank_serial_pattern1 = chxj_compile_for_preg_replace(p, "/SN[0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z][0-9a-zA-Z] ");
  }  
}


static char *
s_create_workfile_name(
                request_rec          *r, 
                mod_chxj_config      *conf, 
                const char           *user_agent, 
                query_string_param_t *qsp)
{
  int  ii;
  int  jj;
  int  len;
  char *w = apr_palloc(r->pool, 256);
  char *fname;
  char *new_user_agent;

  s_init_serial_pattern(r->server->process->pool);

  /* for DoCoMo */
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern1, "", user_agent);
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern2, "", new_user_agent);
  new_user_agent = chxj_preg_replace(r->pool, v_docomo_serial_pattern3, "", new_user_agent);

  /* for SoftBank */
  new_user_agent = chxj_preg_replace(r->pool, v_softbank_serial_pattern1, " ", new_user_agent);

  DBG(r, "old user_agent:[%s] ==> new user_agent:[%s]", user_agent, new_user_agent);


  memset(w, 0, 256);
  switch (qsp->mode) {
  case IMG_CONV_MODE_THUMBNAIL:
    fname = apr_psprintf(r->pool, "%s.%s.thumbnail", r->filename, new_user_agent);
    DBG(r, "mode=thumbnail [%s]", fname);
    break;

  case IMG_CONV_MODE_WALLPAPER:
  case IMG_CONV_MODE_EZGET:
    fname = apr_psprintf(r->pool, "%s.%s.wallpaper", r->filename, new_user_agent);
    DBG(r, "mode=WallPaper [%s]", fname);
    break;

  case IMG_CONV_MODE_NORMAL:
  default:

    fname = apr_psprintf(r->pool, "%s.%s", r->filename, new_user_agent);

    if (qsp->width)
      fname = apr_psprintf(r->pool, "%s.w%d", fname, qsp->width);

    if (qsp->height)
      fname = apr_psprintf(r->pool, "%s.h%d", fname, qsp->height);

    DBG(r,"mode=normal [%s]", fname);
    break;
  }
  if (qsp->ua_flag == UA_IGN) {
    fname = apr_psprintf(r->pool, "%s.IGN", fname);
  }

  len = strlen(fname);
  jj=0;
  for  (ii=0; ii<len; ii++) {
    if (fname[ii] == '/' 
    ||  fname[ii] == ' ' 
    ||  fname[ii] == '-' 
    ||  fname[ii] == '(' 
    ||  fname[ii] == ')') {
      w[jj++] = '_';
    }
    else {
      w[jj++] = fname[ii];
    }
  }

  return apr_psprintf(r->pool, "%s/%s", conf->image_cache_dir,w);
}


static unsigned short
s_add_crc(const char *writedata, apr_size_t writebyte)
{
  unsigned short crc = 0xffff;
  apr_size_t     ii;
  unsigned char  ch;

  for(ii=0;ii<writebyte;ii++) {
    ch  = writedata[ii];
    crc = AU_CRC_TBL[(crc>>8^ch)&0xff]^(crc<<8);
  }
  return crc;
}


int
chxj_trans_name(request_rec *r)
{
  const char      *ccp;
  char            *docroot;
  int             len;
  apr_finfo_t     st;
  apr_status_t    rv;
  mod_chxj_config *conf;
  int             ii;
  char            *ext[] = {
          "jpg",
          "jpeg",
          "png",
          "bmp",
          "gif",
          "qrc",    /* QRCode出力用ファイルの拡張子 */
          "",
  };
  char     *fname;
  char     *idx;
  char     *filename_sv;
  int      do_ext_check = TRUE;
  int      next_ok      = FALSE;

  DBG(r, "start chxj_trans_name()");

  conf = chxj_get_module_config(r->per_dir_config, &chxj_module);

  if (conf == NULL) {
    DBG(r, "end chxj_trans_name() conf is null[%s]", r->uri);
    return DECLINED;
  }

  if (conf->image != CHXJ_IMG_ON) {
    DBG(r, "end chxj_trans_name() conf not found");
    return DECLINED;
  }


  DBG(r,"Match URI[%s]", r->uri);


  if (r->filename == NULL) 
    r->filename = apr_pstrdup(r->pool, r->uri);

  filename_sv = NULL;
  if ((idx = strchr(r->filename, ':')) != NULL) 
    filename_sv = idx+1;
  else 
    filename_sv = r->filename;

  DBG(r,"r->filename[%s]", filename_sv);

  ccp = ap_document_root(r);
  if (ccp == NULL)
    return HTTP_INTERNAL_SERVER_ERROR;

  docroot = apr_pstrdup(r->pool, ccp);
  len = strlen(docroot);

  if (docroot[len-1] == '/') 
    docroot[len-1] = '\0';


  if (r->server->path 
  &&  *filename_sv == *r->server->path 
  &&  strncmp(filename_sv, r->server->path, r->server->pathlen) == 0)
    filename_sv = apr_pstrcat(r->pool, docroot, (filename_sv + r->server->pathlen), NULL);
  else
    filename_sv = apr_pstrcat(r->pool, docroot, filename_sv, NULL);

  DBG(r,"URI[%s]", filename_sv);

  do_ext_check = TRUE;
  for (ii=0; ii<7-1; ii++) {
    char* pos = strrchr(filename_sv, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        do_ext_check = FALSE;
        fname = apr_psprintf(r->pool, "%s", filename_sv);
        break;
      }
    }
  }

  if (do_ext_check) {
    for (ii=0; ii<7; ii++) {
      if (strlen(ext[ii]) == 0) {
        fname = apr_psprintf(r->pool, "%s", filename_sv);
      }
      else 
        fname = apr_psprintf(r->pool, "%s.%s", filename_sv, ext[ii]);
  
      DBG(r,"search [%s]", fname);
  
      rv = apr_stat(&st, fname, APR_FINFO_MIN, r->pool);
      if (rv == APR_SUCCESS) {
        if (st.filetype != APR_DIR)
          break;
      }
  
      fname = NULL;
    }
  }
  if (fname == NULL) {
    DBG(r,"NotFound [%s]", r->filename);
    return DECLINED;
  }
  for (ii=0; ii<7-1; ii++) {
    char* pos = strrchr(fname, '.');
    if (pos && pos++) {
      if (strcasecmp(pos, ext[ii]) == 0) {
        next_ok = TRUE;
        break;
      }
    }
  }

  if (! next_ok)  {
    DBG(r,"NotFound [%s]", r->filename);
    return DECLINED;
  }

  if (r->handler == NULL || strcasecmp(r->handler, "chxj-qrcode") != 0) {
    DBG(r,"Found [%s]", fname);

    r->filename = apr_psprintf(r->pool, "%s", fname);
  
    if (strcasecmp("qrc", ext[ii]) == 0)
      r->handler = apr_psprintf(r->pool, "chxj-qrcode");
    else
      r->handler = apr_psprintf(r->pool, "chxj-picture");
  }
  DBG(r, "end chxj_trans_name()");
  return OK;
}



/**
 * It converts it from QUERYSTRING.
 *
 * @param r   [i]
 */
static query_string_param_t *
s_get_query_string_param(request_rec *r)
{
  char *pair;
  char *name;
  char *value;
  char *pstate;
  char *vstate;
  char *s;
  query_string_param_t *param;

  s = apr_pstrdup(r->pool, r->parsed_uri.query);
  param = apr_palloc(r->pool, sizeof(query_string_param_t));
  param->mode       = IMG_CONV_MODE_NORMAL;
  param->user_agent = NULL;
  param->ua_flag    = UA_USE;
  param->name       = NULL;
  param->offset     = 0;
  param->count      = 0;
  param->width      = 0;
  param->height     = 0;

  if (s == NULL) return param;

  for (;;) {
    if ((pair = apr_strtok(s, "&", &pstate)) == NULL) break;
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);

    switch (*name) {
    case 'm':
    case 'M':
      if (value && (strcasecmp(name, "mode") == 0 || strcasecmp(name, "m") == 0)) {

        switch (*value) {
        case 't':
        case 'T':
          if (strcasecmp(value, "thumbnail") == 0 || strcasecmp(value, "tb") == 0)
            param->mode = IMG_CONV_MODE_THUMBNAIL;
          break;
  
        case 'w':
        case 'W':
          if (strcasecmp(value, "WP") == 0 || strcasecmp(value, "WallPaper") == 0)
            param->mode = IMG_CONV_MODE_WALLPAPER;
          break;
  
        case 'e':
        case 'E':
          if (strcasecmp(value, "EZGET") == 0)
            param->mode = IMG_CONV_MODE_EZGET;
          break;
        default:
          break;
        }
      }
      break;

    case 'u':
    case 'U':
      if (value && (strcasecmp(name, "ua") == 0 || strcasecmp(name, "user-agent") == 0)) {
        ap_unescape_url(value);

        if ((*value == 'i' || *value == 'I') && strcasecmp(value, "IGN") == 0)
          param->ua_flag = UA_IGN;

        param->user_agent = apr_pstrdup(r->pool, value);
      }
      break;

    case 'n':
    case 'N':
      if (value && strcasecmp(name, "name") == 0)
        param->name = apr_pstrdup(r->pool, value);
      break;

    case 'o':
    case 'O':
      if (value && strcasecmp(name, "offset") == 0 && (! chxj_chk_numeric(value)))
        param->offset = chxj_atoi(value);

      break;

    case 'c':
    case 'C':
      if (value && strcasecmp(name, "count") == 0 && (! chxj_chk_numeric(value)))
        param->count = chxj_atoi(value);
      break;

    case 'w':
    case 'W':
      if (value && strcasecmp(name, "w") == 0 && (! chxj_chk_numeric(value)))
        param->width = chxj_atoi(value);
      break;

    case 'h':
    case 'H':
      if (value && strcasecmp(name, "h") == 0 && (! chxj_chk_numeric(value)))
        param->height = chxj_atoi(value);
      break;

    default:
      break;
    }
  }

  if (param->mode == IMG_CONV_MODE_NORMAL && param->name)
    param->mode = IMG_CONV_MODE_WALLPAPER;

  return param;
}
/*
 * vim:ts=2 et
 */
