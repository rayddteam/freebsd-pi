# $FreeBSD: projects/armv6/sys/modules/bios/smapi/Makefile 234858 2012-05-01 04:01:22Z gonzo $
#

.PATH:  ${.CURDIR}/../../../i386/bios

KMOD=	smapi
SRCS=	smapi.c smapi_bios.S \
	bus_if.h device_if.h
WERROR=

.include <bsd.kmod.mk>

# XXX: clang integrated-as doesn't grok 16-bit assembly yet
CFLAGS.smapi_bios.S=	${CLANG_NO_IAS}
CFLAGS+=		${CFLAGS.${.IMPSRC:T}}
