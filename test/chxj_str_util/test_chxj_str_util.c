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
#include <iconv.h>


/*===========================================================================*/
/* chxj_chk_numeric()                                                        */
/*===========================================================================*/
void test_chxj_chk_numeric_001();
void test_chxj_chk_numeric_002();
void test_chxj_chk_numeric_003();
void test_chxj_chk_numeric_004();
void test_chxj_chk_numeric_005();
void test_chxj_chk_numeric_006();
void test_chxj_chk_numeric_007();
void test_chxj_chk_numeric_008();
void test_chxj_chk_numeric_009();
void test_chxj_chk_numeric_010();
void test_chxj_chk_numeric_011();
void test_chxj_chk_numeric_012();
void test_chxj_chk_numeric_013();
void test_chxj_chk_numeric_014();
void test_chxj_chk_numeric_015();
void test_chxj_chk_numeric_016();
/*===========================================================================*/
/* chxj_atoi()                                                               */
/*===========================================================================*/
void test_chxj_atoi_001();
void test_chxj_atoi_002();
void test_chxj_atoi_003();
void test_chxj_atoi_004();
void test_chxj_atoi_005();
void test_chxj_atoi_006();
void test_chxj_atoi_007();
void test_chxj_atoi_008();
void test_chxj_atoi_009();
void test_chxj_atoi_010();
void test_chxj_atoi_011();
void test_chxj_atoi_012();
void test_chxj_atoi_013();
void test_chxj_atoi_014();
void test_chxj_atoi_015();
void test_chxj_atoi_016();
void test_chxj_atoi_017();
void test_chxj_atoi_018();
void test_chxj_atoi_019();
/* pend */

int
main()
{
  CU_pSuite str_util_suite;
  CU_initialize_registry();
  str_util_suite = CU_add_suite("test chxj_convert_str_util()", NULL, NULL);
  /*=========================================================================*/
  /* chxj_chk_numeric()                                                      */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_chk_numeric 001",                                  test_chxj_chk_numeric_001);
  CU_add_test(str_util_suite, "chxj_chk_numeric 002",                                  test_chxj_chk_numeric_002);
  CU_add_test(str_util_suite, "chxj_chk_numeric 003",                                  test_chxj_chk_numeric_003);
  CU_add_test(str_util_suite, "chxj_chk_numeric 004",                                  test_chxj_chk_numeric_004);
  CU_add_test(str_util_suite, "chxj_chk_numeric 005",                                  test_chxj_chk_numeric_005);
  CU_add_test(str_util_suite, "chxj_chk_numeric 006",                                  test_chxj_chk_numeric_006);
  CU_add_test(str_util_suite, "chxj_chk_numeric 007",                                  test_chxj_chk_numeric_007);
  CU_add_test(str_util_suite, "chxj_chk_numeric 008",                                  test_chxj_chk_numeric_008);
  CU_add_test(str_util_suite, "chxj_chk_numeric 009",                                  test_chxj_chk_numeric_009);
  CU_add_test(str_util_suite, "chxj_chk_numeric 010",                                  test_chxj_chk_numeric_010);
  CU_add_test(str_util_suite, "chxj_chk_numeric 011",                                  test_chxj_chk_numeric_011);
  CU_add_test(str_util_suite, "chxj_chk_numeric 012",                                  test_chxj_chk_numeric_012);
  CU_add_test(str_util_suite, "chxj_chk_numeric 013",                                  test_chxj_chk_numeric_013);
  CU_add_test(str_util_suite, "chxj_chk_numeric 014",                                  test_chxj_chk_numeric_014);
  CU_add_test(str_util_suite, "chxj_chk_numeric 015",                                  test_chxj_chk_numeric_015);
  CU_add_test(str_util_suite, "chxj_chk_numeric 016",                                  test_chxj_chk_numeric_016);
  /*=========================================================================*/
  /* chxj_atoi()                                                             */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_atoi 001",                                         test_chxj_atoi_001);
  CU_add_test(str_util_suite, "chxj_atoi 002",                                         test_chxj_atoi_002);
  CU_add_test(str_util_suite, "chxj_atoi 003",                                         test_chxj_atoi_003);
  CU_add_test(str_util_suite, "chxj_atoi 004",                                         test_chxj_atoi_004);
  CU_add_test(str_util_suite, "chxj_atoi 005",                                         test_chxj_atoi_005);
  CU_add_test(str_util_suite, "chxj_atoi 006",                                         test_chxj_atoi_006);
  CU_add_test(str_util_suite, "chxj_atoi 007",                                         test_chxj_atoi_007);
  CU_add_test(str_util_suite, "chxj_atoi 008",                                         test_chxj_atoi_008);
  CU_add_test(str_util_suite, "chxj_atoi 009",                                         test_chxj_atoi_009);
  CU_add_test(str_util_suite, "chxj_atoi 010",                                         test_chxj_atoi_010);
  CU_add_test(str_util_suite, "chxj_atoi 011",                                         test_chxj_atoi_011);
  CU_add_test(str_util_suite, "chxj_atoi 012",                                         test_chxj_atoi_012);
  CU_add_test(str_util_suite, "chxj_atoi 013",                                         test_chxj_atoi_013);
  CU_add_test(str_util_suite, "chxj_atoi 014",                                         test_chxj_atoi_014);
  CU_add_test(str_util_suite, "chxj_atoi 015",                                         test_chxj_atoi_015);
  CU_add_test(str_util_suite, "chxj_atoi 016",                                         test_chxj_atoi_016);
  CU_add_test(str_util_suite, "chxj_atoi 017",                                         test_chxj_atoi_017);
  CU_add_test(str_util_suite, "chxj_atoi 018",                                         test_chxj_atoi_018);
  CU_add_test(str_util_suite, "chxj_atoi 019",                                         test_chxj_atoi_019);
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}



#define APR_INIT \
  request_rec r; \
  apr_pool_t *p; \
  do { \
    apr_initialize(); \
    apr_pool_create(&p, NULL); \
    r.pool = p; \
    r.hostname = apr_pstrdup(p, "localhost"); \
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
/* chxj_chk_numeric()                                                        */
/*===========================================================================*/
void test_chxj_chk_numeric_001()
{
#define  TEST_STRING ""
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_002()
{
#define  TEST_STRING "a"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_003()
{
#define  TEST_STRING "1a"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_004()
{
#define  TEST_STRING NULL
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_005()
{
#define  TEST_STRING "0"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_006()
{
#define  TEST_STRING "1"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_007()
{
#define  TEST_STRING "2"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_008()
{
#define  TEST_STRING "3"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_009()
{
#define  TEST_STRING "4"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_010()
{
#define  TEST_STRING "5"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_011()
{
#define  TEST_STRING "6"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_012()
{
#define  TEST_STRING "7"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_013()
{
#define  TEST_STRING "8"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_014()
{
#define  TEST_STRING "9"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_015()
{
#define  TEST_STRING "+1"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_chk_numeric_016()
{
#define  TEST_STRING "-1"
  int ret;
  APR_INIT;

  ret = chxj_chk_numeric(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
/*===========================================================================*/
/* chxj_atoi()                                                               */
/*===========================================================================*/
void test_chxj_atoi_001()
{
#define  TEST_STRING ""
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_002()
{
#define  TEST_STRING NULL
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_003()
{
#define  TEST_STRING "                   "
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_004()
{
#define  TEST_STRING "                   "
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_005()
{
#define  TEST_STRING "abc123"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_006()
{
#define  TEST_STRING "123abc"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 123);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_007()
{
#define  TEST_STRING "1"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_008()
{
#define  TEST_STRING "12"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 12);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_009()
{
#define  TEST_STRING "-"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_010()
{
#define  TEST_STRING "+"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_011()
{
#define  TEST_STRING "--"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_012()
{
#define  TEST_STRING "++"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_013()
{
#define  TEST_STRING "+-"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_014()
{
#define  TEST_STRING "-+"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 0);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_015()
{
#define  TEST_STRING "-1"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_016()
{
#define  TEST_STRING "-1-"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == -1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_017()
{
#define  TEST_STRING "+1"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_018()
{
#define  TEST_STRING "+1+"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_chxj_atoi_019()
{
#define  TEST_STRING "+1-"
  int ret;
  APR_INIT;

  ret = chxj_atoi(TEST_STRING);
  
  CU_ASSERT(ret == 1);

  APR_TERM;
#undef TEST_STRING
}
/*
 * vim:ts=2 et
 */
