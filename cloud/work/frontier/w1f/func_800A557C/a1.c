typedef float f32;
typedef int s32;

extern f32 D_80123B98, D_80123B9C, D_80123BA0, D_80123BA4, D_80123BA8;
extern f32 D_80123BAC, D_80123BB0, D_80123BB4;
extern f32 D_80123BB8, D_80123BBC, D_80123BC0, D_80123BC4;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
f32 modff(f32, f32 *);

f32 func_800A557C(f32 x) {
    f32 p0; f32 x1; f32 xn; f32 y; f32 f; f32 g; f32 xnum; f32 xden;

    if (fabsf(x) > D_80123B98) {
        return 0.0f;
    }
    y = modff(x * D_80123B9C, &xn);
    if (fabsf(y) >= 0.5f) {
        xn += (x < 0.0f) ? -1.0f : 1.0f;
    }
    f = modff(x, &x1);
    f = ((x1 - xn * D_80123BA0) + f) - xn * D_80123BA4;
    if (fabsf(f) < D_80123BA8) {
        xnum = f;
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
