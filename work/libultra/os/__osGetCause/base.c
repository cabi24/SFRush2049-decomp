/*
 * Function: __osGetCause
 * Address:  0x8000DBB0
 * Category: libultra/os
 * Status:   WIP
 *
 * Get CP0 Cause register ($13)
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Read the CP0 Cause register */
u32 __osGetCause(void) {
    u32 cause;
    __asm__ volatile("mfc0 %0, $13" : "=r"(cause));
    return cause;
}
