# $FreeBSD: projects/armv6/sys/boot/pc98/btx/btxldr/Makefile 234858 2012-05-01 04:01:22Z gonzo $

PROG=	btxldr
INTERNALPROG=
NO_MAN=
SRCS=	btxldr.S

CFLAGS+=-DLOADER_ADDRESS=${LOADER_ADDRESS}

.if defined(BTXLDR_VERBOSE)
CFLAGS+=-DBTXLDR_VERBOSE
.endif

LDFLAGS=-e start -Ttext ${LOADER_ADDRESS} -Wl,-N,-S,--oformat,binary

.include <bsd.prog.mk>

# XXX: clang integrated-as doesn't grok .codeNN directives yet
CFLAGS.btxldr.S=	${CLANG_NO_IAS}
CFLAGS+=		${CFLAGS.${.IMPSRC:T}}
