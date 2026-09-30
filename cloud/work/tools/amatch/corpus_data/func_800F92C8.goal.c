f32 func_800F92C8(f32 a, f32 b, f32 x, f32 outA, f32 outB)
{
    f32 d;
    f32 t;

    if (a < b) {
        if (x < a) return outA;
        if (b < x) return outB;
    } else {
        if (x < b) return outB;
        if (a < x) return outA;
    }
    d = a - b;
    if (fabsf(d) < D_80124638) {
        return (outA + outB) * 0.5f;
    }
    t = (outA - outB) / d;
    return t * x + (outA - t * a);
}