/*
 * Function: __divdi3
 * Address:  0x8000D9FC
 * Category: libgcc
 * Status:   WIP
 *
 * Signed 64-bit division.
 * Uses MIPS III ddiv instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Signed 64-bit divide */
s64 __divdi3(s64 dividend, s64 divisor) {
    return dividend / divisor;
}
