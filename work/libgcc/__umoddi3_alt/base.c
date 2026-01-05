/*
 * Function: __umoddi3_alt
 * Address:  0x8000D9C0
 * Category: libgcc
 * Status:   WIP
 *
 * Alternate unsigned 64-bit modulo with stack-based arguments.
 * Stores argument registers to stack, loads as 64-bit, performs ddivu.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Alternate entry point for __umoddi3 with explicit stack handling */
u64 __umoddi3_alt(u64 dividend, u64 divisor) {
    return dividend % divisor;
}
