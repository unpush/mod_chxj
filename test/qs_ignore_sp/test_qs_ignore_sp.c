#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "apr.h"
#include "apr_strings.h"
#include "chxj_apache.h"
#include "chxj_str_util.h"
#include "chxj_str_util.c"
#include "qs_ignore_sp.c"
#include "qs_log.c"
#include "qs_malloc.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_parse_tag.c"
#include <iconv.h>


/*===========================================================================*/
/* qs_ignore_sp()                                                            */
/*===========================================================================*/
void test_qs_ignore_sp_001();
void test_qs_ignore_sp_002();
void test_qs_ignore_sp_003();
void test_qs_ignore_sp_004();
void test_qs_ignore_sp_005();
void test_qs_ignore_sp_006();
void test_qs_ignore_sp_007();
void test_qs_ignore_sp_008();
void test_qs_ignore_sp_009();
/*===========================================================================*/
/* qs_ignore_sp_and_quote()                                                  */
/*===========================================================================*/
void test_qs_ignore_sp_and_quote_001();
void test_qs_ignore_sp_and_quote_002();
void test_qs_ignore_sp_and_quote_003();
void test_qs_ignore_sp_and_quote_004();
void test_qs_ignore_sp_and_quote_005();
void test_qs_ignore_sp_and_quote_006();
void test_qs_ignore_sp_and_quote_007();
void test_qs_ignore_sp_and_quote_008();
void test_qs_ignore_sp_and_quote_009();
void test_qs_ignore_sp_and_quote_010();
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

int
main()
{
  CU_pSuite str_util_suite;
  CU_initialize_registry();
  str_util_suite = CU_add_suite("test qs_ignore_sp.c", NULL, NULL);
  /*=========================================================================*/
  /* qs_ignore_sp()                                                          */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_ignore_sp() 001",                               test_qs_ignore_sp_001);
  CU_add_test(str_util_suite, "qs_ignore_sp() 002",                               test_qs_ignore_sp_002);
  CU_add_test(str_util_suite, "qs_ignore_sp() 003",                               test_qs_ignore_sp_003);
  CU_add_test(str_util_suite, "qs_ignore_sp() 004",                               test_qs_ignore_sp_004);
  CU_add_test(str_util_suite, "qs_ignore_sp() 005",                               test_qs_ignore_sp_005);
  CU_add_test(str_util_suite, "qs_ignore_sp() 006",                               test_qs_ignore_sp_006);
  CU_add_test(str_util_suite, "qs_ignore_sp() 007",                               test_qs_ignore_sp_007);
  CU_add_test(str_util_suite, "qs_ignore_sp() 008",                               test_qs_ignore_sp_008);
  CU_add_test(str_util_suite, "qs_ignore_sp() 009",                               test_qs_ignore_sp_009);
  /*=========================================================================*/
  /* qs_ignore_sp_and_quote()                                                */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 001",                     test_qs_ignore_sp_and_quote_001);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 002",                     test_qs_ignore_sp_and_quote_002);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 003",                     test_qs_ignore_sp_and_quote_003);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 004",                     test_qs_ignore_sp_and_quote_004);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 005",                     test_qs_ignore_sp_and_quote_005);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 006",                     test_qs_ignore_sp_and_quote_006);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 007",                     test_qs_ignore_sp_and_quote_007);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 008",                     test_qs_ignore_sp_and_quote_008);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 009",                     test_qs_ignore_sp_and_quote_009);
  CU_add_test(str_util_suite, "qs_ignore_sp_and_quote() 010",                     test_qs_ignore_sp_and_quote_010);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}



#define APR_INIT \
  apr_pool_t *p; \
  Doc doc;  \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    memset(&doc, 0, sizeof(Doc)); \
    doc.pool = p; \
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
/* qs_parse_attr()                                                           */
/*===========================================================================*/
#if 0
int
qs_ignore_sp(Doc *UNUSED(doc), const char *s, int len)
{
  int ii;
  char *sp;

  sp = (char *)s;

  for(ii=0;
      *sp && is_white_space(*sp) && ii<len;
      ii++, sp++)
          ;

  return (sp - s);
}
#endif
void test_qs_ignore_sp_001()
{
#define TEST_STRING "a"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_002()
{
#define TEST_STRING " a"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_003()
{
#define TEST_STRING "   a"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 3);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_004()
{
#define TEST_STRING "\t\n\ra"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 3);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_005()
{
#define TEST_STRING " a "
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_006()
{
#define TEST_STRING ""
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_007()
{
#define TEST_STRING NULL
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, 0);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_008()
{
#define TEST_STRING "abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, -1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_009()
{
#define TEST_STRING "abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp(&doc, TEST_STRING, 0);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_ignore_sp_and_quote()                                                  */
/*===========================================================================*/
#if 0
int
qs_ignore_sp_and_quote(Doc *UNUSED(doc), const char *s, int len)
{
  int   ii;
  char  *sp = (char *)s;

  for(ii=0;
      *sp && (is_white_space(*sp) || is_quote(*sp)) && ii<len;
      ii++, sp++)
        ;

  return (sp - s);
}
#endif
void test_qs_ignore_sp_and_quote_001()
{
#define TEST_STRING "abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_002()
{
#define TEST_STRING " abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_003()
{
#define TEST_STRING "  abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 2);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_004()
{
#define TEST_STRING "\n\r\tabc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 3);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_005()
{
#define TEST_STRING ""
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_006()
{
#define TEST_STRING "   "
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 3);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_007()
{
#define TEST_STRING " \"  "
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 4);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_008()
{
#define TEST_STRING " '  "
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, sizeof(TEST_STRING)-1);
  CU_ASSERT(ret == 4);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_009()
{
#define TEST_STRING NULL
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, 0);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_ignore_sp_and_quote_010()
{
#define TEST_STRING "abc"
  int ret;
  APR_INIT;

  ret = qs_ignore_sp_and_quote(&doc, TEST_STRING, -1);
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
