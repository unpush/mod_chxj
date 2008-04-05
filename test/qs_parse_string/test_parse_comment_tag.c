#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>

#include "httpd.h"
#include "qs_malloc.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include <iconv.h>


void test_parse_comment_tag_001();
void test_parse_comment_tag_002();
void test_parse_comment_tag_003();
void test_parse_comment_tag_004();
void test_parse_comment_tag_005();
void test_parse_comment_tag_006();
void test_parse_comment_tag_007();
void test_parse_comment_tag_008();
void test_parse_comment_tag_009();
void test_parse_comment_tag_010();


void ap_log_rerror(const char *file, int line, int level, apr_status_t status, const request_rec *r, const char *fmt, ...)
{
  va_list ap;
  fprintf(stderr, "ERROR LOG %s:%d ", file,line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt,ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
    doc.r = &r; \
  } \
  while (0)

#define APR_TERM \
  do { \
    apr_terminate(); \
  } while (0)


int
main()
{
  CU_pSuite comment_tag_suite;
  CU_initialize_registry();
  comment_tag_suite = CU_add_suite("test qs_parse_string()", NULL, NULL);
  CU_add_test(comment_tag_suite, "test normal pattern",             test_parse_comment_tag_001);
  CU_add_test(comment_tag_suite, "test inner tag1",                 test_parse_comment_tag_002);
  CU_add_test(comment_tag_suite, "test inner tag2",                 test_parse_comment_tag_003);
  CU_add_test(comment_tag_suite, "test short tag",                  test_parse_comment_tag_004);
  CU_add_test(comment_tag_suite, "test not close comment tag",      test_parse_comment_tag_005);
  CU_add_test(comment_tag_suite, "test close only comment tag",     test_parse_comment_tag_006);
  CU_add_test(comment_tag_suite, "test null string",                test_parse_comment_tag_007);
  CU_add_test(comment_tag_suite, "test null",                       test_parse_comment_tag_008);
  CU_add_test(comment_tag_suite, "test minus len",                  test_parse_comment_tag_009);
  CU_add_test(comment_tag_suite, "test ambiguos close tag",         test_parse_comment_tag_010);
#if 0
  CU_console_run_tests();
  CU_automated_run_tests();
  CU_ErrorCode CU_basic_run_tests();
#else
  CU_basic_run_tests();
#endif
  CU_cleanup_registry();

  return(0);
}


void test_parse_comment_tag_001() 
{
#define  TEST_STRING "<!-- comment -->"
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);

  APR_TERM;
#undef TEST_STRING
}

void test_parse_comment_tag_002() 
{
#define  TEST_STRING "<!-- <html><head></head><body></body></html> -->"
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);

  APR_TERM;
#undef TEST_STRING
}

void test_parse_comment_tag_003() 
{
#define  TEST_STRING "<html><!-- <html><head></head><body></body></html> --><body>"
  Doc doc;
  Node *node;
  Node *nn;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  for (nn = qs_get_child_node(&doc,node);
       nn;
       nn = qs_get_next_node(&doc,nn)) {
    CU_ASSERT(STRCASEEQ('h','H',"html",nn->name) || STRCASEEQ('b','B',"body",nn->name));
  }

  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_004() 
{
#define  TEST_STRING "<!---->"
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);
  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_005() 
{
#define  TEST_STRING "<!--"
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);
  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_006() 
{
#define  TEST_STRING "-->"
  Doc doc;
  Node *node;
  Node *nn;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  for (nn = qs_get_child_node(&doc,node);
       nn;
       nn = qs_get_next_node(&doc,nn)) {
    CU_ASSERT(STRCASEEQ('t','T',"text",nn->name));
  }
  
  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_007() 
{
#define  TEST_STRING ""
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);
  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_008() 
{
#define  TEST_STRING NULL
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);
  APR_TERM;
#undef TEST_STRING
}
void test_parse_comment_tag_009() 
{
#define  TEST_STRING ""
  Doc doc;
  Node *node;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING, -1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child == NULL);
  APR_TERM;
#undef TEST_STRING
}

void test_parse_comment_tag_010() 
{
#define  TEST_STRING "<!--'//-->'"
  Doc doc;
  Node *node;
  Node *nn;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  node = qs_parse_string(&doc, TEST_STRING,sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  for (nn = qs_get_child_node(&doc,node);
       nn;
       nn = qs_get_next_node(&doc,nn)) {
    CU_ASSERT(STRCASEEQ('t','T',"text",nn->name));
    CU_ASSERT(strcmp(nn->otext, "'") == 0);
  }
  APR_TERM;
#undef TEST_STRING
}
