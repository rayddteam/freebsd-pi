#$FreeBSD: projects/armv6/kerberos5/lib/libkdc/Makefile 233294 2012-03-22 08:48:42Z stas $

LIB=	kdc
LDFLAGS=	-Wl,--no-undefined
VERSION_MAP= ${KRB5DIR}/kdc/version-script.map
LDADD=	-lroken -lhdb -lhx509 -lkrb5 -lheimntlm -lasn1 -lcrypt -lcrypto
DPADD=	${LIBROKEN} ${LIBHDB} ${LIBHX509} ${LIBKRB5} ${LIBHEIMNTLM} \
	${LIBASN1} ${LIBCRYPT} ${LIBCRYPTO}

INCS=	kdc.h \
	kdc-protos.h

KRB5INCS=	windc_plugin.h
KRB5INCSDIR=	${INCLUDEDIR}/krb5
INCSGROUPS=	INCS KRB5INCS

SRCS=	kdc-private.h \
	kdc-protos.h \
	default_config.c \
	set_dbinfo.c \
	digest.c \
	kdc_locl.h\
	kerberos5.c \
	krb5tgs.c \
	pkinit.c \
	log.c \
	misc.c \
	kx509.c \
	process.c \
	windc.c \
	rx.h

CFLAGS+=	-I${KRB5DIR}/lib/roken \
		-I${KRB5DIR}/lib/krb5 \
		-I${KRB5DIR}/lib/hdb \
		-I${KRB5DIR}/kdc

.include <bsd.lib.mk>

.PATH: ${KRB5DIR}/kdc
