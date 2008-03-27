CLEANFILES = .libs/mod_chxj *~

libmod_chxj_la_SOURCES = mod_chxj.c \
    qs_parse_string.c \
    qs_log.c \
    qs_malloc.c \
    qs_parse_tag.c \
    qs_parse_attr.c \
    qs_ignore_sp.c \
    qs_parse_file.c \
    chxj_load_device_data.c \
    chxj_load_emoji_data.c  \
    chxj_tag_util.c  \
    chxj_specified_device.c  \
    chxj_xhtml_mobile_1_0.c \
    chxj_hdml.c \
    chxj_chtml10.c \
    chxj_chtml20.c \
    chxj_chtml30.c  \
    chxj_jhtml.c \
    chxj_str_util.c  \
    chxj_img_conv.c  \
    chxj_dump.c \
	chxj_img_conv_format.c \
    chxj_qr_code.c \
    chxj_calc_ecc.c \
    chxj_apply_convrule.c \
    chxj_encoding.c \
    chxj_node_convert.c \
    chxj_url_encode.c \
    chxj_cookie.c \
    chxj_preg_replace.c \
    chxj_emoji.c \
    chxj_jreserved_tag.c \
    chxj_mysql.c \
    chxj_memcache.c \
    chxj_dbm.c


libmod_chxj_la_CFLAGS = $(AM_CFLAGS) -O2 -prefer-pic @QDEFS@ @DDEFS@ -I. -I../include 
AM_CFLAGS = @CFLAGS@ -O2 -prefer-pic @QDEFS@ @DDEFS@ -I. -I../include 

lib_LTLIBRARIES = libmod_chxj.la

make_so:
	@if test ! -L mod_chxj.so ; then ln -s .libs/libmod_chxj.so mod_chxj.so ; fi

clean:
	rm -fr .libs
	rm -f *.o *.lo *.la
	rm -f mod_chxj.so

install: make_so
	@@APXS@ -i -n mod_chxj mod_chxj.so

activate: make_so
	@@APXS@ -i -a -n mod_chxj mod_chxj.so
