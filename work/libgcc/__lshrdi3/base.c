/*
 * Function: __lshrdi3
 * Address:  0x8000D8F0
 * Category: libgcc
 * Status:   WIP
 *
 * 64-bit logical right shift using MIPS III 64-bit instructions.
 * Returns (value >> shift).
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* 64-bit logical right shift */
u64 __lshrdi3(u64 value, s32 shift) {
    return value >> shift;
}
