# $FreeBSD: projects/armv6/lib/libc/powerpc64/gen/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS += _ctx_start.S fabs.S flt_rounds.c fpgetmask.c fpgetround.c \
	fpgetsticky.c fpsetmask.c fpsetround.c getcontextx.c \
	infinity.c ldexp.c makecontext.c _setjmp.S \
	setjmp.S sigsetjmp.S signalcontext.c syncicache.c \
	_set_tp.c


