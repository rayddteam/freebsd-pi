/* $FreeBSD: projects/armv6/lib/libcompiler_rt/__sync_lock_test_and_set_4.c 232120 2012-02-24 18:39:55Z cognet $ */
#define	NAME		__sync_lock_test_and_set_4
#define	TYPE		uint32_t
#define	CMPSET		atomic_cmpset_32
#define	EXPRESSION	value

#include "__sync_fetch_and_op_n.h"
