# $FreeBSD: projects/armv6/gnu/usr.bin/dialog/Makefile 228136 2011-11-29 15:24:19Z cognet $

DIALOG=	${.CURDIR}/../../../contrib/dialog
PROG=   dialog

DPADD=	${LIBDIALOG} ${LIBNCURSES} ${LIBM}
LDADD=	-ldialog -lncursesw -lm
CFLAGS+= -I${.CURDIR} -I${DIALOG}
.PATH:	${DIALOG}

WARNS?= 3

.include <bsd.prog.mk>
