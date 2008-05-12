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
/* qs_new_tag();                                                             */
/*===========================================================================*/
void test_qs_parse_attr_001();
void test_qs_parse_attr_002();
void test_qs_parse_attr_003();
void test_qs_parse_attr_004();
void test_qs_parse_attr_005();
void test_qs_parse_attr_006();
void test_qs_parse_attr_007();
void test_qs_parse_attr_008();
void test_qs_parse_attr_009();
void test_qs_parse_attr_010();
void test_qs_parse_attr_011();
/*===========================================================================*/
/* qs_new_attr()                                                             */
/*===========================================================================*/
void test_qs_new_attr_001();
void test_qs_new_attr_002();
void test_qs_new_attr_003();
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
  str_util_suite = CU_add_suite("test qs_parse_attr.c", NULL, NULL);
  /*=========================================================================*/
  /* qs_new_tag()                                                            */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_parse_attr() 001",                               test_qs_parse_attr_001);
  CU_add_test(str_util_suite, "qs_parse_attr() 002",                               test_qs_parse_attr_002);
  CU_add_test(str_util_suite, "qs_parse_attr() 003",                               test_qs_parse_attr_003);
  CU_add_test(str_util_suite, "qs_parse_attr() 004",                               test_qs_parse_attr_004);
  CU_add_test(str_util_suite, "qs_parse_attr() 005",                               test_qs_parse_attr_005);
  CU_add_test(str_util_suite, "qs_parse_attr() 006",                               test_qs_parse_attr_006);
  CU_add_test(str_util_suite, "qs_parse_attr() 007",                               test_qs_parse_attr_007);
  CU_add_test(str_util_suite, "qs_parse_attr() 008",                               test_qs_parse_attr_008);
  CU_add_test(str_util_suite, "qs_parse_attr() 009",                               test_qs_parse_attr_009);
  CU_add_test(str_util_suite, "qs_parse_attr() 010",                               test_qs_parse_attr_010);
  CU_add_test(str_util_suite, "qs_parse_attr() 011",                               test_qs_parse_attr_011);
  /*=========================================================================*/
  /* qs_new_attr()                                                           */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_new_attr() 001",                                 test_qs_new_attr_001);
  CU_add_test(str_util_suite, "qs_new_attr() 002",                                 test_qs_new_attr_002);
  CU_add_test(str_util_suite, "qs_new_attr() 003",                                 test_qs_new_attr_003);
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
void test_qs_parse_attr_001()
{
#define TEST_STRING "aaa=\"bbb\" ccc=\"ddd\""
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name,  "aaa") == 0);
  CU_ASSERT(strcmp(ret->value, "bbb") == 0);
  CU_ASSERT(pos == 8);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_002()
{
#define TEST_STRING "aaa=bbb ccc=\"ddd\""
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name,  "aaa") == 0);
  CU_ASSERT(strcmp(ret->value, "bbb") == 0);
  CU_ASSERT(pos == 7);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_003()
{
#define TEST_STRING "aaa=bbb ccc=ddd"
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name,  "aaa") == 0);
  CU_ASSERT(strcmp(ret->value, "bbb") == 0);
  CU_ASSERT(pos == 7);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_004()
{
#define TEST_STRING ""
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_005()
{
#define TEST_STRING NULL
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, 0, &pos);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_006()
{
#define TEST_STRING "abc=def"
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(NULL,TEST_STRING, 0, &pos);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_007()
{
#define TEST_STRING "abc=def"
  Attr *ret;
  int  pos;
  APR_INIT;

  doc.pool = NULL;
  ret = qs_parse_attr(&doc,TEST_STRING, 0, &pos);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_008()
{
#define TEST_STRING "abc def=bbb"
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(strcmp(ret->value, "") == 0);
  CU_ASSERT(pos == 4);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_009()
{
#define TEST_STRING "abc='aaa\\'bbb'"
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(strcmp(ret->value, "aaa\\'bbb") == 0);
  CU_ASSERT(pos == 13);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_010()
{
#define TEST_STRING "abc=\"aaa'bbb\""
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(strcmp(ret->value, "aaa'bbb") == 0);
  CU_ASSERT(pos == 12);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_attr_011()
{
#define TEST_STRING "abc='aaa\"bbb'"
  Attr *ret;
  int  pos;
  APR_INIT;

  ret = qs_parse_attr(&doc,TEST_STRING, sizeof(TEST_STRING)-1, &pos);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(strcmp(ret->value, "aaa\"bbb") == 0);
  CU_ASSERT(pos == 12);

  APR_TERM;
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_new_attr()                                                             */
/*===========================================================================*/
#if 0
Attr *
qs_new_attr(Doc *doc)
{
  Attr *attr;

  if (!doc) {
    QX_LOGGER_FATAL("runtime exception: qs_new_attr(): doc is null");
    return NULL;
  }
  if (!doc->pool) {
    QX_LOGGER_FATAL("runtime exception: qs_new_attr(): doc->pool is null");
    return NULL;
  }

  attr = (Attr *)apr_palloc(doc->pool,sizeof(Attr));
  if (attr == NULL) {
    QX_LOGGER_FATAL("Out Of Memory");
    return NULL;
  }

  attr->next   = NULL;
  attr->parent = NULL;
  attr->name   = NULL;
  attr->value  = NULL;

  return attr;
}
#endif
void test_qs_new_attr_001()
{
  Attr *ret;
  APR_INIT;

  ret = qs_new_attr(&doc);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret->parent == NULL);
  CU_ASSERT(ret->next == NULL);
  CU_ASSERT(ret->name == NULL);
  CU_ASSERT(ret->value == NULL);

  APR_TERM;
}
void test_qs_new_attr_002()
{
  Attr *ret;
  APR_INIT;

  doc.pool = NULL;
  ret = qs_new_attr(&doc);
  CU_ASSERT(ret == NULL);
  APR_TERM;
}
void test_qs_new_attr_003()
{
  Attr *ret;
  APR_INIT;

  ret = qs_new_attr(NULL);
  CU_ASSERT(ret == NULL);
  APR_TERM;
}
/*
 * vim:ts=2 et
 */
