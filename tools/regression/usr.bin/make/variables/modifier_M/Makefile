# $FreeBSD: projects/armv6/tools/regression/usr.bin/make/variables/modifier_M/Makefile 232120 2012-02-24 18:39:55Z cognet $
#
# Test the M modifier.
#
FILES	= \
		main.c globals.h \
		util.c util.h \
		map.c map.h \
		parser.y lexer.l \
		cmdman.1 format.5
test1:
	@echo "all files: ${FILES}"
	@echo "cfiles: ${FILES:M*.c}"
	@echo "hfiles: ${FILES:M*.h}"
	@echo "grammar and lexer: ${FILES:M*.[ly]}"
	@echo "man page: ${FILES:M*.[1-9]}"
	@echo "utility files: ${FILES:Mutil.?}"
	@echo "m files: ${FILES:Mm*}"
