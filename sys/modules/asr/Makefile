# $FreeBSD: projects/armv6/sys/modules/asr/Makefile 232120 2012-02-24 18:39:55Z cognet $

.PATH: ${.CURDIR}/../../dev/asr

KMOD=	asr
SRCS=	asr.c
SRCS+=	opt_scsi.h opt_cam.h
SRCS+=	device_if.h bus_if.h pci_if.h

.if ${MACHINE_CPUARCH} == "i386"
SRCS+=	opt_asr.h
.endif

.include <bsd.kmod.mk>

CWARNFLAGS.asr.c=	${NO_WARRAY_BOUNDS}
CWARNFLAGS+=		${CWARNFLAGS.${.IMPSRC:T}}
