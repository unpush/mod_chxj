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
void test_qs_new_tag_001();
void test_qs_new_tag_002();
void test_qs_new_tag_003();
/*===========================================================================*/
/* qs_add_attr();                                                            */
/*===========================================================================*/
void test_qs_add_attr_001();
void test_qs_add_attr_002();
void test_qs_add_attr_003();
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
  str_util_suite = CU_add_suite("test qs_parse_tag.c", NULL, NULL);
  /*=========================================================================*/
  /* qs_new_tag()                                                            */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_new_tag() 001",                                  test_qs_new_tag_001);
  CU_add_test(str_util_suite, "qs_new_tag() 002",                                  test_qs_new_tag_002);
  CU_add_test(str_util_suite, "qs_new_tag() 003",                                  test_qs_new_tag_003);
  /*=========================================================================*/
  /* qs_add_attr()                                                           */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_add_attr() 001",                                 test_qs_add_attr_001);
  CU_add_test(str_util_suite, "qs_add_attr() 002",                                 test_qs_add_attr_002);
  CU_add_test(str_util_suite, "qs_add_attr() 003",                                 test_qs_add_attr_003);
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
/* qs_new_tag()                                                              */
/*===========================================================================*/
void test_qs_new_tag_001()
{
  Node *ret;
  APR_INIT;

  ret = qs_new_tag(&doc);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret->next == NULL);
  CU_ASSERT(ret->parent == NULL);
  CU_ASSERT(ret->child == NULL);
  CU_ASSERT(ret->child_tail == NULL);
  CU_ASSERT(ret->attr == NULL);
  CU_ASSERT(ret->attr_tail == NULL);
  CU_ASSERT(ret->name == NULL);
  CU_ASSERT(ret->value == NULL);

  APR_TERM;
}
void test_qs_new_tag_002()
{
  Node *ret;
  APR_INIT;

  ret = qs_new_tag(NULL);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
void test_qs_new_tag_003()
{
  Node *ret;
  APR_INIT;
  doc.pool = NULL;
  ret = qs_new_tag(&doc);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
/*===========================================================================*/
/* qs_new_tag()                                                              */
/*===========================================================================*/
#if 0
Node *
qs_add_attr(Doc *doc, Node *node, Attr *attr)
{
  if (node == NULL) {
    QX_LOGGER_FATAL("runtime exception: qs_add_attr(): node is null");
    return NULL;
  }

  attr->parent = node;
  attr->next   = NULL;

  if (node->attr == NULL) {
    node->attr      = attr;
    node->attr_tail = attr;

    return node;
  }

  node->attr_tail->next = attr;
  node->attr_tail       = attr;

  return node;
}
#endif
void test_qs_add_attr_001()
{
  Node *node;
  Node *ret;
  Attr *attr;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  ret = qs_add_attr(&doc, node, attr);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret == node);
  CU_ASSERT(ret->attr      == attr);
  CU_ASSERT(ret->attr_tail == attr);
  CU_ASSERT(ret->attr->next == NULL);
  CU_ASSERT(ret->attr->parent == node);

  APR_TERM;
}
void test_qs_add_attr_002()
{
  Node *node;
  Node *ret;
  Attr *attr1;
  Attr *attr2;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr1 = qs_new_attr(&doc);
  attr2 = qs_new_attr(&doc);
  ret = qs_add_attr(&doc, node, attr1);
  ret = qs_add_attr(&doc, node, attr2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret == node);
  CU_ASSERT(ret->attr == attr1);
  CU_ASSERT(ret->attr->next == attr2);
  CU_ASSERT(ret->attr_tail  == attr2);
  CU_ASSERT(ret->attr->parent == node);
  CU_ASSERT(ret->attr->next->parent == node);

  APR_TERM;
}
void test_qs_add_attr_003()
{
  Node *node;
  Node *ret;
  Attr *attr1;
  Attr *attr2;
  Attr *attr3;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr1 = qs_new_attr(&doc);
  attr2 = qs_new_attr(&doc);
  attr3 = qs_new_attr(&doc);
  ret = qs_add_attr(&doc, node, attr1);
  ret = qs_add_attr(&doc, node, attr2);
  ret = qs_add_attr(&doc, node, attr3);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret == node);
  CU_ASSERT(ret->attr == attr1);
  CU_ASSERT(ret->attr->next == attr2);
  CU_ASSERT(ret->attr->next->next == attr3);
  CU_ASSERT(ret->attr_tail  == attr3);
  CU_ASSERT(ret->attr->parent == node);
  CU_ASSERT(ret->attr->next->parent == node);
  CU_ASSERT(ret->attr->next->next->parent == node);

  APR_TERM;
}
/*
 * vim:ts=2 et
 */
