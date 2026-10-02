/* Range reduction followed by an odd/even rational result.
 * External coefficients retain their verified native symbols: numerical values
 * and a precise trigonometric identity are not established by this code. */
typedef float f32;
typedef signed int s32;
extern f32 fabsf(f32), modff(f32, f32 *);
#ifdef __sgi
#pragma intrinsic(fabsf)
#endif
extern f32 D_80123B98, D_80123B9C, D_80123BA0, D_80123BA4;
extern f32 D_80123BA8, D_80123BAC, D_80123BB0, D_80123BB4;
extern f32 D_80123BB8, D_80123BBC, D_80123BC0, D_80123BC4;

f32 func_800A557C(f32 x)
{
    f32 whole, rounded, fraction, sign, reduced, squared, denominator;
    if (D_80123B98 < fabsf(x))
        return 0.0f;
    fraction = modff(x * D_80123B9C, &rounded);
    if (fabsf(fraction) >= 0.5f) {
        if (x < 0.0f) sign = -1.0f;
        else sign = 1.0f;
        rounded += sign;
    }
    fraction = modff(x, &whole);
    reduced = ((whole - rounded * D_80123BA0) + fraction)
              - rounded * D_80123BA4;
    if (fabsf(reduced) < D_80123BA8) {
        denominator = 1.0f;
    } else {
        squared = reduced * reduced;
        reduced += (((D_80123BAC * squared + D_80123BB0) * squared
                     + D_80123BB4) * squared) * reduced;
        denominator = (((D_80123BB8 * squared + D_80123BBC) * squared
                        + D_80123BC0) * squared + D_80123BC4) * squared + 1.0f;
    }
    if ((s32)rounded & 1)
        return denominator / (-reduced);
    return reduced / denominator;
}
