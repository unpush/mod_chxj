#!/bin/sh
# autogen.sh - generates configure using the autotools
# $Id$
#export WANT_AUTOCONF=2.5
rm -f ./aclocal.m4
rm -f ./Makefile
rm -f ./Makefile.in
rm -f ./configure
rm -f ./build/config.guess
rm -f ./build/config.log
rm -f ./build/config.sub
rm -f ./build/config.status
rm -f ./build/install-sh
rm -f ./build/libtool
rm -f ./build/ltmain.sh
rm -f ./build/missing
rm -f ./build/mkinstalldirs
#libtoolize --force --copy
aclocal 
autoheader
# automake -a -c 
automake
autoconf
rm -rf autom4te.cache

