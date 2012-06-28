/* $NetBSD: lttf2.c,v 1.1 2011/01/17 10:08:35 matt Exp $ */

/*
 * Written by Matt Thomas, 2011.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: projects/armv6/lib/libc/softfloat/lttf2.c 232120 2012-02-24 18:39:55Z cognet $");

#ifdef FLOAT128

flag __lttf2(float128, float128);

flag
__lttf2(float128 a, float128 b)
{

	/* libgcc1.c says -(a < b) */
	return -float128_lt(a, b);
}

#endif /* FLOAT128 */
