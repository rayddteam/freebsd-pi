# $FreeBSD: projects/armv6/kerberos5/usr.bin/string2key/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	string2key
MAN=	string2key.8
CFLAGS+=	-I${KRB5DIR}/kdc \
		-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/krb5 \
		-I${KRB5DIR}/lib/roken \
		-I${KRB5DIR}/lib/windc
DPADD=	${LIBHDB} ${LIBKRB5} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} \
	${LIBCRYPT} ${LIBVERS}
LDADD=	-lhdb -lkrb5 -lroken -lasn1 -lcrypto -lcrypt ${LIBVERS}
SRCS=	string2key.c headers.h

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kdc
