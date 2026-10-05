typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

static f32 vlen(f32 x, f32 y, f32 z) { return sqrtf(x * x + y * y + z * z); }
f32 func_8008E0B8(f32 *v) {
    f32 x = v[0], y = v[1], z = v[2];
    f32 len, inv;
    len = vlen(x, y, z);
    if (len <= 1e-5f) return 0.0f;
    inv = 1.0f / len;
    v[0] = x * inv; v[1] = y * inv; v[2] = z * inv;
    return len;
}
