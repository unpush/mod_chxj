#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_INIT(mod_chxj, 0.1.0)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR([src/mod_chxj.c])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(src/config.h)


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([apr-0],  [main])

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
AC_ARG_WITH(apr-header, [  --with-apr-header=ARG    The directory with the header file of apr library is specified. ]) 
if test "x$with_apr_header" = "x"; then
    AC_MSG_ERROR([
Please specify the --with-apr-header option.
])
fi
if test "x$with_apr_header" = "xyes"; then
    AC_MSG_ERROR([
Please specify the --with-apr-header option. And, please specify the place of the header file. 

])
fi

AC_ARG_WITH(apr-header,   [  --with-apr-header=ARG    The directory with the header file of apr library is specified. ]) 
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


AC_SUBST(with_apache_header)
AC_SUBST(with_apr_header)
AC_SUBST(CC)
AC_SUBST(QDEFS)
AC_SUBST(DDEFS)
AC_SUBST(LIBS)

AC_PATH_PROG(APXS, apxs, no,
    /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/local/apache/bin)
if test "x$APXS" = "xno"; then
    AC_MSG_ERROR([apxs not found.])
fi

AC_PATH_PROG(LIBTOOL, libtool, no,
    /usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin)
if test "x$LIBTOOL" = "xno"; then
    AC_MSG_ERROR([libtool not found.])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset memcpy strcasecmp strncasecmp strstr])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
