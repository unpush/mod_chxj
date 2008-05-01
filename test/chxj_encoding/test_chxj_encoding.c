#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "chxj_apache.h"
#include "apr.h"
#include "qs_malloc.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include "qs_parse_file.c"
#include "chxj_buffered_write.c"
#include "chxj_chtml10.c"
#include "chxj_chtml20.c"
#include "chxj_chtml30.c"
#include "chxj_chtml40.c"
#include "chxj_chtml50.c"
#include "chxj_jxhtml.c"
#include "chxj_jhtml.c"
#include "chxj_hdml.c"
#include "chxj_xhtml_mobile_1_0.c"
#include "chxj_node_convert.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_cookie.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_dbm.c"
#include "chxj_str_util.c"
#include <iconv.h>


/*===========================================================================*/
/* chxj_encoding()                                                           */
/*===========================================================================*/
void test_chxj_encoding_001();
void test_chxj_encoding_002();
void test_chxj_encoding_003();
void test_chxj_encoding_004();
void test_chxj_encoding_005();
/*===========================================================================*/
/* chxj_rencoding()                                                          */
/*===========================================================================*/
void test_chxj_rencoding_001();
void test_chxj_rencoding_002();
void test_chxj_rencoding_003();
void test_chxj_rencoding_004();
void test_chxj_rencoding_005();
/* pend */

void test_log_rerror(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}
void test_log_error(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

char *chxj_qr_code_blob_handler(request_rec* r, const char* indata, size_t* len)
{
  return NULL;
}

char *test_os_escape_path(apr_pool_t *p, const char *path, int partial)
{
  return (char *)path;
}

void test_set_content_type(request_rec *r, const char *ct)
{
  fprintf(stderr, "%s:%d set_content_type(%s)\n", __FILE__,__LINE__,ct);
}

const char *test_run_http_scheme(request_rec *r)
{
  static char *s = "http";
  return s;
}

void * test_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
  static mod_chxj_config cfg;
  memset(&cfg, 0, sizeof(mod_chxj_config));
  cfg.new_line_type = NLTYPE_NONE;
  return &cfg;
}

chxjconvrule_entry *
chxj_apply_convrule(request_rec* r, apr_array_header_t* convrules)
{
  static chxjconvrule_entry entries;
  memset(&entries, 0, sizeof(chxjconvrule_entry));
  entries.encoding = apr_pstrdup(r->pool, "UTF8");
  return &entries;
}

char *test_ap_escape_html(apr_pool_t *pool, const char *s)
{
  return (char *)s;
}

int
main()
{
  CU_pSuite str_util_suite;
  CU_initialize_registry();
  str_util_suite = CU_add_suite("test chxj_encoding.c", NULL, NULL);
  /*=========================================================================*/
  /* chxj_encoding()                                                         */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_encoding() 001",                               test_chxj_encoding_001);
  CU_add_test(str_util_suite, "chxj_encoding() 002",                               test_chxj_encoding_002);
  CU_add_test(str_util_suite, "chxj_encoding() 003",                               test_chxj_encoding_003);
  CU_add_test(str_util_suite, "chxj_encoding() 004",                               test_chxj_encoding_004);
  CU_add_test(str_util_suite, "chxj_encoding() 005",                               test_chxj_encoding_005);
  /*=========================================================================*/
  /* chxj_rencoding()                                                        */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_rencoding() 001",                              test_chxj_rencoding_001);
  CU_add_test(str_util_suite, "chxj_rencoding() 002",                              test_chxj_rencoding_002);
  CU_add_test(str_util_suite, "chxj_rencoding() 003",                              test_chxj_rencoding_003);
  CU_add_test(str_util_suite, "chxj_rencoding() 004",                              test_chxj_rencoding_004);
  CU_add_test(str_util_suite, "chxj_rencoding() 005",                              test_chxj_rencoding_005);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}



#define APR_INIT \
  apr_pool_t *p; \
  request_rec r; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
  } \
  while (0)

#define APR_TERM \
  do { \
    apr_terminate(); \
  } while (0)

#define COOKIE_INIT(X) \
  do { \
    X.cookie_id = "test_cookie_id"; \
  } while (0) \

#define SPEC_INIT(X) \
  do { \
    X.html_spec_type = CHXJ_SPEC_Chtml_1_0; \
  } while (0)




/*===========================================================================*/
/* chxj_encoding()                                                           */
/*===========================================================================*/
void test_chxj_encoding_001()
{
#define TEST_STRING "あいうえお"
#define RESULT_STRING "\x82\xa0\x82\xa2\x82\xa4\x82\xa6\x82\xa8"
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_encoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_encoding_002()
{
#define TEST_STRING ""
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_encoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_encoding_003()
{
#define TEST_STRING NULL
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = 0;
  ret = chxj_encoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_encoding_004()
{
#define TEST_STRING NULL
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = -1;
  ret = chxj_encoding(&r, "", &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == -1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_encoding_005()
{
#define TEST_STRING "\x82\xa0\x82\xa2\x82\xa4\x82\xa6\x82\xa8"
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_encoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* chxj_rencoding()                                                          */
/*===========================================================================*/
void test_chxj_rencoding_001()
{
#define TEST_STRING "\x82\xa0\x82\xa2\x82\xa4\x82\xa6\x82\xa8"
#define RESULT_STRING "あいうえお"
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_rencoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_rencoding_002()
{
#define TEST_STRING ""
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_rencoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_rencoding_003()
{
#define TEST_STRING NULL
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = 0;
  ret = chxj_rencoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_rencoding_004()
{
#define TEST_STRING NULL
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = -1;
  ret = chxj_rencoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);
  CU_ASSERT(len == -1);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_chxj_rencoding_005()
{
#define TEST_STRING "あいうえお"
#define RESULT_STRING ""
  char *ret;
  APR_INIT;
  apr_size_t len;

  len = sizeof(TEST_STRING)-1;
  ret = chxj_rencoding(&r, TEST_STRING, &len);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  fprintf(stderr, "len:[%d]\n", len);
  CU_ASSERT(len == 12);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
