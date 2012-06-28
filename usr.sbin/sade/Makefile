# $FreeBSD: projects/armv6/usr.sbin/sade/Makefile 228136 2011-11-29 15:24:19Z cognet $

.if ${MACHINE_CPUARCH} != "ia64"
_wizard=	wizard.c
.endif

PROG=	sade
MAN=	sade.8
SRCS=	command.c devices.c \
	disks.c dmenu.c \
	globals.c install.c \
	label.c main.c menus.c misc.c \
	msg.c system.c termcap.c \
	variable.c ${_wizard}
WARNS?=	3

# command.c

.if ${MACHINE} == "pc98"
CFLAGS+= -DPC98
.endif
CFLAGS+= -I${.CURDIR}/../../contrib/dialog -I.

DPADD=	${LIBDIALOG} ${LIBNCURSESW} ${LIBM} ${LIBUTIL} ${LIBDISK}
LDADD=	-ldialog -lncursesw -lm -lutil -ldisk

.include <bsd.prog.mk>
