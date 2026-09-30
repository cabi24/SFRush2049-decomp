typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
    f32 len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    f32 inv;
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] *= inv; v[1] *= inv; v[2] *= inv;
    return len;
}
