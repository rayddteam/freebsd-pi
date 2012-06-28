# $FreeBSD: projects/armv6/usr.sbin/boot0cfg/Makefile 232120 2012-02-24 18:39:55Z cognet $

PROG=	boot0cfg
MAN=	boot0cfg.8

DPADD=	${LIBGEOM} ${LIBBSDXML} ${LIBSBUF}
LDADD=	-lgeom -lbsdxml -lsbuf

NO_WCAST_ALIGN=

.include <bsd.prog.mk>
