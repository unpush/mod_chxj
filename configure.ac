#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_INIT([mod_chxj],[1.0.0])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([src/mod_chxj.c])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([include/config.h])


# Checks for programs.
AC_PROG_CPP
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG(C)

# Checks for libraries.
AC_CHECK_LIB([apr-1],  [main])

AC_ARG_WITH(apache-header, [  --with-apache-header=ARG The directory with the header file of apache2.x is specified. ]) 
#[, ACTION-IF-GIVEN[, ACTION-IF-NOT-GIVEN]])

if test "x$with_apache_header" = "xyes"; then
    AC_MSG_ERROR([
Please specify the --with-apache-header option. And, please specify the place of the header file. 

])
fi

dnl ==================================================
dnl
dnl apxs
dnl
dnl ==================================================
AC_ARG_WITH(apxs, [  --with-apxs=ARG Path of apxs program file.])
if test "x$with_apxs" = "x" ; then
  APXS=""
fi
if test "x$with_apxs" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apxs option. And, please specify the place of the apxs program. 
])
fi

dnl ==================================================
dnl
dnl apr-config
dnl
dnl ==================================================
AC_ARG_WITH(apr-config, [  --with-apr-config=ARG  Path of apr-config program file.])
if test "x$with_apr_config" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apr-config option. And, please specify the place of the apr-config program. 
])
fi

dnl ==================================================
dnl
dnl apu-config
dnl
dnl ==================================================
AC_ARG_WITH(apu-config, [  --with-apu-config=ARG  Path of apu-config program file.])
if test "x$with_apu_config" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-apu-config option. And, please specify the place of the apu-config program. 
])
fi

dnl ==================================================
dnl
dnl filename in img tag convertion.
dnl
dnl ==================================================
AC_ARG_ENABLE(img-conv-f, [  --enable-img-conv-f      Please specify it when you change the file name of the image being written in the 
source by the automatic operation.])
if test "x$enable_img_conv_f" = "xyes"; then
  QDEFS=""
else
  QDEFS="-DIMG_NOT_CONVERT_FILENAME"
fi


dnl ==================================================
dnl
dnl SAVE COOKIE to MySQL Mode
dnl
dnl ==================================================
AC_ARG_ENABLE(mysql-cookie, [  --enable-mysql-cookie    Please specify it when you save Cookie in MySQL. ])
AC_ARG_WITH(mysql-header,   [  --with-mysql-header=ARG  Path of "mysql.h"])
AC_ARG_WITH(mysql-lib-dir,  [  --with-mysql-lib-dir=ARG Path of libmysqlclient])
if test "x$enable_mysql_cookie" = "xyes"; then
  dnl ======================================
  dnl use MYSQL COOKIE
  dnl ======================================
  if test "x$with_mysql_header" = "x" -o "x$with_mysql_header" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-mysql-header And, please specify the place of the "mysql.h" header file. 
])
  fi
  if test "x$with_mysql_lib_dir" = "x" -o "x$with_mysql_lib_dir" = "xyes" ; then
    AC_MSG_ERROR([
Please specify the --with-mysql-lib-dir And, please specify the place of mysql library directory. 
])
  fi
  MYSQL_COOKIE_DEFS="-DUSE_MYSQL_COOKIE -I${with_mysql_header}"
  MYSQL_COOKIE_LIBS="-L${with_mysql_lib_dir} -lmysqlclient_r"
else
  dnl ======================================
  dnl no use MYSQL COOKIE
  dnl ======================================
  MYSQL_COOKIE_DEFS=""
  MYSQL_COOKIE_LIBS=""
fi

dnl ==================================================
dnl
dnl dump log for DEBUG
dnl
dnl ==================================================
AC_ARG_ENABLE(dump-log, [  --enable-dump-log   When the log of conversion is output, it specifies it with the output file. ])
if test "x$enable_dump_log" = "xyes"; then
  DDEFS="-DDUMP_LOG=\\\"/tmp/dump.log\\\""
else
    if test "x$enable_dump_log" != "x" ; then
        DDEFS="-DDUMP_LOG=\\\"${enable_dump_log}\\\""
    fi
fi


if test "x$with_apxs" = "x" ; then
  AC_PATH_PROG(APXS2, apxs2, no,
               /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin)
  if test "x$APXS2" = "xno"; then
      AC_PATH_PROG(APXS, apxs, no,
                   /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin)
      if test "x$APXS" = "xno" ; then
        AC_MSG_ERROR([apxs not found.])
      fi
  else
    APXS="$APXS2"
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

if test "x$with_apache_header" = "x" ; then
  with_apache_header=`${APXS} -q includedir`
  AC_SUBST(with_apache_header)
fi

case $host_os in
  freebsd*)
    CFLAGS="${CFLAGS} -I/usr/local/include "
    ;;
esac




LIBS="${LIBS} `${WAND_CONFIG} --ldflags` `${WAND_CONFIG} --libs | sed -e s/-ldpstk//` "
LIBS="${LIBS} `${APR_CONFIG} --ldflags` `${APR_CONFIG} --libs` "
LIBS="${LIBS} `${APU_CONFIG} --ldflags` `${APU_CONFIG} --libs` "
LIBS="${LIBS} ${MYSQL_COOKIE_LIBS}"
CFLAGS="${CFLAGS} `${WAND_CONFIG} --cppflags` `${WAND_CONFIG} --cflags`"
CFLAGS="${CFLAGS} `${APR_CONFIG} --includes` `${APR_CONFIG} --cflags` `${APR_CONFIG} --cppflags`"
CFLAGS="${CFLAGS} `${APU_CONFIG} --includes`"
CFLAGS="${CFLAGS} -I${with_apache_header}"
CFLAGS="${CFLAGS} ${MYSQL_COOKIE_DEFS}"
CPPFLAGS="${CPPFLAGS} `${APR_CONFIG} --includes` "
CPPFLAGS="${CPPFLAGS} -I${with_apache_header}"
CPPFLAGS="${CPPFLAGS} ${MYSQL_COOKIE_DEFS}"


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h strings.h unistd.h fcntl.h sys/types.h sys/stat.h sys/mman.h libgen.h])
APR_HEADER_DIR="`${APR_CONFIG} --includes | sed -e 's/-I//g' | sed -e 's/ //g'`"
AC_CHECK_HEADERS([apr_shm.h apr_global_mutex.h])
AC_CHECK_HEADERS([apr.h])
AC_CHECK_HEADERS([ap_config.h])
AC_CHECK_HEADERS([ap_regex.h],use_pcre_flag="no",use_pcre_flag="yes")
if test "x$use_pcre_flag" = "xyes" ; then
  if test "x$with_pcre_config" = "x" ; then
    AC_PATH_PROG(PCRE_CONFIG, pcre-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
    if test "x$PCRE_CONFIG" = "xno" ; then
      AC_PATH_PROG(PCRE_1_CONFIG, pcre-1-config, no, /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
      if test "x$PCRE_1_CONFIG" = "xno" ; then
        AC_MSG_ERROR([pcre-config and pcre-1-config not found.])
      else
        PCRE_CONFIG="$PCRE_1_CONFIG"
      fi
    fi
  fi
  if test "x$with_pcre_config" != "x" ; then
    PCRE_CONFIG=$with_pcre_config
  fi
  CFLAGS="${CFLAGS} `${PCRE_CONFIG} --cflags`"
else
  AC_DEFINE([HAVE_AP_REGEX_H], [], [Define to 1 if you have the <ap_regex.h> header file.])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([open mmap close memset memcpy strcasecmp strncasecmp strstr])

AC_SUBST(with_apache_header)
AC_SUBST(CC)
AC_SUBST(QDEFS)
AC_SUBST(DDEFS)
AC_SUBST(LIBS)
AC_SUBST(CFLAGS)


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
