# $FreeBSD: projects/armv6/lib/libc/sparc64/gen/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS+=	_ctx_start.S _setjmp.S fabs.S fixunsdfsi.S flt_rounds.c fpgetmask.c \
	fpgetround.c fpgetsticky.c fpsetmask.c fpsetround.c \
	getcontextx.c infinity.c ldexp.c makecontext.c \
	signalcontext.c setjmp.S sigsetjmp.S _set_tp.c
