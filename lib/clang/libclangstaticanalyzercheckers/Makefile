# $FreeBSD: projects/armv6/lib/clang/libclangstaticanalyzercheckers/Makefile 234858 2012-05-01 04:01:22Z gonzo $

LIB=	clangstaticanalyzercheckers

SRCDIR=	tools/clang/lib/StaticAnalyzer/Checkers
SRCS=	AdjustedReturnValueChecker.cpp \
	AnalyzerStatsChecker.cpp \
	ArrayBoundChecker.cpp \
	ArrayBoundCheckerV2.cpp \
	AttrNonNullChecker.cpp \
	BasicObjCFoundationChecks.cpp \
	BoolAssignmentChecker.cpp \
	BuiltinFunctionChecker.cpp \
	CStringChecker.cpp \
	CStringSyntaxChecker.cpp \
	CallAndMessageChecker.cpp \
	CastSizeChecker.cpp \
	CastToStructChecker.cpp \
	CheckObjCDealloc.cpp \
	CheckObjCInstMethSignature.cpp \
	CheckSecuritySyntaxOnly.cpp \
	CheckSizeofPointer.cpp \
	CheckerDocumentation.cpp \
	ChrootChecker.cpp \
	ClangCheckers.cpp \
	CommonBugCategories.cpp \
	DeadStoresChecker.cpp \
	DebugCheckers.cpp \
	DereferenceChecker.cpp \
	DivZeroChecker.cpp \
	FixedAddressChecker.cpp \
	GenericTaintChecker.cpp \
	IdempotentOperationChecker.cpp \
	IteratorsChecker.cpp \
	LLVMConventionsChecker.cpp \
	MacOSKeychainAPIChecker.cpp \
	MacOSXAPIChecker.cpp \
	MallocChecker.cpp \
	MallocOverflowSecurityChecker.cpp \
	MallocSizeofChecker.cpp \
	NSAutoreleasePoolChecker.cpp \
	NSErrorChecker.cpp \
	NoReturnFunctionChecker.cpp \
	OSAtomicChecker.cpp \
	ObjCAtSyncChecker.cpp \
	ObjCContainersASTChecker.cpp \
	ObjCContainersChecker.cpp \
	ObjCSelfInitChecker.cpp \
	ObjCUnusedIVarsChecker.cpp \
	PointerArithChecker.cpp \
	PointerSubChecker.cpp \
	PthreadLockChecker.cpp \
	RetainCountChecker.cpp \
	ReturnPointerRangeChecker.cpp \
	ReturnUndefChecker.cpp \
	StackAddrEscapeChecker.cpp \
	StreamChecker.cpp \
	TaintTesterChecker.cpp \
	UndefBranchChecker.cpp \
	UndefCapturedBlockVarChecker.cpp \
	UndefResultChecker.cpp \
	UndefinedArraySubscriptChecker.cpp \
	UndefinedAssignmentChecker.cpp \
	UnixAPIChecker.cpp \
	UnreachableCodeChecker.cpp \
	VLASizeChecker.cpp \
	VirtualCallChecker.cpp

TGHDRS=	AttrList \
	Attrs \
	Checkers \
	DeclNodes \
	DiagnosticCommonKinds \
	StmtNodes

.include "../clang.lib.mk"
