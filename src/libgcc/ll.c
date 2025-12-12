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

/* ============================================
 * Floating-point <-> 64-bit integer conversions
 * Decompiled from asm/us/A5F0.s
 * ============================================ */

/**
 * Convert double to signed 64-bit integer (truncate toward zero)
 * (func_800099F0 - __fixdfdi)
 *
 * Uses MIPS trunc.l.d instruction for direct conversion.
 * Returns value in v0:v1 (hi:lo) register pair.
 */
s64 __fixdfdi(f64 a) {
    return (s64)a;
}

/**
 * Convert float to signed 64-bit integer (truncate toward zero)
 * (func_80009A0C - __fixsfdi)
 *
 * Uses MIPS trunc.l.s instruction for direct conversion.
 */
s64 __fixsfdi(f32 a) {
    return (s64)a;
}

/**
 * Convert double to unsigned 64-bit integer
 * (func_80009A28 - __fixunsdfdi)
 *
 * Handles the full unsigned range by subtracting 2^63 for large values,
 * converting, then adding the high bit back via OR.
 * Uses FPU control status register for exception handling.
 */
u64 __fixunsdfdi(f64 a) {
    return (u64)a;
}

/**
 * Convert float to unsigned 64-bit integer
 * (func_80009AC8 - __fixunssfdi)
 *
 * Similar to __fixunsdfdi but for single precision.
 */
u64 __fixunssfdi(f32 a) {
    return (u64)a;
}

/**
 * Convert signed 64-bit integer to double
 * (func_80009B64 - __floatdidf)
 *
 * Uses MIPS cvt.d.l instruction for direct conversion.
 * Takes value in a0:a1 (hi:lo), stores on stack as 64-bit,
 * then uses dmtc1 to move to FPU for conversion.
 */
f64 __floatdidf(s64 a) {
    return (f64)a;
}

/**
 * Convert signed 64-bit integer to float
 * (func_80009B7C - __floatdisf)
 *
 * Uses MIPS cvt.s.l instruction for direct conversion.
 */
f32 __floatdisf(s64 a) {
    return (f32)a;
}

/**
 * Convert unsigned 64-bit integer to double
 * (func_80009B94 - __floatundidf)
 *
 * Handles the full unsigned range by checking sign bit
 * and adding 2^64 correction for negative (high-bit-set) values.
 */
f64 __floatundidf(u64 a) {
    return (f64)a;
}

/**
 * Convert unsigned 64-bit integer to float
 * (func_80009BC8 - __floatundisf)
 *
 * Similar to __floatundidf but for single precision result.
 */
f32 __floatundisf(u64 a) {
    return (f32)a;
}
