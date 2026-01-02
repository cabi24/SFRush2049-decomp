/*
 * Function: osWritebackDCacheAll
 * Address:  0x8000870C
 * Category: libultra/cache
 * Status:   WIP
 *
 * writeback all data cache
 * This is a stub - stores args to stack and returns
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Stub that stores 4 args to stack and returns */
void osWritebackDCacheAll(u32 a0, u32 a1, u32 a2, u32 a3) {
    /* volatile to force spills */
    volatile u32 v0 = a0;
    volatile u32 v1 = a1;
    volatile u32 v2 = a2;
    volatile u32 v3 = a3;
    (void)v0; (void)v1; (void)v2; (void)v3;
}
