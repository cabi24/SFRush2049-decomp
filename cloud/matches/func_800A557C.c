/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also MATCH at -O2) */
/*
 * Single-precision tangent (Cody & Waite style, as in SGI libm): rejects
 * |x| > 6.7465e9 (returns 0), reduces by pi/2 with two modff calls and a
 * two-part pi/2 (1.5708008, -4.4544551e-06), then evaluates the rational
 * approximation xnum/xden and returns xden / -xnum for odd quadrants.
 * External callee 0x80002A64 is modff(f32, f32 *) in the static segment.
 * D_80123B98..D_80123BC4 are this function's own twelve float literals in
 * retail .rodata, referenced as externs so the scorer verifies the addresses.
 *
 * Shaping facts, each verified necessary:
 * - the second modff writes its integer part into `g`, which is later reused
 *   for f*f.  With a separate variable the f*f value is coloured $f0 instead
 *   of $f2 and five registers rotate (the whole near_miss_B29 residual);
 * - `f = xnum;` before the test (copy lands in $f12, numerator stays $f14);
 * - declaration order xnum, g, xn, ... fixes the homes of g (sp+48) and
 *   xn (sp+44).
 */
typedef float f32;
typedef int s32;

extern f32 D_80123B98, D_80123B9C, D_80123BA0, D_80123BA4, D_80123BA8;
extern f32 D_80123BAC, D_80123BB0, D_80123BB4;
extern f32 D_80123BB8, D_80123BBC, D_80123BC0, D_80123BC4;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
f32 modff(f32, f32 *);

f32 func_800A557C(f32 x) {
    f32 xnum;
    f32 g;
    f32 xn;
    f32 y;
    f32 f;
    f32 xden;

    if (fabsf(x) > D_80123B98) {
        return 0.0f;
    }
    y = modff(x * D_80123B9C, &xn);
    if (fabsf(y) >= 0.5f) {
        xn += (x < 0.0f) ? -1.0f : 1.0f;
    }
    f = modff(x, &g);
    xnum = ((g - xn * D_80123BA0) + f) - xn * D_80123BA4;
    f = xnum;
    if (fabsf(xnum) < D_80123BA8) {
        xden = 1.0f;
    } else {
        g = f * f;
        xnum = ((D_80123BAC * g + D_80123BB0) * g + D_80123BB4) * g * f + f;
        xden = (((D_80123BB8 * g + D_80123BBC) * g + D_80123BC0) * g + D_80123BC4) * g + 1.0f;
    }
    if ((s32) xn & 1) {
        return xden / -xnum;
    }
    return xnum / xden;
}
