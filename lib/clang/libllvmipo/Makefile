# $FreeBSD: projects/armv6/lib/clang/libllvmipo/Makefile 232120 2012-02-24 18:39:55Z cognet $

.include <bsd.own.mk>

LIB=	llvmipo

SRCDIR=	lib/Transforms/IPO
SRCS=	ArgumentPromotion.cpp \
	ConstantMerge.cpp \
	DeadArgumentElimination.cpp \
	ExtractGV.cpp \
	FunctionAttrs.cpp \
	GlobalDCE.cpp \
	GlobalOpt.cpp \
	IPConstantPropagation.cpp \
	InlineAlways.cpp \
	InlineSimple.cpp \
	Inliner.cpp \
	Internalize.cpp \
	LoopExtractor.cpp \
	MergeFunctions.cpp \
	PartialInlining.cpp \
	PassManagerBuilder.cpp \
	PruneEH.cpp \
	StripDeadPrototypes.cpp \
	StripSymbols.cpp

.if ${MK_CLANG_EXTRAS} != "no"
SRCS+=	IPO.cpp
.endif

TGHDRS=	Intrinsics

.include "../clang.lib.mk"
