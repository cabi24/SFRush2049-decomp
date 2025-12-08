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
 * (func_8000D8F0 - __lshrdi3)
 */
u64 __lshrdi3(u64 a, u32 shift) {
    return a >> shift;
}

/**
 * Unsigned 64-bit modulo
 * (func_8000D91C - __umoddi3)
 */
u64 __umoddi3(u64 a, u64 b) {
    return a % b;
}

/**
 * Unsigned 64-bit division
 * (func_8000D958 - __udivdi3)
 */
u64 __udivdi3(u64 a, u64 b) {
    return a / b;
}

/**
 * Arithmetic left shift for 64-bit integers
 * (func_8000D994 - __ashldi3)
 */
s64 __ashldi3(s64 a, u32 shift) {
    return a << shift;
}

/**
 * Signed 64-bit division
 * (func_8000D9FC - __divdi3)
 */
s64 __divdi3(s64 a, s64 b) {
    return a / b;
}

/**
 * 64-bit multiplication
 * (func_8000DA58 - __muldi3)
 */
s64 __muldi3(s64 a, s64 b) {
    return a * b;
}

/**
 * Unsigned 64-bit divmod (returns quotient, stores remainder)
 * (func_8000DA88 - __udivmoddi4)
 *
 * @param quotient Pointer to store quotient
 * @param remainder Pointer to store remainder
 * @param a Dividend
 * @param b Divisor (passed as lower 16 bits)
 */
void __udivmoddi4(u64 *quotient, u64 *remainder, u64 a, u16 b) {
    *quotient = a / b;
    *remainder = a % b;
}

/**
 * Signed 64-bit modulo
 * (func_8000DAE8 - __moddi3)
 */
s64 __moddi3(s64 a, s64 b) {
    return a % b;
}

/**
 * Arithmetic right shift for 64-bit signed integers
 * (func_8000DB84 - __ashrdi3)
 */
s64 __ashrdi3(s64 a, u32 shift) {
    return a >> shift;
}
