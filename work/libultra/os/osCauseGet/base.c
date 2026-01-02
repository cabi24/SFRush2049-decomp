/*
 * Function: osCauseGet
 * Address:  0x8000DBB0
 * Category: libultra/os
 * Status:   WIP
 *
 * Read CP0 Cause register ($13)
 * NOTE: Same as __osGetCause (alias at same address)
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Read the CP0 Cause register */
u32 osCauseGet(void) {
    u32 cause;
    __asm__ volatile("mfc0 %0, $13" : "=r"(cause));
    return cause;
}
