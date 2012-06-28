# $FreeBSD: projects/armv6/kerberos5/usr.bin/hxtool/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	hxtool
NO_MAN=	1
CFLAGS+=	-I${KRB5DIR}/lib/hx509 \
		-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/roken \
		-I${KRB5DIR}/lib/sl -I.
DPADD=	${LIBHX509} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} ${LIBCRYPT} ${LIBSL} ${LIBVERS} ${LIBEDIT}
LDADD=	-lhx509 -lroken -lasn1 -lcrypto -lcrypt ${LIBSL} ${LIBVERS} -ledit
SRCS=	hxtool.c hxtool-commands.c hxtool-commands.h

hxtool-commands.c hxtool-commands.h: hxtool-commands.in
	slc ${.ALLSRC:M*.in}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/lib/hx509
