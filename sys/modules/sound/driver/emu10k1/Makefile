# $FreeBSD: projects/armv6/sys/modules/sound/driver/emu10k1/Makefile 232120 2012-02-24 18:39:55Z cognet $

.PATH: ${.CURDIR}/../../../../dev/sound/pci

KMOD=	snd_emu10k1
SRCS=	device_if.h bus_if.h pci_if.h
SRCS+= mpufoi_if.h
SRCS+=	emu10k1.c

.include <bsd.kmod.mk>
