# $FreeBSD: projects/armv6/usr.sbin/pkg/Makefile 234351 2012-04-16 20:41:25Z bapt $

PROG=	pkg
SRC=	pkg.c

NO_MAN=	yes
DPADD=	${LIBARCHIVE} ${LIBELF} ${LIBFETCH}
LDADD=	-larchive -lelf -lfetch

.include <bsd.prog.mk>
