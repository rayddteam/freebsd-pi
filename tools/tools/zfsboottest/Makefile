# $FreeBSD: projects/armv6/tools/tools/zfsboottest/Makefile 226747 2011-10-25 19:47:28Z cognet $

.PATH:		${.CURDIR}/../../../sys/boot/zfs ${.CURDIR}/../../../sys/cddl/boot/zfs

BINDIR?=	/usr/bin
SCRIPTSDIR?=	/usr/bin

PROG=		zfsboottest
SCRIPTS=	zfsboottest.sh
SCRIPTSNAME=	zfsboottest.sh
NO_MAN=

CFLAGS=	-O1 \
	-I${.CURDIR}/../../../sys/boot/zfs \
	-I${.CURDIR}/../../../sys/cddl/boot/zfs \
	-I. \
	-fdiagnostics-show-option \
	-W -Wextra -Wno-sign-compare -Wno-unused-parameter \
	-Werror
LDFLAGS+=-lmd

.if ${MACHINE_CPUARCH} == "amd64"
beforedepend zfsboottest.o: machine
CLEANFILES+=	machine
machine:
	ln -sf ${.CURDIR}/../../../sys/i386/include machine
.endif

.include <bsd.prog.mk>
