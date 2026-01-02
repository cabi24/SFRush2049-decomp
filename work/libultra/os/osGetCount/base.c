/*
 * Function: osGetCount
 * Address:  0x8000C970
 * Category: libultra/os
 * Status:   WIP
 *
 * Read CP0 Count register ($9)
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Read the CPU cycle counter (CP0 reg 9) */
u32 osGetCount(void) {
    u32 count;
    __asm__ volatile("mfc0 %0, $9" : "=r"(count));
    return count;
}
