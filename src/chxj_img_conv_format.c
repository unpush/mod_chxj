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
#include "mod_chxj.h"
#include "chxj_img_conv_format.h"
#include "chxj_specified_device.h"

#include "http_core.h"
#include <wand/magick_wand.h>

#define EXIT_MAGICK_ERROR() \
  do { \
    char *description; ExceptionType severity; \
    description=MagickGetException(magick_wand,&severity); \
    ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r,"%s %s %d %s\n",__FILE__,(__func__),__LINE__,description); \
    description=(char *) MagickRelinquishMemory(description); \
    DestroyMagickWand(magick_wand); \
  }while(1) 

typedef enum img_conv_mode_t {
  IMG_CONV_MODE_NORMAL = 0,
  IMG_CONV_MODE_THUMBNAIL,
  IMG_CONV_MODE_WALLPAPER,
} img_conv_mode_t;

typedef struct _query_string_param_t {
  img_conv_mode_t   mode;
  char *user_agent;
} query_string_param_t;

/*----------------------------------------------------------------------------*/
/* for AU CRC table                                                           */
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



static char* chxj_create_workfile(request_rec* r, 
                                  mod_chxj_config* conf, 
                                  const char* user_agent, 
                                  img_conv_mode_t mode);
static apr_status_t chxj_create_cache_file(request_rec* r, 
                                           const char* tmpfile, 
                                           device_table* spec,
                                           apr_finfo_t* st,
                                           img_conv_mode_t mode);
static apr_status_t chxj_send_cache_file(request_rec* r, const char* tmpfile);
static query_string_param_t* chxj_get_query_string_param(request_rec *r);
static unsigned short chxj_add_crc(const char* writedata, apr_size_t witebyte);
static MagickWand* chxj_fixup_size(MagickWand* magick_wand, 
                request_rec* r, device_table* spec, img_conv_mode_t mode);
static MagickWand* chxj_fixup_color(MagickWand* magick_wand, 
                request_rec* r, device_table* spec, img_conv_mode_t mode);
static MagickWand* chxj_fixup_depth(MagickWand* magick_wand, 
                request_rec* r, device_table* spec);
static MagickWand* chxj_img_down_sizing(MagickWand* magick_wand, 
                request_rec* r, device_table* spec);

static MagickWand* chxj_add_copyright(MagickWand* magick_wand, request_rec* r, device_table* spec);


int
chxj_img_conv_format(request_rec *r)
{
  apr_status_t   rv;
  apr_finfo_t    st;
  apr_finfo_t    cache_st;
  char*          user_agent;
  device_table*  spec;
  char*          tmpfile;

  query_string_param_t *qsp = chxj_get_query_string_param(r);


  mod_chxj_config* conf;

  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, 
                  "chxj_img_conv_format[%s]", r->the_request);

  if (strcasecmp(r->handler, "chxj-picture"))
  {
    return DECLINED;
  }

  if (r->header_only) 
  {
    return DECLINED;
  }

  /*--------------------------------------------------------------------------*/
  /* User-Agent to spec                                                       */
  /*--------------------------------------------------------------------------*/
  if (qsp->user_agent != NULL)
  {
    user_agent = apr_pstrdup(r->pool, qsp->user_agent);
  }
  else
  {
    user_agent = (char*)apr_table_get(r->headers_in, HTTP_USER_AGENT);
  }
  spec = chxj_specified_device(r, user_agent);

  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "found device_name=[%s]", 
                  spec->device_name);
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "User-Agent=[%s]", 
                  user_agent);
  if (spec->width == 0 || spec->heigh == 0)
  {
    return HTTP_NOT_FOUND;
  }

  /*--------------------------------------------------------------------------*/
  /* Create Workfile Name                                                     */
  /*--------------------------------------------------------------------------*/
  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  tmpfile = chxj_create_workfile(r, conf, user_agent, qsp->mode);
  ap_log_rerror(APLOG_MARK,APLOG_DEBUG, 0, r, "workfile=[%s]", tmpfile);

  rv = apr_stat(&st, r->filename, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
  {
    return HTTP_NOT_FOUND;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"found [%s]", r->filename);
  rv = apr_stat(&cache_st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS || cache_st.ctime < st.mtime)
  {
    /*------------------------------------------------------------------------*/
    /* It tries to make the cash file when it doesn't exist or there is       */
    /* change time later since the making time of the cash file.              */
    /*------------------------------------------------------------------------*/
    rv = chxj_create_cache_file(r,tmpfile, spec, &st, qsp->mode);
    if (rv != OK)
    {
      return rv;
    }
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"color=[%d]", spec->color);
  rv = chxj_send_cache_file(r, tmpfile);
  if (rv != OK)
  {
    return rv;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"end chxj_img_conv_format");

  return OK;
}

static apr_status_t
chxj_create_cache_file(request_rec* r, const char* tmpfile, device_table* spec, apr_finfo_t* st, img_conv_mode_t mode)
{
  apr_status_t       rv;
  apr_size_t         readbyte;
  apr_size_t         writebyte;
  unsigned short     crc;

  char*              writedata = NULL;
  char*              readdata = NULL;

  apr_file_t*        fout;
  apr_file_t*        fin;

  MagickBooleanType  status;
  MagickWand*        magick_wand;

  rv = apr_file_open(&fin, 
                  r->filename, 
                  APR_READ|APR_BINARY ,
                  APR_OS_DEFAULT, 
                  r->pool);
  if (rv != APR_SUCCESS)
  {
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                    "file open failed.[%s]", r->filename);
    return HTTP_NOT_FOUND;
  }

  readdata = apr_palloc(r->pool, st->size);
  rv = apr_file_read_full(fin, (void*)readdata, st->size, &readbyte);
  if (rv != APR_SUCCESS || readbyte != st->size)
  {
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,
                    "file read failed.[%s]", r->filename);
    apr_file_close(fin);

    return HTTP_NOT_FOUND;
  }


  magick_wand=NewMagickWand();
  status=MagickReadImageBlob(magick_wand,readdata, readbyte);
  if (status == MagickFalse)
  {
    EXIT_MAGICK_ERROR();
    return HTTP_NOT_FOUND;
  }

  /*--------------------------------------------------------------------------*/
  /* The size of the image is changed.                                        */
  /*--------------------------------------------------------------------------*/
  magick_wand = chxj_fixup_size(magick_wand, r, spec, mode);
  if (magick_wand == NULL)
  {
    return HTTP_NOT_FOUND;
  }

  /*--------------------------------------------------------------------------*/
  /* The colors of the image is changed.                                      */
  /*--------------------------------------------------------------------------*/
  magick_wand = chxj_fixup_color(magick_wand, r,spec, mode);
  if (magick_wand == NULL)
  {
    return HTTP_NOT_FOUND;
  }

  /*--------------------------------------------------------------------------*/
  /* DEPTH of the image is changed.                                           */
  /*--------------------------------------------------------------------------*/
  magick_wand = chxj_fixup_depth(magick_wand, r, spec);
  if (magick_wand == NULL)
  {
    return HTTP_NOT_FOUND;
  }

  /*--------------------------------------------------------------------------*/
  /* Add Comment (Copyright and so on.)                                       */
  /*--------------------------------------------------------------------------*/
  magick_wand = chxj_add_copyright(magick_wand, r, spec);
  if (magick_wand == NULL)
  {
    return HTTP_NOT_FOUND;
  }


  if (spec->available_jpeg == 1)
  {
    status = MagickSetImageCompression(magick_wand,JPEGCompression);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    status = MagickSetImageFormat(magick_wand, "jpg");
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }

    magick_wand = chxj_img_down_sizing(magick_wand, r, spec);
    if (magick_wand == NULL)
    {
      return HTTP_NOT_FOUND;
    }

    r->content_type = apr_psprintf(r->pool, "image/jpeg");
    writedata=MagickGetImageBlob(magick_wand, &writebyte);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert to jpg");
  }
  else
  if (spec->available_png == 1)
  {
    status = MagickSetImageCompression(magick_wand,ZipCompression);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    status = MagickSetImageFormat(magick_wand, "png");
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    magick_wand = chxj_img_down_sizing(magick_wand, r, spec);
    if (magick_wand == NULL)
    {
      return HTTP_NOT_FOUND;
    }

    writedata = MagickGetImageBlob(magick_wand, &writebyte);

    r->content_type = apr_psprintf(r->pool, "image/png");
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert to png");
  }
  else
  if (spec->available_gif == 1)
  {
    status = MagickSetImageCompression(magick_wand,LZWCompression);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }

    status = MagickSetImageFormat(magick_wand, "gif");
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }

    magick_wand = chxj_img_down_sizing(magick_wand, r, spec);
    if (magick_wand == NULL)
    {
      return HTTP_NOT_FOUND;
    }

    writedata=MagickGetImageBlob(magick_wand, &writebyte);
    r->content_type = apr_psprintf(r->pool, "image/gif");
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert to gif");
  }
  else
  if (spec->available_bmp2 == 1 || spec->available_bmp4 == 1)
  {
    status = MagickSetImageCompression(magick_wand,NoCompression);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }
    status = MagickSetImageFormat(magick_wand, "bmp");
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return HTTP_NOT_FOUND;
    }

    magick_wand = chxj_img_down_sizing(magick_wand, r, spec);
    if (magick_wand == NULL)
    {
      return HTTP_NOT_FOUND;
    }

    writedata=MagickGetImageBlob(magick_wand, &writebyte);

    r->content_type = apr_psprintf(r->pool, "image/bmp");
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert to bmp(unsupported)");
  }
  if (writebyte == 0 || writedata == NULL)
  {
    DestroyMagickWand(magick_wand);
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,"convert failure to Jpeg [%s]", tmpfile);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  
  /* to cache */
  rv = apr_file_open(&fout, tmpfile,
                  APR_WRITE| APR_CREATE | APR_BINARY | APR_SHARELOCK ,APR_OS_DEFAULT,
                  r->pool);
  if (rv != APR_SUCCESS)
  {
    DestroyMagickWand(magick_wand);
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,"file open error.[%s]", tmpfile);
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  rv = apr_file_write(fout, (void*)writedata, &writebyte);
  if (rv != APR_SUCCESS)
  {
    DestroyMagickWand(magick_wand);
    apr_file_close(fout);
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  /*--------------------------------------------------------------------------*/
  /* CRC is added for AU for EzGET.                                           */
  /*--------------------------------------------------------------------------*/
  if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
  ||  spec->html_spec_type == CHXJ_SPEC_Hdml)
  {
    crc = chxj_add_crc(writedata, writebyte);
    rv = apr_file_putc((crc >> 8)  & 0xff, fout);
    if (rv != APR_SUCCESS)
    {
      DestroyMagickWand(magick_wand);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
    rv = apr_file_putc( crc        & 0xff, fout);
    if (rv != APR_SUCCESS)
    {
      DestroyMagickWand(magick_wand);
      return HTTP_INTERNAL_SERVER_ERROR;
    }
  }
  DestroyMagickWand(magick_wand);
  rv = apr_file_close(fout);
  if (rv != APR_SUCCESS)
  {
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r,"file write error.[%s]", tmpfile);
    return HTTP_INTERNAL_SERVER_ERROR;
  }

  return OK;
}

static MagickWand* 
chxj_fixup_size(MagickWand* magick_wand, request_rec* r, device_table* spec, img_conv_mode_t mode)
{
  MagickBooleanType  status;
  int oldw;
  int oldh;
  int neww;
  int newh;
  int c_width;
  int c_heigh;

  oldw = MagickGetImageWidth(magick_wand);
  oldh = MagickGetImageHeight(magick_wand);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"detect width=[%d]", oldw);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"detect heigh=[%d]", oldh);
  neww = oldw;
  newh = oldh;

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"detect spec width=[%d]", spec->width);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"detect spec heigh=[%d]", spec->heigh);

  c_width = spec->width;
  c_heigh = spec->heigh;

  if (mode == IMG_CONV_MODE_THUMBNAIL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"**** detect thumbnail mode ****");
    if (neww > c_width)
    {
      newh = (int)((double)newh * (double)((double)c_width / (double)neww));
      neww = (int)((double)neww * (double)((double)c_width / (double)neww));
    }
    if (newh > c_heigh)
    {
      neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
      newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
    }

    neww = (int)((double)(neww / 3) * 0.8);
    newh = (int)((double)(newh / 3) * 0.8);
  }
  else
  if (mode == IMG_CONV_MODE_WALLPAPER)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"**** detect wallpaper mode ****");

    if (spec->wp_width != 0 && spec->wp_heigh != 0)
    {
      c_width = spec->wp_width;
      c_heigh = spec->wp_heigh;
    }
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"calc new width and height");
#if 0
    if (neww > newh)
    {
      newh = (int)((double)newh * (double)((double)c_width / (double)neww));
      neww = (int)((double)neww * (double)((double)c_width / (double)neww));
    }
    else
    {
      neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
      newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
    }
#else
    neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
    newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
#endif
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"newh = [%d] neww = [%d]", newh, neww);
  }
  else
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"**** detect normal mode ****");
    if (neww > c_width)
    {
      newh = (int)((double)newh * (double)((double)c_width / (double)neww));
      neww = (int)((double)neww * (double)((double)c_width / (double)neww));
    }
    if (newh > c_heigh)
    {
      neww = (int)((double)neww * (double)((double)c_heigh / (double)newh));
      newh = (int)((double)newh * (double)((double)c_heigh / (double)newh));
    }
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert width=[%d --> %d]", oldw, neww);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"convert heigh=[%d --> %d]", oldh, newh);

  MagickResetIterator(magick_wand);
  while (MagickNextImage(magick_wand) != MagickFalse)
  {
    if (mode == IMG_CONV_MODE_WALLPAPER)
    {
      status = MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0);
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
      status = MagickCropImage(magick_wand, 
                      (unsigned long)c_width, 
                      (unsigned long)c_heigh,
                      (long)((neww - c_width) / 2),
                      (long)((newh - c_heigh) / 2));
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
    else
    {
      status = MagickResizeImage(magick_wand,neww,newh,LanczosFilter,1.0);
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
    status = MagickSetImageResolution(magick_wand,(double)spec->dpi_width, (double)spec->dpi_heigh);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }

  }

  return magick_wand;
}
static MagickWand*
chxj_fixup_color(MagickWand* magick_wand, request_rec* r, device_table* spec, img_conv_mode_t mode)
{
  MagickBooleanType  status;

  if (spec->color >= 256)
  {
    status = MagickQuantizeImage(magick_wand,
                               spec->color,
                               RGBColorspace,
                               0,
                               1,
                               0);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else 
  {
    status = MagickQuantizeImage(magick_wand,
                               spec->color,
                               GRAYColorspace,
                               0,
                               1,
                               0);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  return magick_wand;
}
static MagickWand*
chxj_fixup_depth(MagickWand* magick_wand, request_rec* r, device_table* spec)
{
  MagickBooleanType  status;

  if (spec->color == 15680000)
  {
    status = MagickSetImageDepth(magick_wand, 24);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 262144)
  {
    status = MagickSetImageDepth(magick_wand, 18);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 65536)
  {
    status = MagickSetImageDepth(magick_wand, 16);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4096)
  {
    status = MagickSetImageDepth(magick_wand, 12);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 256)
  {
    status = MagickSetImageDepth(magick_wand, 8);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 4)
  {
    status = MagickSetImageDepth(magick_wand, 2);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  else
  if (spec->color == 2)
  {
    status = MagickSetImageDepth(magick_wand, 1);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  return magick_wand;
}

static MagickWand*
chxj_add_copyright(MagickWand* magick_wand, request_rec* r, device_table* spec)
{
  MagickBooleanType  status;
  mod_chxj_config* conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (conf->image_copyright != NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "Add COPYRIGHT [%s]", conf->image_copyright);
    if (spec->html_spec_type == CHXJ_SPEC_Jhtml)
    {
      apr_table_setn(r->headers_out, "x-jphone-copyright", "no-transfer");
      status = MagickCommentImage(magick_wand, apr_psprintf(r->pool, "Copyright(C) %s", conf->image_copyright));
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
    else
    if (spec->html_spec_type == CHXJ_SPEC_XHtml_Mobile_1_0
    ||  spec->html_spec_type == CHXJ_SPEC_Hdml)
    {
      status = MagickCommentImage(magick_wand, apr_psprintf(r->pool, "kddi_copyright=on,%s", conf->image_copyright));
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
    else
    {
      status = MagickCommentImage(magick_wand, apr_psprintf(r->pool, "copy=\"NO\",%s", conf->image_copyright));
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
    }
  }
  else
  {
    status = MagickCommentImage(magick_wand, "mod_chxj");
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
  }
  return magick_wand;
}

static MagickWand*
chxj_img_down_sizing(MagickWand* magick_wand, request_rec* r, device_table* spec)
{
  MagickBooleanType  status;
  unsigned long quality = 70;
  apr_size_t    writebyte = 0;
  char*         writedata;

  do {
    status = MagickSetImageCompressionQuality(magick_wand, quality);
    if (status == MagickFalse)
    {
      EXIT_MAGICK_ERROR();
      return NULL;
    }
    writedata = MagickGetImageBlob(magick_wand, &writebyte);

    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "quality=[%ld] size=[%d]", (long)quality, writebyte);

    if (spec->cache == 0)
    {
      break;
    }
    if (writebyte <= spec->cache)
    {
      break;
    }
    quality -= 10;
    if (quality == 0 || quality > 100)
    {
      break;
    }
  }
  while (1);

  if (spec->cache > 0 
  &&  writebyte   > spec->cache)
  {
    unsigned long now_color = spec->color;
    unsigned long depth     = 0;
    do {
      switch(now_color) 
      {
      case 2:      depth = 1; break;
      case 4:      now_color = 2;        depth = 1;  break;
      case 256:    now_color = 4;        depth = 2;  break;
      case 4096:   now_color = 256;      depth = 8;  break;
      case 65536:  now_color = 4096;     depth = 12; break;
      case 262144: now_color = 65536;    depth = 16; break;
      case 15680000: now_color = 262144; depth = 18; break;
      default:
        now_color = 2;
        break;
      }
      if (now_color <= 2)
      {
        break;
      }
      if (now_color >= 256)
      {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             RGBColorspace,
                             0,
                             1,
                             0);
      }
      else
      {
        status = MagickQuantizeImage(magick_wand,
                             now_color,
                             GRAYColorspace,
                             0,
                             1,
                             0);
      }
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
      status = MagickSetImageDepth(magick_wand, depth);
      if (status == MagickFalse)
      {
        EXIT_MAGICK_ERROR();
        return NULL;
      }
      writedata=MagickGetImageBlob(magick_wand, &writebyte);

      ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "now_color=[%ld] size=[%d]", (long)now_color, writebyte);

      if (writebyte <= spec->cache)
      {
        break;
      }
    }
    while(now_color > 2);
  }
  return magick_wand;
}

static apr_status_t 
chxj_send_cache_file(request_rec* r, const char* tmpfile)
{
  apr_status_t rv;
  apr_finfo_t  st;
  apr_file_t*  fout;
  apr_size_t   sendbyte;
  char*        contentLength;

  rv = apr_stat(&st, tmpfile, APR_FINFO_MIN, r->pool);
  if (rv != APR_SUCCESS)
  {
    return HTTP_NOT_FOUND;
  }
  contentLength = apr_psprintf(r->pool, "%d", (int)st.size);
  apr_table_setn(r->headers_out, "Content-Length", (const char*)contentLength);

  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"Content-Length:[%d]", (int)st.size);

  rv = apr_file_open(&fout, tmpfile, 
    APR_READ | APR_BINARY, APR_OS_DEFAULT, r->pool);
  if (rv != APR_SUCCESS) 
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"tmpfile open failed[%s]", tmpfile);
    return HTTP_NOT_FOUND;
  }
  ap_send_fd(fout, r, 0, st.size, &sendbyte);
  apr_file_close(fout);
  ap_rflush(r);
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r,"send file data[%d]byte", sendbyte);
  return OK;
}



static char*
chxj_create_workfile(request_rec* r, mod_chxj_config* conf, const char* user_agent, img_conv_mode_t mode)
{
  int ii;
  int jj;
  int len;
  char* w = apr_palloc(r->pool, 256);
  char* fname;

  memset(w, 0, 256);
  switch (mode) {
  case IMG_CONV_MODE_THUMBNAIL:
    fname = apr_psprintf(r->pool, "%s.%s.thumbnail", r->filename, user_agent);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "mode=thumbnail [%s]", fname);
    break;
  case IMG_CONV_MODE_WALLPAPER:
    fname = apr_psprintf(r->pool, "%s.%s.wallpaper", r->filename, user_agent);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "mode=WallPaper [%s]", fname);
    break;
  case IMG_CONV_MODE_NORMAL:
  default:
    fname = apr_psprintf(r->pool, "%s.%s", r->filename, user_agent);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "mode=normal [%s]", fname);
    break;
  }

  len = strlen(fname);
  jj=0;
  for  (ii=0; ii<len; ii++) 
  {
    if (fname[ii] == '/' || fname[ii] == ' ' || fname[ii] == '-')
    {
      w[jj++] = '_';
    }
    else
    {
      w[jj++] = fname[ii];
    }
  }

  return apr_psprintf(r->pool, "%s/%s", conf->image_cache_dir,w);
}

static unsigned short
chxj_add_crc(const char* writedata, apr_size_t writebyte)
{
  unsigned short crc = 0xffff;
  apr_size_t     ii;
  unsigned char  ch;

  for(ii=0;ii<writebyte;ii++)
  {
    ch = writedata[ii];
    crc = AU_CRC_TBL[(crc>>8^ch)&0xff]^(crc<<8);
  }
  return crc;
}

int
chxj_trans_name(request_rec *r)
{
  const char* ccp;
  char* docroot;
  int len;
  ap_regex_t *regexp;
  ap_regmatch_t match[10];
  apr_finfo_t st;
  apr_status_t rv;
  mod_chxj_config* conf;
  int rtn;
  int ii;
  char*      ext[4] = {
          "jpg",
          "png",
          "bmp",
          "gif",
  };
  char*    fname;

  conf = ap_get_module_config(r->per_dir_config, &chxj_module);
  if (conf->image_uri == NULL)
  {
    return DECLINED;
  }

  regexp = ap_pregcomp(r->pool, (const char*)conf->image_uri, AP_REG_EXTENDED|AP_REG_ICASE);
  if (regexp == NULL)
  {
    return DECLINED;
  }

  rtn = ap_regexec(regexp, r->uri, regexp->re_nsub + 1, match, 0);
  if (rtn != 0)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "Not match URI[%s]", r->uri);
    return DECLINED;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "Match URI[%s]", r->uri);

  if (r->filename == NULL) 
  {
    r->filename = apr_pstrdup(r->pool, r->uri);
  }

  ccp = ap_document_root(r);
  if (ccp == NULL)
  {
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  docroot = apr_pstrdup(r->pool, ccp);
  len = strlen(docroot);

  if (docroot[len-1] == '/') 
  {
    docroot[len-1] = '\0';
  }
  if (r->server->path 
  &&  strncmp(r->filename, r->server->path, r->server->pathlen) == 0) 
  {
    r->filename = apr_pstrcat(r->pool, docroot, (r->filename + r->server->pathlen), NULL);
  }
  else 
  {
    r->filename = apr_pstrcat(r->pool, docroot, r->filename, NULL);
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "URI[%s]", r->filename);

  for (ii=0; ii<4; ii++) 
  {
    fname = apr_psprintf(r->pool, "%s.%s", r->filename, ext[ii]);
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "search [%s]", fname);

    rv = apr_stat(&st, fname, APR_FINFO_MIN, r->pool);
    if (rv == APR_SUCCESS)
    {
      break;
    }
    fname = NULL;
  }
  if (fname == NULL)
  {
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "NotFound [%s]", r->filename);
    return HTTP_NOT_FOUND;
  }
  ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "Found [%s]", fname);
  r->filename = apr_psprintf(r->pool, "%s", fname);
  r->handler = apr_psprintf(r->pool, "chxj-picture");
  return OK;
}



/**
 * It converts it from QUERYSTRING.
 *
 * @param r   [i]
 */
static query_string_param_t*
chxj_get_query_string_param(request_rec *r)
{
  char* pair;
  char* name;
  char* value;
  char* pstate;
  char* vstate;
  char* s = apr_pstrdup(r->pool, r->parsed_uri.query);
  query_string_param_t* param;

  param = apr_palloc(r->pool, sizeof(query_string_param_t));
  param->mode       = IMG_CONV_MODE_NORMAL;
  param->user_agent = NULL;

  ap_log_rerror(APLOG_MARK,
                APLOG_DEBUG,
                0, 
                r, 
                "query_string[%s]", s);
  if (s == NULL)
  {
    return param;
  }

  for (;;) 
  {
    pair = apr_strtok(s, "&", &pstate);
    if (pair == NULL)
    {
      break;
    } 
    s = NULL;

    name  = apr_strtok(pair, "=", &vstate);
    value = apr_strtok(NULL, "=", &vstate);
    if (strcasecmp(name, "mode") == 0 && value != NULL) 
    {
      if (strcasecmp(value, "thumbnail") == 0)
      {
        param->mode = IMG_CONV_MODE_THUMBNAIL;
      }
      else
      if (strcasecmp(value, "WP") == 0)
      {
        param->mode = IMG_CONV_MODE_WALLPAPER;
      }
    }
    else
    if (strcasecmp(name, "user-agent") == 0 && value != NULL)
    {
      ap_unescape_url(value);
      param->user_agent = apr_pstrdup(r->pool, value);
    }
  }
  return param;
}
/*
 * vim:ts=2 et
 */
