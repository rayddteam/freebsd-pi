#	$FreeBSD: projects/armv6/usr.bin/dc/Makefile 228136 2011-11-29 15:24:19Z cognet $
#	$OpenBSD: Makefile,v 1.2 2006/11/26 11:31:09 deraadt Exp $

PROG=	dc
SRCS=	dc.c bcode.c inout.c mem.c stack.c
DPADD=	${LIBCRYPTO}
LDADD=	-lcrypto

.include <bsd.prog.mk>
