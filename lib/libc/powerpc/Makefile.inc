# $FreeBSD: projects/armv6/lib/libc/powerpc/Makefile.inc 156613 2006-03-13 01:15:01Z deischen $

# Long double is 64-bits
MDSRCS+=machdep_ldisd.c
SYM_MAPS+=${.CURDIR}/powerpc/Symbol.map
