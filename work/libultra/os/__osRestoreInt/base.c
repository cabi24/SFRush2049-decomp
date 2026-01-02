/*
 * Function: __osRestoreInt
 * Address:  0x8000C520
 * Category: libultra/os
 * Status:   WIP
 *
 * Restore interrupt state by ORing bits back into Status register
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* OR the given bits into CP0 Status register */
void __osRestoreInt(u32 mask) {
    u32 sr;
    __asm__ volatile(
        "mfc0 %0, $12\n\t"
        "or %0, %0, %1\n\t"
        "mtc0 %0, $12\n\t"
        "nop\n\t"
        "nop"
        : "=&r"(sr)
        : "r"(mask)
    );
}
