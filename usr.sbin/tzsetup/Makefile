# $FreeBSD: projects/armv6/usr.sbin/tzsetup/Makefile 228136 2011-11-29 15:24:19Z cognet $

PROG=	tzsetup
MAN=	tzsetup.8

CFLAGS+= -I${.CURDIR}/../../contrib/dialog -I.

WARNS?=	3

DPADD=	${LIBDIALOG} ${LIBNCURSESW} ${LIBM}
LDADD=	-ldialog -lncursesw -lm

.include <bsd.prog.mk>
