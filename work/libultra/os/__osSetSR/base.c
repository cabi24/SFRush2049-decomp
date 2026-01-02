/*
 * Function: __osSetSR
 * Address:  0x8000D770
 * Category: libultra/os
 * Status:   WIP
 *
 * Set CP0 Status register ($12)
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Write to CP0 Status register */
void __osSetSR(u32 sr) {
    __asm__ volatile("mtc0 %0, $12; nop" : : "r"(sr));
}
