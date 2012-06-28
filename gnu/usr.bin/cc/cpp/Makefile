# $FreeBSD: projects/armv6/gnu/usr.bin/cc/cpp/Makefile 234858 2012-05-01 04:01:22Z gonzo $

.include <bsd.own.mk>

.include "../Makefile.inc"
.include "../Makefile.fe"

PROG=	gcpp
SRCS+=	cppspec.c

.if ${MK_CLANG_IS_CC} == "no"
LINKS=	${BINDIR}/gcpp ${BINDIR}/cpp
MLINKS=	gcpp.1 cpp.1
.endif

.include <bsd.prog.mk>
