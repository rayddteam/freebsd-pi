# $FreeBSD: projects/armv6/sys/modules/drm/r128/Makefile 232120 2012-02-24 18:39:55Z cognet $

.PATH:	${.CURDIR}/../../../dev/drm
KMOD	= r128
SRCS	= r128_cce.c r128_drv.c r128_irq.c r128_state.c
SRCS	+=device_if.h bus_if.h pci_if.h opt_drm.h

.include <bsd.kmod.mk>

CWARNFLAGS.r128_cce.c=		${NO_WUNUSED_VALUE} ${NO_WCONSTANT_CONVERSION}
CWARNFLAGS.r128_state.c=	${NO_WUNUSED_VALUE}
CWARNFLAGS+=			${CWARNFLAGS.${.IMPSRC:T}}
