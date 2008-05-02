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
#include "chxj_tag_util.c"
#include <iconv.h>


/*===========================================================================*/
/* qs_new_tag();                                                             */
/*===========================================================================*/
void test_qs_get_value_attr_001();
void test_qs_get_value_attr_002();
void test_qs_get_value_attr_003();
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
  str_util_suite = CU_add_suite("test chxj_tag_util.c", NULL, NULL);
  /*=========================================================================*/
  /* qs_new_tag()                                                            */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_value_attr() 001",                               test_qs_get_value_attr_001);
  CU_add_test(str_util_suite, "qs_get_value_attr() 002",                               test_qs_get_value_attr_002);
  CU_add_test(str_util_suite, "qs_get_value_attr() 003",                               test_qs_get_value_attr_003);
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
/* qs_get_value_attr()                                                       */
/*===========================================================================*/
#if 0
char *
qs_get_value_attr(Doc *doc, Node *node, request_rec *r)
#endif
void test_qs_get_value_attr_001()
{
  Node *node;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  ret = qs_get_value_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_get_value_attr_002()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "value");
  attr->value = apr_pstrdup(p, "atai");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_value_attr(&doc,node,p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, "atai") == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_get_value_attr_003()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "value1");
  attr->value = apr_pstrdup(p, "atai");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_value_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
