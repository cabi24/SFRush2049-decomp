/*
 * Function: __ashldi3
 * Address:  0x8000D994
 * Category: libgcc
 * Status:   WIP
 *
 * 64-bit arithmetic left shift using MIPS III 64-bit instructions.
 * Returns (value << shift).
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* 64-bit arithmetic left shift */
s64 __ashldi3(s64 value, s32 shift) {
    return value << shift;
}
