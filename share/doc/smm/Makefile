#	From: @(#)Makefile	8.1 (Berkeley) 6/10/93
# $FreeBSD: projects/armv6/share/doc/smm/Makefile 220517 2011-04-10 12:43:13Z uqs $

.include <bsd.own.mk>

# The following modules do not describe FreeBSD:
# 14.uucpimpl, 15.uucpnet

# The following modules do not build/install:
# 13.amd (documentation is TeXinfo)
# 16.security 17.password (encumbered)

SUBDIR=	title \
	contents \
	01.setup \
	02.config \
	03.fsck \
	04.quotas \
	05.fastfs \
	06.nfs \
	07.lpd \
	${_08.sendmailop} \
	11.timedop \
	12.timed \
	18.net

.if ${MK_SENDMAIL} != "no"
_08.sendmailop=	08.sendmailop 
.endif

.include <bsd.subdir.mk>
