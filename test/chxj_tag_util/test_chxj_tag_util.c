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
/* qs_get_value_attr();                                                      */
/*===========================================================================*/
void test_qs_get_value_attr_001();
void test_qs_get_value_attr_002();
void test_qs_get_value_attr_003();
/*===========================================================================*/
/* qs_get_checked_attr()                                                     */
/*===========================================================================*/
void test_qs_get_checked_attr_001();
void test_qs_get_checked_attr_002();
void test_qs_get_checked_attr_003();
/*===========================================================================*/
/* qs_get_type_attr()                                                        */
/*===========================================================================*/
void test_qs_get_type_attr_001();
void test_qs_get_type_attr_002();
void test_qs_get_type_attr_003();
/*===========================================================================*/
/* qs_alloc_zero_byte_string()                                               */
/*===========================================================================*/
void test_qs_alloc_zero_byte_string_001();
void test_qs_alloc_zero_byte_string_002();
/*===========================================================================*/
/* qs_trim_string()                                                          */
/*===========================================================================*/
void test_qs_trim_string_001();
void test_qs_trim_string_002();
void test_qs_trim_string_003();
void test_qs_trim_string_004();
void test_qs_trim_string_005();
void test_qs_trim_string_006();
/*===========================================================================*/
/* qs_get_selected_value_text()                                              */
/*===========================================================================*/
void test_qs_get_selected_value_text_001();
void test_qs_get_selected_value_text_002();
void test_qs_get_selected_value_text_003();
void test_qs_get_selected_value_text_004();
void test_qs_get_selected_value_text_005();
/*===========================================================================*/
/* qs_get_selected_value()                                                   */
/*===========================================================================*/
void test_qs_get_selected_value_001();
void test_qs_get_selected_value_002();
void test_qs_get_selected_value_003();
void test_qs_get_selected_value_004();
void test_qs_get_selected_value_005();
/*===========================================================================*/
/* qs_get_name_attr()                                                        */
/*===========================================================================*/
void test_qs_get_name_attr_001();
void test_qs_get_name_attr_002();
void test_qs_get_name_attr_003();
void test_qs_get_name_attr_004();
void test_qs_get_name_attr_005();
void test_qs_get_name_attr_006();
/*===========================================================================*/
/* qs_get_size_attr()                                                        */
/*===========================================================================*/
void test_qs_get_size_attr_001();
void test_qs_get_size_attr_002();
void test_qs_get_size_attr_003();
void test_qs_get_size_attr_004();
void test_qs_get_size_attr_005();
void test_qs_get_size_attr_006();
/*===========================================================================*/
/* qs_get_accesskey_attr()                                                   */
/*===========================================================================*/
void test_qs_get_accesskey_attr_001();
void test_qs_get_accesskey_attr_002();
void test_qs_get_accesskey_attr_003();
void test_qs_get_accesskey_attr_004();
void test_qs_get_accesskey_attr_005();
void test_qs_get_accesskey_attr_006();
/*===========================================================================*/
/* qs_get_istyle_attr()                                                      */
/*===========================================================================*/
void test_qs_get_istyle_attr_001();
void test_qs_get_istyle_attr_002();
void test_qs_get_istyle_attr_003();
void test_qs_get_istyle_attr_004();
void test_qs_get_istyle_attr_005();
void test_qs_get_istyle_attr_006();
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
  /* qs_get_value_attr()                                                     */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_value_attr() 001",                               test_qs_get_value_attr_001);
  CU_add_test(str_util_suite, "qs_get_value_attr() 002",                               test_qs_get_value_attr_002);
  CU_add_test(str_util_suite, "qs_get_value_attr() 003",                               test_qs_get_value_attr_003);
  /*=========================================================================*/
  /* qs_get_checked_attr()                                                   */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_checked_attr() 001",                             test_qs_get_checked_attr_001);
  CU_add_test(str_util_suite, "qs_get_checked_attr() 002",                             test_qs_get_checked_attr_002);
  CU_add_test(str_util_suite, "qs_get_checked_attr() 003",                             test_qs_get_checked_attr_003);
  /*=========================================================================*/
  /* qs_get_type_attr()                                                      */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_type_attr() 001",                                test_qs_get_type_attr_001);
  CU_add_test(str_util_suite, "qs_get_type_attr() 002",                                test_qs_get_type_attr_002);
  CU_add_test(str_util_suite, "qs_get_type_attr() 003",                                test_qs_get_type_attr_003);
  /*=========================================================================*/
  /* qs_alloc_zero_byte_string()                                             */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_alloc_zero_byte_string() 001",                       test_qs_alloc_zero_byte_string_001);
  CU_add_test(str_util_suite, "qs_alloc_zero_byte_string() 002",                       test_qs_alloc_zero_byte_string_002);
  /*=========================================================================*/
  /* qs_trim_string()                                                        */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_trim_string() 001",                                  test_qs_trim_string_001);
  CU_add_test(str_util_suite, "qs_trim_string() 002",                                  test_qs_trim_string_002);
  CU_add_test(str_util_suite, "qs_trim_string() 003",                                  test_qs_trim_string_003);
  CU_add_test(str_util_suite, "qs_trim_string() 004",                                  test_qs_trim_string_004);
  CU_add_test(str_util_suite, "qs_trim_string() 005",                                  test_qs_trim_string_005);
  CU_add_test(str_util_suite, "qs_trim_string() 006",                                  test_qs_trim_string_006);
  /*=========================================================================*/
  /* qs_get_selected_value_text()                                            */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_selected_value_text() 001",                      test_qs_get_selected_value_text_001);
  CU_add_test(str_util_suite, "qs_get_selected_value_text() 002",                      test_qs_get_selected_value_text_002);
  CU_add_test(str_util_suite, "qs_get_selected_value_text() 003",                      test_qs_get_selected_value_text_003);
  CU_add_test(str_util_suite, "qs_get_selected_value_text() 004",                      test_qs_get_selected_value_text_004);
  CU_add_test(str_util_suite, "qs_get_selected_value_text() 005",                      test_qs_get_selected_value_text_005);
  /*=========================================================================*/
  /* qs_get_selected_value()                                                 */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_selected_value() 001",                           test_qs_get_selected_value_001);
  CU_add_test(str_util_suite, "qs_get_selected_value() 002",                           test_qs_get_selected_value_002);
  CU_add_test(str_util_suite, "qs_get_selected_value() 003",                           test_qs_get_selected_value_003);
  CU_add_test(str_util_suite, "qs_get_selected_value() 004",                           test_qs_get_selected_value_004);
  CU_add_test(str_util_suite, "qs_get_selected_value() 005",                           test_qs_get_selected_value_005);
  /*=========================================================================*/
  /* qs_get_name_attr()                                                      */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_name_attr() 001",                                test_qs_get_name_attr_001);
  CU_add_test(str_util_suite, "qs_get_name_attr() 002",                                test_qs_get_name_attr_002);
  CU_add_test(str_util_suite, "qs_get_name_attr() 003",                                test_qs_get_name_attr_003);
  CU_add_test(str_util_suite, "qs_get_name_attr() 004",                                test_qs_get_name_attr_004);
  CU_add_test(str_util_suite, "qs_get_name_attr() 005",                                test_qs_get_name_attr_005);
  CU_add_test(str_util_suite, "qs_get_name_attr() 006",                                test_qs_get_name_attr_006);
  /*=========================================================================*/
  /* qs_get_size_attr()                                                      */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_size_attr() 001",                                test_qs_get_size_attr_001);
  CU_add_test(str_util_suite, "qs_get_size_attr() 002",                                test_qs_get_size_attr_002);
  CU_add_test(str_util_suite, "qs_get_size_attr() 003",                                test_qs_get_size_attr_003);
  CU_add_test(str_util_suite, "qs_get_size_attr() 004",                                test_qs_get_size_attr_004);
  CU_add_test(str_util_suite, "qs_get_size_attr() 005",                                test_qs_get_size_attr_005);
  CU_add_test(str_util_suite, "qs_get_size_attr() 006",                                test_qs_get_size_attr_006);
  /*=========================================================================*/
  /* qs_get_accesskey_attr()                                                 */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 001",                           test_qs_get_accesskey_attr_001);
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 002",                           test_qs_get_accesskey_attr_002);
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 003",                           test_qs_get_accesskey_attr_003);
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 004",                           test_qs_get_accesskey_attr_004);
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 005",                           test_qs_get_accesskey_attr_005);
  CU_add_test(str_util_suite, "qs_get_accesskey_attr() 006",                           test_qs_get_accesskey_attr_006);
  /*=========================================================================*/
  /* qs_get_istyle_attr()                                                    */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 001",                              test_qs_get_istyle_attr_001);
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 002",                              test_qs_get_istyle_attr_002);
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 003",                              test_qs_get_istyle_attr_003);
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 004",                              test_qs_get_istyle_attr_004);
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 005",                              test_qs_get_istyle_attr_005);
  CU_add_test(str_util_suite, "qs_get_istyle_attr() 006",                              test_qs_get_istyle_attr_006);
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
    doc.buf.pool = p; \
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
void test_qs_get_value_attr_001()
{
  Node *node;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  ret = qs_get_value_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
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
}
/*===========================================================================*/
/* qs_get_checked_attr()                                                     */
/*===========================================================================*/
void test_qs_get_checked_attr_001()
{
  Node *node;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  ret = qs_get_checked_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
void test_qs_get_checked_attr_002()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "checked");
  attr->value = apr_pstrdup(p, "");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_checked_attr(&doc,node,p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, "checked") == 0);

  APR_TERM;
}
void test_qs_get_checked_attr_003()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "checkedd");
  attr->value = apr_pstrdup(p, "");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_checked_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
/*===========================================================================*/
/* qs_get_type_attr()                                                        */
/*===========================================================================*/
void test_qs_get_type_attr_001()
{
  Node *node;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  ret = qs_get_type_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
void test_qs_get_type_attr_002()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "type");
  attr->value = apr_pstrdup(p, "hidden");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_type_attr(&doc,node,p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, "hidden") == 0);

  APR_TERM;
}
void test_qs_get_type_attr_003()
{
  Node *node;
  Attr *attr;
  char *ret;
  APR_INIT;

  node = qs_new_tag(&doc);
  attr = qs_new_attr(&doc);
  attr->name = apr_pstrdup(p, "typed");
  attr->value = apr_pstrdup(p, "");
  qs_add_attr(&doc,node,attr);
  ret = qs_get_type_attr(&doc,node,p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
/*===========================================================================*/
/* qs_alloc_zero_byte_string()                                               */
/*===========================================================================*/
void test_qs_alloc_zero_byte_string_001()
{
  char *ret;
  APR_INIT;

  ret = qs_alloc_zero_byte_string(p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(ret[0] == 0);

  APR_TERM;
}
void test_qs_alloc_zero_byte_string_002()
{
  char *ret;
  APR_INIT;

  ret = qs_alloc_zero_byte_string(NULL);
  CU_ASSERT(ret == NULL);

  APR_TERM;
}
/*===========================================================================*/
/* qs_trim_string()                                                          */
/*===========================================================================*/
void test_qs_trim_string_001()
{
#define TEST_STRING " a "
#define RESULT_STRING "a"
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_trim_string_002()
{
#define TEST_STRING "\na\n"
#define RESULT_STRING "a"
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_trim_string_003()
{
#define TEST_STRING "\ra\r"
#define RESULT_STRING "a"
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_trim_string_004()
{
#define TEST_STRING "\ta\t"
#define RESULT_STRING "a"
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_trim_string_005()
{
#define TEST_STRING "   "
#define RESULT_STRING ""
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_trim_string_006()
{
#define TEST_STRING " a a "
#define RESULT_STRING "a a"
  char *ret;
  APR_INIT;

  ret = qs_trim_string(p, TEST_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_selected_value_text()                                              */
/*===========================================================================*/
void test_qs_get_selected_value_text_001()
{
#define TEST_STRING "<select><option value='1' selected>a</option><option value='2'>b</option></select>"
#define RESULT_STRING "a"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value_text(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_text_002()
{
#define TEST_STRING "<select><option value='1'>a</option><option value='2' selected>b</option></select>"
#define RESULT_STRING "b"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value_text(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_text_003()
{
#define TEST_STRING "<select><option value='1' selected></option></select>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value_text(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_text_004()
{
#define TEST_STRING "<select><option value='1'><option value='2'></option></select>"
#define RESULT_STRING NULL
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value_text(&doc, node, p);
  CU_ASSERT(ret == RESULT_STRING);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_text_005()
{
#define TEST_STRING "<select></select>"
#define RESULT_STRING NULL
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value_text(&doc, node, p);
  CU_ASSERT(ret == RESULT_STRING);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_selected_value()                                                   */
/*===========================================================================*/
void test_qs_get_selected_value_001()
{
#define TEST_STRING "<select><option value='1' selected>a</option><option value='2'>b</option></select>"
#define RESULT_STRING "1"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_002()
{
#define TEST_STRING "<select><option value='1'>a</option><option value='2' selected>b</option></select>"
#define RESULT_STRING "2"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_003()
{
#define TEST_STRING "<select><option value='1' selected></option></select>"
#define RESULT_STRING "1"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value(&doc, node, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_004()
{
#define TEST_STRING "<select><option value='1'><option value='2'></option></select>"
#define RESULT_STRING NULL
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value(&doc, node, p);
  CU_ASSERT(ret == RESULT_STRING);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_selected_value_005()
{
#define TEST_STRING "<select></select>"
#define RESULT_STRING NULL
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_selected_value(&doc, node, p);
  CU_ASSERT(ret == RESULT_STRING);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_name_attr()                                                        */
/*===========================================================================*/
void test_qs_get_name_attr_001()
{
#define TEST_STRING "<input name=\"a\">"
#define RESULT_STRING "a"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_name_attr_002()
{
#define TEST_STRING "<input name='b'>"
#define RESULT_STRING "b"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_name_attr_003()
{
#define TEST_STRING "<input name=''>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_name_attr_004()
{
#define TEST_STRING "<input name>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_name_attr_005()
{
#define TEST_STRING "<input name='あい\"う\"えお'>"
#define RESULT_STRING "あい\"う\"えお"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_name_attr_006()
{
#define TEST_STRING "<input>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_name_attr(&doc, node->child, p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_size_attr()                                                        */
/*===========================================================================*/
void test_qs_get_size_attr_001()
{
#define TEST_STRING "<input size=\"a\">"
#define RESULT_STRING "a"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_size_attr_002()
{
#define TEST_STRING "<input size='b'>"
#define RESULT_STRING "b"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_size_attr_003()
{
#define TEST_STRING "<input size=''>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_size_attr_004()
{
#define TEST_STRING "<input size>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_size_attr_005()
{
#define TEST_STRING "<input size='あい\"う\"えお'>"
#define RESULT_STRING "あい\"う\"えお"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_size_attr_006()
{
#define TEST_STRING "<input>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_size_attr(&doc, node->child, p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_accesskey_attr()                                                   */
/*===========================================================================*/
void test_qs_get_accesskey_attr_001()
{
#define TEST_STRING "<input accesskey=\"a\">"
#define RESULT_STRING "a"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_accesskey_attr_002()
{
#define TEST_STRING "<input accesskey='b'>"
#define RESULT_STRING "b"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_accesskey_attr_003()
{
#define TEST_STRING "<input accesskey=''>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_accesskey_attr_004()
{
#define TEST_STRING "<input accesskey>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_accesskey_attr_005()
{
#define TEST_STRING "<input accesskey='あい\"う\"えお'>"
#define RESULT_STRING "あい\"う\"えお"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_accesskey_attr_006()
{
#define TEST_STRING "<input>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_accesskey_attr(&doc, node->child, p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*===========================================================================*/
/* qs_get_istyle_attr()                                                      */
/*===========================================================================*/
void test_qs_get_istyle_attr_001()
{
#define TEST_STRING "<input istyle=\"a\">"
#define RESULT_STRING "a"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_istyle_attr_002()
{
#define TEST_STRING "<input istyle='b'>"
#define RESULT_STRING "b"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_istyle_attr_003()
{
#define TEST_STRING "<input istyle=''>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_istyle_attr_004()
{
#define TEST_STRING "<input istyle>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_istyle_attr_005()
{
#define TEST_STRING "<input istyle='あい\"う\"えお'>"
#define RESULT_STRING "あい\"う\"えお"
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
void test_qs_get_istyle_attr_006()
{
#define TEST_STRING "<input>"
#define RESULT_STRING ""
  Node *node;
  char *ret;
  APR_INIT;
 
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)); 
  ret = qs_get_istyle_attr(&doc, node->child, p);
  CU_ASSERT(ret == NULL);

  APR_TERM;
#undef RESULT_STRING
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
