# $FreeBSD: projects/armv6/tools/regression/lib/msun/Makefile 226747 2011-10-25 19:47:28Z cognet $

TESTS=	test-cexp test-conj test-csqrt test-ctrig \
	test-exponential test-fenv test-fma \
	test-fmaxmin test-ilogb test-invtrig test-logarithm test-lrint \
	test-lround test-nan test-nearbyint test-next test-rem test-trig
CFLAGS+= -O0 -lm

.PHONY: tests
tests: ${TESTS}
	for p in ${TESTS}; do ${.OBJDIR}/$$p; done

.PHONY: clean
clean:
	-rm -f ${TESTS}
