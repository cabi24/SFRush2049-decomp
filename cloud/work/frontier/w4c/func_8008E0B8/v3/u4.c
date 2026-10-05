typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

typedef struct { f32 x, y, z; } V3;
f32 func_8008E0B8(V3 *v) {
    V3 t;
    f32 len, inv;
    t = *v;
    len = sqrtf(t.x * t.x + t.y * t.y + t.z * t.z);
    if (len <= 1e-5f) return 0.0f;
    inv = 1.0f / len;
    v->x = t.x * inv; v->y = t.y * inv; v->z = t.z * inv;
    return len;
}
