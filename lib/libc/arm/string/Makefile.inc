#	@(#)Makefile.inc	8.1 (Berkeley) 6/4/93
# $FreeBSD: projects/armv6/lib/libc/arm/string/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

MDSRCS+=bcmp.c bcopy.S bzero.S ffs.S memchr.c memcmp.S memcpy.S \
	memmove.S memset.S strcat.c strchr.c strcmp.S strcpy.c strlen.S \
	strncmp.S strrchr.c swab.c wcschr.c wcscmp.c wcslen.c wmemchr.c
