/* $NetBSD: letf2.c,v 1.1 2011/01/17 10:08:35 matt Exp $ */

/*
 * Written by Matt Thomas, 2011.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: projects/armv6/lib/libc/softfloat/letf2.c 232120 2012-02-24 18:39:55Z cognet $");

#ifdef FLOAT128

flag __letf2(float128, float128);

flag
__letf2(float128 a, float128 b)
{

	/* libgcc1.c says 1 - (a <= b) */
	return 1 - float128_le(a, b);
}

#endif /* FLOAT128 */
