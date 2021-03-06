# $FreeBSD: projects/armv6/share/mk/bsd.dtrace.mk 212461 2010-09-11 10:11:59Z rpaulo $
#
# Copyright (c) 2010 The FreeBSD Foundation 
# All rights reserved. 
# 
# This software was developed by Rui Paulo under sponsorship from the
# FreeBSD Foundation. 
#  
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions 
# are met: 
# 1. Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright 
#    notice, this list of conditions and the following disclaimer in the 
#    documentation and/or other materials provided with the distribution. 
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND 
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE 
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
# SUCH DAMAGE. 
# 
#
# The only variable that you should define on your Makefile is 'DTRACEOBJS'.
# You must include this file before bsd.lib.mk or bsd.prog.mk.
#

.if defined(WITH_DTRACE)

CFLAGS+=-DWITH_DTRACE
DTRACEHEADERS=${DTRACEOBJS:S/o$/h/}
DTRACESRCS=${DTRACEOBJS:S/o$/d/}
CLEANFILES+=${DTRACEOBJS} ${DTRACEHEADERS}

DPADD+=${LIBELF}
LDADD+=-lelf

.if defined(PROG)
_DTRACELINKING=${OBJS}
OBJS+=${DTRACEOBJS}
.else
_DTRACELINKING=${SOBJS}
SOBJS+=${DTRACEOBJS}
.endif

${DTRACEOBJS}:

beforedepend:
	${DTRACE} -C -h -s ${DTRACESRCS}
beforelinking:
	${DTRACE} -G -s ${DTRACESRCS} ${_DTRACELINKING:S/${DTRACEOBJS}//}

.endif
