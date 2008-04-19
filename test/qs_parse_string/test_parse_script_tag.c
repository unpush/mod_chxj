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


void test_parse_script_tag_001();
void test_parse_script_tag_002();
void test_parse_script_tag_003();
void test_parse_script_tag_004();
void test_parse_script_tag_005();
void test_parse_script_tag_006();
void test_parse_script_tag_007();
void test_parse_script_tag_008();
void test_parse_script_tag_009();
void test_parse_script_tag_010();


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
  CU_pSuite script_tag_suite;
  CU_initialize_registry();
  script_tag_suite = CU_add_suite("test qs_parse_string()", NULL, NULL);
  CU_add_test(script_tag_suite, "test normal pattern",             test_parse_script_tag_001);
  CU_add_test(script_tag_suite, "test inner tag1",                 test_parse_script_tag_002);
  CU_add_test(script_tag_suite, "test inner tag2",                 test_parse_script_tag_003);
  CU_add_test(script_tag_suite, "test short tag",                  test_parse_script_tag_004);
  CU_add_test(script_tag_suite, "test not close script tag",      test_parse_script_tag_005);
#if 0
  CU_add_test(script_tag_suite, "test close only script tag",     test_parse_script_tag_006);
  CU_add_test(script_tag_suite, "test null string",                test_parse_script_tag_007);
  CU_add_test(script_tag_suite, "test null",                       test_parse_script_tag_008);
  CU_add_test(script_tag_suite, "test minus len",                  test_parse_script_tag_009);
  CU_add_test(script_tag_suite, "test ambiguos close tag",         test_parse_script_tag_010);
#endif
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

static void dump(Node *node, int indent);

static void dump(Node *node, int indent) 
{
  Node *child; 
  for (child = node->child;
       child;
       child = child->next) {
    fprintf(stderr, "%d:%*.*s:[%s] value:[%s]\n", indent, 10 + (indent * 2), 10 + (indent * 2), "Node", child->name, child->otext);
    dump(child, indent+1);
  }
}

void test_parse_script_tag_001() 
{
#define  TEST_STRING "<script>abc</script>"
  Doc doc;
  Node *node;
  Node *node2;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  fprintf(stderr, "====\n");
  dump(node, 1);
  fprintf(stderr, "====\n");
  node2 = node->child;
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("script",node2->name) == 0);
  CU_ASSERT(node2->child != NULL);
  node2 = node2->child;
  CU_ASSERT(node2 != NULL);
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("text",node2->name) == 0);

  APR_TERM;
#undef TEST_STRING
}

void test_parse_script_tag_002() 
{
#define  TEST_STRING "<script>‚ ‚ ‚ abc</script>"
  Doc doc;
  Node *node;
  Node *node2;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  fprintf(stderr, "====\n");
  dump(node, 1);
  fprintf(stderr, "====\n");
  node2 = node->child;
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("script",node2->name) == 0);
  CU_ASSERT(node2->child != NULL);
  node2 = node2->child;
  CU_ASSERT(node2 != NULL);
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("text",node2->name) == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_parse_script_tag_003() 
{
#define  TEST_STRING "<script />abc"
  Doc doc;
  Node *node;
  Node *node2;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);
  fprintf(stderr, "====\n");
  dump(node, 1);
  fprintf(stderr, "====\n");
  node2 = node->child;
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("script",node2->name) == 0);
  CU_ASSERT(node2->child == NULL);

  APR_TERM;
#undef TEST_STRING
}
void test_parse_script_tag_004() 
{
#define  TEST_STRING "<script>'</script>'</script>"
  Doc doc;
  Node *node;
  Node *node2;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);

  fprintf(stderr, "====\n");
  dump(node, 1);
  fprintf(stderr, "====\n");

  node2 = node->child;
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("script",node2->name) == 0);
  CU_ASSERT(node2->child != NULL);

  node2 = node2->child;
  CU_ASSERT(node2 != NULL);
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("text",node2->name) == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_parse_script_tag_005() 
{
#define  TEST_STRING "<script>\"</script>\"</script>"
  Doc doc;
  Node *node;
  Node *node2;
  APR_INIT;

  qs_init_malloc(&doc);
  qs_init_root_node(&doc);
  doc.parse_mode = PARSE_MODE_CHTML;

  node = qs_parse_string(&doc, TEST_STRING, sizeof(TEST_STRING)-1);

  CU_ASSERT(node != NULL);
  CU_ASSERT(node->name != NULL);
  CU_ASSERT(strcasecmp("ROOT",node->name) == 0);
  CU_ASSERT(node->child != NULL);

  fprintf(stderr, "====\n");
  dump(node, 1);
  fprintf(stderr, "====\n");

  node2 = node->child;
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("script",node2->name) == 0);
  CU_ASSERT(node2->child != NULL);

  node2 = node2->child;
  CU_ASSERT(node2 != NULL);
  CU_ASSERT(node2->name != NULL);
  CU_ASSERT(strcasecmp("text",node2->name) == 0);

  APR_TERM;
#undef TEST_STRING
}

