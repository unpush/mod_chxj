#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_INIT([mod_chxj],[0.8.3-1])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([src/mod_chxj.c])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(include/config.h)


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([apr-1],  [main])

AC_ARG_WITH(apache-header, [  --with-apache-header=ARG The directory with the header file of apache2.0 is specified. ]) 
#[, ACTION-IF-GIVEN[, ACTION-IF-NOT-GIVEN]])
if test "x$with_apache_header" = "x"; then
    AC_MSG_ERROR([
Please specify the --with-apache-header option.
])
fi
if test "x$with_apache_header" = "xyes"; then
    AC_MSG_ERROR([
Please specify the --with-apache-header option. And, please specify the place of the header file. 

])
fi

AC_ARG_WITH(apxs, [  --with-apxs=ARG Path of apxs program file.])
if test "x$with_apxs" = "x" ; then
  APXS=""
fi
if test "x$with_apxs" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apxs option. And, please specify the place of the apxs program. 
])
fi

AC_ARG_WITH(apr-config, [  --with-apr-config=ARG  Path of apr-config program file.])
if test "x$with_apr_config" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apr-config option. And, please specify the place of the apr-config program. 
])
fi

AC_ARG_WITH(apu-config, [  --with-apu-config=ARG  Path of apu-config program file.])
if test "x$with_apu_config" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apu-config option. And, please specify the place of the apu-config program. 
])
fi


AC_ARG_ENABLE(img-conv-f, [  --enable-img-conv-f      Please specify it when you change the file name of the image being written in the 
source by the automatic operation.])
if test "x$enable_img_conv_f" = "xyes"; then
  QDEFS=""
else
  QDEFS="-DIMG_NOT_CONVERT_FILENAME"
fi

AC_ARG_ENABLE(dump-log, [  --enable-dump-log   When the log of conversion is output, it specifies it with the output file. ])
if test "x$enable_dump_log" = "xyes"; then
  DDEFS="-DDUMP_LOG=\\\"/tmp/dump.log\\\""
else
    if test "x$enable_dump_log" != "x" ; then
        DDEFS="-DDUMP_LOG=\\\"${enable_dump_log}\\\""
    fi
fi


if test "x$with_apxs" = "x" ; then
  AC_PATH_PROG(APXS, apxs, no,
      /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin)
  if test "x$APXS" = "xno"; then
      AC_MSG_ERROR([apxs not found.])
  fi
fi
if test "x$with_apxs" != "x" ; then
  APXS=$with_apxs
fi 

AC_PATH_PROG(LIBTOOL, libtool, no,
    /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
if test "x$LIBTOOL" = "xno"; then
    AC_MSG_ERROR([libtool not found.])
fi


AC_PATH_PROG(WAND_CONFIG, Wand-config, no,
   /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
if test "x$WAND_CONFIG" = "xno" ; then
    AC_MSG_ERROR([wand-config not found.])
fi

if test "x$with_apr_config" = "x" ; then
  AC_PATH_PROG(APR_CONFIG, apr-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
  if test "x$APR_CONFIG" = "xno" ; then
    AC_PATH_PROG(APR_1_CONFIG, apr-1-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
    if test "x$APR_1_CONFIG" = "xno" ; then
      AC_MSG_ERROR([apr-config and apr-1-config not found.])
    else
      APR_CONFIG="$APR_1_CONFIG"
    fi
  fi
fi
if test "x$with_apr_config" != "x" ; then
  APR_CONFIG=$with_apr_config
fi
if test "x$with_apu_config" = "x" ; then
  AC_PATH_PROG(APU_CONFIG, apu-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
  if test "x$APU_CONFIG" = "xno" ; then
    AC_PATH_PROG(APU_1_CONFIG, apu-1-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
    if test "x$APU_1_CONFIG" = "xno" ; then
      AC_MSG_ERROR([apu-config and apu-1-config not found.])
    else
      APU_CONFIG="$APU_1_CONFIG"
    fi
  fi
fi
if test "x$with_apu_config" != "x" ; then
  APU_CONFIG=$with_apu_config
fi

AC_ARG_WITH(iconv, [  --with-iconv=ARG  specify the place of the libiconv directory.])
AC_ARG_WITH(iconv_hook, [  --with-iconv_hook=ARG  specify the place of the libiconv_hook directory.])
if test "x$with_iconv" = "x" -a "x$with_iconv_hook" = "x" ; then
  AC_CHECK_LIB([iconv_hook], [iconv_open], [
      with_iconv="ok"
      AC_DEFINE([HAVE_LIBICONV_HOOK], [], [Define to 1 if you have the iconv library file.])
      LIBS="${LIBS} -liconv_hook"
  ],[
    AC_CHECK_LIB([iconv], [iconv_open], [
      with_iconv="ok"
      AC_DEFINE([HAVE_LIBICONV], [], [Define to 1 if you have the iconv library file.])
      LIBS="${LIBS} -liconv"
  ], [
      witn_iconv=""
  ],[])
  ],[])
fi

if test "x$with_iconv" != "xok" ; then
  if test "x$with_iconv" = "xyes" ; then
      AC_MSG_ERROR([
  Please specify the --with-iconv option. And, please specify the place of the libiconv program. 
  ])
  fi
  if test "x$with_iconv" != "x" ; then
    LIBS="${LIBS} -L$with_iconv -liconv "
    AC_DEFINE([HAVE_LIBICONV], [], [Define to 1 if you have the iconv library file.])
  fi
fi
if test "x$with_iconv" = "x" ; then
  if test "x$with_iconv_hook" = "xyes" ; then
      AC_MSG_ERROR([
  Please specify the --with-iconv-hook option. And, please specify the place of the libiconv_hook program. 
  ])
  fi
  if test "x$with_iconv_hook" != "x" ; then
    LIBS="${LIBS} -L${with_iconv_hook} -liconv_hook "
    AC_DEFINE([HAVE_LIBICONV_HOOK], [], [Define to 1 if you have the iconv_hook library file.])
  fi
  if test "x$with_iconv_hook" = "x" ; then
      AC_MSG_ERROR([
  Please specify the --with-iconv-hook option. And, please specify the place of the libiconv_hook program. 
  ])
  fi
fi


case $host_os in
  freebsd*)
    CFLAGS="${CFLAGS} -I/usr/local/include "
    ;;
esac


LIBS="${LIBS} `${WAND_CONFIG} --ldflags` `${WAND_CONFIG} --libs | sed -e s/-ldpstk//` "
LIBS="${LIBS} `${APR_CONFIG} --ldflags` `${APR_CONFIG} --libs` "
LIBS="${LIBS} `${APU_CONFIG} --ldflags` `${APU_CONFIG} --libs` "
CFLAGS="${CFLAGS} `${WAND_CONFIG} --cppflags` `${WAND_CONFIG} --cflags`"
CFLAGS="${CFLAGS} `${APR_CONFIG} --includes` `${APR_CONFIG} --cflags` `${APR_CONFIG} --cppflags`"
CFLAGS="${CFLAGS} `${APU_CONFIG} --includes`"
CPPFLAGS="${CPPFLAGS} `${APR_CONFIG} --includes` -I${with_apache_header}"
AC_SUBST(with_apache_header)
AC_SUBST(CC)
AC_SUBST(QDEFS)
AC_SUBST(DDEFS)
AC_SUBST(LIBS)
AC_SUBST(CFLAGS)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h strings.h unistd.h fcntl.h sys/types.h sys/stat.h sys/mman.h libgen.h])
APR_HEADER_DIR="`${APR_CONFIG} --includes | sed -e 's/-I//g' | sed -e 's/ //g'`"
AC_CHECK_HEADERS([apr_shm.h apr_global_mutex.h])
AC_CHECK_HEADERS([apr.h])
AC_CHECK_HEADERS([ap_config.h])
AC_CHECK_HEADERS([ap_regex.h],
  AC_DEFINE([HAVE_AP_REGEX_H], [], [Define to 1 if you have the <ap_regex.h> header file.]),[],
[[
#ifdef HAVE_APR_H
#include "apr.h"
#endif
#ifdef HAVE_AP_CONFIG_H
#include "ap_config.h"
#endif
]])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([open mmap close memset memcpy strcasecmp strncasecmp strstr])



AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
