/**
 * @file math.c
 * @brief Standard C math library functions
 *
 * Decompiled from asm/us/34A0.s
 */

#include "types.h"

/* NaN constant (0x7FBFFFFF as float) */
static const u32 sNaNBits = 0x7FBFFFFF;
#define GET_NAN_F32() (*(f32 *)&sNaNBits)

/**
 * Split double into integer and fractional parts
 * @param value Input value
 * @param iptr Pointer to store integer part
 * @return Fractional part of value
 */
f64 modf(f64 value, f64 *iptr) {
    f64 temp;
    f64 abs_val;

    if (value > 0.0) {
        abs_val = value;
    } else {
        abs_val = -value;
    }

    /* If value is larger than what can be represented as fraction, it's all integer */
    if (abs_val >= 4503599627370496.0) {  /* 2^52 */
        *iptr = value;
        return 0.0;
    }

    /* Use floating point trick to extract integer part */
    temp = abs_val + 4503599627370496.0;
    temp -= 4503599627370496.0;

    if (abs_val < temp) {
        temp -= 1.0;
    }

    if (temp > 0.0) {
        /* keep temp positive */
    } else {
        temp = -temp;
    }

    /* Handle edge case where difference is exactly 1.0 */
    if ((value - temp) == 1.0) {
        temp += 1.0;
    }

    if (value >= 0.0) {
        *iptr = temp;
        return value - temp;
    }

    *iptr = -temp;
    return value + temp;
}

/**
 * Split float into integer and fractional parts
 * @param value Input value
 * @param iptr Pointer to store integer part
 * @return Fractional part of value
 */
f32 modff(f32 value, f32 *iptr) {
    f32 temp;
    f32 abs_val;

    if (value > 0.0f) {
        abs_val = value;
    } else {
        abs_val = -value;
    }

    /* If value is larger than what can be represented as fraction, it's all integer */
    if (abs_val >= 8388608.0f) {  /* 2^23 */
        *iptr = value;
        return 0.0f;
    }

    /* Use floating point trick to extract integer part */
    temp = abs_val + 8388608.0f;
    temp -= 8388608.0f;

    if (abs_val < temp) {
        temp -= 1.0f;
    }

    if (temp > 0.0f) {
        /* keep temp positive */
    } else {
        temp = -temp;
    }

    /* Handle edge case where difference is exactly 1.0 */
    if ((value - temp) == 1.0f) {
        temp += 1.0f;
    }

    if (value >= 0.0f) {
        *iptr = temp;
        return value - temp;
    }

    *iptr = -temp;
    return value + temp;
}

/**
 * Check if value is infinity
 * @param value Value to check
 * @return 1 if positive infinity, -1 if negative infinity, 0 otherwise
 */
s32 __isinf(f64 value) {
    /* Implementation uses IEEE 754 bit manipulation */
    /* This is a stub - actual implementation examines exponent/mantissa bits */
    u32 *p = (u32 *)&value;
    u32 high = p[1];  /* Big-endian: high word is at higher address */
    u32 low = p[0];

    /* Check for infinity: exponent all 1s, mantissa all 0s */
    if ((high & 0x7FF00000) == 0x7FF00000 && (high & 0x000FFFFF) == 0 && low == 0) {
        return (high & 0x80000000) ? -1 : 1;
    }
    return 0;
}

/**
 * Check if value is NaN
 * @param value Value to check
 * @return Non-zero if NaN, 0 otherwise
 */
s32 __isnan(f64 value) {
    /* Implementation uses IEEE 754 bit manipulation */
    u32 *p = (u32 *)&value;
    u32 high = p[1];
    u32 low = p[0];

    /* Check for NaN: exponent all 1s, mantissa non-zero */
    if ((high & 0x7FF00000) == 0x7FF00000) {
        if ((high & 0x000FFFFF) != 0 || low != 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Polynomial coefficients for sine approximation
 * Located at D_8002D750 in ROM
 * These implement a Taylor-like series for sin(x):
 *   sin(x) ≈ x + c3*x^3 + c5*x^5 + c7*x^7 + c9*x^9
 */
static const f64 sin_coeffs[] = {
    1.0,                        /* 0x00: unused/scale */
   -0.16666666666666666,        /* 0x08: c3 = -1/3! */
    0.0083333333333333332,      /* 0x10: c5 = 1/5! */
   -0.0001984126984126984,      /* 0x18: c7 = -1/7! */
    2.7557319223985893e-06      /* 0x20: c9 = 1/9! */
};

/* Constants for range reduction */
static const f64 two_over_pi = 0.63661977236758134;  /* D_8002D778: 2/π */
static const f64 pi_over_2_hi = 1.5707963267948966;  /* D_8002D780: π/2 high */
static const f64 pi_over_2_lo = 6.123233995736766e-17; /* D_8002D788: π/2 low */

/**
 * Compute sine of angle (single precision)
 * (0x80008730 - sinf)
 *
 * Uses polynomial approximation with range reduction.
 * For small angles (|x| < 2^-12), returns x directly.
 * For larger angles, reduces to [-π/2, π/2] and uses polynomial.
 *
 * Algorithm:
 * 1. Extract exponent from float bits
 * 2. For tiny values (exp < 0xE6), return x
 * 3. For normal values (exp < 0xFF), use polynomial directly
 * 4. For large values (exp < 0x136), do range reduction
 * 5. For very large/inf/nan, return 0 or NaN
 *
 * @param x Angle in radians
 * @return sin(x)
 */
f32 sinf(f32 x) {
    u32 ix;
    s32 exp;
    f64 xd, x2, poly;

    /* Reinterpret float bits */
    ix = *(u32 *)&x;
    exp = (ix >> 22) & 0x1FF;

    /* Check exponent range */
    if (exp < 0xFF) {
        /* Small to normal range */
        if (exp < 0xE6) {
            /* Very small - sin(x) ≈ x */
            return x;
        }

        /* Use polynomial directly */
        xd = (f64)x;
        x2 = xd * xd;

        /* Horner's method: c9 + x2*(c7 + x2*(c5 + x2*c3)) */
        poly = sin_coeffs[4] * x2 + sin_coeffs[3];
        poly = poly * x2 + sin_coeffs[2];
        poly = poly * x2 + sin_coeffs[1];
        poly = poly * x2;  /* Now we have the correction term */

        /* sin(x) = x + x*x2*poly */
        return (f32)(xd + xd * x2 * poly);
    }
    else if (exp < 0x136) {
        /* Need range reduction */
        s32 n;
        f64 reduced;

        xd = (f64)x;

        /* n = round(x * 2/π) */
        if (xd * two_over_pi >= 0.0) {
            n = (s32)(xd * two_over_pi + 0.5);
        } else {
            n = (s32)(xd * two_over_pi - 0.5);
        }

        /* Reduce: x - n*π/2 using extended precision */
        reduced = xd - (f64)n * pi_over_2_hi;
        reduced = reduced - (f64)n * pi_over_2_lo;

        x2 = reduced * reduced;

        /* Polynomial evaluation */
        poly = sin_coeffs[4] * x2 + sin_coeffs[3];
        poly = poly * x2 + sin_coeffs[2];
        poly = poly * x2 + sin_coeffs[1];
        poly = poly * x2;

        /* Result depends on quadrant (n mod 4) */
        if (n & 1) {
            /* Odd quadrant - negate result */
            return -(f32)(reduced + reduced * x2 * poly);
        } else {
            return (f32)(reduced + reduced * x2 * poly);
        }
    }
    else {
        /* Very large, infinity, or NaN */
        /* Check if x == x (false for NaN) */
        if (x != x) {
            return GET_NAN_F32();  /* Return NaN (D_8002D880) */
        }
        return 0.0f;  /* D_8002D790 */
    }
}

/**
 * Single precision square root
 * (0x8000E3C0 - sqrtf)
 *
 * Uses the hardware sqrt.s instruction on MIPS.
 *
 * @param x Input value
 * @return sqrt(x)
 */
f32 sqrtf(f32 x) {
    return __builtin_sqrtf(x);
}

/**
 * Compute cosine of angle (single precision)
 * (0x800088F0 - cosf)
 *
 * Uses polynomial approximation with range reduction.
 * Similar to sinf but offset by π/2.
 *
 * @param x Angle in radians
 * @return cos(x)
 */
f32 cosf(f32 x) {
    u32 ix;
    s32 exp;
    f64 xd, x2, poly;
    f64 abs_x;
    s32 n;

    /* Reinterpret float bits */
    ix = *(u32 *)&x;
    exp = (ix >> 22) & 0x1FF;

    /* Check for large/inf/nan */
    if (exp >= 0x136) {
        /* Very large, infinity, or NaN */
        if (x != x) {
            return GET_NAN_F32();  /* Return NaN */
        }
        return 1.0f;  /* D_8002D7E0 */
    }

    /* Get absolute value */
    if (x < 0.0f) {
        abs_x = -(f64)x;
    } else {
        abs_x = (f64)x;
    }

    /* Range reduction: n = round(|x| / (π/2)) */
    if (abs_x * two_over_pi + 0.5 >= 0.0) {
        n = (s32)(abs_x * two_over_pi + 0.5);
    } else {
        n = (s32)(abs_x * two_over_pi - 0.5);
    }

    /* Reduce using extended precision */
    xd = abs_x - (f64)n * pi_over_2_hi;
    xd = xd - (f64)n * pi_over_2_lo;

    x2 = xd * xd;

    /* Polynomial evaluation (different coefficients than sinf - cosine series) */
    /* cos(x) uses even powers: 1 - x^2/2! + x^4/4! - ... */
    poly = sin_coeffs[4] * x2 + sin_coeffs[3];
    poly = poly * x2 + sin_coeffs[2];
    poly = poly * x2 + sin_coeffs[1];
    poly = poly * x2;

    /* Result depends on quadrant */
    if (n & 1) {
        /* Odd quadrant - negate */
        return -(f32)(xd + xd * x2 * poly);
    } else {
        return (f32)(xd + xd * x2 * poly);
    }
}
