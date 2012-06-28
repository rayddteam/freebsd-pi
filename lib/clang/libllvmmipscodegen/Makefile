# $FreeBSD: projects/armv6/lib/clang/libllvmmipscodegen/Makefile 234858 2012-05-01 04:01:22Z gonzo $

LIB=	llvmmipscodegen

SRCDIR=	lib/Target/Mips
SRCS=	MipsAnalyzeImmediate.cpp \
	MipsAsmPrinter.cpp \
	MipsCodeEmitter.cpp \
	MipsDelaySlotFiller.cpp \
	MipsEmitGPRestore.cpp \
	MipsExpandPseudo.cpp \
	MipsFrameLowering.cpp \
	MipsISelDAGToDAG.cpp \
	MipsISelLowering.cpp \
	MipsInstrInfo.cpp \
	MipsJITInfo.cpp \
	MipsMCInstLower.cpp \
	MipsMachineFunction.cpp \
	MipsRegisterInfo.cpp \
	MipsSelectionDAGInfo.cpp \
	MipsSubtarget.cpp \
	MipsTargetMachine.cpp \
	MipsTargetObjectFile.cpp

TGHDRS=	Intrinsics \
	MipsGenAsmWriter \
	MipsGenCallingConv \
	MipsGenCodeEmitter \
	MipsGenDAGISel \
	MipsGenInstrInfo \
	MipsGenRegisterInfo \
	MipsGenSubtargetInfo

.include "../clang.lib.mk"
