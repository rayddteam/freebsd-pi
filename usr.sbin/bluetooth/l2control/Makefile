# $Id: Makefile,v 1.7 2003/08/14 20:06:22 max Exp $
# $FreeBSD: projects/armv6/usr.sbin/bluetooth/l2control/Makefile 121054 2003-10-12 22:04:24Z emax $

PROG=		l2control
MAN=		l2control.8
SRCS=		l2cap.c l2control.c
WARNS?=		2

DPADD=		${LIBBLUETOOTH}
LDADD=		-lbluetooth

.include <bsd.prog.mk>
