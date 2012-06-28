/* $NetBSD: negxf2.c,v 1.2 2004/09/27 10:16:24 he Exp $ */

/*
 * Written by Ben Harris, 2000.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: projects/armv6/lib/libc/softfloat/negxf2.c 232120 2012-02-24 18:39:55Z cognet $");

#ifdef FLOATX80

floatx80 __negxf2(floatx80);

floatx80
__negxf2(floatx80 a)
{

	/* libgcc1.c says -a */
	return __mulxf3(a,__floatsixf(-1));
}
#endif /* FLOATX80 */
