typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern void dbg(f32 *);
f32 func_8008E0B8(f32 *v) {
    f32 p0,p1,p2,p3,p4,p5;
    f32 x = v[0], y = v[1], z = v[2];
    f32 len, inv;
    if (0) dbg(&z);
    len = sqrtf(x * x + y * y + z * z);
    if (len <= 1e-5f) return 0.0f;
    inv = 1.0f / len;
    v[0] = x * inv; v[1] = y * inv; v[2] = z * inv;
    return len;
}
