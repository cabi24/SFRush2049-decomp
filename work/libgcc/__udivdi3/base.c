/*
 * Function: __udivdi3
 * Address:  0x8000D958
 * Category: libgcc
 * Status:   WIP
 *
 * Unsigned 64-bit division.
 * Uses MIPS III ddivu instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Unsigned 64-bit divide */
u64 __udivdi3(u64 dividend, u64 divisor) {
    return dividend / divisor;
}
