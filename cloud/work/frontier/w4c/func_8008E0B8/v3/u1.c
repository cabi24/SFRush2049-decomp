typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 func_8008E0B8(f32 *v) {
    f32 t[3];
    f32 len, inv;
    t[0] = v[0]; t[1] = v[1]; t[2] = v[2];
    len = sqrtf(t[0] * t[0] + t[1] * t[1] + t[2] * t[2]);
    if (len <= 1e-5f) return 0.0f;
    inv = 1.0f / len;
    v[0] = t[0] * inv; v[1] = t[1] * inv; v[2] = t[2] * inv;
    return len;
}
