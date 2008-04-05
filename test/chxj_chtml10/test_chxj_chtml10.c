#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST

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
#include "chxj_jhtml.c"
#include "chxj_hdml.c"
#include "chxj_xhtml_mobile_1_0.c"
#include "chxj_node_exchange.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_cookie.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_dbm.c"
#include "chxj_str_util.c"
#include <iconv.h>



void test_chtml10_001();
void test_chtml10_002();
void test_chtml10_comment_tag_001();
void test_chtml10_a_tag_name_attribute_001(); 
void test_chtml10_a_tag_name_attribute_002();
void test_chtml10_a_tag_name_attribute_003();
void test_chtml10_a_tag_name_attribute_004();
void test_chtml10_a_tag_href_attribute_001();
void test_chtml10_a_tag_href_attribute_002();
void test_chtml10_a_tag_href_attribute_003();
void test_chtml10_a_tag_href_attribute_004();
void test_chtml10_a_tag_href_attribute_005();
void test_chtml10_a_tag_href_attribute_006(); 
void test_chtml10_a_tag_accesskey_attribute_001();
void test_chtml10_a_tag_accesskey_attribute_002();
void test_chtml10_a_tag_accesskey_attribute_003();
void test_chtml10_base_tag_001(); 
void test_chtml10_base_tag_href_attribute_001(); 
void test_chtml10_base_tag_href_attribute_002(); 
void test_chtml10_base_tag_href_attribute_003(); 
void test_chtml10_base_tag_href_attribute_004(); 
void test_chtml10_blockquote_tag_001(); 
void test_chtml10_blockquote_tag_002(); 
void test_chtml10_blockquote_tag_003(); 
void test_chtml10_blockquote_tag_004(); 

int
main()
{
  CU_pSuite chtml10_suite;
  CU_initialize_registry();
  chtml10_suite = CU_add_suite("test chxj_exchange_chtml10()", NULL, NULL);
  CU_add_test(chtml10_suite, "test void src1",                             test_chtml10_001);
  CU_add_test(chtml10_suite, "test void src2",                             test_chtml10_002);
  CU_add_test(chtml10_suite, "test comment tag1",                          test_chtml10_comment_tag_001);
  CU_add_test(chtml10_suite, "test a tag name attr1",                      test_chtml10_a_tag_name_attribute_001); 
  CU_add_test(chtml10_suite, "test a tag name attr2",                      test_chtml10_a_tag_name_attribute_002); 
  CU_add_test(chtml10_suite, "test a tag name attr3 with japanese.",       test_chtml10_a_tag_name_attribute_003); 
  CU_add_test(chtml10_suite, "test a tag name attr4 with japanese.",       test_chtml10_a_tag_name_attribute_004); 
  CU_add_test(chtml10_suite, "test a tag href attr1 with void attribute.", test_chtml10_a_tag_href_attribute_001); 
  CU_add_test(chtml10_suite, "test a tag href attr2 with other site link.", test_chtml10_a_tag_href_attribute_002); 
  CU_add_test(chtml10_suite, "test a tag href attr3 with local link.",      test_chtml10_a_tag_href_attribute_003); 
  CU_add_test(chtml10_suite, "test a tag href attr4 with maker.",           test_chtml10_a_tag_href_attribute_004); 
  CU_add_test(chtml10_suite, "test a tag href attr5 with void maker.",      test_chtml10_a_tag_href_attribute_005); 
  CU_add_test(chtml10_suite, "test a tag href attr6 with no cookie.",      test_chtml10_a_tag_href_attribute_006); 
  CU_add_test(chtml10_suite, "test a tag accesskey attribute.",             test_chtml10_a_tag_accesskey_attribute_001); 
  CU_add_test(chtml10_suite, "test a tag accesskey attribute with void char.", test_chtml10_a_tag_accesskey_attribute_002); 
  CU_add_test(chtml10_suite, "test a tag accesskey attribute with no value", test_chtml10_a_tag_accesskey_attribute_003); 
  CU_add_test(chtml10_suite, "test base tag no attribute.",                  test_chtml10_base_tag_001); 
  CU_add_test(chtml10_suite, "test base tag href attribute with no value.",  test_chtml10_base_tag_href_attribute_001); 
  CU_add_test(chtml10_suite, "test base tag href attribute with void value.",  test_chtml10_base_tag_href_attribute_002); 
  CU_add_test(chtml10_suite, "test base tag href attribute with normal value.",  test_chtml10_base_tag_href_attribute_003); 
  CU_add_test(chtml10_suite, "test base tag href attribute with normal value.",  test_chtml10_base_tag_href_attribute_004); 
  CU_add_test(chtml10_suite, "test blockquote tag with void value.",             test_chtml10_blockquote_tag_001); 
  CU_add_test(chtml10_suite, "test blockquote tag with value.",             test_chtml10_blockquote_tag_002); 
  CU_add_test(chtml10_suite, "test blockquote tag with japanese value.",             test_chtml10_blockquote_tag_003); 
  CU_add_test(chtml10_suite, "test blockquote tag with hankaku kana value.",             test_chtml10_blockquote_tag_004); 
  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}

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

#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
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



void test_chtml10_001() 
{
#define  TEST_STRING "<!-- comment -->"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_chtml10(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_chtml10_002() 
{
#define  TEST_STRING ""
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_chtml10(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}

void test_chtml10_comment_tag_001() 
{
#define  TEST_STRING "<html><!--</html><body>--><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_chtml10(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
fprintf(stderr, "%s",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_name_attribute_001() 
{
#define  TEST_STRING "<html><head></head><body><a name=\"abc\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a name=\"abc\">abc</a></body></html>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_chtml10(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
fprintf(stderr, "%s",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_name_attribute_002() 
{
#define  TEST_STRING "<html><head></head><body><a name=\"\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a name=\"\">abc</a></body></html>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_chtml10(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
fprintf(stderr, "%s",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_name_attribute_003() 
{
#define  TEST_STRING "<html><head></head><body><a name=\"あああ\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a name=\"あああ\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  destlen = sizeof(TEST_STRING)-1;
  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_name_attribute_004() 
{
#define  TEST_STRING "<html><head></head><body><a name=\"ｱｱｱ\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a name=\"ｱｱｱ\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_001() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"?_chxj_cc=test_cookie_id\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_002() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"http://www.google.co.jp/\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"http://www.google.co.jp/\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_003() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html?_chxj_cc=test_cookie_id\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_004() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html#abc\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html#abc?_chxj_cc=test_cookie_id\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_005() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html#\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html#?_chxj_cc=test_cookie_id\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_href_attribute_006() 
{
#define  TEST_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a href=\"a.html\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_accesskey_attribute_001() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey=\"1\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"1\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_accesskey_attribute_002() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_a_tag_accesskey_attribute_003() 
{
#define  TEST_STRING "<html><head></head><body><a accesskey>abc</a></body></html>"
#define  RESULT_STRING "<html><head></head><body><a accesskey=\"\">abc</a></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_base_tag_001() 
{
#define  TEST_STRING "<html><head><base></head><body></body></html>"
#define  RESULT_STRING "<html><head><base></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_base_tag_href_attribute_001() 
{
#define  TEST_STRING "<html><head><base href></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_base_tag_href_attribute_002() 
{
#define  TEST_STRING "<html><head><base href=\"\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_base_tag_href_attribute_003() 
{
#define  TEST_STRING "<html><head><base href=\"http://www.google.co.jp/\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\"http://www.google.co.jp/\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_base_tag_href_attribute_004() 
{
#define  TEST_STRING "<html><head><base href=\".\"></head><body></body></html>"
#define  RESULT_STRING "<html><head><base href=\".\"></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_blockquote_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><blockquote></blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote></blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_blockquote_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>abc</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>abc</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_blockquote_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>亀さん</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>亀さん</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chtml10_blockquote_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><blockquote>ﾊﾝｶｸ</blockquote></body></html>"
#define  RESULT_STRING "<html><head></head><body><blockquote>ﾊﾝｶｸ</blockquote></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_chtml10(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*
 * vim:ts=2 et
 */
