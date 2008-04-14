#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#define CHXJ_TEST
#define IMG_NOT_CONVERT_FILENAME

#include "chxj_apache.h"
#include "apr.h"
#include "qs_malloc.c"
#include "qs_log.c"
#include "qs_parse_tag.c"
#include "qs_parse_string.c"
#include "qs_parse_attr.c"
#include "qs_ignore_sp.c"
#include "qs_parse_file.c"
#include "chxj_buffered_write.c"
#include "chxj_chtml10.c"
#include "chxj_chtml20.c"
#include "chxj_chtml30.c"
#include "chxj_jhtml.c"
#include "chxj_hdml.c"
#include "chxj_xhtml_mobile_1_0.c"
#include "chxj_node_exchange.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_cookie.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_dbm.c"
#include "chxj_str_util.c"
#include <iconv.h>



void test_xhtml_001();
void test_xhtml_002();
void test_xhtml_comment_tag_001();

void test_xhtml_a_tag_name_attribute_001();
void test_xhtml_a_tag_name_attribute_002();
void test_xhtml_a_tag_name_attribute_003();
void test_xhtml_a_tag_name_attribute_004();
void test_xhtml_a_tag_href_attribute_001();
void test_xhtml_a_tag_href_attribute_002();
void test_xhtml_a_tag_href_attribute_003();
void test_xhtml_a_tag_href_attribute_004();
void test_xhtml_a_tag_href_attribute_005();
void test_xhtml_a_tag_href_attribute_006();
void test_xhtml_a_tag_accesskey_attribute_001();
void test_xhtml_a_tag_accesskey_attribute_002();
void test_xhtml_a_tag_accesskey_attribute_003();

void test_xhtml_base_tag_001();
void test_xhtml_base_tag_href_attribute_001();
void test_xhtml_base_tag_href_attribute_002();
void test_xhtml_base_tag_href_attribute_003();
void test_xhtml_base_tag_href_attribute_004();

void test_xhtml_blockquote_tag_001();
void test_xhtml_blockquote_tag_002();
void test_xhtml_blockquote_tag_003();
void test_xhtml_blockquote_tag_004();

void test_xhtml_body_tag_001();
void test_xhtml_body_tag_002();
void test_xhtml_body_tag_003();
void test_xhtml_body_tag_004();
void test_xhtml_body_tag_005();
void test_xhtml_body_tag_006();
void test_xhtml_body_tag_007();
void test_xhtml_body_tag_008();
void test_xhtml_body_tag_009();
void test_xhtml_body_tag_010();
void test_xhtml_body_tag_011();
void test_xhtml_body_tag_012();
void test_xhtml_body_tag_013();
void test_xhtml_body_tag_014();
void test_xhtml_body_tag_015();

void test_xhtml_br_tag_001();
void test_xhtml_br_tag_002();
void test_xhtml_br_tag_003();
void test_xhtml_br_tag_004();
void test_xhtml_br_tag_005();
void test_xhtml_br_tag_006();
void test_xhtml_br_tag_007();

void test_xhtml_center_tag_001();

void test_xhtml_dir_tag_001();
void test_xhtml_dir_tag_002();

void test_xhtml_dl_tag_001();

void test_xhtml_dt_tag_001();
void test_xhtml_dt_tag_002();

void test_xhtml_dd_tag_001();
void test_xhtml_dd_tag_002();

void test_xhtml_div_tag_001();
void test_xhtml_div_tag_002();
void test_xhtml_div_tag_003();
void test_xhtml_div_tag_004();
void test_xhtml_div_tag_005();
void test_xhtml_div_tag_006();
void test_xhtml_div_tag_007();

void test_xhtml_form_tag_001();
void test_xhtml_form_tag_002();
void test_xhtml_form_tag_003();
void test_xhtml_form_tag_004();
void test_xhtml_form_tag_005();
void test_xhtml_form_tag_006();
void test_xhtml_form_tag_007();
void test_xhtml_form_tag_008();
void test_xhtml_form_tag_009();

void test_xhtml_head_tag_001();
void test_xhtml_head_tag_002();

void test_xhtml_h1_tag_001(); 
void test_xhtml_h1_tag_002(); 
void test_xhtml_h1_tag_003(); 
void test_xhtml_h1_tag_004(); 
void test_xhtml_h1_tag_005(); 
void test_xhtml_h1_tag_006(); 
void test_xhtml_h1_tag_007(); 
void test_xhtml_h1_tag_008(); 
void test_xhtml_h1_tag_009(); 
void test_xhtml_h1_tag_010(); 

void test_xhtml_h2_tag_001(); 
void test_xhtml_h2_tag_002(); 
void test_xhtml_h2_tag_003(); 
void test_xhtml_h2_tag_004(); 
void test_xhtml_h2_tag_005(); 
void test_xhtml_h2_tag_006(); 
void test_xhtml_h2_tag_007(); 
void test_xhtml_h2_tag_008(); 
void test_xhtml_h2_tag_009(); 
void test_xhtml_h2_tag_010(); 

void test_xhtml_h3_tag_001(); 
void test_xhtml_h3_tag_002(); 
void test_xhtml_h3_tag_003(); 
void test_xhtml_h3_tag_004(); 
void test_xhtml_h3_tag_005(); 
void test_xhtml_h3_tag_006(); 
void test_xhtml_h3_tag_007(); 
void test_xhtml_h3_tag_008(); 
void test_xhtml_h3_tag_009(); 
void test_xhtml_h3_tag_010(); 

void test_xhtml_h4_tag_001(); 
void test_xhtml_h4_tag_002(); 
void test_xhtml_h4_tag_003(); 
void test_xhtml_h4_tag_004(); 
void test_xhtml_h4_tag_005(); 
void test_xhtml_h4_tag_006(); 
void test_xhtml_h4_tag_007(); 
void test_xhtml_h4_tag_008(); 
void test_xhtml_h4_tag_009(); 
void test_xhtml_h4_tag_010(); 

void test_xhtml_h5_tag_001(); 
void test_xhtml_h5_tag_002(); 
void test_xhtml_h5_tag_003(); 
void test_xhtml_h5_tag_004(); 
void test_xhtml_h5_tag_005(); 
void test_xhtml_h5_tag_006(); 
void test_xhtml_h5_tag_007(); 
void test_xhtml_h5_tag_008(); 
void test_xhtml_h5_tag_009(); 
void test_xhtml_h5_tag_010(); 

void test_xhtml_h6_tag_001(); 
void test_xhtml_h6_tag_002(); 
void test_xhtml_h6_tag_003(); 
void test_xhtml_h6_tag_004(); 
void test_xhtml_h6_tag_005(); 
void test_xhtml_h6_tag_006(); 
void test_xhtml_h6_tag_007(); 
void test_xhtml_h6_tag_008(); 
void test_xhtml_h6_tag_009(); 
void test_xhtml_h6_tag_010(); 

void test_xhtml_hr_tag_001();
void test_xhtml_hr_tag_002();
void test_xhtml_hr_tag_003();
void test_xhtml_hr_tag_004();
void test_xhtml_hr_tag_005();
void test_xhtml_hr_tag_006();
void test_xhtml_hr_tag_007();
void test_xhtml_hr_tag_008();
void test_xhtml_hr_tag_009();
void test_xhtml_hr_tag_010();
void test_xhtml_hr_tag_011();
void test_xhtml_hr_tag_012();
void test_xhtml_hr_tag_013();
void test_xhtml_hr_tag_014();
void test_xhtml_hr_tag_015();
void test_xhtml_hr_tag_016();
void test_xhtml_hr_tag_017();
void test_xhtml_hr_tag_018();

void test_xhtml_html_tag_001();

void test_xhtml_img_tag_001();
void test_xhtml_img_tag_002();
void test_xhtml_img_tag_003();
void test_xhtml_img_tag_004();
void test_xhtml_img_tag_005();
void test_xhtml_img_tag_006();
void test_xhtml_img_tag_007();
void test_xhtml_img_tag_008();
void test_xhtml_img_tag_009();
void test_xhtml_img_tag_010();
void test_xhtml_img_tag_011();
void test_xhtml_img_tag_012();
void test_xhtml_img_tag_013();
void test_xhtml_img_tag_014();
void test_xhtml_img_tag_015();
void test_xhtml_img_tag_016();
void test_xhtml_img_tag_017();
void test_xhtml_img_tag_018();
void test_xhtml_img_tag_019();
void test_xhtml_img_tag_020();
void test_xhtml_img_tag_021();
void test_xhtml_img_tag_022();
void test_xhtml_img_tag_023();
void test_xhtml_img_tag_024();
void test_xhtml_img_tag_025();
void test_xhtml_img_tag_026();
void test_xhtml_img_tag_027();
void test_xhtml_img_tag_028();
void test_xhtml_img_tag_029();
void test_xhtml_img_tag_030();
void test_xhtml_img_tag_031();
void test_xhtml_img_tag_032();
void test_xhtml_img_tag_033();
void test_xhtml_img_tag_034();
void test_xhtml_img_tag_035();
void test_xhtml_img_tag_036();
void test_xhtml_img_tag_037();

void test_xhtml_input_tag_001();
void test_xhtml_input_tag_002();
void test_xhtml_input_tag_003();
void test_xhtml_input_tag_004();
void test_xhtml_input_tag_005();
void test_xhtml_input_tag_006();
void test_xhtml_input_tag_007();
void test_xhtml_input_tag_008();
void test_xhtml_input_tag_009();
void test_xhtml_input_tag_010();
void test_xhtml_input_tag_011();
void test_xhtml_input_tag_012();
void test_xhtml_input_tag_013();
void test_xhtml_input_tag_014();
void test_xhtml_input_tag_015();
void test_xhtml_input_tag_016();
void test_xhtml_input_tag_017();
void test_xhtml_input_tag_018();
void test_xhtml_input_tag_019();
void test_xhtml_input_tag_020();
void test_xhtml_input_tag_021();
void test_xhtml_input_tag_022();
void test_xhtml_input_tag_023();
void test_xhtml_input_tag_024();
void test_xhtml_input_tag_025();
void test_xhtml_input_tag_026();
void test_xhtml_input_tag_027();
void test_xhtml_input_tag_028();
void test_xhtml_input_tag_029();
void test_xhtml_input_tag_030();
void test_xhtml_input_tag_031();
void test_xhtml_input_tag_032();
void test_xhtml_input_tag_033();
void test_xhtml_input_tag_034();
void test_xhtml_input_tag_035();
void test_xhtml_input_tag_036();
void test_xhtml_input_tag_037();
void test_xhtml_input_tag_038();
void test_xhtml_input_tag_039();
void test_xhtml_input_tag_040();
void test_xhtml_input_tag_041();
void test_xhtml_input_tag_042();
void test_xhtml_input_tag_043();
void test_xhtml_input_tag_044();
void test_xhtml_input_tag_045();
void test_xhtml_input_tag_046();
void test_xhtml_input_tag_047();

void test_xhtml_li_tag_001();
void test_xhtml_li_tag_002();
void test_xhtml_li_tag_003();
void test_xhtml_li_tag_004();
void test_xhtml_li_tag_005();
void test_xhtml_li_tag_006();
void test_xhtml_li_tag_007();
void test_xhtml_li_tag_008();
void test_xhtml_li_tag_009();
void test_xhtml_li_tag_010();
void test_xhtml_li_tag_011();
void test_xhtml_li_tag_012();
void test_xhtml_li_tag_013();

void test_xhtml_menu_tag_001();
void test_xhtml_menu_tag_002();
void test_xhtml_menu_tag_003();
void test_xhtml_menu_tag_004();
void test_xhtml_menu_tag_005();

void test_xhtml_ol_tag_001();
void test_xhtml_ol_tag_002();
void test_xhtml_ol_tag_003();
void test_xhtml_ol_tag_004();
void test_xhtml_ol_tag_005();
void test_xhtml_ol_tag_006();
void test_xhtml_ol_tag_007();
void test_xhtml_ol_tag_008();
void test_xhtml_ol_tag_009();
void test_xhtml_ol_tag_010();
void test_xhtml_ol_tag_011();
void test_xhtml_ol_tag_012();
void test_xhtml_ol_tag_013();
void test_xhtml_ol_tag_014();
void test_xhtml_ol_tag_015();
void test_xhtml_ol_tag_016();
void test_xhtml_ol_tag_017();
void test_xhtml_ol_tag_018();

void test_xhtml_option_tag_001();
void test_xhtml_option_tag_002();
void test_xhtml_option_tag_003();
void test_xhtml_option_tag_004();
void test_xhtml_option_tag_005();
void test_xhtml_option_tag_006();
void test_xhtml_option_tag_007();

void test_xhtml_p_tag_001();
void test_xhtml_p_tag_002();
void test_xhtml_p_tag_003();
void test_xhtml_p_tag_004();
void test_xhtml_p_tag_005();
void test_xhtml_p_tag_006();
void test_xhtml_p_tag_007();
void test_xhtml_p_tag_008();

void test_xhtml_plaintext_tag_001();
void test_xhtml_plaintext_tag_002();
void test_xhtml_plaintext_tag_003();
void test_xhtml_plaintext_tag_004();

void test_xhtml_pre_tag_001();
void test_xhtml_pre_tag_002();
void test_xhtml_pre_tag_003();
void test_xhtml_pre_tag_004();
void test_xhtml_pre_tag_005();

void test_xhtml_select_tag_001();
void test_xhtml_select_tag_002();
void test_xhtml_select_tag_003();
void test_xhtml_select_tag_004();
void test_xhtml_select_tag_005();
void test_xhtml_select_tag_006();
void test_xhtml_select_tag_007();
void test_xhtml_select_tag_008();
void test_xhtml_select_tag_009();
void test_xhtml_select_tag_010();
void test_xhtml_select_tag_011();
void test_xhtml_select_tag_012();
void test_xhtml_select_tag_013();
void test_xhtml_select_tag_014();

void test_xhtml_textarea_tag_001();
void test_xhtml_textarea_tag_002();
void test_xhtml_textarea_tag_003();
void test_xhtml_textarea_tag_004();
void test_xhtml_textarea_tag_005();
void test_xhtml_textarea_tag_006();
void test_xhtml_textarea_tag_007();
void test_xhtml_textarea_tag_008();
void test_xhtml_textarea_tag_009();
void test_xhtml_textarea_tag_010();
void test_xhtml_textarea_tag_011();
void test_xhtml_textarea_tag_012();
void test_xhtml_textarea_tag_013();
void test_xhtml_textarea_tag_014();
void test_xhtml_textarea_tag_015();
void test_xhtml_textarea_tag_016();
void test_xhtml_textarea_tag_017();
void test_xhtml_textarea_tag_018();
void test_xhtml_textarea_tag_019();
void test_xhtml_textarea_tag_020();
void test_xhtml_textarea_tag_021();
void test_xhtml_textarea_tag_022();
void test_xhtml_textarea_tag_023();
void test_xhtml_textarea_tag_024();
void test_xhtml_textarea_tag_025();
void test_xhtml_textarea_tag_026();

void test_xhtml_title_tag_001();
void test_xhtml_title_tag_002();

void test_xhtml_ul_tag_001();
void test_xhtml_ul_tag_002();
void test_xhtml_ul_tag_003();
void test_xhtml_ul_tag_004();
void test_xhtml_ul_tag_005();

void test_xhtml_blink_tag_001();
void test_xhtml_blink_tag_002();
void test_xhtml_blink_tag_003();
void test_xhtml_blink_tag_004();

void test_xhtml_marquee_tag_001();
void test_xhtml_marquee_tag_002();
void test_xhtml_marquee_tag_003();
void test_xhtml_marquee_tag_004();
void test_xhtml_marquee_tag_005();
void test_xhtml_marquee_tag_006();
void test_xhtml_marquee_tag_007();
void test_xhtml_marquee_tag_008();
void test_xhtml_marquee_tag_009();
void test_xhtml_marquee_tag_010();
void test_xhtml_marquee_tag_011();
void test_xhtml_marquee_tag_012();
void test_xhtml_marquee_tag_013();
void test_xhtml_marquee_tag_014();
void test_xhtml_marquee_tag_015();
void test_xhtml_marquee_tag_016();
void test_xhtml_marquee_tag_017();
void test_xhtml_marquee_tag_018();

void test_xhtml_meta_tag_001();
void test_xhtml_meta_tag_002();
void test_xhtml_meta_tag_003();
void test_xhtml_meta_tag_004();
void test_xhtml_meta_tag_005();
void test_xhtml_meta_tag_006();
void test_xhtml_meta_tag_007();
void test_xhtml_meta_tag_008();
void test_xhtml_meta_tag_009();
/* pend */

int
main()
{
  CU_pSuite xhtml_suite;
  CU_initialize_registry();
  xhtml_suite = CU_add_suite("test chxj_exchange_xhtml_mobile_1_0()", NULL, NULL);

  CU_add_test(xhtml_suite, "test void src1",                                    test_xhtml_001);
  CU_add_test(xhtml_suite, "test void src2",                                    test_xhtml_002);
  CU_add_test(xhtml_suite, "test comment tag1",                                 test_xhtml_comment_tag_001);

  CU_add_test(xhtml_suite, "test a tag name attr1",                             test_xhtml_a_tag_name_attribute_001);
  CU_add_test(xhtml_suite, "test a tag name attr2",                             test_xhtml_a_tag_name_attribute_002);
  CU_add_test(xhtml_suite, "test a tag name attr3 with japanese.",              test_xhtml_a_tag_name_attribute_003);
  CU_add_test(xhtml_suite, "test a tag name attr4 with japanese.",              test_xhtml_a_tag_name_attribute_004);
  CU_add_test(xhtml_suite, "test a tag href attr1 with void attribute.",        test_xhtml_a_tag_href_attribute_001);
  CU_add_test(xhtml_suite, "test a tag href attr2 with other site link.",       test_xhtml_a_tag_href_attribute_002);
  CU_add_test(xhtml_suite, "test a tag href attr3 with local link.",            test_xhtml_a_tag_href_attribute_003);
  CU_add_test(xhtml_suite, "test a tag href attr4 with maker.",                 test_xhtml_a_tag_href_attribute_004);
  CU_add_test(xhtml_suite, "test a tag href attr5 with void maker.",            test_xhtml_a_tag_href_attribute_005);
  CU_add_test(xhtml_suite, "test a tag href attr6 with no cookie.",             test_xhtml_a_tag_href_attribute_006);
  CU_add_test(xhtml_suite, "test a tag accesskey attribute.",                   test_xhtml_a_tag_accesskey_attribute_001);
  CU_add_test(xhtml_suite, "test a tag accesskey attribute with void char.",    test_xhtml_a_tag_accesskey_attribute_002);
  CU_add_test(xhtml_suite, "test a tag accesskey attribute with no value",      test_xhtml_a_tag_accesskey_attribute_003);

  CU_add_test(xhtml_suite, "test base tag no attribute.",                       test_xhtml_base_tag_001);
  CU_add_test(xhtml_suite, "test base tag href attribute with no value.",       test_xhtml_base_tag_href_attribute_001);
  CU_add_test(xhtml_suite, "test base tag href attribute with void value.",     test_xhtml_base_tag_href_attribute_002);
  CU_add_test(xhtml_suite, "test base tag href attribute with normal value.",   test_xhtml_base_tag_href_attribute_003);
  CU_add_test(xhtml_suite, "test base tag href attribute with normal value.",   test_xhtml_base_tag_href_attribute_004);

  CU_add_test(xhtml_suite, "test <blockquote> with void value.",                test_xhtml_blockquote_tag_001);
  CU_add_test(xhtml_suite, "test <blockquote> with value.",                     test_xhtml_blockquote_tag_002);
  CU_add_test(xhtml_suite, "test <blockquote> with japanese value.",            test_xhtml_blockquote_tag_003);
  CU_add_test(xhtml_suite, "test <blockquote> with hankaku kana value.",        test_xhtml_blockquote_tag_004);

  CU_add_test(xhtml_suite, "test <body> .",                                     test_xhtml_body_tag_001);
  CU_add_test(xhtml_suite, "test <body> with bgcolor attribute 1.",             test_xhtml_body_tag_002);
  CU_add_test(xhtml_suite, "test <body> with bgcolor attribute 2.",             test_xhtml_body_tag_003);
  CU_add_test(xhtml_suite, "test <body> with bgcolor attribute 3.",             test_xhtml_body_tag_004);
  CU_add_test(xhtml_suite, "test <body> with text attribute 1.",                test_xhtml_body_tag_005);
  CU_add_test(xhtml_suite, "test <body> with text attribute 2.",                test_xhtml_body_tag_006);
  CU_add_test(xhtml_suite, "test <body> with text attribute 3.",                test_xhtml_body_tag_007);
  CU_add_test(xhtml_suite, "test <body> with link attribute 1.",                test_xhtml_body_tag_008);
  CU_add_test(xhtml_suite, "test <body> with link attribute 2.",                test_xhtml_body_tag_009);
  CU_add_test(xhtml_suite, "test <body> with link attribute 3.",                test_xhtml_body_tag_010);
  CU_add_test(xhtml_suite, "test <body> with vlink attribute 1.",               test_xhtml_body_tag_011);
  CU_add_test(xhtml_suite, "test <body> with vlink attribute 2.",               test_xhtml_body_tag_012);
  CU_add_test(xhtml_suite, "test <body> with vlink attribute 3.",               test_xhtml_body_tag_013);
  CU_add_test(xhtml_suite, "test <body> with alink attribute.",                 test_xhtml_body_tag_014);
  CU_add_test(xhtml_suite, "test <body> with unknown attribute.",               test_xhtml_body_tag_015);

  CU_add_test(xhtml_suite, "test <br>.",                                        test_xhtml_br_tag_001);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(left).",             test_xhtml_br_tag_002);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(right).",            test_xhtml_br_tag_003);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(all).",              test_xhtml_br_tag_004);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(void).",             test_xhtml_br_tag_005);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(no value).",         test_xhtml_br_tag_006);
  CU_add_test(xhtml_suite, "test <br> with clear attribute(unknown value).",    test_xhtml_br_tag_007);

  CU_add_test(xhtml_suite, "test <center>.",                                    test_xhtml_center_tag_001);

  CU_add_test(xhtml_suite, "test <dir>.",                                       test_xhtml_dir_tag_001);
  CU_add_test(xhtml_suite, "test <dir> with no <li>.",                          test_xhtml_dir_tag_002);

  CU_add_test(xhtml_suite, "test <dl>.",                                        test_xhtml_dl_tag_001);

  CU_add_test(xhtml_suite, "test <dt>.",                                        test_xhtml_dt_tag_001);
  CU_add_test(xhtml_suite, "test <dt> with void value.",                        test_xhtml_dt_tag_002);

  CU_add_test(xhtml_suite, "test <dd>.",                                        test_xhtml_dd_tag_001);
  CU_add_test(xhtml_suite, "test <dd> with void value.",                        test_xhtml_dd_tag_002);

  CU_add_test(xhtml_suite, "test <div>.",                                       test_xhtml_div_tag_001);
  CU_add_test(xhtml_suite, "test <div> with align attribute(left).",            test_xhtml_div_tag_002);
  CU_add_test(xhtml_suite, "test <div> with align attribute(right).",           test_xhtml_div_tag_003);
  CU_add_test(xhtml_suite, "test <div> with align attribute(center).",          test_xhtml_div_tag_004);
  CU_add_test(xhtml_suite, "test <div> with align attribute(void).",            test_xhtml_div_tag_005);
  CU_add_test(xhtml_suite, "test <div> with align attribute(unknown).",         test_xhtml_div_tag_006);
  CU_add_test(xhtml_suite, "test <div> with style attribute.",                  test_xhtml_div_tag_007);

  CU_add_test(xhtml_suite, "test <form>.",                                      test_xhtml_form_tag_001);
  CU_add_test(xhtml_suite, "test <form method>.",                               test_xhtml_form_tag_002);
  CU_add_test(xhtml_suite, "test <form method=\"post\">.",                      test_xhtml_form_tag_003);
  CU_add_test(xhtml_suite, "test <form method=\"get\">.",                       test_xhtml_form_tag_004);
  CU_add_test(xhtml_suite, "test <form method=\"abc\">.",                       test_xhtml_form_tag_005);
  CU_add_test(xhtml_suite, "test <form action>.",                               test_xhtml_form_tag_006);
  CU_add_test(xhtml_suite, "test <form action> with null cookie.",              test_xhtml_form_tag_007);
  CU_add_test(xhtml_suite, "test <form action> with other site .",              test_xhtml_form_tag_008);
  CU_add_test(xhtml_suite, "test <form action method>.",                        test_xhtml_form_tag_009);

  CU_add_test(xhtml_suite, "test <head>.",                                      test_xhtml_head_tag_001);
  CU_add_test(xhtml_suite, "test <head> with value.",                           test_xhtml_head_tag_002);

  CU_add_test(xhtml_suite, "test <h1>.",                                        test_xhtml_h1_tag_001);
  CU_add_test(xhtml_suite, "test <h1> with value.",                             test_xhtml_h1_tag_002);
  CU_add_test(xhtml_suite, "test <h1> with value(japanese).",                   test_xhtml_h1_tag_003);
  CU_add_test(xhtml_suite, "test <h1> with value(japanese-hankaku).",           test_xhtml_h1_tag_004);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(non).",              test_xhtml_h1_tag_005);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(void).",             test_xhtml_h1_tag_006);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(left).",             test_xhtml_h1_tag_007);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(right).",            test_xhtml_h1_tag_008);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(center).",           test_xhtml_h1_tag_009);
  CU_add_test(xhtml_suite, "test <h1> with align attribute(unkown).",           test_xhtml_h1_tag_010);

  /*=========================================================================*/
  /* <h2>                                                                    */
  /*=========================================================================*/
  CU_add_test(xhtml_suite, "test <h2>.",                                        test_xhtml_h2_tag_001);
  CU_add_test(xhtml_suite, "test <h2> with value.",                             test_xhtml_h2_tag_002);
  CU_add_test(xhtml_suite, "test <h2> with value(japanese).",                   test_xhtml_h2_tag_003);
  CU_add_test(xhtml_suite, "test <h2> with value(japanese-hankaku).",           test_xhtml_h2_tag_004);
  CU_add_test(xhtml_suite, "test <h2> with align attribute(non).",              test_xhtml_h2_tag_005);
#if 0
  CU_add_test(xhtml_suite, "test <h2> with align attribute(void).",             test_xhtml_h2_tag_006);
  CU_add_test(xhtml_suite, "test <h2> with align attribute(left).",             test_xhtml_h2_tag_007);
  CU_add_test(xhtml_suite, "test <h2> with align attribute(right).",            test_xhtml_h2_tag_008);
  CU_add_test(xhtml_suite, "test <h2> with align attribute(center).",           test_xhtml_h2_tag_009);
  CU_add_test(xhtml_suite, "test <h2> with align attribute(unkown).",           test_xhtml_h2_tag_010);
  CU_add_test(xhtml_suite, "test <h3>.",                                        test_xhtml_h3_tag_001);
  CU_add_test(xhtml_suite, "test <h3> with value.",                             test_xhtml_h3_tag_002);
  CU_add_test(xhtml_suite, "test <h3> with value(japanese).",                   test_xhtml_h3_tag_003);
  CU_add_test(xhtml_suite, "test <h3> with value(japanese-hankaku).",           test_xhtml_h3_tag_004);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(non).",              test_xhtml_h3_tag_005);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(void).",             test_xhtml_h3_tag_006);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(left).",             test_xhtml_h3_tag_007);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(right).",            test_xhtml_h3_tag_008);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(center).",           test_xhtml_h3_tag_009);
  CU_add_test(xhtml_suite, "test <h3> with align attribute(unkown).",           test_xhtml_h3_tag_010);
  CU_add_test(xhtml_suite, "test <h4>.",                                        test_xhtml_h4_tag_001);
  CU_add_test(xhtml_suite, "test <h4> with value.",                             test_xhtml_h4_tag_002);
  CU_add_test(xhtml_suite, "test <h4> with value(japanese).",                   test_xhtml_h4_tag_003);
  CU_add_test(xhtml_suite, "test <h4> with value(japanese-hankaku).",           test_xhtml_h4_tag_004);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(non).",              test_xhtml_h4_tag_005);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(void).",             test_xhtml_h4_tag_006);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(left).",             test_xhtml_h4_tag_007);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(right).",            test_xhtml_h4_tag_008);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(center).",           test_xhtml_h4_tag_009);
  CU_add_test(xhtml_suite, "test <h4> with align attribute(unkown).",           test_xhtml_h4_tag_010);
  CU_add_test(xhtml_suite, "test <h5>.",                                        test_xhtml_h5_tag_001);
  CU_add_test(xhtml_suite, "test <h5> with value.",                             test_xhtml_h5_tag_002);
  CU_add_test(xhtml_suite, "test <h5> with value(japanese).",                   test_xhtml_h5_tag_003);
  CU_add_test(xhtml_suite, "test <h5> with value(japanese-hankaku).",           test_xhtml_h5_tag_004);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(non).",              test_xhtml_h5_tag_005);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(void).",             test_xhtml_h5_tag_006);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(left).",             test_xhtml_h5_tag_007);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(right).",            test_xhtml_h5_tag_008);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(center).",           test_xhtml_h5_tag_009);
  CU_add_test(xhtml_suite, "test <h5> with align attribute(unkown).",           test_xhtml_h5_tag_010);
  CU_add_test(xhtml_suite, "test <h6>.",                                        test_xhtml_h6_tag_001);
  CU_add_test(xhtml_suite, "test <h6> with value.",                             test_xhtml_h6_tag_002);
  CU_add_test(xhtml_suite, "test <h6> with value(japanese).",                   test_xhtml_h6_tag_003);
  CU_add_test(xhtml_suite, "test <h6> with value(japanese-hankaku).",           test_xhtml_h6_tag_004);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(non).",              test_xhtml_h6_tag_005);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(void).",             test_xhtml_h6_tag_006);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(left).",             test_xhtml_h6_tag_007);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(right).",            test_xhtml_h6_tag_008);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(center).",           test_xhtml_h6_tag_009);
  CU_add_test(xhtml_suite, "test <h6> with align attribute(unkown).",           test_xhtml_h6_tag_010);

  CU_add_test(xhtml_suite, "test <hr>.",                                        test_xhtml_hr_tag_001);
  CU_add_test(xhtml_suite, "test <hr />.",                                      test_xhtml_hr_tag_002);
  CU_add_test(xhtml_suite, "test <hr align> with no value.",                    test_xhtml_hr_tag_003);
  CU_add_test(xhtml_suite, "test <hr align> with void value.",                  test_xhtml_hr_tag_004);
  CU_add_test(xhtml_suite, "test <hr align> with value(left).",                 test_xhtml_hr_tag_005);
  CU_add_test(xhtml_suite, "test <hr align> with value(right).",                test_xhtml_hr_tag_006);
  CU_add_test(xhtml_suite, "test <hr align> with value(center).",               test_xhtml_hr_tag_007);
  CU_add_test(xhtml_suite, "test <hr align> with value(unknown).",              test_xhtml_hr_tag_008);
  CU_add_test(xhtml_suite, "test <hr size> with no value.",                     test_xhtml_hr_tag_009);
  CU_add_test(xhtml_suite, "test <hr size> with void value.",                   test_xhtml_hr_tag_010);
  CU_add_test(xhtml_suite, "test <hr size> with non numeric value.",            test_xhtml_hr_tag_011);
  CU_add_test(xhtml_suite, "test <hr size> with numeric value.",                test_xhtml_hr_tag_012);
  CU_add_test(xhtml_suite, "test <hr width> with no value.",                    test_xhtml_hr_tag_013);
  CU_add_test(xhtml_suite, "test <hr width> with void value.",                  test_xhtml_hr_tag_014);
  CU_add_test(xhtml_suite, "test <hr width> with non numeric value.",           test_xhtml_hr_tag_015);
  CU_add_test(xhtml_suite, "test <hr width> with numeric value.",               test_xhtml_hr_tag_016);
  CU_add_test(xhtml_suite, "test <hr noshade>.",                                test_xhtml_hr_tag_017);
  CU_add_test(xhtml_suite, "test <hr color>.",                                  test_xhtml_hr_tag_018);

  CU_add_test(xhtml_suite, "test <html>.",                                      test_xhtml_html_tag_001);

  CU_add_test(xhtml_suite, "test <img>." ,                                      test_xhtml_img_tag_001);
  CU_add_test(xhtml_suite, "test <img src> with no value." ,                    test_xhtml_img_tag_002);
  CU_add_test(xhtml_suite, "test <img src> with void value." ,                  test_xhtml_img_tag_003);
  CU_add_test(xhtml_suite, "test <img src> with alphabetic value." ,            test_xhtml_img_tag_004);
  CU_add_test(xhtml_suite, "test <img src> with japanese value." ,              test_xhtml_img_tag_005);
  CU_add_test(xhtml_suite, "test <img src> with japanese-hankaku value." ,      test_xhtml_img_tag_006);
  CU_add_test(xhtml_suite, "test <img align> with no value." ,                  test_xhtml_img_tag_007);
  CU_add_test(xhtml_suite, "test <img align> with void value." ,                test_xhtml_img_tag_008);
  CU_add_test(xhtml_suite, "test <img align> with value(top)." ,                test_xhtml_img_tag_009);
  CU_add_test(xhtml_suite, "test <img align> with value(middle)." ,             test_xhtml_img_tag_020);
  CU_add_test(xhtml_suite, "test <img align> with value(bottom)." ,             test_xhtml_img_tag_011);
  CU_add_test(xhtml_suite, "test <img align> with value(left)." ,               test_xhtml_img_tag_012);
  CU_add_test(xhtml_suite, "test <img align> with value(right)." ,              test_xhtml_img_tag_013);
  CU_add_test(xhtml_suite, "test <img align> with value(unkown)." ,             test_xhtml_img_tag_014);
  CU_add_test(xhtml_suite, "test <img width> with no value." ,                  test_xhtml_img_tag_015);
  CU_add_test(xhtml_suite, "test <img width> with void value." ,                test_xhtml_img_tag_016);
  CU_add_test(xhtml_suite, "test <img width> with alphabetic value." ,          test_xhtml_img_tag_017);
  CU_add_test(xhtml_suite, "test <img width> with numeric value." ,             test_xhtml_img_tag_018);
  CU_add_test(xhtml_suite, "test <img width> with percentage value." ,          test_xhtml_img_tag_019);
  CU_add_test(xhtml_suite, "test <img height> with no value." ,                 test_xhtml_img_tag_020);
  CU_add_test(xhtml_suite, "test <img height> with void value." ,               test_xhtml_img_tag_021);
  CU_add_test(xhtml_suite, "test <img height> with alphabetic value." ,         test_xhtml_img_tag_022);
  CU_add_test(xhtml_suite, "test <img height> with numeric value." ,            test_xhtml_img_tag_023);
  CU_add_test(xhtml_suite, "test <img height> with percentage value." ,         test_xhtml_img_tag_024);
  CU_add_test(xhtml_suite, "test <img hspace> with no value." ,                 test_xhtml_img_tag_025);
  CU_add_test(xhtml_suite, "test <img hspace> with void value." ,               test_xhtml_img_tag_026);
  CU_add_test(xhtml_suite, "test <img hspace> with alphabetic value." ,         test_xhtml_img_tag_027);
  CU_add_test(xhtml_suite, "test <img hspace> with numeric value." ,            test_xhtml_img_tag_028);
  CU_add_test(xhtml_suite, "test <img vspace> with no value." ,                 test_xhtml_img_tag_029);
  CU_add_test(xhtml_suite, "test <img vspace> with void value." ,               test_xhtml_img_tag_030);
  CU_add_test(xhtml_suite, "test <img vspace> with alphabetic value." ,         test_xhtml_img_tag_031);
  CU_add_test(xhtml_suite, "test <img vspace> with numeric value." ,            test_xhtml_img_tag_032);
  CU_add_test(xhtml_suite, "test <img alt>." ,                                  test_xhtml_img_tag_033);
  CU_add_test(xhtml_suite, "test <img alt> with void value." ,                  test_xhtml_img_tag_034);
  CU_add_test(xhtml_suite, "test <img alt> with alphabetic value." ,            test_xhtml_img_tag_035);
  CU_add_test(xhtml_suite, "test <img alt> with japanese value." ,              test_xhtml_img_tag_036);
  CU_add_test(xhtml_suite, "test <img alt> with japanese-hankaku value." ,      test_xhtml_img_tag_037);

  CU_add_test(xhtml_suite, "test <input>." ,                                    test_xhtml_input_tag_001);
  CU_add_test(xhtml_suite, "test <input type>." ,                               test_xhtml_input_tag_002);
  CU_add_test(xhtml_suite, "test <input type> with void value." ,               test_xhtml_input_tag_003);
  CU_add_test(xhtml_suite, "test <input type> with value(text)." ,              test_xhtml_input_tag_004);
  CU_add_test(xhtml_suite, "test <input type> with value(password)." ,          test_xhtml_input_tag_005);
  CU_add_test(xhtml_suite, "test <input type> with value(checkbox)." ,          test_xhtml_input_tag_006);
  CU_add_test(xhtml_suite, "test <input type> with value(radio)." ,             test_xhtml_input_tag_007);
  CU_add_test(xhtml_suite, "test <input type> with value(hidden)." ,            test_xhtml_input_tag_008);
  CU_add_test(xhtml_suite, "test <input type> with value(submit)." ,            test_xhtml_input_tag_009);
  CU_add_test(xhtml_suite, "test <input type> with value(reset)." ,             test_xhtml_input_tag_010);
  CU_add_test(xhtml_suite, "test <input type> with value(unknown)." ,           test_xhtml_input_tag_011);
  CU_add_test(xhtml_suite, "test <input name>." ,                               test_xhtml_input_tag_012);
  CU_add_test(xhtml_suite, "test <input name> with void value." ,               test_xhtml_input_tag_013);
  CU_add_test(xhtml_suite, "test <input name> with alphabetic value." ,         test_xhtml_input_tag_014);
  CU_add_test(xhtml_suite, "test <input name> with japanese value." ,           test_xhtml_input_tag_015);
  CU_add_test(xhtml_suite, "test <input name> with japanese-hankaku value." ,   test_xhtml_input_tag_016);
  CU_add_test(xhtml_suite, "test <input value>." ,                              test_xhtml_input_tag_017);
  CU_add_test(xhtml_suite, "test <input value> with void value." ,              test_xhtml_input_tag_018);
  CU_add_test(xhtml_suite, "test <input value> with alphabetic value." ,        test_xhtml_input_tag_019);
  CU_add_test(xhtml_suite, "test <input value> with japanese value." ,          test_xhtml_input_tag_020);
  CU_add_test(xhtml_suite, "test <input value> with japanese-hankaku value." ,  test_xhtml_input_tag_021);
  CU_add_test(xhtml_suite, "test <input size>." ,                               test_xhtml_input_tag_022);
  CU_add_test(xhtml_suite, "test <input size> with void value." ,               test_xhtml_input_tag_023);
  CU_add_test(xhtml_suite, "test <input size> with alphabetic value." ,         test_xhtml_input_tag_024);
  CU_add_test(xhtml_suite, "test <input size> with numeric value." ,            test_xhtml_input_tag_025);
  CU_add_test(xhtml_suite, "test <input maxlength>." ,                          test_xhtml_input_tag_026);
  CU_add_test(xhtml_suite, "test <input maxlength> with void value." ,          test_xhtml_input_tag_027);
  CU_add_test(xhtml_suite, "test <input maxlength> with alphabetic value." ,    test_xhtml_input_tag_028);
  CU_add_test(xhtml_suite, "test <input maxlength> with numeric value." ,       test_xhtml_input_tag_029);
  CU_add_test(xhtml_suite, "test <input checked>." ,                            test_xhtml_input_tag_030);
  CU_add_test(xhtml_suite, "test <input accesskey>." ,                          test_xhtml_input_tag_031);
  CU_add_test(xhtml_suite, "test <input accesskey> with void value." ,          test_xhtml_input_tag_032);
  CU_add_test(xhtml_suite, "test <input accesskey> with value." ,               test_xhtml_input_tag_033);
  CU_add_test(xhtml_suite, "test <input istyle> 1." ,                           test_xhtml_input_tag_034);
  CU_add_test(xhtml_suite, "test <input istyle> 2." ,                           test_xhtml_input_tag_035);
  CU_add_test(xhtml_suite, "test <input istyle> 3." ,                           test_xhtml_input_tag_036);
  CU_add_test(xhtml_suite, "test <input istyle> 4." ,                           test_xhtml_input_tag_037);
  CU_add_test(xhtml_suite, "test <input istyle> 5." ,                           test_xhtml_input_tag_038);
  CU_add_test(xhtml_suite, "test <input istyle> 6." ,                           test_xhtml_input_tag_039);
  CU_add_test(xhtml_suite, "test <input istyle> 7." ,                           test_xhtml_input_tag_040);
  CU_add_test(xhtml_suite, "test <input istyle> 8." ,                           test_xhtml_input_tag_041);
  CU_add_test(xhtml_suite, "test <input istyle> 9." ,                           test_xhtml_input_tag_042);
  CU_add_test(xhtml_suite, "test <input istyle> 10." ,                          test_xhtml_input_tag_043);
  CU_add_test(xhtml_suite, "test <input istyle> 11." ,                          test_xhtml_input_tag_044);
  CU_add_test(xhtml_suite, "test <input istyle> 12." ,                          test_xhtml_input_tag_045);
  CU_add_test(xhtml_suite, "test <input istyle> 13." ,                          test_xhtml_input_tag_046);
  CU_add_test(xhtml_suite, "test <input istyle> 14." ,                          test_xhtml_input_tag_047);

  CU_add_test(xhtml_suite, "test <li>." ,                                       test_xhtml_li_tag_001);
  CU_add_test(xhtml_suite, "test <li> with alphabetic value." ,                 test_xhtml_li_tag_002);
  CU_add_test(xhtml_suite, "test <li> with japanese value." ,                   test_xhtml_li_tag_003);
  CU_add_test(xhtml_suite, "test <li> with japanese-hankaku value." ,           test_xhtml_li_tag_004);
  CU_add_test(xhtml_suite, "test <li> type attribute 1." ,                      test_xhtml_li_tag_005);
  CU_add_test(xhtml_suite, "test <li> type attribute 2." ,                      test_xhtml_li_tag_006);
  CU_add_test(xhtml_suite, "test <li> type attribute 3." ,                      test_xhtml_li_tag_007);
  CU_add_test(xhtml_suite, "test <li> type attribute 4." ,                      test_xhtml_li_tag_008);
  CU_add_test(xhtml_suite, "test <li> type attribute 5." ,                      test_xhtml_li_tag_009);
  CU_add_test(xhtml_suite, "test <li> type attribute 6." ,                      test_xhtml_li_tag_010);
  CU_add_test(xhtml_suite, "test <li> type attribute 7." ,                      test_xhtml_li_tag_011);
  CU_add_test(xhtml_suite, "test <li> type attribute 8." ,                      test_xhtml_li_tag_012);
  CU_add_test(xhtml_suite, "test <li> type attribute 9." ,                      test_xhtml_li_tag_013);

  CU_add_test(xhtml_suite, "test <menu>." ,                                     test_xhtml_menu_tag_001);
  CU_add_test(xhtml_suite, "test <menu> 2." ,                                   test_xhtml_menu_tag_002);
  CU_add_test(xhtml_suite, "test <menu> 3." ,                                   test_xhtml_menu_tag_003);
  CU_add_test(xhtml_suite, "test <menu> 4." ,                                   test_xhtml_menu_tag_004);
  CU_add_test(xhtml_suite, "test <menu> 5." ,                                   test_xhtml_menu_tag_005);

  CU_add_test(xhtml_suite, "test <ol>." ,                                       test_xhtml_ol_tag_001);
  CU_add_test(xhtml_suite, "test <ol> 2." ,                                     test_xhtml_ol_tag_002);
  CU_add_test(xhtml_suite, "test <ol> 3." ,                                     test_xhtml_ol_tag_003);
  CU_add_test(xhtml_suite, "test <ol> 4." ,                                     test_xhtml_ol_tag_004);
  CU_add_test(xhtml_suite, "test <ol> 5." ,                                     test_xhtml_ol_tag_005);
  CU_add_test(xhtml_suite, "test <ol> 6." ,                                     test_xhtml_ol_tag_006);
  CU_add_test(xhtml_suite, "test <ol> 7." ,                                     test_xhtml_ol_tag_007);
  CU_add_test(xhtml_suite, "test <ol> 8." ,                                     test_xhtml_ol_tag_008);
  CU_add_test(xhtml_suite, "test <ol> 9." ,                                     test_xhtml_ol_tag_009);
  CU_add_test(xhtml_suite, "test <ol> 10." ,                                    test_xhtml_ol_tag_010);
  CU_add_test(xhtml_suite, "test <ol> 11." ,                                    test_xhtml_ol_tag_011);
  CU_add_test(xhtml_suite, "test <ol> 12." ,                                    test_xhtml_ol_tag_012);
  CU_add_test(xhtml_suite, "test <ol> 13." ,                                    test_xhtml_ol_tag_013);
  CU_add_test(xhtml_suite, "test <ol> 14." ,                                    test_xhtml_ol_tag_014);
  CU_add_test(xhtml_suite, "test <ol> 15." ,                                    test_xhtml_ol_tag_015);
  CU_add_test(xhtml_suite, "test <ol> 16." ,                                    test_xhtml_ol_tag_016);
  CU_add_test(xhtml_suite, "test <ol> 17." ,                                    test_xhtml_ol_tag_017);
  CU_add_test(xhtml_suite, "test <ol> 18." ,                                    test_xhtml_ol_tag_018);

  CU_add_test(xhtml_suite, "test <option>." ,                                   test_xhtml_option_tag_001);
  CU_add_test(xhtml_suite, "test <option value> with no value." ,               test_xhtml_option_tag_002);
  CU_add_test(xhtml_suite, "test <option value> with void value." ,             test_xhtml_option_tag_003);
  CU_add_test(xhtml_suite, "test <option value> with alphabetic value." ,       test_xhtml_option_tag_004);
  CU_add_test(xhtml_suite, "test <option value> with japanese value." ,         test_xhtml_option_tag_005);
  CU_add_test(xhtml_suite, "test <option value> with japanese-kana value." ,    test_xhtml_option_tag_006);
  CU_add_test(xhtml_suite, "test <option selected>." ,                          test_xhtml_option_tag_007);

  CU_add_test(xhtml_suite, "test <p> 1." ,                                      test_xhtml_p_tag_001);
  CU_add_test(xhtml_suite, "test <p> 2." ,                                      test_xhtml_p_tag_002);
  CU_add_test(xhtml_suite, "test <p> 3." ,                                      test_xhtml_p_tag_003);
  CU_add_test(xhtml_suite, "test <p> 4." ,                                      test_xhtml_p_tag_004);
  CU_add_test(xhtml_suite, "test <p> 5." ,                                      test_xhtml_p_tag_005);
  CU_add_test(xhtml_suite, "test <p> 6." ,                                      test_xhtml_p_tag_006);
  CU_add_test(xhtml_suite, "test <p> 7." ,                                      test_xhtml_p_tag_007);
  CU_add_test(xhtml_suite, "test <p> 8." ,                                      test_xhtml_p_tag_008);

  CU_add_test(xhtml_suite, "test <plaintext> 1." ,                              test_xhtml_plaintext_tag_001);
  CU_add_test(xhtml_suite, "test <plaintext> 2." ,                              test_xhtml_plaintext_tag_002);
  CU_add_test(xhtml_suite, "test <plaintext> 3. value is not closed." ,         test_xhtml_plaintext_tag_003);
  CU_add_test(xhtml_suite, "test <plaintext> 3. value is not closed and hankaku." , test_xhtml_plaintext_tag_004);

  CU_add_test(xhtml_suite, "test <pre> 1." ,                                    test_xhtml_pre_tag_001);
  CU_add_test(xhtml_suite, "test <pre> 2." ,                                    test_xhtml_pre_tag_002);
  CU_add_test(xhtml_suite, "test <pre> 3." ,                                    test_xhtml_pre_tag_003);
  CU_add_test(xhtml_suite, "test <pre> 4." ,                                    test_xhtml_pre_tag_004);
  CU_add_test(xhtml_suite, "test <pre> 5." ,                                    test_xhtml_pre_tag_005);

  CU_add_test(xhtml_suite, "test <select> 1." ,                                 test_xhtml_select_tag_001);
  CU_add_test(xhtml_suite, "test <select> 2." ,                                 test_xhtml_select_tag_002);
  CU_add_test(xhtml_suite, "test <select> 3." ,                                 test_xhtml_select_tag_003);
  CU_add_test(xhtml_suite, "test <select> 4." ,                                 test_xhtml_select_tag_004);
  CU_add_test(xhtml_suite, "test <select> 5." ,                                 test_xhtml_select_tag_005);
  CU_add_test(xhtml_suite, "test <select> 6." ,                                 test_xhtml_select_tag_006);
  CU_add_test(xhtml_suite, "test <select> 7." ,                                 test_xhtml_select_tag_007);
  CU_add_test(xhtml_suite, "test <select> 8." ,                                 test_xhtml_select_tag_008);
  CU_add_test(xhtml_suite, "test <select> 9." ,                                 test_xhtml_select_tag_009);
  CU_add_test(xhtml_suite, "test <select> 10." ,                                test_xhtml_select_tag_010);
  CU_add_test(xhtml_suite, "test <select> 11." ,                                test_xhtml_select_tag_011);
  CU_add_test(xhtml_suite, "test <select> 12." ,                                test_xhtml_select_tag_012);
  CU_add_test(xhtml_suite, "test <select> 13." ,                                test_xhtml_select_tag_013);
  CU_add_test(xhtml_suite, "test <select> 14." ,                                test_xhtml_select_tag_014);

  CU_add_test(xhtml_suite, "test <textarea> 1." ,                               test_xhtml_textarea_tag_001);
  CU_add_test(xhtml_suite, "test <textarea> 2." ,                               test_xhtml_textarea_tag_002);
  CU_add_test(xhtml_suite, "test <textarea> 3." ,                               test_xhtml_textarea_tag_003);
  CU_add_test(xhtml_suite, "test <textarea> 4." ,                               test_xhtml_textarea_tag_004);
  CU_add_test(xhtml_suite, "test <textarea> 5." ,                               test_xhtml_textarea_tag_005);
  CU_add_test(xhtml_suite, "test <textarea> 6." ,                               test_xhtml_textarea_tag_006);
  CU_add_test(xhtml_suite, "test <textarea> 7." ,                               test_xhtml_textarea_tag_007);
  CU_add_test(xhtml_suite, "test <textarea> 8." ,                               test_xhtml_textarea_tag_008);
  CU_add_test(xhtml_suite, "test <textarea> 9." ,                               test_xhtml_textarea_tag_009);
  CU_add_test(xhtml_suite, "test <textarea> 10." ,                              test_xhtml_textarea_tag_010);
  CU_add_test(xhtml_suite, "test <textarea> 11." ,                              test_xhtml_textarea_tag_011);
  CU_add_test(xhtml_suite, "test <textarea> 12." ,                              test_xhtml_textarea_tag_012);
  CU_add_test(xhtml_suite, "test <textarea> 13." ,                              test_xhtml_textarea_tag_013);
  CU_add_test(xhtml_suite, "test <textarea> 14." ,                              test_xhtml_textarea_tag_014);
  CU_add_test(xhtml_suite, "test <textarea> 15." ,                              test_xhtml_textarea_tag_015);
  CU_add_test(xhtml_suite, "test <textarea> 16." ,                              test_xhtml_textarea_tag_016);
  CU_add_test(xhtml_suite, "test <textarea> 17." ,                              test_xhtml_textarea_tag_017);
  CU_add_test(xhtml_suite, "test <textarea> 18." ,                              test_xhtml_textarea_tag_018);
  CU_add_test(xhtml_suite, "test <textarea> 19." ,                              test_xhtml_textarea_tag_019);
  CU_add_test(xhtml_suite, "test <textarea> 20." ,                              test_xhtml_textarea_tag_020);
  CU_add_test(xhtml_suite, "test <textarea> 21." ,                              test_xhtml_textarea_tag_021);
  CU_add_test(xhtml_suite, "test <textarea> 22." ,                              test_xhtml_textarea_tag_022);
  CU_add_test(xhtml_suite, "test <textarea> 23." ,                              test_xhtml_textarea_tag_023);
  CU_add_test(xhtml_suite, "test <textarea> 24." ,                              test_xhtml_textarea_tag_024);
  CU_add_test(xhtml_suite, "test <textarea> 25." ,                              test_xhtml_textarea_tag_025);
  CU_add_test(xhtml_suite, "test <textarea> 26." ,                              test_xhtml_textarea_tag_026);

  CU_add_test(xhtml_suite, "test <title> 1." ,                                  test_xhtml_title_tag_001);
  CU_add_test(xhtml_suite, "test <title> 2." ,                                  test_xhtml_title_tag_002);

  CU_add_test(xhtml_suite, "test <ul> 1." ,                                     test_xhtml_ul_tag_001);
  CU_add_test(xhtml_suite, "test <ul> 2." ,                                     test_xhtml_ul_tag_002);
  CU_add_test(xhtml_suite, "test <ul> 3." ,                                     test_xhtml_ul_tag_003);
  CU_add_test(xhtml_suite, "test <ul> 4." ,                                     test_xhtml_ul_tag_004);
  CU_add_test(xhtml_suite, "test <ul> 5." ,                                     test_xhtml_ul_tag_005);

  CU_add_test(xhtml_suite, "test <blink> 1." ,                                  test_xhtml_blink_tag_001);
  CU_add_test(xhtml_suite, "test <blink> 2." ,                                  test_xhtml_blink_tag_002);
  CU_add_test(xhtml_suite, "test <blink> 3." ,                                  test_xhtml_blink_tag_003);
  CU_add_test(xhtml_suite, "test <blink> 4." ,                                  test_xhtml_blink_tag_004);

  CU_add_test(xhtml_suite, "test <marquee> 1." ,                                test_xhtml_marquee_tag_001);
  CU_add_test(xhtml_suite, "test <marquee> 2." ,                                test_xhtml_marquee_tag_002);
  CU_add_test(xhtml_suite, "test <marquee> 3." ,                                test_xhtml_marquee_tag_003);
  CU_add_test(xhtml_suite, "test <marquee> 4." ,                                test_xhtml_marquee_tag_004);
  CU_add_test(xhtml_suite, "test <marquee> 5." ,                                test_xhtml_marquee_tag_005);
  CU_add_test(xhtml_suite, "test <marquee> 6." ,                                test_xhtml_marquee_tag_006);
  CU_add_test(xhtml_suite, "test <marquee> 7." ,                                test_xhtml_marquee_tag_007);
  CU_add_test(xhtml_suite, "test <marquee> 8." ,                                test_xhtml_marquee_tag_008);
  CU_add_test(xhtml_suite, "test <marquee> 9." ,                                test_xhtml_marquee_tag_009);
  CU_add_test(xhtml_suite, "test <marquee> 10." ,                               test_xhtml_marquee_tag_010);
  CU_add_test(xhtml_suite, "test <marquee> 11." ,                               test_xhtml_marquee_tag_011);
  CU_add_test(xhtml_suite, "test <marquee> 12." ,                               test_xhtml_marquee_tag_012);
  CU_add_test(xhtml_suite, "test <marquee> 13." ,                               test_xhtml_marquee_tag_013);
  CU_add_test(xhtml_suite, "test <marquee> 14." ,                               test_xhtml_marquee_tag_014);
  CU_add_test(xhtml_suite, "test <marquee> 15." ,                               test_xhtml_marquee_tag_015);
  CU_add_test(xhtml_suite, "test <marquee> 16." ,                               test_xhtml_marquee_tag_016);
  CU_add_test(xhtml_suite, "test <marquee> 17." ,                               test_xhtml_marquee_tag_017);
  CU_add_test(xhtml_suite, "test <marquee> 18." ,                               test_xhtml_marquee_tag_018);

  CU_add_test(xhtml_suite, "test <meta> 1." ,                                   test_xhtml_meta_tag_001);
  CU_add_test(xhtml_suite, "test <meta> 2." ,                                   test_xhtml_meta_tag_002);
  CU_add_test(xhtml_suite, "test <meta> 3." ,                                   test_xhtml_meta_tag_003);
  CU_add_test(xhtml_suite, "test <meta> 4." ,                                   test_xhtml_meta_tag_004);
  CU_add_test(xhtml_suite, "test <meta> 5." ,                                   test_xhtml_meta_tag_005);
  CU_add_test(xhtml_suite, "test <meta> 6." ,                                   test_xhtml_meta_tag_006);
  CU_add_test(xhtml_suite, "test <meta> 7." ,                                   test_xhtml_meta_tag_007);
  CU_add_test(xhtml_suite, "test <meta> 8." ,                                   test_xhtml_meta_tag_008);
  CU_add_test(xhtml_suite, "test <meta> 9." ,                                   test_xhtml_meta_tag_009);
#endif
  /* aend */

  CU_basic_run_tests();
  CU_cleanup_registry();

  return(0);
}

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

char *chxj_qr_code_blob_handler(request_rec* r, const char* indata, size_t* len)
{
  return NULL;
}

char *test_os_escape_path(apr_pool_t *p, const char *path, int partial)
{
  return (char *)path;
}

void test_set_content_type(request_rec *r, const char *ct)
{
  fprintf(stderr, "%s:%d set_content_type(%s)\n", __FILE__,__LINE__,ct);
}

const char *test_run_http_scheme(request_rec *r)
{
  static char *s = "http";
  return s;
}

void * test_get_module_config(const ap_conf_vector_t *cv, const module *m)
{
  static mod_chxj_config cfg;
  memset(&cfg, 0, sizeof(mod_chxj_config));
  return &cfg;
}

chxjconvrule_entry *
chxj_apply_convrule(request_rec* r, apr_array_header_t* convrules)
{
  static chxjconvrule_entry entries;
  memset(&entries, 0, sizeof(chxjconvrule_entry));
  entries.encoding = apr_pstrdup(r->pool, "UTF8");
  return &entries;
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
    X.html_spec_type = CHXJ_SPEC_XHtml_Mobile_1_0; \
  } while (0)



void test_xhtml_001() 
{
#define  TEST_STRING "<!-- comment -->"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_xhtml_002() 
{
#define  TEST_STRING ""
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
#define  XHTML_HEADER "<?xml version=\"1.0\" encoding=\"Windows-31J\"?>\r\n" \
                      "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\"\r\n" \
                      " \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\r\n" 

void test_xhtml_comment_tag_001() 
{
#define  TEST_STRING "<html><!--</html><body>--><head></head><body></body></html>"
#define  RESULT_STRING XHTML_HEADER \
                       "<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n" \
                       "<head>\r\n" \
                       "</head>\r\n" \
                       "<body>\r\n" \
                       "</body>\r\n" \
                       "</html>\r\n"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <A>                                                                        */
/*============================================================================*/
void test_xhtml_a_tag_name_attribute_001() 
{
#define  TEST_STRING "<a name=\"abc\">abc</a>"
#define  RESULT_STRING "<a id=\"abc\">abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_name_attribute_002() 
{
#define  TEST_STRING "<a name=\"\">abc</a>"
#define  RESULT_STRING "<a>abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_name_attribute_003() 
{
#define  TEST_STRING "<a name=\"あああ\">abc</a>"
#define  RESULT_STRING "<a id=\"あああ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  destlen = sizeof(TEST_STRING)-1;
  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_name_attribute_004() 
{
#define  TEST_STRING "<a name=\"ｱｱｱ\">abc</a>"
#define  RESULT_STRING "<a id=\"ｱｱｱ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_001() 
{
#define  TEST_STRING "<a href=\"\">abc</a>"
#define  RESULT_STRING "<a>abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_002() 
{
#define  TEST_STRING "<a href=\"http://www.google.co.jp/\">abc</a>"
#define  RESULT_STRING "<a href=\"http://www.google.co.jp/\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_003() 
{
#define  TEST_STRING "<a href=\"a.html\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_004() 
{
#define  TEST_STRING "<a href=\"a.html#abc\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html#abc\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_005() 
{
#define  TEST_STRING "<a href=\"a.html#\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html#\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_href_attribute_006() 
{
#define  TEST_STRING "<a href=\"a.html\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_accesskey_attribute_001() 
{
#define  TEST_STRING "<a accesskey=\"1\">abc</a>"
#define  RESULT_STRING "<a accesskey=\"1\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_accesskey_attribute_002() 
{
#define  TEST_STRING "<a accesskey=\"\">abc</a>"
#define  RESULT_STRING "<a accesskey=\"\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_a_tag_accesskey_attribute_003() 
{
#define  TEST_STRING "<a accesskey>abc</a>"
#define  RESULT_STRING "<a accesskey=\"\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BASE>                                                                     */
/*============================================================================*/
void test_xhtml_base_tag_001() 
{
#define  TEST_STRING "<base>"
#define  RESULT_STRING "<base />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_base_tag_href_attribute_001() 
{
#define  TEST_STRING "<base href>"
#define  RESULT_STRING "<base href=\"\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_base_tag_href_attribute_002() 
{
#define  TEST_STRING "<base href=\"\">"
#define  RESULT_STRING "<base href=\"\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_base_tag_href_attribute_003() 
{
#define  TEST_STRING "<base href=\"http://www.google.co.jp/\">"
#define  RESULT_STRING "<base href=\"http://www.google.co.jp/\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_base_tag_href_attribute_004() 
{
#define  TEST_STRING "<base href=\".\">"
#define  RESULT_STRING "<base href=\".\" />r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BLOCKQUOTE>                                                               */
/*============================================================================*/
void test_xhtml_blockquote_tag_001() 
{
#define  TEST_STRING "<blockquote></blockquote>"
#define  RESULT_STRING "<blockquote></blockquote>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blockquote_tag_002() 
{
#define  TEST_STRING "<blockquote>abc</blockquote>"
#define  RESULT_STRING "<blockquote>abc</blockquote>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blockquote_tag_003() 
{
#define  TEST_STRING "<blockquote>亀さん</blockquote>"
#define  RESULT_STRING "<blockquote>亀さん</blockquote>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blockquote_tag_004() 
{
#define  TEST_STRING "<blockquote>ﾊﾝｶｸ</blockquote>"
#define  RESULT_STRING "<blockquote>ﾊﾝｶｸ</blockquote>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BODY>                                                                     */
/*============================================================================*/
void test_xhtml_body_tag_001() 
{
#define  TEST_STRING "<body></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_002() 
{
#define  TEST_STRING "<body bgcolor></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_003() 
{
#define  TEST_STRING "<body bgcolor=\"\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_004() 
{
#define  TEST_STRING "<body bgcolor=\"#ff0000\"></body>"
#define  RESULT_STRING "<body bgcolor=\"#ff0000\">\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_005() 
{
#define  TEST_STRING "<body text></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_006() 
{
#define  TEST_STRING "<body text=\"\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_007() 
{
#define  TEST_STRING "<body text=\"#ff0000\"></body>"
#define  RESULT_STRING "<body text=\"#ff0000\">\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_008() 
{
#define  TEST_STRING "<body link></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_009() 
{
#define  TEST_STRING "<body link=\"\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_010() 
{
#define  TEST_STRING "<body link=\"#ff0000\"></body>"
#define  RESULT_STRING "<body link=\"#ff0000\">\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_011() 
{
#define  TEST_STRING "<body vlink></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_012() 
{
#define  TEST_STRING "<body vlink=\"\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_013() 
{
#define  TEST_STRING "<body vlink=\"#ff0000\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_014() 
{
#define  TEST_STRING "<body alink=\"#FF0000\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_body_tag_015() 
{
#define  TEST_STRING "<body boyoyon=\"#FF0000\"></body>"
#define  RESULT_STRING "<body>\r\n</body>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BR>                                                                       */
/*============================================================================*/
void test_xhtml_br_tag_001() 
{
#define  TEST_STRING "<br>"
#define  RESULT_STRING "<br />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_002() 
{
#define  TEST_STRING "<br clear=\"left\">"
#define  RESULT_STRING "<br clear=\"left\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_003() 
{
#define  TEST_STRING "<br clear=\"right\">"
#define  RESULT_STRING "<br clear=\"right\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_004() 
{
#define  TEST_STRING "<br clear=\"all\">"
#define  RESULT_STRING "<br clear=\"all\" />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_005() 
{
#define  TEST_STRING "<br clear=\"\">"
#define  RESULT_STRING "<br />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_006() 
{
#define  TEST_STRING "<br clear>"
#define  RESULT_STRING "<br />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_br_tag_007() 
{
#define  TEST_STRING "<br clear=\"abc\">"
#define  RESULT_STRING "<br />\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <CENTER>                                                                   */
/*============================================================================*/
void test_xhtml_center_tag_001()
{
#define  TEST_STRING "<center>あいうえお</center>"
#define  RESULT_STRING "<center>あいうえお</center>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DIR>                                                                      */
/*============================================================================*/
void test_xhtml_dir_tag_001()
{
#define  TEST_STRING "<dir><li>あああ</li><li>いいい</li></dir>"
#define  RESULT_STRING "<dir><li>あああ</li><li>いいい</li></dir>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_dir_tag_002()
{
#define  TEST_STRING "<dir></dir>"
#define  RESULT_STRING "<dir></dir>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DL>                                                                       */
/*============================================================================*/
void test_xhtml_dl_tag_001()
{
#define  TEST_STRING "<dl></dl>"
#define  RESULT_STRING "<dl></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DT>                                                                       */
/*============================================================================*/
void test_xhtml_dt_tag_001()
{
#define  TEST_STRING "<dl><dt>あああ</dt><dt>いいい</dt></dl>"
#define  RESULT_STRING "<dl><dt>あああ</dt><dt>いいい</dt></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_dt_tag_002()
{
#define  TEST_STRING "<dl><dt></dt><dt></dt></dl>"
#define  RESULT_STRING "<dl><dt></dt><dt></dt></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DD>                                                                       */
/*============================================================================*/
void test_xhtml_dd_tag_001()
{
#define  TEST_STRING "<dl><dt>あああ</dt><dd>いいい</dd><dt>ああああ</dt><dd>いいいい</dd></dl>"
#define  RESULT_STRING "<dl><dt>あああ</dt><dd>いいい</dd><dt>ああああ</dt><dd>いいいい</dd></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_dd_tag_002()
{
#define  TEST_STRING "<dl><dd></dd><dd></dd></dl>"
#define  RESULT_STRING "<dl><dd></dd><dd></dd></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <DIV>                                                                      */
/*============================================================================*/
void test_xhtml_div_tag_001() 
{
#define  TEST_STRING "<div></div>"
#define  RESULT_STRING "<div></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_002() 
{
#define  TEST_STRING "<div align=\"left\"></div>"
#define  RESULT_STRING "<div align=\"left\"></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_003() 
{
#define  TEST_STRING "<div align=\"right\"></div>"
#define  RESULT_STRING "<div align=\"right\"></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_004() 
{
#define  TEST_STRING "<div align=\"center\"></div>"
#define  RESULT_STRING "<div align=\"center\"></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_005() 
{
#define  TEST_STRING "<div align=\"\"></div>"
#define  RESULT_STRING "<div></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_006() 
{
#define  TEST_STRING "<div align=\"abc\"></div>"
#define  RESULT_STRING "<div></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_div_tag_007() 
{
#define  TEST_STRING "<div style=\"aaa\"></div>"
#define  RESULT_STRING "<div></div>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* FORM                                                                       */
/*============================================================================*/
void test_xhtml_form_tag_001() 
{
#define  TEST_STRING "<form></form>"
#define  RESULT_STRING "<form></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_002() 
{
#define  TEST_STRING "<form method></form>"
#define  RESULT_STRING "<form method=\"\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_003() 
{
#define  TEST_STRING "<form method=\"post\"></form>"
#define  RESULT_STRING "<form method=\"post\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_004() 
{
#define  TEST_STRING "<form method=\"get\"></form>"
#define  RESULT_STRING "<form method=\"get\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_005() 
{
#define  TEST_STRING "<form method=\"abc\"></form>"
#define  RESULT_STRING "<form method=\"abc\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_006() 
{
#define  TEST_STRING "<form action></form>"
#define  RESULT_STRING "<form action=\"\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_007() 
{
#define  TEST_STRING "<form action></form>"
#define  RESULT_STRING "<form action=\"\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_008() 
{
#define  TEST_STRING "<form action=\"http://www.google.co.jp/\"></form>"
#define  RESULT_STRING "<form action=\"http://www.google.co.jp/\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_form_tag_009() 
{
#define  TEST_STRING "<form method=\"post\" action=\"hogehoge\"></form>"
#define  RESULT_STRING "<form method=\"post\" action=\"hogehoge\"></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HEAD>                                                                     */
/*============================================================================*/
void test_xhtml_head_tag_001()
{
#define  TEST_STRING "<head></head>"
#define  RESULT_STRING "<head>\r\n</head>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_head_tag_002()
{
#define  TEST_STRING "<head><title>あああ</title></head>"
#define  RESULT_STRING "<head>\r\n<title>あああ</title>\r\n</head>\r\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <H>                                                                        */
/*============================================================================*/
void test_xhtml_h1_tag_001() 
{
#define  TEST_STRING "<h1></h1>"
#define  RESULT_STRING "<h1></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_002() 
{
#define  TEST_STRING "<h1>abc</h1>"
#define  RESULT_STRING "<h1>abc</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_003() 
{
#define  TEST_STRING "<h1>亀さん</h1>"
#define  RESULT_STRING "<h1>亀さん</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_004() 
{
#define  TEST_STRING "<h1>ﾊﾝｶｸ</h1>"
#define  RESULT_STRING "<h1>ﾊﾝｶｸ</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_005() 
{
#define  TEST_STRING "<h1 align></h1>"
#define  RESULT_STRING "<h1></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_006() 
{
#define  TEST_STRING "<h1 align=\"\"></h1>"
#define  RESULT_STRING "<h1></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_007() 
{
#define  TEST_STRING "<h1 align=\"left\"></h1>"
#define  RESULT_STRING "<h1 style=\"text-align:left\"></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_008() 
{
#define  TEST_STRING "<h1 align=\"right\"></h1>"
#define  RESULT_STRING "<h1 style=\"text-align:right\"></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_009() 
{
#define  TEST_STRING "<h1 align=\"center\"></h1>"
#define  RESULT_STRING "<h1 style=\"text-align:center\"></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h1_tag_010() 
{
#define  TEST_STRING "<h1 align=\"unkown\"></h1>"
#define  RESULT_STRING "<h1></h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_001() 
{
#define  TEST_STRING "<h2></h2>"
#define  RESULT_STRING "<h2></h2>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_002() 
{
#define  TEST_STRING "<h2>abc</h2>"
#define  RESULT_STRING "<h2>abc</h2>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_003() 
{
#define  TEST_STRING "<h2>亀さん</h2>"
#define  RESULT_STRING "<h2>亀さん</h2>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_004() 
{
#define  TEST_STRING "<h2>ﾊﾝｶｸ</h2>"
#define  RESULT_STRING "<h2>ﾊﾝｶｸ</h2>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_005() 
{
#define  TEST_STRING "<h2 align></h2>"
#define  RESULT_STRING "<h2></h2>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "ret=[%s]",ret);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/* KONNO */
void test_xhtml_h2_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><h2></h2></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"left\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><h2 align=\"left\"></h2></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"right\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><h2 align=\"right\"></h2></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"center\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><h2 align=\"center\"></h2></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h2_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h2 align=\"unkown\"></h2></body></html>"
#define  RESULT_STRING "<html><head></head><body><h2></h2></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h3></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h3>abc</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3>abc</h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h3>亀さん</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3>亀さん</h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h3>ﾊﾝｶｸ</h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3>ﾊﾝｶｸ</h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h3 align></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"left\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3 align=\"left\"></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"right\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3 align=\"right\"></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"center\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3 align=\"center\"></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h3_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h3 align=\"unkown\"></h3></body></html>"
#define  RESULT_STRING "<html><head></head><body><h3></h3></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h4></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h4>abc</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4>abc</h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h4>亀さん</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4>亀さん</h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h4>ﾊﾝｶｸ</h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4>ﾊﾝｶｸ</h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h4 align></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"left\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4 align=\"left\"></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"right\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4 align=\"right\"></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"center\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4 align=\"center\"></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h4_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h4 align=\"unkown\"></h4></body></html>"
#define  RESULT_STRING "<html><head></head><body><h4></h4></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h5></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h5>abc</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5>abc</h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h5>亀さん</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5>亀さん</h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h5>ﾊﾝｶｸ</h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5>ﾊﾝｶｸ</h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h5 align></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"left\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5 align=\"left\"></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"right\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5 align=\"right\"></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"center\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5 align=\"center\"></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h5_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h5 align=\"unkown\"></h5></body></html>"
#define  RESULT_STRING "<html><head></head><body><h5></h5></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><h6></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><h6>abc</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6>abc</h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><h6>亀さん</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6>亀さん</h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><h6>ﾊﾝｶｸ</h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6>ﾊﾝｶｸ</h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><h6 align></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"left\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6 align=\"left\"></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"right\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6 align=\"right\"></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"center\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6 align=\"center\"></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_h6_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><h6 align=\"unkown\"></h6></body></html>"
#define  RESULT_STRING "<html><head></head><body><h6></h6></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HR>                                                                       */
/*============================================================================*/
void test_xhtml_hr_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><hr></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><hr /></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><hr align></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"left\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"left\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"right\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"right\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"center\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr align=\"center\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><hr align=\"unknown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><hr size></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><hr size=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr size=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><hr width></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\" noshade></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\" noshade></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_hr_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><hr width=\"10\" color=\"#ff0000\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><hr width=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <HTML>                                                                     */
/*============================================================================*/
void test_xhtml_html_tag_001()
{
#define  TEST_STRING "<html><head></head><body></body></html>"
#define  RESULT_STRING "<html><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <IMG>                                                                      */
/*============================================================================*/
void test_xhtml_img_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><img></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><img src></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"aaa\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"aaa?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"亀さん\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"亀さん?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><img src=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img src=\"ﾊﾝｶｸ?_chxj_cc=test_cookie_id&_chxj_nc=true\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><img align></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"top\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"top\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"middle\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"middle\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"bottom\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"bottom\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"left\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"left\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"right\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img align=\"right\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><img align=\"unkown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><img width></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_019() 
{
#define  TEST_STRING "<html><head></head><body><img width=\"10%\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img width=\"10%\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_020() 
{
#define  TEST_STRING "<html><head></head><body><img height></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_021() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_022() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_023() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_024() 
{
#define  TEST_STRING "<html><head></head><body><img height=\"10%\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img height=\"10%\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_025() 
{
#define  TEST_STRING "<html><head></head><body><img hspace></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_026() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_027() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img hspace=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_028() 
{
#define  TEST_STRING "<html><head></head><body><img hspace=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img hspace=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_029() 
{
#define  TEST_STRING "<html><head></head><body><img vspace></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_030() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_031() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img vspace=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_032() 
{
#define  TEST_STRING "<html><head></head><body><img vspace=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img vspace=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_033() 
{
#define  TEST_STRING "<html><head></head><body><img alt></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_034() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_035() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"abc\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"abc\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_036() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"亀さん\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"亀さん\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_img_tag_037() 
{
#define  TEST_STRING "<html><head></head><body><img alt=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><img alt=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <INPUT>                                                                    */
/*============================================================================*/
void test_xhtml_input_tag_001() 
{
#define  TEST_STRING "<html><head></head><body><input></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_002() 
{
#define  TEST_STRING "<html><head></head><body><input type></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_003() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_004() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"text\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"text\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_005() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"password\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"password\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_006() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"checkbox\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"checkbox\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_007() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"radio\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"radio\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_008() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"hidden\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"hidden\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_009() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"submit\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"submit\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_010() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"reset\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input type=\"reset\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_011() 
{
#define  TEST_STRING "<html><head></head><body><input type=\"unknown\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_012() 
{
#define  TEST_STRING "<html><head></head><body><input name></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_013() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_014() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_015() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"亀\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"亀\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_016() 
{
#define  TEST_STRING "<html><head></head><body><input name=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input name=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_017() 
{
#define  TEST_STRING "<html><head></head><body><input value></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_018() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_019() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_020() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"亀\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"亀\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_021() 
{
#define  TEST_STRING "<html><head></head><body><input value=\"ﾊﾝｶｸ\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input value=\"ﾊﾝｶｸ\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_022() 
{
#define  TEST_STRING "<html><head></head><body><input size></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_023() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_024() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input size=\"a\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_025() 
{
#define  TEST_STRING "<html><head></head><body><input size=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input size=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_026() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_027() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_028() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input maxlength=\"0\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_029() 
{
#define  TEST_STRING "<html><head></head><body><input maxlength=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input maxlength=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_030() 
{
#define  TEST_STRING "<html><head></head><body><input checked></body></html>"
#define  RESULT_STRING "<html><head></head><body><input checked></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_031() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_032() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_033() 
{
#define  TEST_STRING "<html><head></head><body><input accesskey=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input accesskey=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_034() 
{
#define  TEST_STRING "<html><head></head><body><input istyle></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_035() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_036() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"0\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_037() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"1\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"1\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_038() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"2\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"2\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_039() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"3\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"3\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_040() 
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"4\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"4\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_041()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"5\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_042()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"a\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_043()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"1\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"1\" maxlength=\"20\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_044()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"2\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"2\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_045()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"3\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"3\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_046()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"4\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input istyle=\"4\" maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_input_tag_047()
{
#define  TEST_STRING "<html><head></head><body><input istyle=\"a\" maxlength=\"10\"></body></html>"
#define  RESULT_STRING "<html><head></head><body><input maxlength=\"10\"></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <LI>                                                                       */
/*============================================================================*/
void test_xhtml_li_tag_001() 
{
#define  TEST_STRING "<li></li>"
#define  RESULT_STRING "<li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_002() 
{
#define  TEST_STRING "<li>abc</li>"
#define  RESULT_STRING "<li>abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_003() 
{
#define  TEST_STRING "<li>あいうえお</li>"
#define  RESULT_STRING "<li>あいうえお"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_004() 
{
#define  TEST_STRING "<li>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_005() 
{
#define  TEST_STRING "<li type>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_006() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_007() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_008() 
{
#define  TEST_STRING "<li type=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"1\">ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_009() 
{
#define  TEST_STRING "<li type=\"a\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"a\">ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_010() 
{
#define  TEST_STRING "<li type=\"A\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"A\">ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_011() 
{
#define  TEST_STRING "<li value>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_012() 
{
#define  TEST_STRING "<li value=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_li_tag_013() 
{
#define  TEST_STRING "<li value=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li value=\"1\">ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <MENU>                                                                     */
/*============================================================================*/
void test_xhtml_menu_tag_001() 
{
#define  TEST_STRING "<menu></menu>"
#define  RESULT_STRING "<menu></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_menu_tag_002() 
{
#define  TEST_STRING "<menu><li></li><li></li></menu>"
#define  RESULT_STRING "<menu><li><li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_menu_tag_003() 
{
#define  TEST_STRING "<menu><li>abc</li><li>def</li></menu>"
#define  RESULT_STRING "<menu><li>abc<li>def</menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_menu_tag_004() 
{
#define  TEST_STRING "<menu><li>あああ</li><li>いいい</li></menu>"
#define  RESULT_STRING "<menu><li>あああ<li>いいい</menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_menu_tag_005() 
{
#define  TEST_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ<li>ﾊﾝｶｸ</menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <OL>                                                                       */
/*============================================================================*/
void test_xhtml_ol_tag_001() 
{
#define  TEST_STRING "<ol></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_002() 
{
#define  TEST_STRING "<ol><li></li></ol>"
#define  RESULT_STRING "<ol><li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_003() 
{
#define  TEST_STRING "<ol><li>abc</li></ol>"
#define  RESULT_STRING "<ol><li>abc</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_004() 
{
#define  TEST_STRING "<ol><li>abc</li><li>def</li></ol>"
#define  RESULT_STRING "<ol><li>abc<li>def</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_005() 
{
#define  TEST_STRING "<ol><li>あ</li></ol>"
#define  RESULT_STRING "<ol><li>あ</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_006() 
{
#define  TEST_STRING "<ol><li>あ</li><li>い</li></ol>"
#define  RESULT_STRING "<ol><li>あ<li>い</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_007() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_008() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li><li>ｶﾅﾀﾞﾖ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ<li>ｶﾅﾀﾞﾖ</ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_009() 
{
#define  TEST_STRING "<ol type></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_010() 
{
#define  TEST_STRING "<ol type=\"\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_011() 
{
#define  TEST_STRING "<ol type=\"1\"></ol>"
#define  RESULT_STRING "<ol type=\"1\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_012() 
{
#define  TEST_STRING "<ol type=\"a\"></ol>"
#define  RESULT_STRING "<ol type=\"a\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_013() 
{
#define  TEST_STRING "<ol type=\"A\"></ol>"
#define  RESULT_STRING "<ol type=\"A\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_014() 
{
#define  TEST_STRING "<ol type=\"b\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_015() 
{
#define  TEST_STRING "<ol start></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_016() 
{
#define  TEST_STRING "<ol start=\"\"></ol>"
#define  RESULT_STRING "<ol></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_017() 
{
#define  TEST_STRING "<ol start=\"1\"></ol>"
#define  RESULT_STRING "<ol start=\"1\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ol_tag_018() 
{
#define  TEST_STRING "<ol start=\"a\"></ol>"
#define  RESULT_STRING "<ol start=\"a\"></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <OPTION>                                                                   */
/*============================================================================*/
void test_xhtml_option_tag_001() 
{
#define  TEST_STRING "<option></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_002() 
{
#define  TEST_STRING "<option value></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_003() 
{
#define  TEST_STRING "<option value=\"\"></option>"
#define  RESULT_STRING "<option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_004() 
{
#define  TEST_STRING "<option value=\"a\"></option>"
#define  RESULT_STRING "<option value=\"a\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_005() 
{
#define  TEST_STRING "<option value=\"亀\"></option>"
#define  RESULT_STRING "<option value=\"亀\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_006() 
{
#define  TEST_STRING "<option value=\"ﾊﾝｶｸ\"></option>"
#define  RESULT_STRING "<option value=\"ﾊﾝｶｸ\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_option_tag_007() 
{
#define  TEST_STRING "<option selected></option>"
#define  RESULT_STRING "<option selected>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <P>                                                                        */
/*============================================================================*/
void test_xhtml_p_tag_001() 
{
#define  TEST_STRING "<p></p>"
#define  RESULT_STRING "<p></p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_002() 
{
#define  TEST_STRING "<p>あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_003() 
{
#define  TEST_STRING "<p align>あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_004() 
{
#define  TEST_STRING "<p align=\"\">あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_005() 
{
#define  TEST_STRING "<p align=\"right\">あああ</p>"
#define  RESULT_STRING "<p align=\"right\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_006() 
{
#define  TEST_STRING "<p align=\"left\">あああ</p>"
#define  RESULT_STRING "<p align=\"left\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_007() 
{
#define  TEST_STRING "<p align=\"center\">あああ</p>"
#define  RESULT_STRING "<p align=\"center\">あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_p_tag_008() 
{
#define  TEST_STRING "<p align=\"unknown\">あああ</p>"
#define  RESULT_STRING "<p>あああ</p>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <PLAINTEXT>                                                                */
/*============================================================================*/
void test_xhtml_plaintext_tag_001() 
{
#define  TEST_STRING "<plaintext>あああ</plaintext>"
#define  RESULT_STRING "<plaintext>あああ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_plaintext_tag_002() 
{
#define  TEST_STRING "<plaintext><h1>あああ</h1><h1>いいい</h1></plaintext>"
#define  RESULT_STRING "<plaintext><h1>あああ</h1><h1>いいい</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_plaintext_tag_003() 
{
#define  TEST_STRING "<plaintext><h1>あああ<h1>いいい</h1></plaintext>"
#define  RESULT_STRING "<plaintext><h1>あああ<h1>いいい</h1>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_plaintext_tag_004() 
{
#define  TEST_STRING "<plaintext><h1>ﾊﾝｶｸ<h1>ﾊﾝｶｸ</plaintext>"
#define  RESULT_STRING "<plaintext><h1>ﾊﾝｶｸ<h1>ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <PRE>                                                                      */
/*============================================================================*/
void test_xhtml_pre_tag_001() 
{
#define  TEST_STRING "<pre></pre>"
#define  RESULT_STRING "<pre></pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_pre_tag_002() 
{
#define  TEST_STRING "<pre>\n</pre>"
#define  RESULT_STRING "<pre>\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_pre_tag_003() 
{
#define  TEST_STRING "<pre>\n\n</pre>"
#define  RESULT_STRING "<pre>\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_pre_tag_004() 
{
#define  TEST_STRING "<pre><h1>あああ\n\n</pre>"
#define  RESULT_STRING "<pre><h1>あああ\n\n</h1></pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_pre_tag_005() 
{
#define  TEST_STRING "<pre><h1>ﾊﾝｶｸ</h1>\n\n</pre>"
#define  RESULT_STRING "<pre><h1>ﾊﾝｶｸ</h1>\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <SELECT>                                                                   */
/*============================================================================*/
void test_xhtml_select_tag_001() 
{
#define  TEST_STRING "<select></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_002() 
{
#define  TEST_STRING "<select>あああ</select>"
#define  RESULT_STRING "<select>あああ</select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_003() 
{
#define  TEST_STRING "<select name></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_004() 
{
#define  TEST_STRING "<select name=\"\"></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_005() 
{
#define  TEST_STRING "<select name=\"abc\"></select>"
#define  RESULT_STRING "<select name=\"abc\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_006() 
{
#define  TEST_STRING "<select name=\"あああ\"></select>"
#define  RESULT_STRING "<select name=\"あああ\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_007() 
{
#define  TEST_STRING "<select name=\"ﾊﾝｶｸ\"></select>"
#define  RESULT_STRING "<select name=\"ﾊﾝｶｸ\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_008() 
{
#define  TEST_STRING "<select size></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_009() 
{
#define  TEST_STRING "<select size=\"\"></select>"
#define  RESULT_STRING "<select></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_010() 
{
#define  TEST_STRING "<select size=\"abc\"></select>"
#define  RESULT_STRING "<select size=\"abc\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_011() 
{
#define  TEST_STRING "<select size=\"11\"></select>"
#define  RESULT_STRING "<select size=\"11\"></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_012() 
{
#define  TEST_STRING "<select multiple></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_013() 
{
#define  TEST_STRING "<select multiple=\"\"></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_select_tag_014() 
{
#define  TEST_STRING "<select multiple=\"abc\"></select>"
#define  RESULT_STRING "<select multiple></select>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <TEXTAREA>                                                                 */
/*============================================================================*/
void test_xhtml_textarea_tag_001() 
{
#define  TEST_STRING "<textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_002() 
{
#define  TEST_STRING "<textarea></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_003() 
{
#define  TEST_STRING "<textarea>あああ</textarea>"
#define  RESULT_STRING "<textarea>あああ</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_004() 
{
#define  TEST_STRING "<textarea>あああ\nいいい</textarea>"
#define  RESULT_STRING "<textarea>あああ\nいいい</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_005() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_006() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい\n</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい\n</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_007() 
{
#define  TEST_STRING "<textarea>あああ\r\nいいい\r\n</textarea>"
#define  RESULT_STRING "<textarea>あああ\r\nいいい\r\n</textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_008() 
{
#define  TEST_STRING "<textarea name></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_009() 
{
#define  TEST_STRING "<textarea name=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_010() 
{
#define  TEST_STRING "<textarea name=\"aaa\"></textarea>"
#define  RESULT_STRING "<textarea name=\"aaa\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_011() 
{
#define  TEST_STRING "<textarea rows></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_012() 
{
#define  TEST_STRING "<textarea rows=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_013() 
{
#define  TEST_STRING "<textarea rows=\"abc\"></textarea>"
#define  RESULT_STRING "<textarea rows=\"abc\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_014() 
{
#define  TEST_STRING "<textarea cols></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_015() 
{
#define  TEST_STRING "<textarea cols=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_016() 
{
#define  TEST_STRING "<textarea cols=\"123\"></textarea>"
#define  RESULT_STRING "<textarea cols=\"123\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_017() 
{
#define  TEST_STRING "<textarea accesskey></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_018() 
{
#define  TEST_STRING "<textarea accesskey=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_019() 
{
#define  TEST_STRING "<textarea accesskey=\"10\"></textarea>"
#define  RESULT_STRING "<textarea accesskey=\"10\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_020() 
{
#define  TEST_STRING "<textarea istyle></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_021() 
{
#define  TEST_STRING "<textarea istyle=\"\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_022() 
{
#define  TEST_STRING "<textarea istyle=\"1\"></textarea>"
#define  RESULT_STRING "<textarea istyle=\"1\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_023() 
{
#define  TEST_STRING "<textarea istyle=\"2\"></textarea>"
#define  RESULT_STRING "<textarea istyle=\"2\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_024() 
{
#define  TEST_STRING "<textarea istyle=\"3\"></textarea>"
#define  RESULT_STRING "<textarea istyle=\"3\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_025() 
{
#define  TEST_STRING "<textarea istyle=\"4\"></textarea>"
#define  RESULT_STRING "<textarea istyle=\"4\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_textarea_tag_026() 
{
#define  TEST_STRING "<textarea istyle=\"a\"></textarea>"
#define  RESULT_STRING "<textarea></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <TITLE>                                                                    */
/*============================================================================*/
void test_xhtml_title_tag_001()
{
#define  TEST_STRING "<title></title>"
#define  RESULT_STRING "<title></title>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_title_tag_002()
{
#define  TEST_STRING "<title>あああ</title>"
#define  RESULT_STRING "<title>あああ</title>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <UL>                                                                       */
/*============================================================================*/
void test_xhtml_ul_tag_001() 
{
#define  TEST_STRING "<ul>"
#define  RESULT_STRING "<ul></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ul_tag_002() 
{
#define  TEST_STRING "<ul></ul>"
#define  RESULT_STRING "<ul></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ul_tag_003() 
{
#define  TEST_STRING "<ul><li></ul>"
#define  RESULT_STRING "<ul><li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ul_tag_004() 
{
#define  TEST_STRING "<ul abc><li></ul>"
#define  RESULT_STRING "<ul><li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_ul_tag_005() 
{
#define  TEST_STRING "<ul abc><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <BLINK>                                                                    */
/*============================================================================*/
void test_xhtml_blink_tag_001() 
{
#define  TEST_STRING "<blink></blink>"
#define  RESULT_STRING "<blink></blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blink_tag_002() 
{
#define  TEST_STRING "<blink>abc</blink>"
#define  RESULT_STRING "<blink>abc</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blink_tag_003() 
{
#define  TEST_STRING "<blink>あああ</blink>"
#define  RESULT_STRING "<blink>あああ</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_blink_tag_004() 
{
#define  TEST_STRING "<blink>ﾊﾝｶｸ</blink>"
#define  RESULT_STRING "<blink>ﾊﾝｶｸ</blink>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* MARQUEE                                                                    */
/*============================================================================*/
void test_xhtml_marquee_tag_001() 
{
#define  TEST_STRING "<marquee></marquee>"
#define  RESULT_STRING "<marquee></marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_002() 
{
#define  TEST_STRING "<marquee>abc</marquee>"
#define  RESULT_STRING "<marquee>abc</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_003() 
{
#define  TEST_STRING "<marquee>あああ</marquee>"
#define  RESULT_STRING "<marquee>あああ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_004() 
{
#define  TEST_STRING "<marquee>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_005() 
{
#define  TEST_STRING "<marquee direction>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_006() 
{
#define  TEST_STRING "<marquee direction=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_007() 
{
#define  TEST_STRING "<marquee direction=\"left\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee direction=\"left\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_008() 
{
#define  TEST_STRING "<marquee direction=\"right\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee direction=\"right\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_009() 
{
#define  TEST_STRING "<marquee direction=\"center\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_010() 
{
#define  TEST_STRING "<marquee behavior>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_011() 
{
#define  TEST_STRING "<marquee behavior=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_012() 
{
#define  TEST_STRING "<marquee behavior=\"scroll\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee behavior=\"scroll\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_013() 
{
#define  TEST_STRING "<marquee behavior=\"slide\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee behavior=\"slide\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_014() 
{
#define  TEST_STRING "<marquee behavior=\"alternate\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee behavior=\"alternate\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_015() 
{
#define  TEST_STRING "<marquee behavior=\"unknown\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_016() 
{
#define  TEST_STRING "<marquee loop>ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_017() 
{
#define  TEST_STRING "<marquee loop=\"\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee>ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_marquee_tag_018() 
{
#define  TEST_STRING "<marquee loop=\"1\">ﾊﾝｶｸ</marquee>"
#define  RESULT_STRING "<marquee loop=\"1\">ﾊﾝｶｸ</marquee>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <META>                                                                     */
/*============================================================================*/
void test_xhtml_meta_tag_001() 
{
#define  TEST_STRING "<meta>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_002() 
{
#define  TEST_STRING "<meta http-equiv>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_003() 
{
#define  TEST_STRING "<meta http-equiv=\"\">"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_004() 
{
#define  TEST_STRING "<meta http-equiv=\"content-type\">"
#define  RESULT_STRING "<meta http-equiv=\"content-type\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_005() 
{
#define  TEST_STRING "<meta http-equiv=\"abc\">"
#define  RESULT_STRING "<meta http-equiv=\"abc\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_006() 
{
#define  TEST_STRING "<meta content>"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_007() 
{
#define  TEST_STRING "<meta content=\"\">"
#define  RESULT_STRING "<meta>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_008() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Windows-31J\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_xhtml_meta_tag_009() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Windows-31J\">"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_exchange_xhtml_mobile_1_0(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*
 * vim:ts=2 et
 */
