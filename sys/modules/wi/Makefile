# $FreeBSD: projects/armv6/sys/modules/wi/Makefile 232120 2012-02-24 18:39:55Z cognet $

.PATH: ${.CURDIR}/../../dev/wi

KMOD=	if_wi
SRCS=	if_wi.c if_wi_pccard.c if_wi_pci.c \
	card_if.h device_if.h bus_if.h pci_if.h pccarddevs.h \
	opt_wlan.h

.include <bsd.kmod.mk>
