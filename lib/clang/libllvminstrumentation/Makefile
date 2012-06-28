# $FreeBSD: projects/armv6/lib/clang/libllvminstrumentation/Makefile 234858 2012-05-01 04:01:22Z gonzo $

LIB=	llvminstrumentation

SRCDIR=	lib/Transforms/Instrumentation
SRCS=	AddressSanitizer.cpp \
	EdgeProfiling.cpp \
	FunctionBlackList.cpp \
	GCOVProfiling.cpp \
	Instrumentation.cpp \
	OptimalEdgeProfiling.cpp \
	PathProfiling.cpp \
	ProfilingUtils.cpp \
	ThreadSanitizer.cpp

TGHDRS=	Intrinsics

.include "../clang.lib.mk"
