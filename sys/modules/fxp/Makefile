# $FreeBSD: projects/armv6/sys/modules/fxp/Makefile 226747 2011-10-25 19:47:28Z cognet $

.PATH: ${.CURDIR}/../../dev/fxp

KMOD=	if_fxp
SRCS=	device_if.h bus_if.h if_fxp.c inphy.c miibus_if.h miidevs.h pci_if.h

.include <bsd.kmod.mk>
