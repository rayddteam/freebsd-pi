# $FreeBSD: projects/armv6/usr.bin/compile_et/Makefile 234858 2012-05-01 04:01:22Z gonzo $

.PATH: ${.CURDIR}/../../contrib/com_err

LIBVERS=	${.OBJDIR}/../../kerberos5/lib/libvers/libvers.a

PROG=	compile_et
SRCS=	compile_et.c parse.y lex.l
LDADD=	-lroken ${LIBVERS}
DPADD=	${LIBROKEN} ${LIBVERS}
CFLAGS+=-I. -I${.CURDIR}/../../contrib/com_err

WARNS?=	0

.include <bsd.prog.mk>
