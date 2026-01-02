/*
 * Function: osWritebackDCache_full
 * Address:  0x800086F0
 * Category: libultra/cache
 * Status:   WIP
 *
 * Stub that saves 4 args to stack and returns.
 * Same pattern as osWritebackDCacheAll.
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Stub - saves args and returns */
void osWritebackDCache_full(u32 a0, u32 a1, u32 a2, u32 a3) {
    volatile u32 v0 = a0;
    volatile u32 v1 = a1;
    volatile u32 v2 = a2;
    volatile u32 v3 = a3;
    (void)v0; (void)v1; (void)v2; (void)v3;
}
