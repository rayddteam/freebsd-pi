# $FreeBSD: projects/armv6/usr.bin/mkesdb/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS+=	lex.l yacc.y
CFLAGS+= -I${.CURDIR} -I${.CURDIR}/../mkesdb \
	 -I${.CURDIR}/../../lib/libc/iconv
