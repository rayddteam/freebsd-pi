# $FreeBSD: projects/armv6/lib/libcxxrt/Makefile 234858 2012-05-01 04:01:22Z gonzo $

SRCDIR=       ${.CURDIR}/../../contrib/libcxxrt

SHLIB_MAJOR=  1
SHLIBDIR?=    /lib

.PATH: ${SRCDIR}

LIB=          cxxrt

SRCS+=        libelftc_dem_gnu3.c\
              terminate.cc\
              dynamic_cast.cc\
              memory.cc\
              auxhelper.cc\
              exception.cc\
              stdexcept.cc\
              typeinfo.cc\
              guard.cc

WARNS=        0
CFLAGS+=      -I${SRCDIR}
VERSION_MAP=  ${.CURDIR}/Version.map

.include <bsd.lib.mk>
