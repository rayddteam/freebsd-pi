#	$NetBSD: Makefile.inc,v 1.2 2000/10/10 21:51:54 jeffs Exp $
# $FreeBSD: projects/armv6/lib/libc/mips/string/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS+=	bcmp.S bcopy.S bzero.S ffs.S memchr.c memcmp.c memset.c \
	memcpy.S memmove.S \
	strcat.c strchr.S strcmp.S strcpy.c strcspn.c strlen.S \
	strncat.c strncmp.c strncpy.c strrchr.S strpbrk.c strsep.c \
	strspn.c strstr.c swab.c
