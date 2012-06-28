/* $FreeBSD: projects/armv6/lib/libcompiler_rt/__sync_fetch_and_sub_8.c 232120 2012-02-24 18:39:55Z cognet $ */
#define	NAME		__sync_fetch_and_sub_8
#define	TYPE		uint64_t
#define	FETCHADD(x, y)	atomic_fetchadd_64(x, -(y))

#include "__sync_fetch_and_op_n.h"
