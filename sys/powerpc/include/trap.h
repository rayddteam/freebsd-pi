/* $FreeBSD: projects/armv6/sys/powerpc/include/trap.h 234858 2012-05-01 04:01:22Z gonzo $ */

#if defined(AIM)
#include <machine/trap_aim.h>
#elif defined(E500)
#include <machine/trap_booke.h>
#endif

