/*
 * Function: bzero_alt (actually __osSpGetStatus!)
 * Address:  0x8000D2A0
 * Category: libc (should be libultra/sp)
 * Status:   WIP
 *
 * MISNAMED: This actually reads SP_STATUS_REG and returns it.
 * Real name should be __osSpGetStatus.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

#define SP_STATUS_REG (*(volatile u32 *)0xA4040010)

/* Returns SP status register value */
u32 bzero_alt(void) {
    return SP_STATUS_REG;
}
