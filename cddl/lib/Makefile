# $FreeBSD: projects/armv6/cddl/lib/Makefile 234858 2012-05-01 04:01:22Z gonzo $

.include <bsd.own.mk>

SUBDIR=	${_drti} \
	libavl \
	libctf \
	${_libdtrace} \
	libnvpair \
	libumem \
	libuutil \
	${_libzfs} \
	${_libzpool}

.if ${MK_ZFS} != "no"
_libzfs=	libzfs
.if ${MK_LIBTHR} != "no"
_libzpool=	libzpool
.endif
.endif

.if ${MACHINE_ARCH} == "amd64" || ${MACHINE_ARCH} == "i386" ||  ${MACHINE_CPUARCH} == "mips"
_drti=		drti
_libdtrace=	libdtrace
.endif

.include <bsd.subdir.mk>
