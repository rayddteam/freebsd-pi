# $FreeBSD: projects/armv6/lib/csu/Makefile.inc 234858 2012-05-01 04:01:22Z gonzo $

SSP_CFLAGS=

SED_FIX_NOTE = -i "" -e '/\.note\.tag/s/progbits/note/'

.include "../Makefile.inc"
