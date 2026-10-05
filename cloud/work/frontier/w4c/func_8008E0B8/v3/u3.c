typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

typedef union { f32 f; int i; } FI;
f32 func_8008E0B8(f32 *v) {
    f32 x = v[0], y = v[1];
    FI z;
    f32 len, inv;
    z.f = v[2];
    len = sqrtf(x * x + y * y + z.f * z.f);
    if (len <= 1e-5f) return 0.0f;
    inv = 1.0f / len;
    v[0] = x * inv; v[1] = y * inv; v[2] = z.f * inv;
    return len;
}
