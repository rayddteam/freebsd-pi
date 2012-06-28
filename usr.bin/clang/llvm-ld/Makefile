# $FreeBSD: projects/armv6/usr.bin/clang/llvm-ld/Makefile 234858 2012-05-01 04:01:22Z gonzo $

PROG_CXX=llvm-ld

SRCDIR=	tools/llvm-ld
SRCS=	Optimize.cpp \
	llvm-ld.cpp

TGHDRS=	Intrinsics
LIBDEPS=llvmbitwriter \
	llvmlinker \
	llvmarchive \
	llvmbitreader \
	llvmipo \
	llvmvectorize \
	llvmscalaropts \
	llvminstcombine \
	llvmtransformutils \
	llvmipa \
	llvmanalysis \
	llvmtarget \
	llvmmc \
	llvmobject \
	llvmcore \
	llvmsupport

.include "../clang.prog.mk"
