# $FreeBSD: projects/armv6/kerberos5/libexec/kfd/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	kfd
MAN=	kfd.8
SRCS=	kfd.c kf_locl.h
CFLAGS+=	-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/roken
DPADD=	${LIBKRB5} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} \
	${LIBCRYPT} ${LIBVERS}
LDADD=	-lkrb5 -lroken -lasn1 -lcrypto -lcrypt \
	${LIBVERS}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/appl/kf
