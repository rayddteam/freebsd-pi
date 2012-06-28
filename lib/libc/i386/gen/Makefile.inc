#	@(#)Makefile.inc	8.1 (Berkeley) 6/4/93
# $FreeBSD: projects/armv6/lib/libc/i386/gen/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS+=	_ctx_start.S _setjmp.S _set_tp.c fabs.S \
	flt_rounds.c getcontextx.c infinity.c ldexp.c makecontext.c \
	rfork_thread.S setjmp.S signalcontext.c sigsetjmp.S
