# $FreeBSD: projects/armv6/kerberos5/tools/make-roken/Makefile 234858 2012-05-01 04:01:22Z gonzo $

PROG=	make-roken
NO_MAN=

CLEANFILES= make-roken.c

.include "../Makefile.inc"

make-roken.c: ${KRB5DIR}/lib/roken/roken.awk ${KRB5DIR}/lib/roken/roken.h.in
	awk -f ${.ALLSRC} > ${.TARGET}

.include <bsd.prog.mk>
