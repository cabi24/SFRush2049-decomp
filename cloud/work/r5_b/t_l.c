typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
    f32 y; f32 z; f32 *pz; f32 inv; f32 len; f32 x;
    x = v[0]; y = v[1]; z = v[2]; pz = &z;
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}