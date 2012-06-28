# $FreeBSD: projects/armv6/lib/libcom_err/Makefile 234858 2012-05-01 04:01:22Z gonzo $

LIB=	com_err
SRCS=	com_err.c error.c
INCS=	${COM_ERRDIR}/com_err.h ${COM_ERRDIR}/com_right.h
MAN=	com_err.3
COM_ERRDIR=	${.CURDIR}/../../contrib/com_err
CFLAGS+=	-I${COM_ERRDIR}

LDFLAGS=	-Wl,--no-undefined
VERSION_MAP=	${COM_ERRDIR}/version-script.map

SUBDIR=	doc

.include <bsd.lib.mk>

.PATH:	${COM_ERRDIR}
