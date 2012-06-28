# $FreeBSD: projects/armv6/usr.bin/clang/lli/Makefile 234858 2012-05-01 04:01:22Z gonzo $

PROG_CXX=lli

SRCDIR=	tools/lli
SRCS=	lli.cpp

LIBDEPS=llvmasmparser \
	llvmbitreader \
	llvmx86codegen \
	llvmx86desc \
	llvmx86info \
	llvmx86instprinter \
	llvmx86utils \
	llvmselectiondag \
	llvmasmprinter \
	llvmmcparser \
	llvminterpreter \
	llvmjit \
	llvmcodegen \
	llvmscalaropts \
	llvminstcombine \
	llvmtransformutils \
	llvmipa \
	llvmanalysis \
	llvmmcjit \
	llvmruntimedyld \
	llvmexecutionengine \
	llvmtarget \
	llvmmc \
	llvmobject \
	llvmcore \
	llvmsupport

.include "../clang.prog.mk"
