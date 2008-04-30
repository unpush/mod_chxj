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
#include "chxj_url_encode.c"
#include <iconv.h>


/*===========================================================================*/
/* chxj_url_encode()                                                         */
/*===========================================================================*/
void test_chxj_url_encode_001();
void test_chxj_url_encode_002();
void test_chxj_url_encode_003();
void test_chxj_url_encode_004();
void test_chxj_url_encode_005();
void test_chxj_url_encode_006();
void test_chxj_url_encode_007();
void test_chxj_url_encode_008();
void test_chxj_url_encode_009();
void test_chxj_url_encode_010();
void test_chxj_url_encode_011();
void test_chxj_url_encode_012();
void test_chxj_url_encode_013();
void test_chxj_url_encode_014();
void test_chxj_url_encode_015();
void test_chxj_url_encode_016();
void test_chxj_url_encode_017();
void test_chxj_url_encode_018();
void test_chxj_url_encode_019();
void test_chxj_url_encode_020();
void test_chxj_url_encode_021();
void test_chxj_url_encode_022();
void test_chxj_url_encode_023();
/*===========================================================================*/
/* chxj_url_decode()                                                         */
/*===========================================================================*/
void test_chxj_url_decode_001();
void test_chxj_url_decode_002();
void test_chxj_url_decode_003();
void test_chxj_url_decode_004();
void test_chxj_url_decode_005();
void test_chxj_url_decode_006();
void test_chxj_url_decode_007();
/* pend */

int
main()
{
  CU_pSuite str_util_suite;
  CU_initialize_registry();
  str_util_suite = CU_add_suite("test chxj_convert_str_util()", NULL, NULL);
  /*=========================================================================*/
  /* chxj_url_encode()                                                       */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_url_encode 001",                                  test_chxj_url_encode_001);
  CU_add_test(str_util_suite, "chxj_url_encode 002",                                  test_chxj_url_encode_002);
  CU_add_test(str_util_suite, "chxj_url_encode 003",                                  test_chxj_url_encode_003);
  CU_add_test(str_util_suite, "chxj_url_encode 004",                                  test_chxj_url_encode_004);
  CU_add_test(str_util_suite, "chxj_url_encode 005",                                  test_chxj_url_encode_005);
  CU_add_test(str_util_suite, "chxj_url_encode 006",                                  test_chxj_url_encode_006);
  CU_add_test(str_util_suite, "chxj_url_encode 007",                                  test_chxj_url_encode_007);
  CU_add_test(str_util_suite, "chxj_url_encode 008",                                  test_chxj_url_encode_008);
  CU_add_test(str_util_suite, "chxj_url_encode 009",                                  test_chxj_url_encode_009);
  CU_add_test(str_util_suite, "chxj_url_encode 010",                                  test_chxj_url_encode_010);
  CU_add_test(str_util_suite, "chxj_url_encode 011",                                  test_chxj_url_encode_011);
  CU_add_test(str_util_suite, "chxj_url_encode 012",                                  test_chxj_url_encode_012);
  CU_add_test(str_util_suite, "chxj_url_encode 013",                                  test_chxj_url_encode_013);
  CU_add_test(str_util_suite, "chxj_url_encode 014",                                  test_chxj_url_encode_014);
  CU_add_test(str_util_suite, "chxj_url_encode 015",                                  test_chxj_url_encode_015);
  CU_add_test(str_util_suite, "chxj_url_encode 016",                                  test_chxj_url_encode_016);
  CU_add_test(str_util_suite, "chxj_url_encode 017",                                  test_chxj_url_encode_017);
  CU_add_test(str_util_suite, "chxj_url_encode 018",                                  test_chxj_url_encode_018);
  CU_add_test(str_util_suite, "chxj_url_encode 019",                                  test_chxj_url_encode_019);
  CU_add_test(str_util_suite, "chxj_url_encode 020",                                  test_chxj_url_encode_020);
  CU_add_test(str_util_suite, "chxj_url_encode 021",                                  test_chxj_url_encode_021);
  CU_add_test(str_util_suite, "chxj_url_encode 022",                                  test_chxj_url_encode_022);
  CU_add_test(str_util_suite, "chxj_url_encode 023",                                  test_chxj_url_encode_023);
  /*=========================================================================*/
  /* chxj_url_decode()                                                       */
  /*=========================================================================*/
  CU_add_test(str_util_suite, "chxj_url_decode 001",                                  test_chxj_url_decode_001);
  CU_add_test(str_util_suite, "chxj_url_decode 002",                                  test_chxj_url_decode_002);
  CU_add_test(str_util_suite, "chxj_url_decode 003",                                  test_chxj_url_decode_003);
  CU_add_test(str_util_suite, "chxj_url_decode 004",                                  test_chxj_url_decode_004);
  CU_add_test(str_util_suite, "chxj_url_decode 005",                                  test_chxj_url_decode_005);
  CU_add_test(str_util_suite, "chxj_url_decode 006",                                  test_chxj_url_decode_006);
  CU_add_test(str_util_suite, "chxj_url_decode 007",                                  test_chxj_url_decode_007);
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
/* chxj_url_encode()                                                         */
/*===========================================================================*/
void test_chxj_url_encode_001()
{
#define  TEST_STRING   "abcdefghijklmnopqrstuvwxyz"
#define  RESULT_STRING "abcdefghijklmnopqrstuvwxyz"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_002()
{
#define  TEST_STRING   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define  RESULT_STRING "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_003()
{
#define  TEST_STRING   "0123456789"
#define  RESULT_STRING "0123456789"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_004()
{
#define  TEST_STRING   " "
#define  RESULT_STRING "+"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_005()
{
#define  TEST_STRING   "%"
#define  RESULT_STRING "%25"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_006()
{
#define  TEST_STRING   "\x00"
#define  RESULT_STRING ""
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_007()
{
#define  TEST_STRING   "\x01"
#define  RESULT_STRING "%01"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_008()
{
#define  TEST_STRING   "\x02"
#define  RESULT_STRING "%02"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_009()
{
#define  TEST_STRING   "\x03"
#define  RESULT_STRING "%03"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_010()
{
#define  TEST_STRING   "\x04"
#define  RESULT_STRING "%04"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_011()
{
#define  TEST_STRING   "\x05"
#define  RESULT_STRING "%05"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_012()
{
#define  TEST_STRING   "\x06"
#define  RESULT_STRING "%06"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_013()
{
#define  TEST_STRING   "\x07"
#define  RESULT_STRING "%07"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_014()
{
#define  TEST_STRING   "\x08"
#define  RESULT_STRING "%08"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_015()
{
#define  TEST_STRING   "\x09"
#define  RESULT_STRING "%09"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_016()
{
#define  TEST_STRING   "\x0a"
#define  RESULT_STRING "%0A"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_017()
{
#define  TEST_STRING   "\x0b"
#define  RESULT_STRING "%0B"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_018()
{
#define  TEST_STRING   "\x0c"
#define  RESULT_STRING "%0C"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_019()
{
#define  TEST_STRING   "\x0d"
#define  RESULT_STRING "%0D"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_020()
{
#define  TEST_STRING   "\x0e"
#define  RESULT_STRING "%0E"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_021()
{
#define  TEST_STRING   "\x0f"
#define  RESULT_STRING "%0F"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_022()
{
#define  TEST_STRING   "\x10"
#define  RESULT_STRING "%10"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_encode_023()
{
#define  TEST_STRING   "\xff"
#define  RESULT_STRING "%FF"
  char *ret;
  APR_INIT;

  ret = chxj_url_encode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}

/*===========================================================================*/
/* chxj_url_decode()                                                         */
/*===========================================================================*/
#if 0
char *
chxj_url_decode(apr_pool_t *pool, const char *src)
{
  char  *dst;
  int   len;
  int   ii;
  int   jj;


  if (!src) return NULL;

  len = strlen(src);
  dst = apr_palloc(pool, len+1);
  memset(dst, 0, len+1);

  for (jj=0,ii=0; src[ii] != '\0' && ii < len; ii++) {
    if (src[ii] == '%') {
      if (ii + 2 <= len && IS_HEXCHAR(src[ii+1]) && IS_HEXCHAR(src[ii+2])) {
        dst[jj++] = s_hex_value(src[ii+1]) * 16 + s_hex_value(src[ii+2]);
        ii+=2;
      }
    }
    else {
      dst[jj++] = src[ii];
    }
  }

  return dst;
}
#endif
void test_chxj_url_decode_001()
{
#define  TEST_STRING   "%FF"
#define  RESULT_STRING "\xff"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_002()
{
#define  TEST_STRING   "+"
#define  RESULT_STRING " "
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_003()
{
#define  TEST_STRING   "abcdefghijklmnopqrstuvwxyz"
#define  RESULT_STRING "abcdefghijklmnopqrstuvwxyz"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_004()
{
#define  TEST_STRING   "%00"
#define  RESULT_STRING "\0"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_005()
{
#define  TEST_STRING   "%11"
#define  RESULT_STRING "\x11"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_006()
{
#define  TEST_STRING   "%1111"
#define  RESULT_STRING "\x11" "11"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_chxj_url_decode_007()
{
#define  TEST_STRING   "ab%1111"
#define  RESULT_STRING "ab\x11" "11"
  char *ret;
  APR_INIT;

  ret = chxj_url_decode(p, TEST_STRING);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(strcmp(ret, RESULT_STRING) == 0);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*
 * vim:ts=2 et
 */
