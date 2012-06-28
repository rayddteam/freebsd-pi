# $FreeBSD: projects/armv6/lib/clang/libllvmipa/Makefile 232120 2012-02-24 18:39:55Z cognet $

.include <bsd.own.mk>

LIB=	llvmipa

SRCDIR=	lib/Analysis/IPA
SRCS=	CallGraph.cpp \
	CallGraphSCCPass.cpp \
	FindUsedTypes.cpp \
	GlobalsModRef.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	IPA.cpp
.endif

TGHDRS=	Intrinsics

.include "../clang.lib.mk"
