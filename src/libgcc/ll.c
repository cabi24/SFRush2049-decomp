/**
 * @file ll.c
 * @brief 64-bit arithmetic support functions (libgcc)
 *
 * These functions provide 64-bit integer operations for the MIPS R4300i.
 * Based on standard libgcc implementations used by N64 games.
 *
 * Decompiled from asm/us/E4F0.s
 */

#include "types.h"

/**
 * Logical right shift for 64-bit unsigned integers
 */
u64 __lshrdi3(u64 a, u32 shift) {
    return a >> shift;
}

/**
 * Arithmetic left shift for 64-bit integers
 */
s64 __ashldi3(s64 a, u32 shift) {
    return a << shift;
}

/**
 * Arithmetic right shift for 64-bit signed integers
 */
s64 __ashrdi3(s64 a, u32 shift) {
    return a >> shift;
}

/**
 * Unsigned 64-bit modulo
 */
u64 __umoddi3(u64 a, u64 b) {
    return a % b;
}

/**
 * Unsigned 64-bit division
 */
u64 __udivdi3(u64 a, u64 b) {
    return a / b;
}

/**
 * Signed 64-bit division
 */
s64 __divdi3(s64 a, s64 b) {
    return a / b;
}

/**
 * Signed 64-bit modulo
 */
s64 __moddi3(s64 a, s64 b) {
    return a % b;
}

/**
 * 64-bit multiplication
 */
s64 __muldi3(s64 a, s64 b) {
    return a * b;
}
