# $FreeBSD: projects/armv6/usr.bin/lzmainfo/Makefile 226747 2011-10-25 19:47:28Z cognet $

PROG=	lzmainfo

XZDIR=	${.CURDIR}/../../contrib/xz/src
LZMALIBDIR=	${.CURDIR}/../../lib/liblzma

.PATH: ${XZDIR}/lzmainfo
SRCS+=	lzmainfo.c

.PATH: ${XZDIR}/common
SRCS+=	tuklib_progname.c \
	tuklib_exit.c

CFLAGS+=	-DHAVE_CONFIG_H \
		-I${LZMALIBDIR} \
		-I${XZDIR}/common

DPADD=	${LIBLZMA}
LDADD=	-llzma

.include <bsd.prog.mk>
