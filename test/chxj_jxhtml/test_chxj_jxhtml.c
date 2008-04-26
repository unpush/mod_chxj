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
#include "chxj_chtml40.c"
#include "chxj_chtml50.c"
#include "chxj_jxhtml.c"
#include "chxj_jhtml.c"
#include "chxj_hdml.c"
#include "chxj_xhtml_mobile_1_0.c"
#include "chxj_node_convert.c"
#include "chxj_tag_util.c"
#include "chxj_encoding.c"
#include "chxj_cookie.c"
#include "chxj_img_conv.c"
#include "chxj_url_encode.c"
#include "chxj_apache.c"
#include "chxj_dbm.c"
#include "chxj_str_util.c"
#include <iconv.h>



void test_jxhtml_001();
void test_jxhtml_002();
void test_jxhtml_comment_tag_001();

void test_jxhtml_a_tag_name_attribute_001();
void test_jxhtml_a_tag_name_attribute_002();
void test_jxhtml_a_tag_name_attribute_003();
void test_jxhtml_a_tag_name_attribute_004();
void test_jxhtml_a_tag_href_attribute_001();
void test_jxhtml_a_tag_href_attribute_002();
void test_jxhtml_a_tag_href_attribute_003();
void test_jxhtml_a_tag_href_attribute_004();
void test_jxhtml_a_tag_href_attribute_005();
void test_jxhtml_a_tag_href_attribute_006();
void test_jxhtml_a_tag_accesskey_attribute_001();
void test_jxhtml_a_tag_accesskey_attribute_002();
void test_jxhtml_a_tag_accesskey_attribute_003();

void test_jxhtml_base_tag_001();
void test_jxhtml_base_tag_href_attribute_001();
void test_jxhtml_base_tag_href_attribute_002();
void test_jxhtml_base_tag_href_attribute_003();
void test_jxhtml_base_tag_href_attribute_004();

void test_jxhtml_blockquote_tag_001();
void test_jxhtml_blockquote_tag_002();
void test_jxhtml_blockquote_tag_003();
void test_jxhtml_blockquote_tag_004();

void test_jxhtml_body_tag_001();
void test_jxhtml_body_tag_002();
void test_jxhtml_body_tag_003();
void test_jxhtml_body_tag_004();
void test_jxhtml_body_tag_005();
void test_jxhtml_body_tag_006();
void test_jxhtml_body_tag_007();
void test_jxhtml_body_tag_008();
void test_jxhtml_body_tag_009();
void test_jxhtml_body_tag_010();
void test_jxhtml_body_tag_011();
void test_jxhtml_body_tag_012();
void test_jxhtml_body_tag_013();
void test_jxhtml_body_tag_014();
void test_jxhtml_body_tag_015();

void test_jxhtml_br_tag_001();
void test_jxhtml_br_tag_002();
void test_jxhtml_br_tag_003();
void test_jxhtml_br_tag_004();
void test_jxhtml_br_tag_005();
void test_jxhtml_br_tag_006();
void test_jxhtml_br_tag_007();

void test_jxhtml_center_tag_001();

void test_jxhtml_dir_tag_001();
void test_jxhtml_dir_tag_002();
void test_jxhtml_dir_tag_003();
void test_jxhtml_dir_tag_004();
void test_jxhtml_dir_tag_005();
void test_jxhtml_dir_tag_006();
void test_jxhtml_dir_tag_007();

void test_jxhtml_dl_tag_001();

void test_jxhtml_dt_tag_001();
void test_jxhtml_dt_tag_002();

void test_jxhtml_dd_tag_001();
void test_jxhtml_dd_tag_002();

void test_jxhtml_div_tag_001();
void test_jxhtml_div_tag_002();
void test_jxhtml_div_tag_003();
void test_jxhtml_div_tag_004();
void test_jxhtml_div_tag_005();
void test_jxhtml_div_tag_006();
void test_jxhtml_div_tag_007();

void test_jxhtml_form_tag_001();
void test_jxhtml_form_tag_002();
void test_jxhtml_form_tag_003();
void test_jxhtml_form_tag_004();
void test_jxhtml_form_tag_005();
void test_jxhtml_form_tag_006();
void test_jxhtml_form_tag_007();
void test_jxhtml_form_tag_008();
void test_jxhtml_form_tag_009();

void test_jxhtml_head_tag_001();
void test_jxhtml_head_tag_002();

void test_jxhtml_h1_tag_001(); 
void test_jxhtml_h1_tag_002(); 
void test_jxhtml_h1_tag_003(); 
void test_jxhtml_h1_tag_004(); 
void test_jxhtml_h1_tag_005(); 
void test_jxhtml_h1_tag_006(); 
void test_jxhtml_h1_tag_007(); 
void test_jxhtml_h1_tag_008(); 
void test_jxhtml_h1_tag_009(); 
void test_jxhtml_h1_tag_010(); 

void test_jxhtml_h2_tag_001(); 
void test_jxhtml_h2_tag_002(); 
void test_jxhtml_h2_tag_003(); 
void test_jxhtml_h2_tag_004(); 
void test_jxhtml_h2_tag_005(); 
void test_jxhtml_h2_tag_006(); 
void test_jxhtml_h2_tag_007(); 
void test_jxhtml_h2_tag_008(); 
void test_jxhtml_h2_tag_009(); 
void test_jxhtml_h2_tag_010(); 

void test_jxhtml_h3_tag_001(); 
void test_jxhtml_h3_tag_002(); 
void test_jxhtml_h3_tag_003(); 
void test_jxhtml_h3_tag_004(); 
void test_jxhtml_h3_tag_005(); 
void test_jxhtml_h3_tag_006(); 
void test_jxhtml_h3_tag_007(); 
void test_jxhtml_h3_tag_008(); 
void test_jxhtml_h3_tag_009(); 
void test_jxhtml_h3_tag_010(); 

void test_jxhtml_h4_tag_001(); 
void test_jxhtml_h4_tag_002(); 
void test_jxhtml_h4_tag_003(); 
void test_jxhtml_h4_tag_004(); 
void test_jxhtml_h4_tag_005(); 
void test_jxhtml_h4_tag_006(); 
void test_jxhtml_h4_tag_007(); 
void test_jxhtml_h4_tag_008(); 
void test_jxhtml_h4_tag_009(); 
void test_jxhtml_h4_tag_010(); 

void test_jxhtml_h5_tag_001(); 
void test_jxhtml_h5_tag_002(); 
void test_jxhtml_h5_tag_003(); 
void test_jxhtml_h5_tag_004(); 
void test_jxhtml_h5_tag_005(); 
void test_jxhtml_h5_tag_006(); 
void test_jxhtml_h5_tag_007(); 
void test_jxhtml_h5_tag_008(); 
void test_jxhtml_h5_tag_009(); 
void test_jxhtml_h5_tag_010(); 

void test_jxhtml_h6_tag_001(); 
void test_jxhtml_h6_tag_002(); 
void test_jxhtml_h6_tag_003(); 
void test_jxhtml_h6_tag_004(); 
void test_jxhtml_h6_tag_005(); 
void test_jxhtml_h6_tag_006(); 
void test_jxhtml_h6_tag_007(); 
void test_jxhtml_h6_tag_008(); 
void test_jxhtml_h6_tag_009(); 
void test_jxhtml_h6_tag_010(); 

void test_jxhtml_hr_tag_001();
void test_jxhtml_hr_tag_002();
void test_jxhtml_hr_tag_003();
void test_jxhtml_hr_tag_004();
void test_jxhtml_hr_tag_005();
void test_jxhtml_hr_tag_006();
void test_jxhtml_hr_tag_007();
void test_jxhtml_hr_tag_008();
void test_jxhtml_hr_tag_009();
void test_jxhtml_hr_tag_010();
void test_jxhtml_hr_tag_011();
void test_jxhtml_hr_tag_012();
void test_jxhtml_hr_tag_013();
void test_jxhtml_hr_tag_014();
void test_jxhtml_hr_tag_015();
void test_jxhtml_hr_tag_016();
void test_jxhtml_hr_tag_017();
void test_jxhtml_hr_tag_018();
void test_jxhtml_hr_tag_018_1();
void test_jxhtml_hr_tag_018_2();

void test_jxhtml_html_tag_001();

void test_jxhtml_img_tag_001();
void test_jxhtml_img_tag_002();
void test_jxhtml_img_tag_003();
void test_jxhtml_img_tag_004();
void test_jxhtml_img_tag_005();
void test_jxhtml_img_tag_006();
void test_jxhtml_img_tag_007();
void test_jxhtml_img_tag_008();
void test_jxhtml_img_tag_009();
void test_jxhtml_img_tag_010();
void test_jxhtml_img_tag_011();
void test_jxhtml_img_tag_012();
void test_jxhtml_img_tag_013();
void test_jxhtml_img_tag_013_1();
void test_jxhtml_img_tag_014();
void test_jxhtml_img_tag_015();
void test_jxhtml_img_tag_016();
void test_jxhtml_img_tag_017();
void test_jxhtml_img_tag_018();
void test_jxhtml_img_tag_019();
void test_jxhtml_img_tag_020();
void test_jxhtml_img_tag_021();
void test_jxhtml_img_tag_022();
void test_jxhtml_img_tag_023();
void test_jxhtml_img_tag_024();
void test_jxhtml_img_tag_025();
void test_jxhtml_img_tag_026();
void test_jxhtml_img_tag_027();
void test_jxhtml_img_tag_028();
void test_jxhtml_img_tag_029();
void test_jxhtml_img_tag_030();
void test_jxhtml_img_tag_031();
void test_jxhtml_img_tag_032();
void test_jxhtml_img_tag_033();
void test_jxhtml_img_tag_034();
void test_jxhtml_img_tag_035();
void test_jxhtml_img_tag_036();
void test_jxhtml_img_tag_037();

void test_jxhtml_input_tag_001();
void test_jxhtml_input_tag_002();
void test_jxhtml_input_tag_003();
void test_jxhtml_input_tag_004();
void test_jxhtml_input_tag_005();
void test_jxhtml_input_tag_006();
void test_jxhtml_input_tag_007();
void test_jxhtml_input_tag_008();
void test_jxhtml_input_tag_009();
void test_jxhtml_input_tag_010();
void test_jxhtml_input_tag_011();
void test_jxhtml_input_tag_012();
void test_jxhtml_input_tag_013();
void test_jxhtml_input_tag_014();
void test_jxhtml_input_tag_015();
void test_jxhtml_input_tag_016();
void test_jxhtml_input_tag_017();
void test_jxhtml_input_tag_018();
void test_jxhtml_input_tag_019();
void test_jxhtml_input_tag_020();
void test_jxhtml_input_tag_021();
void test_jxhtml_input_tag_022();
void test_jxhtml_input_tag_023();
void test_jxhtml_input_tag_024();
void test_jxhtml_input_tag_025();
void test_jxhtml_input_tag_026();
void test_jxhtml_input_tag_027();
void test_jxhtml_input_tag_028();
void test_jxhtml_input_tag_029();
void test_jxhtml_input_tag_030();
void test_jxhtml_input_tag_031();
void test_jxhtml_input_tag_032();
void test_jxhtml_input_tag_033();
void test_jxhtml_input_tag_034();
void test_jxhtml_input_tag_035();
void test_jxhtml_input_tag_036();
void test_jxhtml_input_tag_037();
void test_jxhtml_input_tag_038();
void test_jxhtml_input_tag_039();
void test_jxhtml_input_tag_040();
void test_jxhtml_input_tag_041();
void test_jxhtml_input_tag_042();
void test_jxhtml_input_tag_043();
void test_jxhtml_input_tag_044();
void test_jxhtml_input_tag_045();
void test_jxhtml_input_tag_046();
void test_jxhtml_input_tag_047();

void test_jxhtml_li_tag_001();
void test_jxhtml_li_tag_002();
void test_jxhtml_li_tag_003();
void test_jxhtml_li_tag_004();
void test_jxhtml_li_tag_005();
void test_jxhtml_li_tag_006();
void test_jxhtml_li_tag_007();
void test_jxhtml_li_tag_008();
void test_jxhtml_li_tag_009();
void test_jxhtml_li_tag_010();
void test_jxhtml_li_tag_011();
void test_jxhtml_li_tag_012();
void test_jxhtml_li_tag_013();

void test_jxhtml_menu_tag_001();
void test_jxhtml_menu_tag_002();
void test_jxhtml_menu_tag_003();
void test_jxhtml_menu_tag_004();
void test_jxhtml_menu_tag_005();
void test_jxhtml_menu_tag_006();
void test_jxhtml_menu_tag_007();
void test_jxhtml_menu_tag_008();
void test_jxhtml_menu_tag_009();
void test_jxhtml_menu_tag_010();

void test_jxhtml_ol_tag_001();
void test_jxhtml_ol_tag_002();
void test_jxhtml_ol_tag_003();
void test_jxhtml_ol_tag_004();
void test_jxhtml_ol_tag_005();
void test_jxhtml_ol_tag_006();
void test_jxhtml_ol_tag_007();
void test_jxhtml_ol_tag_008();
void test_jxhtml_ol_tag_009();
void test_jxhtml_ol_tag_010();
void test_jxhtml_ol_tag_011();
void test_jxhtml_ol_tag_012();
void test_jxhtml_ol_tag_013();
void test_jxhtml_ol_tag_014();
void test_jxhtml_ol_tag_015();
void test_jxhtml_ol_tag_016();
void test_jxhtml_ol_tag_017();
void test_jxhtml_ol_tag_018();

void test_jxhtml_option_tag_001();
void test_jxhtml_option_tag_002();
void test_jxhtml_option_tag_003();
void test_jxhtml_option_tag_004();
void test_jxhtml_option_tag_005();
void test_jxhtml_option_tag_006();
void test_jxhtml_option_tag_007();

void test_jxhtml_p_tag_001();
void test_jxhtml_p_tag_002();
void test_jxhtml_p_tag_003();
void test_jxhtml_p_tag_004();
void test_jxhtml_p_tag_005();
void test_jxhtml_p_tag_006();
void test_jxhtml_p_tag_007();
void test_jxhtml_p_tag_008();

void test_jxhtml_plaintext_tag_001();
void test_jxhtml_plaintext_tag_002();
void test_jxhtml_plaintext_tag_003();
void test_jxhtml_plaintext_tag_004();

void test_jxhtml_pre_tag_001();
void test_jxhtml_pre_tag_002();
void test_jxhtml_pre_tag_003();
void test_jxhtml_pre_tag_004();
void test_jxhtml_pre_tag_005();

void test_jxhtml_select_tag_001();
void test_jxhtml_select_tag_002();
void test_jxhtml_select_tag_003();
void test_jxhtml_select_tag_004();
void test_jxhtml_select_tag_005();
void test_jxhtml_select_tag_006();
void test_jxhtml_select_tag_007();
void test_jxhtml_select_tag_008();
void test_jxhtml_select_tag_009();
void test_jxhtml_select_tag_010();
void test_jxhtml_select_tag_011();
void test_jxhtml_select_tag_012();
void test_jxhtml_select_tag_013();
void test_jxhtml_select_tag_014();

void test_jxhtml_textarea_tag_001();
void test_jxhtml_textarea_tag_002();
void test_jxhtml_textarea_tag_003();
void test_jxhtml_textarea_tag_004();
void test_jxhtml_textarea_tag_005();
void test_jxhtml_textarea_tag_006();
void test_jxhtml_textarea_tag_007();
void test_jxhtml_textarea_tag_008();
void test_jxhtml_textarea_tag_009();
void test_jxhtml_textarea_tag_010();
void test_jxhtml_textarea_tag_011();
void test_jxhtml_textarea_tag_012();
void test_jxhtml_textarea_tag_013();
void test_jxhtml_textarea_tag_014();
void test_jxhtml_textarea_tag_015();
void test_jxhtml_textarea_tag_016();
void test_jxhtml_textarea_tag_017();
void test_jxhtml_textarea_tag_018();
void test_jxhtml_textarea_tag_019();
void test_jxhtml_textarea_tag_020();
void test_jxhtml_textarea_tag_021();
void test_jxhtml_textarea_tag_022();
void test_jxhtml_textarea_tag_023();
void test_jxhtml_textarea_tag_024();
void test_jxhtml_textarea_tag_025();
void test_jxhtml_textarea_tag_026();

void test_jxhtml_title_tag_001();
void test_jxhtml_title_tag_002();

void test_jxhtml_ul_tag_001();
void test_jxhtml_ul_tag_002();
void test_jxhtml_ul_tag_003();
void test_jxhtml_ul_tag_004();
void test_jxhtml_ul_tag_005();
void test_jxhtml_ul_tag_006();
void test_jxhtml_ul_tag_007();
void test_jxhtml_ul_tag_008();
void test_jxhtml_ul_tag_009();
void test_jxhtml_ul_tag_010();
void test_jxhtml_ul_tag_011();

void test_jxhtml_blink_tag_001();
void test_jxhtml_blink_tag_002();
void test_jxhtml_blink_tag_003();
void test_jxhtml_blink_tag_004();

void test_jxhtml_marquee_tag_001();
void test_jxhtml_marquee_tag_002();
void test_jxhtml_marquee_tag_003();
void test_jxhtml_marquee_tag_004();
void test_jxhtml_marquee_tag_005();
void test_jxhtml_marquee_tag_006();
void test_jxhtml_marquee_tag_007();
void test_jxhtml_marquee_tag_008();
void test_jxhtml_marquee_tag_009();
void test_jxhtml_marquee_tag_010();
void test_jxhtml_marquee_tag_011();
void test_jxhtml_marquee_tag_012();
void test_jxhtml_marquee_tag_013();
void test_jxhtml_marquee_tag_014();
void test_jxhtml_marquee_tag_015();
void test_jxhtml_marquee_tag_016();
void test_jxhtml_marquee_tag_017();
void test_jxhtml_marquee_tag_018();

void test_jxhtml_meta_tag_001();
void test_jxhtml_meta_tag_002();
void test_jxhtml_meta_tag_003();
void test_jxhtml_meta_tag_004();
void test_jxhtml_meta_tag_005();
void test_jxhtml_meta_tag_006();
void test_jxhtml_meta_tag_007();
void test_jxhtml_meta_tag_008();
void test_jxhtml_meta_tag_009();

void test_jxhtml_font_tag_001();
void test_jxhtml_font_tag_002();
void test_jxhtml_font_tag_003();
void test_jxhtml_font_tag_004();
void test_jxhtml_font_tag_005();
void test_jxhtml_font_tag_006();

void test_jxhtml_param_tag_001();
/* pend */

int
main()
{
  CU_pSuite jxhtml_suite;
  CU_initialize_registry();
  jxhtml_suite = CU_add_suite("test chxj_convert_jxhtml()", NULL, NULL);

  /*=========================================================================*/
  /* <!-- -->                                                                */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test void src1",                                    test_jxhtml_001);
  CU_add_test(jxhtml_suite, "test void src2",                                    test_jxhtml_002);
  CU_add_test(jxhtml_suite, "test comment tag1",                                 test_jxhtml_comment_tag_001);
  /*=========================================================================*/
  /* <A>                                                                     */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test a tag name attr1",                             test_jxhtml_a_tag_name_attribute_001);
  CU_add_test(jxhtml_suite, "test a tag name attr2",                             test_jxhtml_a_tag_name_attribute_002);
  CU_add_test(jxhtml_suite, "test a tag name attr3 with japanese.",              test_jxhtml_a_tag_name_attribute_003);
  CU_add_test(jxhtml_suite, "test a tag name attr4 with japanese.",              test_jxhtml_a_tag_name_attribute_004);
  CU_add_test(jxhtml_suite, "test a tag href attr1 with void attribute.",        test_jxhtml_a_tag_href_attribute_001);
  CU_add_test(jxhtml_suite, "test a tag href attr2 with other site link.",       test_jxhtml_a_tag_href_attribute_002);
  CU_add_test(jxhtml_suite, "test a tag href attr3 with local link.",            test_jxhtml_a_tag_href_attribute_003);
  CU_add_test(jxhtml_suite, "test a tag href attr4 with maker.",                 test_jxhtml_a_tag_href_attribute_004);
  CU_add_test(jxhtml_suite, "test a tag href attr5 with void maker.",            test_jxhtml_a_tag_href_attribute_005);
  CU_add_test(jxhtml_suite, "test a tag href attr6 with no cookie.",             test_jxhtml_a_tag_href_attribute_006);
  CU_add_test(jxhtml_suite, "test a tag accesskey attribute.",                   test_jxhtml_a_tag_accesskey_attribute_001);
  CU_add_test(jxhtml_suite, "test a tag accesskey attribute with void char.",    test_jxhtml_a_tag_accesskey_attribute_002);
  CU_add_test(jxhtml_suite, "test a tag accesskey attribute with no value",      test_jxhtml_a_tag_accesskey_attribute_003);
  /*=========================================================================*/
  /* <BASE>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test base tag no attribute.",                       test_jxhtml_base_tag_001);
  CU_add_test(jxhtml_suite, "test base tag href attribute with no value.",       test_jxhtml_base_tag_href_attribute_001);
  CU_add_test(jxhtml_suite, "test base tag href attribute with void value.",     test_jxhtml_base_tag_href_attribute_002);
  CU_add_test(jxhtml_suite, "test base tag href attribute with normal value 1.", test_jxhtml_base_tag_href_attribute_003);
  CU_add_test(jxhtml_suite, "test base tag href attribute with normal value 2.", test_jxhtml_base_tag_href_attribute_004);
  /*=========================================================================*/
  /* <BLOCKQUOTE>                                                            */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <blockquote> with void value.",                test_jxhtml_blockquote_tag_001);
  CU_add_test(jxhtml_suite, "test <blockquote> with value.",                     test_jxhtml_blockquote_tag_002);
  CU_add_test(jxhtml_suite, "test <blockquote> with japanese value.",            test_jxhtml_blockquote_tag_003);
  CU_add_test(jxhtml_suite, "test <blockquote> with hankaku kana value.",        test_jxhtml_blockquote_tag_004);
  /*=========================================================================*/
  /* <BODY>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <body> .",                                     test_jxhtml_body_tag_001);
  CU_add_test(jxhtml_suite, "test <body> with bgcolor attribute 1.",             test_jxhtml_body_tag_002);
  CU_add_test(jxhtml_suite, "test <body> with bgcolor attribute 2.",             test_jxhtml_body_tag_003);
  CU_add_test(jxhtml_suite, "test <body> with bgcolor attribute 3.",             test_jxhtml_body_tag_004);
  CU_add_test(jxhtml_suite, "test <body> with text attribute 1.",                test_jxhtml_body_tag_005);
  CU_add_test(jxhtml_suite, "test <body> with text attribute 2.",                test_jxhtml_body_tag_006);
  CU_add_test(jxhtml_suite, "test <body> with text attribute 3.",                test_jxhtml_body_tag_007);
  CU_add_test(jxhtml_suite, "test <body> with link attribute 1.",                test_jxhtml_body_tag_008);
  CU_add_test(jxhtml_suite, "test <body> with link attribute 2.",                test_jxhtml_body_tag_009);
  CU_add_test(jxhtml_suite, "test <body> with link attribute 3.",                test_jxhtml_body_tag_010);
  CU_add_test(jxhtml_suite, "test <body> with vlink attribute 1.",               test_jxhtml_body_tag_011);
  CU_add_test(jxhtml_suite, "test <body> with vlink attribute 2.",               test_jxhtml_body_tag_012);
  CU_add_test(jxhtml_suite, "test <body> with vlink attribute 3.",               test_jxhtml_body_tag_013);
  CU_add_test(jxhtml_suite, "test <body> with alink attribute.",                 test_jxhtml_body_tag_014);
  CU_add_test(jxhtml_suite, "test <body> with unknown attribute.",               test_jxhtml_body_tag_015);
  /*=========================================================================*/
  /* <BR>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <br>.",                                        test_jxhtml_br_tag_001);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(left).",             test_jxhtml_br_tag_002);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(right).",            test_jxhtml_br_tag_003);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(all).",              test_jxhtml_br_tag_004);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(void).",             test_jxhtml_br_tag_005);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(no value).",         test_jxhtml_br_tag_006);
  CU_add_test(jxhtml_suite, "test <br> with clear attribute(unknown value).",    test_jxhtml_br_tag_007);
  /*=========================================================================*/
  /* <CENTER>                                                                */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <center>.",                                    test_jxhtml_center_tag_001);
  /*=========================================================================*/
  /* <DIR>                                                                   */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <dir>.",                                       test_jxhtml_dir_tag_001);
  CU_add_test(jxhtml_suite, "test <dir> with no <li>.",                          test_jxhtml_dir_tag_002);
  CU_add_test(jxhtml_suite, "test <dir type> 1",                                 test_jxhtml_dir_tag_003);
  CU_add_test(jxhtml_suite, "test <dir type> 2",                                 test_jxhtml_dir_tag_004);
  CU_add_test(jxhtml_suite, "test <dir type> 3",                                 test_jxhtml_dir_tag_005);
  CU_add_test(jxhtml_suite, "test <dir type> 4",                                 test_jxhtml_dir_tag_006);
  CU_add_test(jxhtml_suite, "test <dir type> 5",                                 test_jxhtml_dir_tag_007);
  /*=========================================================================*/
  /* <DL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <dl>.",                                        test_jxhtml_dl_tag_001);
  /*=========================================================================*/
  /* <DT>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <dt>.",                                        test_jxhtml_dt_tag_001);
  CU_add_test(jxhtml_suite, "test <dt> with void value.",                        test_jxhtml_dt_tag_002);
  /*=========================================================================*/
  /* <DD>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <dd>.",                                        test_jxhtml_dd_tag_001);
  CU_add_test(jxhtml_suite, "test <dd> with void value.",                        test_jxhtml_dd_tag_002);
  /*=========================================================================*/
  /* <DIV>                                                                   */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <div>.",                                       test_jxhtml_div_tag_001);
  CU_add_test(jxhtml_suite, "test <div> with align attribute(left).",            test_jxhtml_div_tag_002);
  CU_add_test(jxhtml_suite, "test <div> with align attribute(right).",           test_jxhtml_div_tag_003);
  CU_add_test(jxhtml_suite, "test <div> with align attribute(center).",          test_jxhtml_div_tag_004);
  CU_add_test(jxhtml_suite, "test <div> with align attribute(void).",            test_jxhtml_div_tag_005);
  CU_add_test(jxhtml_suite, "test <div> with align attribute(unknown).",         test_jxhtml_div_tag_006);
  CU_add_test(jxhtml_suite, "test <div> with style attribute.",                  test_jxhtml_div_tag_007);
  /*=========================================================================*/
  /* <FORM>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <form>.",                                      test_jxhtml_form_tag_001);
  CU_add_test(jxhtml_suite, "test <form method>.",                               test_jxhtml_form_tag_002);
  CU_add_test(jxhtml_suite, "test <form method=\"post\">.",                      test_jxhtml_form_tag_003);
  CU_add_test(jxhtml_suite, "test <form method=\"get\">.",                       test_jxhtml_form_tag_004);
  CU_add_test(jxhtml_suite, "test <form method=\"abc\">.",                       test_jxhtml_form_tag_005);
  CU_add_test(jxhtml_suite, "test <form action>.",                               test_jxhtml_form_tag_006);
  CU_add_test(jxhtml_suite, "test <form action> with null cookie.",              test_jxhtml_form_tag_007);
  CU_add_test(jxhtml_suite, "test <form action> with other site .",              test_jxhtml_form_tag_008);
  CU_add_test(jxhtml_suite, "test <form action method>.",                        test_jxhtml_form_tag_009);
  /*=========================================================================*/
  /* <HEAD>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <head>.",                                      test_jxhtml_head_tag_001);
  CU_add_test(jxhtml_suite, "test <head> with value.",                           test_jxhtml_head_tag_002);
  /*=========================================================================*/
  /* <H1>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h1>.",                                        test_jxhtml_h1_tag_001);
  CU_add_test(jxhtml_suite, "test <h1> with value.",                             test_jxhtml_h1_tag_002);
  CU_add_test(jxhtml_suite, "test <h1> with value(japanese).",                   test_jxhtml_h1_tag_003);
  CU_add_test(jxhtml_suite, "test <h1> with value(japanese-hankaku).",           test_jxhtml_h1_tag_004);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(non).",              test_jxhtml_h1_tag_005);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(void).",             test_jxhtml_h1_tag_006);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(left).",             test_jxhtml_h1_tag_007);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(right).",            test_jxhtml_h1_tag_008);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(center).",           test_jxhtml_h1_tag_009);
  CU_add_test(jxhtml_suite, "test <h1> with align attribute(unkown).",           test_jxhtml_h1_tag_010);
  /*=========================================================================*/
  /* <H2>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h2>.",                                        test_jxhtml_h2_tag_001);
  CU_add_test(jxhtml_suite, "test <h2> with value.",                             test_jxhtml_h2_tag_002);
  CU_add_test(jxhtml_suite, "test <h2> with value(japanese).",                   test_jxhtml_h2_tag_003);
  CU_add_test(jxhtml_suite, "test <h2> with value(japanese-hankaku).",           test_jxhtml_h2_tag_004);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(non).",              test_jxhtml_h2_tag_005);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(void).",             test_jxhtml_h2_tag_006);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(left).",             test_jxhtml_h2_tag_007);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(right).",            test_jxhtml_h2_tag_008);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(center).",           test_jxhtml_h2_tag_009);
  CU_add_test(jxhtml_suite, "test <h2> with align attribute(unkown).",           test_jxhtml_h2_tag_010);
  /*=========================================================================*/
  /* <H3>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h3>.",                                        test_jxhtml_h3_tag_001);
  CU_add_test(jxhtml_suite, "test <h3> with value.",                             test_jxhtml_h3_tag_002);
  CU_add_test(jxhtml_suite, "test <h3> with value(japanese).",                   test_jxhtml_h3_tag_003);
  CU_add_test(jxhtml_suite, "test <h3> with value(japanese-hankaku).",           test_jxhtml_h3_tag_004);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(non).",              test_jxhtml_h3_tag_005);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(void).",             test_jxhtml_h3_tag_006);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(left).",             test_jxhtml_h3_tag_007);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(right).",            test_jxhtml_h3_tag_008);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(center).",           test_jxhtml_h3_tag_009);
  CU_add_test(jxhtml_suite, "test <h3> with align attribute(unkown).",           test_jxhtml_h3_tag_010);
  /*=========================================================================*/
  /* <H4>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h4>.",                                        test_jxhtml_h4_tag_001);
  CU_add_test(jxhtml_suite, "test <h4> with value.",                             test_jxhtml_h4_tag_002);
  CU_add_test(jxhtml_suite, "test <h4> with value(japanese).",                   test_jxhtml_h4_tag_003);
  CU_add_test(jxhtml_suite, "test <h4> with value(japanese-hankaku).",           test_jxhtml_h4_tag_004);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(non).",              test_jxhtml_h4_tag_005);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(void).",             test_jxhtml_h4_tag_006);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(left).",             test_jxhtml_h4_tag_007);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(right).",            test_jxhtml_h4_tag_008);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(center).",           test_jxhtml_h4_tag_009);
  CU_add_test(jxhtml_suite, "test <h4> with align attribute(unkown).",           test_jxhtml_h4_tag_010);
  /*=========================================================================*/
  /* <H5>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h5>.",                                        test_jxhtml_h5_tag_001);
  CU_add_test(jxhtml_suite, "test <h5> with value.",                             test_jxhtml_h5_tag_002);
  CU_add_test(jxhtml_suite, "test <h5> with value(japanese).",                   test_jxhtml_h5_tag_003);
  CU_add_test(jxhtml_suite, "test <h5> with value(japanese-hankaku).",           test_jxhtml_h5_tag_004);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(non).",              test_jxhtml_h5_tag_005);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(void).",             test_jxhtml_h5_tag_006);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(left).",             test_jxhtml_h5_tag_007);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(right).",            test_jxhtml_h5_tag_008);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(center).",           test_jxhtml_h5_tag_009);
  CU_add_test(jxhtml_suite, "test <h5> with align attribute(unkown).",           test_jxhtml_h5_tag_010);
  /*=========================================================================*/
  /* <H6>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <h6>.",                                        test_jxhtml_h6_tag_001);
  CU_add_test(jxhtml_suite, "test <h6> with value.",                             test_jxhtml_h6_tag_002);
  CU_add_test(jxhtml_suite, "test <h6> with value(japanese).",                   test_jxhtml_h6_tag_003);
  CU_add_test(jxhtml_suite, "test <h6> with value(japanese-hankaku).",           test_jxhtml_h6_tag_004);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(non).",              test_jxhtml_h6_tag_005);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(void).",             test_jxhtml_h6_tag_006);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(left).",             test_jxhtml_h6_tag_007);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(right).",            test_jxhtml_h6_tag_008);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(center).",           test_jxhtml_h6_tag_009);
  CU_add_test(jxhtml_suite, "test <h6> with align attribute(unkown).",           test_jxhtml_h6_tag_010);
  /*=========================================================================*/
  /* <HR>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <hr>.",                                        test_jxhtml_hr_tag_001);
  CU_add_test(jxhtml_suite, "test <hr />.",                                      test_jxhtml_hr_tag_002);
  CU_add_test(jxhtml_suite, "test <hr align> with no value.",                    test_jxhtml_hr_tag_003);
  CU_add_test(jxhtml_suite, "test <hr align> with void value.",                  test_jxhtml_hr_tag_004);
  CU_add_test(jxhtml_suite, "test <hr align> with value(left).",                 test_jxhtml_hr_tag_005);
  CU_add_test(jxhtml_suite, "test <hr align> with value(right).",                test_jxhtml_hr_tag_006);
  CU_add_test(jxhtml_suite, "test <hr align> with value(center).",               test_jxhtml_hr_tag_007);
  CU_add_test(jxhtml_suite, "test <hr align> with value(unknown).",              test_jxhtml_hr_tag_008);
  CU_add_test(jxhtml_suite, "test <hr size> with no value.",                     test_jxhtml_hr_tag_009);
  CU_add_test(jxhtml_suite, "test <hr size> with void value.",                   test_jxhtml_hr_tag_010);
  CU_add_test(jxhtml_suite, "test <hr size> with non numeric value.",            test_jxhtml_hr_tag_011);
  CU_add_test(jxhtml_suite, "test <hr size> with numeric value.",                test_jxhtml_hr_tag_012);
  CU_add_test(jxhtml_suite, "test <hr width> with no value.",                    test_jxhtml_hr_tag_013);
  CU_add_test(jxhtml_suite, "test <hr width> with void value.",                  test_jxhtml_hr_tag_014);
  CU_add_test(jxhtml_suite, "test <hr width> with non numeric value.",           test_jxhtml_hr_tag_015);
  CU_add_test(jxhtml_suite, "test <hr width> with numeric value.",               test_jxhtml_hr_tag_016);
  CU_add_test(jxhtml_suite, "test <hr noshade>.",                                test_jxhtml_hr_tag_017);
  CU_add_test(jxhtml_suite, "test <hr color> 1.",                                test_jxhtml_hr_tag_018);
  CU_add_test(jxhtml_suite, "test <hr color> 2.",                                test_jxhtml_hr_tag_018_1);
  CU_add_test(jxhtml_suite, "test <hr color> 3.",                                test_jxhtml_hr_tag_018_2);
  /*=========================================================================*/
  /* <HTML>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <html>.",                                      test_jxhtml_html_tag_001);
  /*=========================================================================*/
  /* <IMG>                                                                   */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <img>." ,                                      test_jxhtml_img_tag_001);
  CU_add_test(jxhtml_suite, "test <img src> with no value." ,                    test_jxhtml_img_tag_002);
  CU_add_test(jxhtml_suite, "test <img src> with void value." ,                  test_jxhtml_img_tag_003);
  CU_add_test(jxhtml_suite, "test <img src> with alphabetic value." ,            test_jxhtml_img_tag_004);
  CU_add_test(jxhtml_suite, "test <img src> with japanese value." ,              test_jxhtml_img_tag_005);
  CU_add_test(jxhtml_suite, "test <img src> with japanese-hankaku value." ,      test_jxhtml_img_tag_006);
  CU_add_test(jxhtml_suite, "test <img align> with no value." ,                  test_jxhtml_img_tag_007);
  CU_add_test(jxhtml_suite, "test <img align> with void value." ,                test_jxhtml_img_tag_008);
  CU_add_test(jxhtml_suite, "test <img align> with value(top)." ,                test_jxhtml_img_tag_009);
  CU_add_test(jxhtml_suite, "test <img align> with value(middle)." ,             test_jxhtml_img_tag_020);
  CU_add_test(jxhtml_suite, "test <img align> with value(bottom)." ,             test_jxhtml_img_tag_011);
  CU_add_test(jxhtml_suite, "test <img align> with value(left)." ,               test_jxhtml_img_tag_012);
  CU_add_test(jxhtml_suite, "test <img align> with value(right)." ,              test_jxhtml_img_tag_013);
  CU_add_test(jxhtml_suite, "test <img align> with value(center)." ,             test_jxhtml_img_tag_013_1);
  CU_add_test(jxhtml_suite, "test <img align> with value(unkown)." ,             test_jxhtml_img_tag_014);
  CU_add_test(jxhtml_suite, "test <img width> with no value." ,                  test_jxhtml_img_tag_015);
  CU_add_test(jxhtml_suite, "test <img width> with void value." ,                test_jxhtml_img_tag_016);
  CU_add_test(jxhtml_suite, "test <img width> with alphabetic value." ,          test_jxhtml_img_tag_017);
  CU_add_test(jxhtml_suite, "test <img width> with numeric value." ,             test_jxhtml_img_tag_018);
  CU_add_test(jxhtml_suite, "test <img width> with percentage value." ,          test_jxhtml_img_tag_019);
  CU_add_test(jxhtml_suite, "test <img height> with no value." ,                 test_jxhtml_img_tag_020);
  CU_add_test(jxhtml_suite, "test <img height> with void value." ,               test_jxhtml_img_tag_021);
  CU_add_test(jxhtml_suite, "test <img height> with alphabetic value." ,         test_jxhtml_img_tag_022);
  CU_add_test(jxhtml_suite, "test <img height> with numeric value." ,            test_jxhtml_img_tag_023);
  CU_add_test(jxhtml_suite, "test <img height> with percentage value." ,         test_jxhtml_img_tag_024);
  CU_add_test(jxhtml_suite, "test <img hspace> with no value." ,                 test_jxhtml_img_tag_025);
  CU_add_test(jxhtml_suite, "test <img hspace> with void value." ,               test_jxhtml_img_tag_026);
  CU_add_test(jxhtml_suite, "test <img hspace> with alphabetic value." ,         test_jxhtml_img_tag_027);
  CU_add_test(jxhtml_suite, "test <img hspace> with numeric value." ,            test_jxhtml_img_tag_028);
  CU_add_test(jxhtml_suite, "test <img vspace> with no value." ,                 test_jxhtml_img_tag_029);
  CU_add_test(jxhtml_suite, "test <img vspace> with void value." ,               test_jxhtml_img_tag_030);
  CU_add_test(jxhtml_suite, "test <img vspace> with alphabetic value." ,         test_jxhtml_img_tag_031);
  CU_add_test(jxhtml_suite, "test <img vspace> with numeric value." ,            test_jxhtml_img_tag_032);
  CU_add_test(jxhtml_suite, "test <img alt>." ,                                  test_jxhtml_img_tag_033);
  CU_add_test(jxhtml_suite, "test <img alt> with void value." ,                  test_jxhtml_img_tag_034);
  CU_add_test(jxhtml_suite, "test <img alt> with alphabetic value." ,            test_jxhtml_img_tag_035);
  CU_add_test(jxhtml_suite, "test <img alt> with japanese value." ,              test_jxhtml_img_tag_036);
  CU_add_test(jxhtml_suite, "test <img alt> with japanese-hankaku value." ,      test_jxhtml_img_tag_037);
  /*=========================================================================*/
  /* <INPUT>                                                                 */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <input>." ,                                    test_jxhtml_input_tag_001);
  CU_add_test(jxhtml_suite, "test <input type>." ,                               test_jxhtml_input_tag_002);
  CU_add_test(jxhtml_suite, "test <input type> with void value." ,               test_jxhtml_input_tag_003);
  CU_add_test(jxhtml_suite, "test <input type> with value(text)." ,              test_jxhtml_input_tag_004);
  CU_add_test(jxhtml_suite, "test <input type> with value(password)." ,          test_jxhtml_input_tag_005);
  CU_add_test(jxhtml_suite, "test <input type> with value(checkbox)." ,          test_jxhtml_input_tag_006);
  CU_add_test(jxhtml_suite, "test <input type> with value(radio)." ,             test_jxhtml_input_tag_007);
  CU_add_test(jxhtml_suite, "test <input type> with value(hidden)." ,            test_jxhtml_input_tag_008);
  CU_add_test(jxhtml_suite, "test <input type> with value(submit)." ,            test_jxhtml_input_tag_009);
  CU_add_test(jxhtml_suite, "test <input type> with value(reset)." ,             test_jxhtml_input_tag_010);
  CU_add_test(jxhtml_suite, "test <input type> with value(unknown)." ,           test_jxhtml_input_tag_011);
  CU_add_test(jxhtml_suite, "test <input name>." ,                               test_jxhtml_input_tag_012);
  CU_add_test(jxhtml_suite, "test <input name> with void value." ,               test_jxhtml_input_tag_013);
  CU_add_test(jxhtml_suite, "test <input name> with alphabetic value." ,         test_jxhtml_input_tag_014);
  CU_add_test(jxhtml_suite, "test <input name> with japanese value." ,           test_jxhtml_input_tag_015);
  CU_add_test(jxhtml_suite, "test <input name> with japanese-hankaku value." ,   test_jxhtml_input_tag_016);
  CU_add_test(jxhtml_suite, "test <input value>." ,                              test_jxhtml_input_tag_017);
  CU_add_test(jxhtml_suite, "test <input value> with void value." ,              test_jxhtml_input_tag_018);
  CU_add_test(jxhtml_suite, "test <input value> with alphabetic value." ,        test_jxhtml_input_tag_019);
  CU_add_test(jxhtml_suite, "test <input value> with japanese value." ,          test_jxhtml_input_tag_020);
  CU_add_test(jxhtml_suite, "test <input value> with japanese-hankaku value." ,  test_jxhtml_input_tag_021);
  CU_add_test(jxhtml_suite, "test <input size>." ,                               test_jxhtml_input_tag_022);
  CU_add_test(jxhtml_suite, "test <input size> with void value." ,               test_jxhtml_input_tag_023);
  CU_add_test(jxhtml_suite, "test <input size> with alphabetic value." ,         test_jxhtml_input_tag_024);
  CU_add_test(jxhtml_suite, "test <input size> with numeric value." ,            test_jxhtml_input_tag_025);
  CU_add_test(jxhtml_suite, "test <input maxlength>." ,                          test_jxhtml_input_tag_026);
  CU_add_test(jxhtml_suite, "test <input maxlength> with void value." ,          test_jxhtml_input_tag_027);
  CU_add_test(jxhtml_suite, "test <input maxlength> with alphabetic value." ,    test_jxhtml_input_tag_028);
  CU_add_test(jxhtml_suite, "test <input maxlength> with numeric value." ,       test_jxhtml_input_tag_029);
  CU_add_test(jxhtml_suite, "test <input checked>." ,                            test_jxhtml_input_tag_030);
  CU_add_test(jxhtml_suite, "test <input accesskey>." ,                          test_jxhtml_input_tag_031);
  CU_add_test(jxhtml_suite, "test <input accesskey> with void value." ,          test_jxhtml_input_tag_032);
  CU_add_test(jxhtml_suite, "test <input accesskey> with value." ,               test_jxhtml_input_tag_033);
  CU_add_test(jxhtml_suite, "test <input istyle> 1." ,                           test_jxhtml_input_tag_034);
  CU_add_test(jxhtml_suite, "test <input istyle> 2." ,                           test_jxhtml_input_tag_035);
  CU_add_test(jxhtml_suite, "test <input istyle> 3." ,                           test_jxhtml_input_tag_036);
  CU_add_test(jxhtml_suite, "test <input istyle> 4." ,                           test_jxhtml_input_tag_037);
  CU_add_test(jxhtml_suite, "test <input istyle> 5." ,                           test_jxhtml_input_tag_038);
  CU_add_test(jxhtml_suite, "test <input istyle> 6." ,                           test_jxhtml_input_tag_039);
  CU_add_test(jxhtml_suite, "test <input istyle> 7." ,                           test_jxhtml_input_tag_040);
  CU_add_test(jxhtml_suite, "test <input istyle> 8." ,                           test_jxhtml_input_tag_041);
  CU_add_test(jxhtml_suite, "test <input istyle> 9." ,                           test_jxhtml_input_tag_042);
  CU_add_test(jxhtml_suite, "test <input istyle> 10." ,                          test_jxhtml_input_tag_043);
  CU_add_test(jxhtml_suite, "test <input istyle> 11." ,                          test_jxhtml_input_tag_044);
  CU_add_test(jxhtml_suite, "test <input istyle> 12." ,                          test_jxhtml_input_tag_045);
  CU_add_test(jxhtml_suite, "test <input istyle> 13." ,                          test_jxhtml_input_tag_046);
  CU_add_test(jxhtml_suite, "test <input istyle> 14." ,                          test_jxhtml_input_tag_047);
  /*=========================================================================*/
  /* <LI>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <li>." ,                                       test_jxhtml_li_tag_001);
  CU_add_test(jxhtml_suite, "test <li> with alphabetic value." ,                 test_jxhtml_li_tag_002);
  CU_add_test(jxhtml_suite, "test <li> with japanese value." ,                   test_jxhtml_li_tag_003);
  CU_add_test(jxhtml_suite, "test <li> with japanese-hankaku value." ,           test_jxhtml_li_tag_004);
  CU_add_test(jxhtml_suite, "test <li> type attribute 1." ,                      test_jxhtml_li_tag_005);
  CU_add_test(jxhtml_suite, "test <li> type attribute 2." ,                      test_jxhtml_li_tag_006);
  CU_add_test(jxhtml_suite, "test <li> type attribute 3." ,                      test_jxhtml_li_tag_007);
  CU_add_test(jxhtml_suite, "test <li> type attribute 4." ,                      test_jxhtml_li_tag_008);
  CU_add_test(jxhtml_suite, "test <li> type attribute 5." ,                      test_jxhtml_li_tag_009);
  CU_add_test(jxhtml_suite, "test <li> type attribute 6." ,                      test_jxhtml_li_tag_010);
  CU_add_test(jxhtml_suite, "test <li> type attribute 7." ,                      test_jxhtml_li_tag_011);
  CU_add_test(jxhtml_suite, "test <li> type attribute 8." ,                      test_jxhtml_li_tag_012);
  CU_add_test(jxhtml_suite, "test <li> type attribute 9." ,                      test_jxhtml_li_tag_013);
  /*=========================================================================*/
  /* <MENU>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <menu>." ,                                     test_jxhtml_menu_tag_001);
  CU_add_test(jxhtml_suite, "test <menu> 2." ,                                   test_jxhtml_menu_tag_002);
  CU_add_test(jxhtml_suite, "test <menu> 3." ,                                   test_jxhtml_menu_tag_003);
  CU_add_test(jxhtml_suite, "test <menu> 4." ,                                   test_jxhtml_menu_tag_004);
  CU_add_test(jxhtml_suite, "test <menu> 5." ,                                   test_jxhtml_menu_tag_005);
  CU_add_test(jxhtml_suite, "test <menu> 6." ,                                   test_jxhtml_menu_tag_006);
  CU_add_test(jxhtml_suite, "test <menu> 7." ,                                   test_jxhtml_menu_tag_007);
  CU_add_test(jxhtml_suite, "test <menu> 8." ,                                   test_jxhtml_menu_tag_008);
  CU_add_test(jxhtml_suite, "test <menu> 9." ,                                   test_jxhtml_menu_tag_009);
  CU_add_test(jxhtml_suite, "test <menu> 10." ,                                  test_jxhtml_menu_tag_010);
  /*=========================================================================*/
  /* <OL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <ol>." ,                                       test_jxhtml_ol_tag_001);
  CU_add_test(jxhtml_suite, "test <ol> 2." ,                                     test_jxhtml_ol_tag_002);
  CU_add_test(jxhtml_suite, "test <ol> 3." ,                                     test_jxhtml_ol_tag_003);
  CU_add_test(jxhtml_suite, "test <ol> 4." ,                                     test_jxhtml_ol_tag_004);
  CU_add_test(jxhtml_suite, "test <ol> 5." ,                                     test_jxhtml_ol_tag_005);
  CU_add_test(jxhtml_suite, "test <ol> 6." ,                                     test_jxhtml_ol_tag_006);
  CU_add_test(jxhtml_suite, "test <ol> 7." ,                                     test_jxhtml_ol_tag_007);
  CU_add_test(jxhtml_suite, "test <ol> 8." ,                                     test_jxhtml_ol_tag_008);
  CU_add_test(jxhtml_suite, "test <ol> 9." ,                                     test_jxhtml_ol_tag_009);
  CU_add_test(jxhtml_suite, "test <ol> 10." ,                                    test_jxhtml_ol_tag_010);
  CU_add_test(jxhtml_suite, "test <ol> 11." ,                                    test_jxhtml_ol_tag_011);
  CU_add_test(jxhtml_suite, "test <ol> 12." ,                                    test_jxhtml_ol_tag_012);
  CU_add_test(jxhtml_suite, "test <ol> 13." ,                                    test_jxhtml_ol_tag_013);
  CU_add_test(jxhtml_suite, "test <ol> 14." ,                                    test_jxhtml_ol_tag_014);
  CU_add_test(jxhtml_suite, "test <ol> 15." ,                                    test_jxhtml_ol_tag_015);
  CU_add_test(jxhtml_suite, "test <ol> 16." ,                                    test_jxhtml_ol_tag_016);
  CU_add_test(jxhtml_suite, "test <ol> 17." ,                                    test_jxhtml_ol_tag_017);
  CU_add_test(jxhtml_suite, "test <ol> 18." ,                                    test_jxhtml_ol_tag_018);
  /*=========================================================================*/
  /* <OPTION>                                                                */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <option>." ,                                   test_jxhtml_option_tag_001);
  CU_add_test(jxhtml_suite, "test <option value> with no value." ,               test_jxhtml_option_tag_002);
  CU_add_test(jxhtml_suite, "test <option value> with void value." ,             test_jxhtml_option_tag_003);
  CU_add_test(jxhtml_suite, "test <option value> with alphabetic value." ,       test_jxhtml_option_tag_004);
  CU_add_test(jxhtml_suite, "test <option value> with japanese value." ,         test_jxhtml_option_tag_005);
  CU_add_test(jxhtml_suite, "test <option value> with japanese-kana value." ,    test_jxhtml_option_tag_006);
  CU_add_test(jxhtml_suite, "test <option selected>." ,                          test_jxhtml_option_tag_007);
  /*=========================================================================*/
  /* <P>                                                                     */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <p> 1." ,                                      test_jxhtml_p_tag_001);
  CU_add_test(jxhtml_suite, "test <p> 2." ,                                      test_jxhtml_p_tag_002);
  CU_add_test(jxhtml_suite, "test <p> 3." ,                                      test_jxhtml_p_tag_003);
  CU_add_test(jxhtml_suite, "test <p> 4." ,                                      test_jxhtml_p_tag_004);
  CU_add_test(jxhtml_suite, "test <p> 5." ,                                      test_jxhtml_p_tag_005);
  CU_add_test(jxhtml_suite, "test <p> 6." ,                                      test_jxhtml_p_tag_006);
  CU_add_test(jxhtml_suite, "test <p> 7." ,                                      test_jxhtml_p_tag_007);
  CU_add_test(jxhtml_suite, "test <p> 8." ,                                      test_jxhtml_p_tag_008);
  /*=========================================================================*/
  /* <PLAINTEXT>                                                             */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <plaintext> 1." ,                              test_jxhtml_plaintext_tag_001);
  CU_add_test(jxhtml_suite, "test <plaintext> 2." ,                              test_jxhtml_plaintext_tag_002);
  CU_add_test(jxhtml_suite, "test <plaintext> 3. value is not closed." ,         test_jxhtml_plaintext_tag_003);
  CU_add_test(jxhtml_suite, "test <plaintext> 3. value is not closed and hankaku." , test_jxhtml_plaintext_tag_004);
  /*=========================================================================*/
  /* <PRE>                                                                   */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <pre> 1." ,                                    test_jxhtml_pre_tag_001);
  CU_add_test(jxhtml_suite, "test <pre> 2." ,                                    test_jxhtml_pre_tag_002);
  CU_add_test(jxhtml_suite, "test <pre> 3." ,                                    test_jxhtml_pre_tag_003);
  CU_add_test(jxhtml_suite, "test <pre> 4." ,                                    test_jxhtml_pre_tag_004);
  CU_add_test(jxhtml_suite, "test <pre> 5." ,                                    test_jxhtml_pre_tag_005);
  /*=========================================================================*/
  /* <SELECT>                                                                */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <select> 1." ,                                 test_jxhtml_select_tag_001);
  CU_add_test(jxhtml_suite, "test <select> 2." ,                                 test_jxhtml_select_tag_002);
  CU_add_test(jxhtml_suite, "test <select> 3." ,                                 test_jxhtml_select_tag_003);
  CU_add_test(jxhtml_suite, "test <select> 4." ,                                 test_jxhtml_select_tag_004);
  CU_add_test(jxhtml_suite, "test <select> 5." ,                                 test_jxhtml_select_tag_005);
  CU_add_test(jxhtml_suite, "test <select> 6." ,                                 test_jxhtml_select_tag_006);
  CU_add_test(jxhtml_suite, "test <select> 7." ,                                 test_jxhtml_select_tag_007);
  CU_add_test(jxhtml_suite, "test <select> 8." ,                                 test_jxhtml_select_tag_008);
  CU_add_test(jxhtml_suite, "test <select> 9." ,                                 test_jxhtml_select_tag_009);
  CU_add_test(jxhtml_suite, "test <select> 10." ,                                test_jxhtml_select_tag_010);
  CU_add_test(jxhtml_suite, "test <select> 11." ,                                test_jxhtml_select_tag_011);
  CU_add_test(jxhtml_suite, "test <select> 12." ,                                test_jxhtml_select_tag_012);
  CU_add_test(jxhtml_suite, "test <select> 13." ,                                test_jxhtml_select_tag_013);
  CU_add_test(jxhtml_suite, "test <select> 14." ,                                test_jxhtml_select_tag_014);
  /*=========================================================================*/
  /* <TEXTAREA>                                                              */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <textarea> 1." ,                               test_jxhtml_textarea_tag_001);
  CU_add_test(jxhtml_suite, "test <textarea> 2." ,                               test_jxhtml_textarea_tag_002);
  CU_add_test(jxhtml_suite, "test <textarea> 3." ,                               test_jxhtml_textarea_tag_003);
  CU_add_test(jxhtml_suite, "test <textarea> 4." ,                               test_jxhtml_textarea_tag_004);
  CU_add_test(jxhtml_suite, "test <textarea> 5." ,                               test_jxhtml_textarea_tag_005);
  CU_add_test(jxhtml_suite, "test <textarea> 6." ,                               test_jxhtml_textarea_tag_006);
  CU_add_test(jxhtml_suite, "test <textarea> 7." ,                               test_jxhtml_textarea_tag_007);
  CU_add_test(jxhtml_suite, "test <textarea> 8." ,                               test_jxhtml_textarea_tag_008);
  CU_add_test(jxhtml_suite, "test <textarea> 9." ,                               test_jxhtml_textarea_tag_009);
  CU_add_test(jxhtml_suite, "test <textarea> 10." ,                              test_jxhtml_textarea_tag_010);
  CU_add_test(jxhtml_suite, "test <textarea> 11." ,                              test_jxhtml_textarea_tag_011);
  CU_add_test(jxhtml_suite, "test <textarea> 12." ,                              test_jxhtml_textarea_tag_012);
  CU_add_test(jxhtml_suite, "test <textarea> 13." ,                              test_jxhtml_textarea_tag_013);
  CU_add_test(jxhtml_suite, "test <textarea> 14." ,                              test_jxhtml_textarea_tag_014);
  CU_add_test(jxhtml_suite, "test <textarea> 15." ,                              test_jxhtml_textarea_tag_015);
  CU_add_test(jxhtml_suite, "test <textarea> 16." ,                              test_jxhtml_textarea_tag_016);
  CU_add_test(jxhtml_suite, "test <textarea> 17." ,                              test_jxhtml_textarea_tag_017);
  CU_add_test(jxhtml_suite, "test <textarea> 18." ,                              test_jxhtml_textarea_tag_018);
  CU_add_test(jxhtml_suite, "test <textarea> 19." ,                              test_jxhtml_textarea_tag_019);
  CU_add_test(jxhtml_suite, "test <textarea> 20." ,                              test_jxhtml_textarea_tag_020);
  CU_add_test(jxhtml_suite, "test <textarea> 21." ,                              test_jxhtml_textarea_tag_021);
  CU_add_test(jxhtml_suite, "test <textarea> 22." ,                              test_jxhtml_textarea_tag_022);
  CU_add_test(jxhtml_suite, "test <textarea> 23." ,                              test_jxhtml_textarea_tag_023);
  CU_add_test(jxhtml_suite, "test <textarea> 24." ,                              test_jxhtml_textarea_tag_024);
  CU_add_test(jxhtml_suite, "test <textarea> 25." ,                              test_jxhtml_textarea_tag_025);
  CU_add_test(jxhtml_suite, "test <textarea> 26." ,                              test_jxhtml_textarea_tag_026);
  /*=========================================================================*/
  /* <TITLE>                                                                 */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <title> 1." ,                                  test_jxhtml_title_tag_001);
  CU_add_test(jxhtml_suite, "test <title> 2." ,                                  test_jxhtml_title_tag_002);
  /*=========================================================================*/
  /* <UL>                                                                    */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <ul> 1." ,                                     test_jxhtml_ul_tag_001);
  CU_add_test(jxhtml_suite, "test <ul> 2." ,                                     test_jxhtml_ul_tag_002);
  CU_add_test(jxhtml_suite, "test <ul> 3." ,                                     test_jxhtml_ul_tag_003);
  CU_add_test(jxhtml_suite, "test <ul> 4." ,                                     test_jxhtml_ul_tag_004);
  CU_add_test(jxhtml_suite, "test <ul> 5." ,                                     test_jxhtml_ul_tag_005);
  CU_add_test(jxhtml_suite, "test <ul> 6." ,                                     test_jxhtml_ul_tag_006);
  CU_add_test(jxhtml_suite, "test <ul> 7." ,                                     test_jxhtml_ul_tag_007);
  CU_add_test(jxhtml_suite, "test <ul> 8." ,                                     test_jxhtml_ul_tag_008);
  CU_add_test(jxhtml_suite, "test <ul> 9." ,                                     test_jxhtml_ul_tag_009);
  CU_add_test(jxhtml_suite, "test <ul> 10." ,                                    test_jxhtml_ul_tag_010);
  CU_add_test(jxhtml_suite, "test <ul> 11." ,                                    test_jxhtml_ul_tag_011);
  /*=========================================================================*/
  /* <BLINK>                                                                 */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <blink> 1." ,                                  test_jxhtml_blink_tag_001);
  CU_add_test(jxhtml_suite, "test <blink> 2." ,                                  test_jxhtml_blink_tag_002);
  CU_add_test(jxhtml_suite, "test <blink> 3." ,                                  test_jxhtml_blink_tag_003);
  CU_add_test(jxhtml_suite, "test <blink> 4." ,                                  test_jxhtml_blink_tag_004);
  /*=========================================================================*/
  /* <MARQUEE>                                                               */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <marquee> 1." ,                                test_jxhtml_marquee_tag_001);
  CU_add_test(jxhtml_suite, "test <marquee> 2." ,                                test_jxhtml_marquee_tag_002);
  CU_add_test(jxhtml_suite, "test <marquee> 3." ,                                test_jxhtml_marquee_tag_003);
  CU_add_test(jxhtml_suite, "test <marquee> 4." ,                                test_jxhtml_marquee_tag_004);
  CU_add_test(jxhtml_suite, "test <marquee> 5." ,                                test_jxhtml_marquee_tag_005);
  CU_add_test(jxhtml_suite, "test <marquee> 6." ,                                test_jxhtml_marquee_tag_006);
  CU_add_test(jxhtml_suite, "test <marquee> 7." ,                                test_jxhtml_marquee_tag_007);
  CU_add_test(jxhtml_suite, "test <marquee> 8." ,                                test_jxhtml_marquee_tag_008);
  CU_add_test(jxhtml_suite, "test <marquee> 9." ,                                test_jxhtml_marquee_tag_009);
  CU_add_test(jxhtml_suite, "test <marquee> 10." ,                               test_jxhtml_marquee_tag_010);
  CU_add_test(jxhtml_suite, "test <marquee> 11." ,                               test_jxhtml_marquee_tag_011);
  CU_add_test(jxhtml_suite, "test <marquee> 12." ,                               test_jxhtml_marquee_tag_012);
  CU_add_test(jxhtml_suite, "test <marquee> 13." ,                               test_jxhtml_marquee_tag_013);
  CU_add_test(jxhtml_suite, "test <marquee> 14." ,                               test_jxhtml_marquee_tag_014);
  CU_add_test(jxhtml_suite, "test <marquee> 15." ,                               test_jxhtml_marquee_tag_015);
  CU_add_test(jxhtml_suite, "test <marquee> 16." ,                               test_jxhtml_marquee_tag_016);
  CU_add_test(jxhtml_suite, "test <marquee> 17." ,                               test_jxhtml_marquee_tag_017);
  CU_add_test(jxhtml_suite, "test <marquee> 18." ,                               test_jxhtml_marquee_tag_018);
  /*=========================================================================*/
  /* <META>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <meta> 1." ,                                   test_jxhtml_meta_tag_001);
  CU_add_test(jxhtml_suite, "test <meta> 2." ,                                   test_jxhtml_meta_tag_002);
  CU_add_test(jxhtml_suite, "test <meta> 3." ,                                   test_jxhtml_meta_tag_003);
  CU_add_test(jxhtml_suite, "test <meta> 4." ,                                   test_jxhtml_meta_tag_004);
  CU_add_test(jxhtml_suite, "test <meta> 5." ,                                   test_jxhtml_meta_tag_005);
  CU_add_test(jxhtml_suite, "test <meta> 6." ,                                   test_jxhtml_meta_tag_006);
  CU_add_test(jxhtml_suite, "test <meta> 7." ,                                   test_jxhtml_meta_tag_007);
  CU_add_test(jxhtml_suite, "test <meta> 8." ,                                   test_jxhtml_meta_tag_008);
  CU_add_test(jxhtml_suite, "test <meta> 9." ,                                   test_jxhtml_meta_tag_009);
  /*=========================================================================*/
  /* <font>                                                                  */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <font> 1." ,                                   test_jxhtml_font_tag_001);
  CU_add_test(jxhtml_suite, "test <font> 2." ,                                   test_jxhtml_font_tag_002);
  CU_add_test(jxhtml_suite, "test <font> 3." ,                                   test_jxhtml_font_tag_003);
  CU_add_test(jxhtml_suite, "test <font> 4." ,                                   test_jxhtml_font_tag_004);
  CU_add_test(jxhtml_suite, "test <font> 5." ,                                   test_jxhtml_font_tag_005);
  CU_add_test(jxhtml_suite, "test <font> 6." ,                                   test_jxhtml_font_tag_006);
  /*=========================================================================*/
  /* <param>                                                                 */
  /*=========================================================================*/
  CU_add_test(jxhtml_suite, "test <param> 1.",                                   test_jxhtml_param_tag_001);
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

char *test_ap_escape_html(apr_pool_t *pool, const char *s)
{
  return (char *)s;
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
    X.html_spec_type = CHXJ_SPEC_Jxhtml; \
  } while (0)



/*===========================================================================*/
/* COMMENT                                                                   */
/*===========================================================================*/
void test_jxhtml_001() 
{
#define  TEST_STRING "<!-- comment -->"
#define TEST_RESULT "\n"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jxhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", TEST_RESULT);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_jxhtml_002() 
{
#define  TEST_STRING ""
#define TEST_RESULT "\n"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jxhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", TEST_RESULT);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp("\n", ret) == 0);
  CU_ASSERT(destlen == 1);

  APR_TERM;
#undef TEST_STRING
}
void test_jxhtml_comment_tag_001() 
{
#define  TEST_STRING "<html><!--</html><body>--><head></head><body></body></html>"
#define  RESULT_STRING "<?xml version='1.0' encoding='Shift_JIS' ?><!DOCTYPE html PUBLIC \"-//J-PHONE//DTD XHTML Basic 1.0 Plus//EN\" \"html-basic10-plus.dtd\"><html version=\"1.0\"><head></head><body></body></html>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jxhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_a_tag_name_attribute_001() 
{
#define  TEST_STRING "<a name=\"abc\">abc</a>"
#define  RESULT_STRING "<a name=\"abc\">abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jxhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_name_attribute_002() 
{
#define  TEST_STRING "<a name=\"\">abc</a>"
#define  RESULT_STRING "<a name=\"\">abc</a>"
  char  *ret;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  ret = chxj_convert_jxhtml(&r, &spec, TEST_STRING, sizeof(TEST_STRING)-1, &destlen, &entry, &cookie);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_name_attribute_003() 
{
#define  TEST_STRING "<a name=\"あああ\">abc</a>"
#define  RESULT_STRING "<a name=\"あああ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);

  destlen = sizeof(TEST_STRING)-1;
  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_name_attribute_004() 
{
#define  TEST_STRING "<a name=\"ｱｱｱ\">abc</a>"
#define  RESULT_STRING "<a name=\"ｱｱｱ\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_001() 
{
#define  TEST_STRING "<a href=\"\">abc</a>"
#define  RESULT_STRING "<a href=\"?_chxj_cc=test_cookie_id\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_003() 
{
#define  TEST_STRING "<a href=\"a.html\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html?_chxj_cc=test_cookie_id\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_004() 
{
#define  TEST_STRING "<a href=\"a.html#abc\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html#abc?_chxj_cc=test_cookie_id\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_005() 
{
#define  TEST_STRING "<a href=\"a.html#\">abc</a>"
#define  RESULT_STRING "<a href=\"a.html#?_chxj_cc=test_cookie_id\">abc</a>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_href_attribute_006() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_accesskey_attribute_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_accesskey_attribute_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_a_tag_accesskey_attribute_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_base_tag_001() 
{
#define  TEST_STRING "<base>"
#define  RESULT_STRING "<base />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_base_tag_href_attribute_001() 
{
#define  TEST_STRING "<base href>"
#define  RESULT_STRING "<base href=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_base_tag_href_attribute_002() 
{
#define  TEST_STRING "<base href=\"\">"
#define  RESULT_STRING "<base href=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_base_tag_href_attribute_003() 
{
#define  TEST_STRING "<base href=\"http://www.google.co.jp/\">"
#define  RESULT_STRING "<base href=\"http://www.google.co.jp/\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_base_tag_href_attribute_004() 
{
#define  TEST_STRING "<base href=\".\">"
#define  RESULT_STRING "<base href=\".\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
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
void test_jxhtml_blockquote_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blockquote_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blockquote_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blockquote_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_body_tag_001() 
{
#define  TEST_STRING "<body></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_002() 
{
#define  TEST_STRING   "<body bgcolor>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_003() 
{
#define  TEST_STRING "<body bgcolor=\"\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_004() 
{
#define  TEST_STRING "<body bgcolor=\"#ff0000\"></body>"
#define  RESULT_STRING "<body bgcolor=\"#ff0000\"></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_005() 
{
#define  TEST_STRING "<body text></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_006() 
{
#define  TEST_STRING "<body text=\"\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_007() 
{
#define  TEST_STRING "<body text=\"#ff0000\"></body>"
#define  RESULT_STRING "<body text=\"#ff0000\"></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_008() 
{
#define  TEST_STRING "<body link></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_009() 
{
#define  TEST_STRING "<body link=\"\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_010() 
{
#define  TEST_STRING   "<body link=\"#ff0000\"></body>"
#define  RESULT_STRING "<body link=\"#ff0000\"></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_011() 
{
#define  TEST_STRING   "<body vlink></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_012() 
{
#define  TEST_STRING   "<body vlink=\"\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_013() 
{
#define  TEST_STRING   "<body vlink=\"#ff0000\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);

  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_014() 
{
#define  TEST_STRING   "<body alink=\"#FF0000\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_body_tag_015() 
{
#define  TEST_STRING "<body boyoyon=\"#FF0000\"></body>"
#define  RESULT_STRING "<body></body>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_br_tag_001() 
{
#define  TEST_STRING "<br>"
#define  RESULT_STRING "<br />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_002() 
{
#define  TEST_STRING "<br clear=\"left\">"
#define  RESULT_STRING "<br clear=\"left\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_003() 
{
#define  TEST_STRING   "<br clear=\"right\">"
#define  RESULT_STRING "<br clear=\"right\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_004() 
{
#define  TEST_STRING "<br clear=\"all\">"
#define  RESULT_STRING "<br clear=\"all\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_005() 
{
#define  TEST_STRING   "<br clear=\"\">"
#define  RESULT_STRING "<br />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_006() 
{
#define  TEST_STRING   "<br clear>"
#define  RESULT_STRING "<br />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_br_tag_007() 
{
#define  TEST_STRING   "<br clear=\"abc\">"
#define  RESULT_STRING "<br />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_center_tag_001()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_dir_tag_001()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_002()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_003()
{
#define  TEST_STRING "<dir type=\"disc\"></dir>"
#define  RESULT_STRING "<dir type=\"disc\"></dir>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_004()
{
#define  TEST_STRING "<dir type=\"circle\"></dir>"
#define  RESULT_STRING "<dir type=\"circle\"></dir>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_005()
{
#define  TEST_STRING "<dir type=\"square\"></dir>"
#define  RESULT_STRING "<dir type=\"square\"></dir>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_006()
{
#define  TEST_STRING "<dir type=\"\"></dir>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dir_tag_007()
{
#define  TEST_STRING "<dir type></dir>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_dl_tag_001()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_dt_tag_001()
{
#define  TEST_STRING   "<dl><dt>あああ</dt><dt>いいい</dt></dl>"
#define  RESULT_STRING "<dl><dt>あああ<dt>いいい</dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dt_tag_002()
{
#define  TEST_STRING   "<dl><dt></dt><dt></dt></dl>"
#define  RESULT_STRING "<dl><dt><dt></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_dd_tag_001()
{
#define  TEST_STRING "<dl><dt>あああ</dt><dd>いいい</dd><dt>ああああ</dt><dd>いいいい</dd></dl>"
#define  RESULT_STRING "<dl><dt>あああ<dd>いいい<dt>ああああ<dd>いいいい</dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_dd_tag_002()
{
#define  TEST_STRING "<dl><dd></dd><dd></dd></dl>"
#define  RESULT_STRING "<dl><dd><dd></dl>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_div_tag_001() 
{
#define  TEST_STRING "<div></div>"
#define  RESULT_STRING "<div></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_002() 
{
#define  TEST_STRING "<div align=\"left\"></div>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_003() 
{
#define  TEST_STRING   "<div align=\"right\"></div>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_004() 
{
#define  TEST_STRING   "<div align=\"center\"></div>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_005() 
{
#define  TEST_STRING   "<div align=\"\"></div>"
#define  RESULT_STRING "<div></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_006() 
{
#define  TEST_STRING   "<div align=\"abc\"></div>"
#define  RESULT_STRING "<div></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_div_tag_007() 
{
#define  TEST_STRING   "<div style=\"aaa\"></div>"
#define  RESULT_STRING "<div></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_form_tag_001() 
{
#define  TEST_STRING "<form></form>"
#define  RESULT_STRING "<form><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_002() 
{
#define  TEST_STRING   "<form method></form>"
#define  RESULT_STRING "<form method=\"\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_003() 
{
#define  TEST_STRING "<form method=\"post\"></form>"
#define  RESULT_STRING "<form method=\"post\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_004() 
{
#define  TEST_STRING   "<form method=\"get\"></form>"
#define  RESULT_STRING "<form method=\"get\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_005() 
{
#define  TEST_STRING   "<form method=\"abc\"></form>"
#define  RESULT_STRING "<form method=\"abc\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_006() 
{
#define  TEST_STRING   "<form action></form>"
#define  RESULT_STRING "<form action=\"\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_007() 
{
#define  TEST_STRING   "<form action></form>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, NULL);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_008() 
{
#define  TEST_STRING   "<form action=\"http://www.google.co.jp/\"></form>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_form_tag_009() 
{
#define  TEST_STRING   "<form method=\"post\" action=\"hogehoge\"></form>"
#define  RESULT_STRING "<form method=\"post\" action=\"hogehoge\"><input type='hidden' name='_chxj_cc' value='test_cookie_id' /></form>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_head_tag_001()
{
#define  TEST_STRING   "<head></head>"
#define  RESULT_STRING "<head></head>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_head_tag_002()
{
#define  TEST_STRING "<head><title>あああ</title></head>"
#define  RESULT_STRING "<head><title>あああ</title></head>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_h1_tag_001() 
{
#define  TEST_STRING "<h1></h1>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_002() 
{
#define  TEST_STRING "<h1>abc</h1>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_003() 
{
#define  TEST_STRING   "<h1>亀さん</h1>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_004() 
{
#define  TEST_STRING   "<h1>ﾊﾝｶｸ</h1>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_005() 
{
#define  TEST_STRING "<h1 align></h1>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_006() 
{
#define  TEST_STRING "<h1 align=\"\"></h1>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_007() 
{
#define  TEST_STRING   "<h1 align=\"left\"></h1>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_008() 
{
#define  TEST_STRING "<h1 align=\"right\"></h1>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_009() 
{
#define  TEST_STRING "<h1 align=\"center\"></h1>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h1_tag_010() 
{
#define  TEST_STRING   "<h1 align=\"unkown\"></h1>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h2>                                                                      */
/*===========================================================================*/
void test_jxhtml_h2_tag_001() 
{
#define  TEST_STRING "<h2></h2>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_002() 
{
#define  TEST_STRING "<h2>abc</h2>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_003() 
{
#define  TEST_STRING "<h2>亀さん</h2>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_004() 
{
#define  TEST_STRING "<h2>ﾊﾝｶｸ</h2>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_005() 
{
#define  TEST_STRING "<h2 align></h2>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_006() 
{
#define  TEST_STRING "<h2 align=\"\"></h2>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_007() 
{
#define  TEST_STRING "<h2 align=\"left\"></h2>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_008() 
{
#define  TEST_STRING "<h2 align=\"right\"></h2>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_009() 
{
#define  TEST_STRING "<h2 align=\"center\"></h2>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h2_tag_010() 
{
#define  TEST_STRING "<h2 align=\"unkown\"></h2>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h3>                                                                      */
/*===========================================================================*/
void test_jxhtml_h3_tag_001() 
{
#define  TEST_STRING "<h3></h3>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_002() 
{
#define  TEST_STRING "<h3>abc</h3>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_003() 
{
#define  TEST_STRING "<h3>亀さん</h3>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_004() 
{
#define  TEST_STRING "<h3>ﾊﾝｶｸ</h3>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_005() 
{
#define  TEST_STRING "<h3 align></h3>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_006() 
{
#define  TEST_STRING "<h3 align=\"\"></h3>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_007() 
{
#define  TEST_STRING "<h3 align=\"left\"></h3>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_008() 
{
#define  TEST_STRING "<h3 align=\"right\"></h3>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_009() 
{
#define  TEST_STRING "<h3 align=\"center\"></h3>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h3_tag_010() 
{
#define  TEST_STRING "<h3 align=\"unkown\"></h3>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h4>                                                                      */
/*===========================================================================*/
void test_jxhtml_h4_tag_001() 
{
#define  TEST_STRING "<h4></h4>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_002() 
{
#define  TEST_STRING "<h4>abc</h4>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_003() 
{
#define  TEST_STRING "<h4>亀さん</h4>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_004() 
{
#define  TEST_STRING "<h4>ﾊﾝｶｸ</h4>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_005() 
{
#define  TEST_STRING "<h4 align></h4>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_006() 
{
#define  TEST_STRING "<h4 align=\"\"></h4>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_007() 
{
#define  TEST_STRING "<h4 align=\"left\"></h4>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_008() 
{
#define  TEST_STRING "<h4 align=\"right\"></h4>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_009() 
{
#define  TEST_STRING "<h4 align=\"center\"></h4>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h4_tag_010() 
{
#define  TEST_STRING "<h4 align=\"unkown\"></h4>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h5>                                                                      */
/*===========================================================================*/
void test_jxhtml_h5_tag_001() 
{
#define  TEST_STRING "<h5></h5>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_002() 
{
#define  TEST_STRING "<h5>abc</h5>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_003() 
{
#define  TEST_STRING "<h5>亀さん</h5>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_004() 
{
#define  TEST_STRING "<h5>ﾊﾝｶｸ</h5>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_005() 
{
#define  TEST_STRING "<h5 align></h5>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_006() 
{
#define  TEST_STRING "<h5 align=\"\"></h5>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_007() 
{
#define  TEST_STRING "<h5 align=\"left\"></h5>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_008() 
{
#define  TEST_STRING "<h5 align=\"right\"></h5>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_009() 
{
#define  TEST_STRING "<h5 align=\"center\"></h5>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h5_tag_010() 
{
#define  TEST_STRING "<h5 align=\"unkown\"></h5>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*===========================================================================*/
/* <h6>                                                                      */
/*===========================================================================*/
void test_jxhtml_h6_tag_001() 
{
#define  TEST_STRING "<h6></h6>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);

  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);

  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_002() 
{
#define  TEST_STRING "<h6>abc</h6>"
#define  RESULT_STRING "abc"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_003() 
{
#define  TEST_STRING "<h6>亀さん</h6>"
#define  RESULT_STRING "亀さん"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_004() 
{
#define  TEST_STRING   "<h6>ﾊﾝｶｸ</h6>"
#define  RESULT_STRING "ﾊﾝｶｸ"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_005() 
{
#define  TEST_STRING   "<h6 align></h6>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_006() 
{
#define  TEST_STRING   "<h6 align=\"\"></h6>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_007() 
{
#define  TEST_STRING   "<h6 align=\"left\"></h6>"
#define  RESULT_STRING "<div align=\"left\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_008() 
{
#define  TEST_STRING   "<h6 align=\"right\"></h6>"
#define  RESULT_STRING "<div align=\"right\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_009() 
{
#define  TEST_STRING   "<h6 align=\"center\"></h6>"
#define  RESULT_STRING "<div align=\"center\"></div>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_h6_tag_010() 
{
#define  TEST_STRING   "<h6 align=\"unkown\"></h6>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_hr_tag_001() 
{
#define  TEST_STRING   "<hr>"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_002() 
{
#define  TEST_STRING   "<hr />"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_003() 
{
#define  TEST_STRING   "<hr align>"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_004() 
{
#define  TEST_STRING   "<hr align=\"\">"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_005() 
{
#define  TEST_STRING   "<hr align=\"left\">"
#define  RESULT_STRING "<hr align=\"left\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_006() 
{
#define  TEST_STRING   "<hr align=\"right\">"
#define  RESULT_STRING "<hr align=\"right\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_007() 
{
#define  TEST_STRING   "<hr align=\"center\">"
#define  RESULT_STRING "<hr align=\"center\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_008() 
{
#define  TEST_STRING   "<hr align=\"unknown\">"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_009() 
{
#define  TEST_STRING   "<hr size>"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_010() 
{
#define  TEST_STRING   "<hr size=\"\">"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_011() 
{
#define  TEST_STRING   "<hr size=\"abc\">"
#define  RESULT_STRING "<hr size=\"abc\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_012() 
{
#define  TEST_STRING   "<hr size=\"10\">"
#define  RESULT_STRING "<hr size=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_013() 
{
#define  TEST_STRING   "<hr width>"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_014() 
{
#define  TEST_STRING   "<hr width=\"\">"
#define  RESULT_STRING "<hr />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_015() 
{
#define  TEST_STRING   "<hr width=\"abc\">"
#define  RESULT_STRING "<hr width=\"abc\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_016() 
{
#define  TEST_STRING   "<hr width=\"10\">"
#define  RESULT_STRING "<hr width=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_017() 
{
#define  TEST_STRING   "<hr width=\"10\" noshade>"
#define  RESULT_STRING "<hr width=\"10\" noshade />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_018() 
{
#define  TEST_STRING   "<hr width=\"10\" color=\"#ff0000\">"
#define  RESULT_STRING "<hr width=\"10\" color=\"#ff0000\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_018_1() 
{
#define  TEST_STRING   "<hr width=\"10\" color=\"\">"
#define  RESULT_STRING "<hr width=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_hr_tag_018_2() 
{
#define  TEST_STRING   "<hr width=\"10\" color>"
#define  RESULT_STRING "<hr width=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_html_tag_001()
{
#define  TEST_STRING "<html><head></head><body></body></html>"
#define  RESULT_STRING "<?xml version='1.0' encoding='Shift_JIS' ?><!DOCTYPE html PUBLIC \"-//J-PHONE//DTD XHTML Basic 1.0 Plus//EN\" \"html-basic10-plus.dtd\"><html version=\"1.0\"><head></head><body></body></html>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_img_tag_001() 
{
#define  TEST_STRING "<img>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_002() 
{
#define  TEST_STRING "<img src>"
#define  RESULT_STRING "<img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_003() 
{
#define  TEST_STRING "<img src=\"\">"
#define  RESULT_STRING "<img src=\"?_chxj_cc=test_cookie_id&_chxj_nc=true\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_004() 
{
#define  TEST_STRING "<img src=\"aaa\">"
#define  RESULT_STRING "<img src=\"aaa?_chxj_cc=test_cookie_id&_chxj_nc=true\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_005() 
{
#define  TEST_STRING "<img src=\"亀さん\">"
#define  RESULT_STRING "<img src=\"亀さん?_chxj_cc=test_cookie_id&_chxj_nc=true\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_006() 
{
#define  TEST_STRING "<img src=\"ﾊﾝｶｸ\">"
#define  RESULT_STRING "<img src=\"ﾊﾝｶｸ?_chxj_cc=test_cookie_id&_chxj_nc=true\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_007() 
{
#define  TEST_STRING "<img align>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_008() 
{
#define  TEST_STRING "<img align=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_009() 
{
#define  TEST_STRING "<img align=\"top\">"
#define  RESULT_STRING "<img align=\"top\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_010() 
{
#define  TEST_STRING "<img align=\"middle\">"
#define  RESULT_STRING "<img align=\"middle\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_011() 
{
#define  TEST_STRING "<img align=\"bottom\">"
#define  RESULT_STRING "<img align=\"bottom\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_012() 
{
#define  TEST_STRING "<img align=\"left\">"
#define  RESULT_STRING "<img align=\"left\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_013() 
{
#define  TEST_STRING "<img align=\"right\">"
#define  RESULT_STRING "<img align=\"right\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_013_1() 
{
#define  TEST_STRING "<img align=\"center\">"
#define  RESULT_STRING "<img align=\"middle\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_014() 
{
#define  TEST_STRING "<img align=\"unkown\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_015() 
{
#define  TEST_STRING "<img width>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_016() 
{
#define  TEST_STRING "<img width=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_017() 
{
#define  TEST_STRING "<img width=\"abc\">"
#define  RESULT_STRING "<img width=\"abc\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_018() 
{
#define  TEST_STRING "<img width=\"10\">"
#define  RESULT_STRING "<img width=\"10\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_019() 
{
#define  TEST_STRING "<img width=\"10%\">"
#define  RESULT_STRING "<img width=\"10%\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_020() 
{
#define  TEST_STRING "<img height>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_021() 
{
#define  TEST_STRING "<img height=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_022() 
{
#define  TEST_STRING "<img height=\"abc\">"
#define  RESULT_STRING "<img height=\"abc\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_023() 
{
#define  TEST_STRING "<img height=\"10\">"
#define  RESULT_STRING "<img height=\"10\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_024() 
{
#define  TEST_STRING "<img height=\"10%\">"
#define  RESULT_STRING "<img height=\"10%\" alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_025() 
{
#define  TEST_STRING "<img hspace>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_026() 
{
#define  TEST_STRING "<img hspace=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_027() 
{
#define  TEST_STRING "<img hspace=\"abc\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_028() 
{
#define  TEST_STRING "<img hspace=\"10\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_029() 
{
#define  TEST_STRING "<img vspace>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_030() 
{
#define  TEST_STRING "<img vspace=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_031() 
{
#define  TEST_STRING "<img vspace=\"abc\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_032() 
{
#define  TEST_STRING "<img vspace=\"10\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_033() 
{
#define  TEST_STRING "<img alt>"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_034() 
{
#define  TEST_STRING "<img alt=\"\">"
#define  RESULT_STRING "<img alt=\"\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_035() 
{
#define  TEST_STRING "<img alt=\"abc\">"
#define  RESULT_STRING "<img alt=\"abc\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_036() 
{
#define  TEST_STRING "<img alt=\"亀さん\">"
#define  RESULT_STRING "<img alt=\"亀さん\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_img_tag_037() 
{
#define  TEST_STRING "<img alt=\"ﾊﾝｶｸ\">"
#define  RESULT_STRING "<img alt=\"ﾊﾝｶｸ\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_input_tag_001() 
{
#define  TEST_STRING "<input>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_002() 
{
#define  TEST_STRING "<input type>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_003() 
{
#define  TEST_STRING "<input type=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_004() 
{
#define  TEST_STRING "<input type=\"text\">"
#define  RESULT_STRING "<input type=\"text\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_005() 
{
#define  TEST_STRING "<input type=\"password\">"
#define  RESULT_STRING "<input type=\"password\" mode=\"numeric\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_006() 
{
#define  TEST_STRING "<input type=\"checkbox\">"
#define  RESULT_STRING "<input type=\"checkbox\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_007() 
{
#define  TEST_STRING "<input type=\"radio\">"
#define  RESULT_STRING "<input type=\"radio\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_008() 
{
#define  TEST_STRING "<input type=\"hidden\">"
#define  RESULT_STRING "<input type=\"hidden\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_009() 
{
#define  TEST_STRING "<input type=\"submit\">"
#define  RESULT_STRING "<input type=\"submit\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_010() 
{
#define  TEST_STRING "<input type=\"reset\">"
#define  RESULT_STRING "<input type=\"reset\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_011() 
{
#define  TEST_STRING "<input type=\"unknown\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_012() 
{
#define  TEST_STRING "<input name>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_013() 
{
#define  TEST_STRING "<input name=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_014() 
{
#define  TEST_STRING "<input name=\"a\">"
#define  RESULT_STRING "<input name=\"a\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_015() 
{
#define  TEST_STRING "<input name=\"亀\">"
#define  RESULT_STRING "<input name=\"亀\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_016() 
{
#define  TEST_STRING "<input name=\"ﾊﾝｶｸ\">"
#define  RESULT_STRING "<input name=\"ﾊﾝｶｸ\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_017() 
{
#define  TEST_STRING "<input value>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_018() 
{
#define  TEST_STRING "<input value=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_019() 
{
#define  TEST_STRING "<input value=\"a\">"
#define  RESULT_STRING "<input value=\"a\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_020() 
{
#define  TEST_STRING "<input value=\"亀\">"
#define  RESULT_STRING "<input value=\"亀\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_021() 
{
#define  TEST_STRING "<input value=\"ﾊﾝｶｸ\">"
#define  RESULT_STRING "<input value=\"ﾊﾝｶｸ\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_022() 
{
#define  TEST_STRING "<input size>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_023() 
{
#define  TEST_STRING "<input size=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_024() 
{
#define  TEST_STRING "<input size=\"a\">"
#define  RESULT_STRING "<input size=\"a\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_025() 
{
#define  TEST_STRING "<input size=\"1\">"
#define  RESULT_STRING "<input size=\"1\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_026() 
{
#define  TEST_STRING "<input maxlength>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_027() 
{
#define  TEST_STRING "<input maxlength=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_028() 
{
#define  TEST_STRING "<input maxlength=\"a\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_029() 
{
#define  TEST_STRING "<input maxlength=\"1\">"
#define  RESULT_STRING "<input maxlength=\"1\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_030() 
{
#define  TEST_STRING "<input checked>"
#define  RESULT_STRING "<input checked />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_031() 
{
#define  TEST_STRING "<input accesskey>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_032() 
{
#define  TEST_STRING "<input accesskey=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_033() 
{
#define  TEST_STRING "<input accesskey=\"1\">"
#define  RESULT_STRING "<input accesskey=\"1\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_034() 
{
#define  TEST_STRING "<input istyle>"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_035() 
{
#define  TEST_STRING "<input istyle=\"\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_036() 
{
#define  TEST_STRING "<input istyle=\"0\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_037() 
{
#define  TEST_STRING "<input istyle=\"1\">"
#define  RESULT_STRING "<input mode=\"hiragana\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_038() 
{
#define  TEST_STRING "<input istyle=\"2\">"
#define  RESULT_STRING "<input mode=\"hankakukana\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_039() 
{
#define  TEST_STRING "<input istyle=\"3\">"
#define  RESULT_STRING "<input mode=\"alphabet\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_040() 
{
#define  TEST_STRING "<input istyle=\"4\">"
#define  RESULT_STRING "<input mode=\"numeric\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_041()
{
#define  TEST_STRING "<input istyle=\"5\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_042()
{
#define  TEST_STRING "<input istyle=\"a\">"
#define  RESULT_STRING "<input />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_043()
{
#define  TEST_STRING   "<input istyle=\"1\" maxlength=\"10\">"
#define  RESULT_STRING "<input mode=\"hiragana\" maxlength=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_044()
{
#define  TEST_STRING   "<input istyle=\"2\" maxlength=\"10\">"
#define  RESULT_STRING "<input mode=\"hankakukana\" maxlength=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_045()
{
#define  TEST_STRING   "<input istyle=\"3\" maxlength=\"10\">"
#define  RESULT_STRING "<input mode=\"alphabet\" maxlength=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_046()
{
#define  TEST_STRING "<input istyle=\"4\" maxlength=\"10\">"
#define  RESULT_STRING "<input mode=\"numeric\" maxlength=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_input_tag_047()
{
#define  TEST_STRING "<input istyle=\"a\" maxlength=\"10\">"
#define  RESULT_STRING "<input maxlength=\"10\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_li_tag_001() 
{
#define  TEST_STRING "<li></li>"
#define  RESULT_STRING "<li></li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_002() 
{
#define  TEST_STRING "<li>abc</li>"
#define  RESULT_STRING "<li>abc</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_003() 
{
#define  TEST_STRING "<li>あいうえお</li>"
#define  RESULT_STRING "<li>あいうえお</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_004() 
{
#define  TEST_STRING "<li>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_005() 
{
#define  TEST_STRING "<li type>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_006() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_007() 
{
#define  TEST_STRING "<li type=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_008() 
{
#define  TEST_STRING "<li type=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"1\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_009() 
{
#define  TEST_STRING "<li type=\"a\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"a\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_010() 
{
#define  TEST_STRING "<li type=\"A\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li type=\"A\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_011() 
{
#define  TEST_STRING "<li value>ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_012() 
{
#define  TEST_STRING "<li value=\"\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li>ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_li_tag_013() 
{
#define  TEST_STRING "<li value=\"1\">ﾊﾝｶｸ</li>"
#define  RESULT_STRING "<li value=\"1\">ﾊﾝｶｸ</li>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_menu_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_002() 
{
#define  TEST_STRING "<menu><li></li><li></li></menu>"
#define  RESULT_STRING "<menu><li></li><li></li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_003() 
{
#define  TEST_STRING "<menu><li>abc</li><li>def</li></menu>"
#define  RESULT_STRING "<menu><li>abc</li><li>def</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_004() 
{
#define  TEST_STRING "<menu><li>あああ</li><li>いいい</li></menu>"
#define  RESULT_STRING "<menu><li>あああ</li><li>いいい</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_005() 
{
#define  TEST_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_006() 
{
#define  TEST_STRING "<menu type=\"disc\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu type=\"disc\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_007() 
{
#define  TEST_STRING "<menu type=\"circle\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu type=\"circle\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_008() 
{
#define  TEST_STRING "<menu type=\"square\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu type=\"square\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_009() 
{
#define  TEST_STRING "<menu type=\"\"><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_menu_tag_010() 
{
#define  TEST_STRING "<menu type><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
#define  RESULT_STRING "<menu><li>ﾊﾝｶｸ</li><li>ﾊﾝｶｸ</li></menu>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_ol_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_002() 
{
#define  TEST_STRING "<ol><li></li></ol>"
#define  RESULT_STRING "<ol><li></li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_003() 
{
#define  TEST_STRING "<ol><li>abc</li></ol>"
#define  RESULT_STRING "<ol><li>abc</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_004() 
{
#define  TEST_STRING "<ol><li>abc</li><li>def</li></ol>"
#define  RESULT_STRING "<ol><li>abc</li><li>def</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_005() 
{
#define  TEST_STRING "<ol><li>あ</li></ol>"
#define  RESULT_STRING "<ol><li>あ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_006() 
{
#define  TEST_STRING "<ol><li>あ</li><li>い</li></ol>"
#define  RESULT_STRING "<ol><li>あ</li><li>い</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_007() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_008() 
{
#define  TEST_STRING "<ol><li>ﾊﾝｶｸ</li><li>ｶﾅﾀﾞﾖ</li></ol>"
#define  RESULT_STRING "<ol><li>ﾊﾝｶｸ</li><li>ｶﾅﾀﾞﾖ</li></ol>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_009() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_010() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_011() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_012() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_013() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_014() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_015() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_016() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_017() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ol_tag_018() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_option_tag_001() 
{
#define  TEST_STRING "<option></option>"
#define  RESULT_STRING "<option></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_002() 
{
#define  TEST_STRING "<option value></option>"
#define  RESULT_STRING "<option></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_003() 
{
#define  TEST_STRING "<option value=\"\"></option>"
#define  RESULT_STRING "<option></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_004() 
{
#define  TEST_STRING "<option value=\"a\"></option>"
#define  RESULT_STRING "<option value=\"a\"></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_005() 
{
#define  TEST_STRING "<option value=\"亀\"></option>"
#define  RESULT_STRING "<option value=\"亀\"></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_006() 
{
#define  TEST_STRING "<option value=\"ﾊﾝｶｸ\"></option>"
#define  RESULT_STRING "<option value=\"ﾊﾝｶｸ\"></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_option_tag_007() 
{
#define  TEST_STRING "<option selected></option>"
#define  RESULT_STRING "<option selected></option>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
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
void test_jxhtml_p_tag_001() 
{
#define  TEST_STRING "<p></p>"
#define  RESULT_STRING "<p></p>"
  char  *ret;
  char  *tmp;
  device_table       spec;
  chxjconvrule_entry entry;
  cookie_t           cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_005() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_006() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_007() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_p_tag_008() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_plaintext_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_plaintext_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_plaintext_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_plaintext_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_pre_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_pre_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_pre_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_pre_tag_004() 
{
#define  TEST_STRING "<pre><h1>あああ\n\n</pre>"
#define  RESULT_STRING "<pre>あああ\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_pre_tag_005() 
{
#define  TEST_STRING "<pre><h1>ﾊﾝｶｸ</h1>\n\n</pre>"
#define  RESULT_STRING "<pre>ﾊﾝｶｸ\n\n</pre>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_select_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_005() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_006() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_007() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_008() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_009() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_010() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_011() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_012() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_013() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_select_tag_014() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_textarea_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_005() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_006() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_007() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_008() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_009() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_010() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_011() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_012() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_013() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_014() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_015() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_016() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_017() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_018() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_019() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_020() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_021() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_022() 
{
#define  TEST_STRING "<textarea istyle=\"1\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"hiragana\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_023() 
{
#define  TEST_STRING "<textarea istyle=\"2\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"hankakukana\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_024() 
{
#define  TEST_STRING "<textarea istyle=\"3\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"alphabet\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_025() 
{
#define  TEST_STRING "<textarea istyle=\"4\"></textarea>"
#define  RESULT_STRING "<textarea mode=\"numeric\"></textarea>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_textarea_tag_026() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_title_tag_001()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_title_tag_002()
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_ul_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_003() 
{
#define  TEST_STRING "<ul><li></ul>"
#define  RESULT_STRING "<ul><li></li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_004() 
{
#define  TEST_STRING "<ul abc><li></ul>"
#define  RESULT_STRING "<ul><li></li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_005() 
{
#define  TEST_STRING "<ul abc><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_006() 
{
#define  TEST_STRING "<ul type=\"disc\"><li>abc</ul>"
#define  RESULT_STRING "<ul type=\"disc\"><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_007() 
{
#define  TEST_STRING "<ul type=\"circle\"><li>abc</ul>"
#define  RESULT_STRING "<ul type=\"circle\"><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_008() 
{
#define  TEST_STRING "<ul type=\"square\"><li>abc</ul>"
#define  RESULT_STRING "<ul type=\"square\"><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_009() 
{
#define  TEST_STRING "<ul type=\"unknown\"><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_010() 
{
#define  TEST_STRING "<ul type=\"\"><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_ul_tag_011() 
{
#define  TEST_STRING "<ul type><li>abc</ul>"
#define  RESULT_STRING "<ul><li>abc</li></ul>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_blink_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blink_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blink_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_blink_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
void test_jxhtml_marquee_tag_001() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_002() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_003() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_004() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_005() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_006() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_007() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_008() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_009() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_010() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_011() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_012() 
{
#define  TEST_STRING "<marquee behavior=\"scroll\">ﾊﾝｶｸ</marquee>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_013() 
{
#define  TEST_STRING "<marquee behavior=\"slide\">ﾊﾝｶｸ</marquee>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_014() 
{
#define  TEST_STRING "<marquee behavior=\"alternate\">ﾊﾝｶｸ</marquee>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_015() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_016() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_017() 
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_marquee_tag_018() 
{
#define  TEST_STRING "<marquee loop=\"1\">ﾊﾝｶｸ</marquee>"
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
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/* KONNO */
/*============================================================================*/
/* <META>                                                                     */
/*============================================================================*/
void test_jxhtml_meta_tag_001() 
{
#define  TEST_STRING   "<meta>"
#define  RESULT_STRING "<meta />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_002() 
{
#define  TEST_STRING "<meta http-equiv>"
#define  RESULT_STRING "<meta />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_003() 
{
#define  TEST_STRING "<meta http-equiv=\"\">"
#define  RESULT_STRING "<meta />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_004() 
{
#define  TEST_STRING "<meta http-equiv=\"content-type\">"
#define  RESULT_STRING "<meta http-equiv=\"content-type\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_005() 
{
#define  TEST_STRING "<meta http-equiv=\"abc\">"
#define  RESULT_STRING "<meta http-equiv=\"abc\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_006() 
{
#define  TEST_STRING "<meta content>"
#define  RESULT_STRING "<meta />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_007() 
{
#define  TEST_STRING "<meta content=\"\">"
#define  RESULT_STRING "<meta />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_008() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"text/html\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=Windows-31J\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_meta_tag_009() 
{
#define  TEST_STRING "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml\">"
#define  RESULT_STRING "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=Windows-31J\" />"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
/*============================================================================*/
/* <FONT>                                                                     */
/*============================================================================*/
void test_jxhtml_font_tag_001() 
{
#define  TEST_STRING "<font>"
#define  RESULT_STRING "<font></font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_font_tag_002() 
{
#define  TEST_STRING "<font>あああ</font>"
#define  RESULT_STRING "<font>あああ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_font_tag_003() 
{
#define  TEST_STRING "<font>ｱｱｱ</font>"
#define  RESULT_STRING "<font>ｱｱｱ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_font_tag_004() 
{
#define  TEST_STRING "<font color>ｱｱｱ</font>"
#define  RESULT_STRING "<font>ｱｱｱ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_font_tag_005() 
{
#define  TEST_STRING "<font color=\"\">ｱｱｱ</font>"
#define  RESULT_STRING "<font>ｱｱｱ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_font_tag_006() 
{
#define  TEST_STRING "<font color=\"#ff0000\">ｱｱｱ</font>"
#define  RESULT_STRING "<font color=\"#ff0000\">ｱｱｱ</font>"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
  CU_ASSERT(ret != NULL);
  CU_ASSERT(strcmp(RESULT_STRING, ret) == 0);
  CU_ASSERT(destlen == sizeof(RESULT_STRING)-1);

  APR_TERM;
#undef TEST_STRING
#undef RESULT_STRING
}
void test_jxhtml_param_tag_001() 
{
#define  TEST_STRING "<param>aaa</param>"
#define  RESULT_STRING "\n"
  char  *ret;
  char  *tmp;
  device_table spec;
  chxjconvrule_entry entry;
  cookie_t cookie;
  apr_size_t destlen;
  APR_INIT;

  COOKIE_INIT(cookie);

  SPEC_INIT(spec);
  destlen = sizeof(TEST_STRING)-1;

  tmp = chxj_encoding(&r, TEST_STRING, &destlen);
  ret = chxj_convert_jxhtml(&r, &spec, tmp, destlen, &destlen, &entry, &cookie);
  ret = chxj_rencoding(&r, ret, &destlen);
  fprintf(stderr, "actual:[%s]\n", ret);
  fprintf(stderr, "expect:[%s]\n", RESULT_STRING);
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
