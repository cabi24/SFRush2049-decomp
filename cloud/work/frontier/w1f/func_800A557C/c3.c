typedef float f32;
typedef int s32;

extern f32 D_80123B98, D_80123B9C, D_80123BA0, D_80123BA4, D_80123BA8;
extern f32 D_80123BAC, D_80123BB0, D_80123BB4;
extern f32 D_80123BB8, D_80123BBC, D_80123BC0, D_80123BC4;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
f32 modff(f32, f32 *);

f32 func_800A557C(f32 x) {
    f32 f;
    f32 g;
    f32 xn;
    f32 y;
    f32 xden;

    if (fabsf(x) > D_80123B98) {
        return 0.0f;
    }
    y = modff(x * D_80123B9C, &xn);
    if (fabsf(y) >= 0.5f) {
        xn += (x < 0.0f) ? -1.0f : 1.0f;
    }
    f = modff(x, &g);
    x = ((g - xn * D_80123BA0) + f) - xn * D_80123BA4;
    f = x;
    if (fabsf(x) < D_80123BA8) {
        xden = 1.0f;
    } else {
        g = f * f;
        x = ((D_80123BAC * g + D_80123BB0) * g + D_80123BB4) * g * f + f;
        xden = (((D_80123BB8 * g + D_80123BBC) * g + D_80123BC0) * g + D_80123BC4) * g + 1.0f;
    }
    if ((s32) xn & 1) {
        return xden / -x;
    }
    return x / xden;
}
