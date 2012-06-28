/* $FreeBSD: projects/armv6/lib/libcompiler_rt/__sync_val_compare_and_swap_4.c 232120 2012-02-24 18:39:55Z cognet $ */
#define	NAME		__sync_val_compare_and_swap_4
#define	TYPE		uint32_t
#define	CMPSET		atomic_cmpset_32

#include "__sync_val_compare_and_swap_n.h"
