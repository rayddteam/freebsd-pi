# $FreeBSD: projects/armv6/sys/boot/uboot/Makefile.inc 228136 2011-11-29 15:24:19Z cognet $

.if ${MACHINE_ARCH} == "powerpc64"
CFLAGS+=	-m32 -mcpu=powerpc
LDFLAGS+=	-m elf32ppc_fbsd
.endif

.include "../Makefile.inc"
