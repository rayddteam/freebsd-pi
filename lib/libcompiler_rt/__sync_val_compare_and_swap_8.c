/* $FreeBSD: projects/armv6/lib/libcompiler_rt/__sync_val_compare_and_swap_8.c 232120 2012-02-24 18:39:55Z cognet $ */
#define	NAME		__sync_val_compare_and_swap_8
#define	TYPE		uint64_t
#define	CMPSET		atomic_cmpset_64

#include "__sync_val_compare_and_swap_n.h"
