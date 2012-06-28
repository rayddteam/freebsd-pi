# $FreeBSD: projects/armv6/lib/clang/libllvmmcjit/Makefile 234858 2012-05-01 04:01:22Z gonzo $

.include <bsd.own.mk>

LIB=	llvmmcjit

SRCDIR=	lib/ExecutionEngine/MCJIT
SRCS=	MCJIT.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	MCJITMemoryManager.cpp
.endif

.include "../clang.lib.mk"
