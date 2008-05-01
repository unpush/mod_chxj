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
void test_qs_add_attr_004();
void test_qs_add_attr_005();
void test_qs_add_attr_006();
/*===========================================================================*/
/* qs_parse_tag()                                                            */
/*===========================================================================*/
void test_qs_parse_tag_001();
void test_qs_parse_tag_002();
void test_qs_parse_tag_003();
void test_qs_parse_tag_004();
void test_qs_parse_tag_005();
void test_qs_parse_tag_006();
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
  CU_add_test(str_util_suite, "qs_add_attr() 004",                                 test_qs_add_attr_004);
  CU_add_test(str_util_suite, "qs_add_attr() 005",                                 test_qs_add_attr_005);
  CU_add_test(str_util_suite, "qs_add_attr() 006",                                 test_qs_add_attr_006);
  /*=========================================================================*/
  /* qs_parse_tag()                                                          */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_parse_tag() 001",                                test_qs_parse_tag_001);
  CU_add_test(str_util_suite, "qs_parse_tag() 002",                                test_qs_parse_tag_002);
  CU_add_test(str_util_suite, "qs_parse_tag() 003",                                test_qs_parse_tag_003);
  CU_add_test(str_util_suite, "qs_parse_tag() 004",                                test_qs_parse_tag_004);
  CU_add_test(str_util_suite, "qs_parse_tag() 005",                                test_qs_parse_tag_005);
  CU_add_test(str_util_suite, "qs_parse_tag() 006",                                test_qs_parse_tag_006);
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
void test_qs_add_attr_004()
{
  Node *node;
  Node *ret;
  Attr *attr1;
  Attr *attr2;
  Attr *attr3;
  Attr *attr4;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr1 = qs_new_attr(&doc);
  attr2 = qs_new_attr(&doc);
  attr3 = qs_new_attr(&doc);
  attr4 = qs_new_attr(&doc);
  ret = qs_add_attr(&doc, node, attr1);
  ret = qs_add_attr(&doc, node, attr2);
  ret = qs_add_attr(&doc, node, attr3);
  ret = qs_add_attr(&doc, node, attr4);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret == node);
  CU_ASSERT(ret->attr == attr1);
  CU_ASSERT(ret->attr->next == attr2);
  CU_ASSERT(ret->attr->next->next == attr3);
  CU_ASSERT(ret->attr->next->next->next == attr4);
  CU_ASSERT(ret->attr_tail  == attr4);
  CU_ASSERT(ret->attr->parent == node);
  CU_ASSERT(ret->attr->next->parent == node);
  CU_ASSERT(ret->attr->next->next->parent == node);
  CU_ASSERT(ret->attr->next->next->next->parent == node);

  APR_TERM;
}
void test_qs_add_attr_005()
{
  Node *node;
  Node *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  ret = qs_add_attr(&doc, node, NULL);
  CU_ASSERT(ret == node);
  CU_ASSERT(ret->attr == NULL);
  CU_ASSERT(ret->attr_tail == NULL);

  APR_TERM;
}
void test_qs_add_attr_006()
{
  Node *node;
  Node *ret;
  Attr *attr;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  ret = qs_add_attr(&doc, NULL, attr);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
/*===========================================================================*/
/* qs_parse_tag()                                                            */
/*===========================================================================*/
#if 0
Node *
qs_parse_tag(Doc *doc, const char *s, int len) 
{
  Node   *node;
  char   *tag_name;
  char   *sp;
  char   *sv_s;
  int    ll;
  int    next_point;

  sv_s = sp = (char *)s;
  ll         = len;
  next_point = 0;

  QX_LOGGER_DEBUG("start parse_tag()");


  /* 
   * s[0] == '<' && s[len] == '>' 
   */
  tag_name = (char *)s_get_tag_name(doc, ++s, --ll);

  node = (Node *)qs_new_tag(doc);
  if (! node) {
    QX_LOGGER_DEBUG("runtime exception: qs_parse_tag(): Out of memory.");
    return NULL;
  }
  node->name = tag_name;
  node->otext = apr_palloc(doc->pool,len+2);
  memset(node->otext, 0, len+2);
  memcpy(node->otext, sp, len+1);

  QX_LOGGER_DEBUG(tag_name);

  ll -= (strlen(tag_name));
  QX_LOGGER_DEBUG_INT("ll",ll);
  sp += (strlen(tag_name)+1);
  for (;;) {
    Attr *attr = qs_parse_attr(doc,sp, ll, &next_point);
    if (attr == NULL) {
      QX_LOGGER_DEBUG("End of QS_PARSE_ATTR()");
      break;
    }
    QX_LOGGER_DEBUG(attr->name);
    QX_LOGGER_DEBUG(attr->value);
    sp += next_point;
    ll -= next_point;
    QX_LOGGER_DEBUG_INT(sp, ll);
    node = (Node *)qs_add_attr(doc,node, attr);
  }

  if (sv_s[len-1] == '/') {
    node->closed_by_itself = 1;
  }
  else {
    node->closed_by_itself = 0;
  }
  QX_LOGGER_DEBUG("end parse_tag()");

  return node;
}
#endif
void test_qs_parse_tag_001()
{
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(NULL, "", 0);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
void test_qs_parse_tag_002()
{
#define TEST_STRING "<abc def=\"abc\">"
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(&doc, TEST_STRING, sizeof(TEST_STRING)-2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(ret->attr != NULL);
  CU_ASSERT(strcmp(ret->attr->name, "def") == 0);
  CU_ASSERT(strcmp(ret->attr->value, "abc") == 0);
  CU_ASSERT(ret->attr_tail == ret->attr);
  CU_ASSERT(ret->attr->next == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_tag_003()
{
#define TEST_STRING "<abc def=\"abc\" aaa=\"111\">"
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(&doc, TEST_STRING, sizeof(TEST_STRING)-2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(ret->attr != NULL);
  CU_ASSERT(strcmp(ret->attr->name, "def") == 0);
  CU_ASSERT(strcmp(ret->attr->value, "abc") == 0);
  CU_ASSERT(strcmp(ret->attr->next->name, "aaa") == 0);
  CU_ASSERT(strcmp(ret->attr->next->value, "111") == 0);
  CU_ASSERT(ret->attr_tail != ret->attr);
  CU_ASSERT(ret->attr_tail == ret->attr->next);
  CU_ASSERT(ret->attr_tail->next == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_tag_004()
{
#define TEST_STRING "<abc />"
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(&doc, TEST_STRING, sizeof(TEST_STRING)-2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(ret->closed_by_itself != 0);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_tag_005()
{
#define TEST_STRING "<abc/>"
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(&doc, TEST_STRING, sizeof(TEST_STRING)-2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc/") == 0);
  CU_ASSERT(ret->closed_by_itself == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_qs_parse_tag_006()
{
#define TEST_STRING "<abc def=\"   abc   \">"
  Node *ret;
  APR_INIT;

  ret = qs_parse_tag(&doc, TEST_STRING, sizeof(TEST_STRING)-2);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret->name, "abc") == 0);
  CU_ASSERT(ret->attr != NULL);
  CU_ASSERT(strcmp(ret->attr->name, "def") == 0);
  CU_ASSERT(strcmp(ret->attr->value, "   abc   ") ==0);
  CU_ASSERT(ret->closed_by_itself == 0);

  APR_TERM;
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
