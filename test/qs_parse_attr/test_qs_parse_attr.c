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
Attr *
qs_parse_attr(Doc *doc, const char *s, int len, int *pos) 
{
  int   ii;
  int   start_pos;
  int   size;
  int   novalue;
  char  *name;
  char  *value;
  Attr  *attr;
  int   use_quote_sq;
  int   use_quote_dq;
  int   backslash;

  use_quote_sq = 0;
  use_quote_dq = 0;
  backslash = 0;

  QX_LOGGER_DEBUG("start qs_parse_attr()");

  /* ignore space */
  ii = start_pos = qs_ignore_sp_and_quote(doc, s, len);
  QX_LOGGER_DEBUG_INT("len",len);

  /* get attr name */
  for (;ii<len; ii++) {
    if (is_white_space(s[ii])) 
      break;

    if (s[ii] == '=')
      break;
  }

  size = ii - start_pos;
  QX_LOGGER_DEBUG_INT("size", size);

  /* 
   * not found 
   */
  if (size == 0) {
    *pos = ii;
    return NULL;
  }

  name = (char *)apr_palloc(doc->pool,size+1);
  memset(name, 0, size+1);
  memcpy(name, &s[start_pos], size);

  QX_LOGGER_DEBUG((char *)name);

  novalue = 0;
  /* find '=' */
  for (;ii<len; ii++) {
    if (is_white_space(s[ii])) 
      /* ignore */
      continue;
    if (s[ii] == '=') 
      ii++;
    else 
      /* novalue */
      novalue = 1;
    break;
  }

  if (ii == len) 
    novalue = 1;

  size = 0;
  if (!novalue) {

    /* 
     * ignore space
     */
    ii += qs_ignore_sp(doc, &s[ii], len-ii);

    backslash = 0;
    for (;ii<len; ii++) {
      if (s[ii] == '\\') {
        backslash = 1;
        break;
      }
      if (s[ii] == '\'') {
        use_quote_sq = 1;
        ii++;
        break;
      }
      if (s[ii] == '"') {
        use_quote_dq = 1;
        ii++;
        break;
      }
      if (!is_white_space(s[ii]))
        break;
    }
  
    start_pos = ii;
    if (backslash && ii + 2 < len)
      ii+=2;
    
    backslash = 0;
    /* 
     * get attr value 
     */
    for (;ii<len; ii++) {
      if (is_sjis_kanji(s[ii])) {
        ii++;
        continue;
      }

      if (is_sjis_kana(s[ii])) 
        continue;

      if (is_white_space(s[ii])) {
        if (! use_quote_sq && ! use_quote_dq) 
          break;
      }

      if (s[ii] == '\\') {
        ii++;
        continue;
      }

      if (s[ii] == '"' && use_quote_dq)
        break;

      if (s[ii] == '\'' && use_quote_sq) 
        break;
    }
    size = ii - start_pos;

    QX_LOGGER_DEBUG_INT("size",size);
  }

  value = (char *)apr_palloc(doc->pool, size+1);
  memset(value, 0, size+1);
  if (size != 0) 
    memcpy(value, &s[start_pos], size);

  attr = qs_new_attr(doc);

  attr->name  = name;
  attr->value = value;

  QX_LOGGER_DEBUG("end qs_parse_attr()");
  *pos = ii;

  return attr;
}
#endif
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
/*
 * vim:ts=2 et
 */
