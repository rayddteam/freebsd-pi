# $FreeBSD: projects/armv6/etc/devd/Makefile 226747 2011-10-25 19:47:28Z cognet $

FILES=	uath.conf usb.conf

.if ${MACHINE} == "powerpc"
FILES+=	apple.conf
.endif

.if ${MACHINE} == "amd64" || ${MACHINE} == "i386"
FILES+=	asus.conf
.endif

NO_OBJ=
FILESDIR=	/etc/devd
FILESMODE=	644

.include <bsd.prog.mk>
