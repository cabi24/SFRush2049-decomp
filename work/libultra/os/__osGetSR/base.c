/*
 * Function: __osGetSR
 * Address:  0x8000D780
 * Category: libultra/os
 * Status:   WIP
 *
 * Get CP0 Status register ($12)
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Read the CP0 Status register */
u32 __osGetSR(void) {
    u32 sr;
    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    return sr;
}
