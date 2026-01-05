/*
 * Function: __isnan
 * Address:  0x80002C60
 * Category: libm
 * Status:   WIP
 *
 * Check if double is NaN (Not a Number).
 * Returns 1 if NaN, 0 otherwise.
 * IEEE 754: NaN has exponent 0x7FF and mantissa != 0.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef union {
    f64 d;
    struct {
        u32 hi;
        u32 lo;
    } w;
} Double;

s32 __isnan(f64 value) {
    Double u;
    s32 exp;

    u.d = value;
    exp = (u.w.hi << 1) >> 21;

    if (exp != 0x7FF) {
        return 0;
    }

    /* Check if mantissa is zero (would be infinity) */
    u.w.hi &= 0x800F;
    if (u.d == 0.0) {
        return 1;
    }
    return 0;
}
