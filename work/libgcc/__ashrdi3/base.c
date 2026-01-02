/*
 * Function: __ashrdi3
 * Address:  0x8000DB84
 * Category: libgcc
 * Status:   WIP
 *
 * 64-bit arithmetic right shift (sign-extending).
 * Returns (value >> shift) with sign extension.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* 64-bit arithmetic right shift */
s64 __ashrdi3(s64 value, s32 shift) {
    return value >> shift;
}
