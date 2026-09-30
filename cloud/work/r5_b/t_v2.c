typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
void func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2]; volatile f32 zz; f32 len; f32 inv;
    zz = z;
    len = sqrtf(x*x + y*y + zz*zz);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = zz*inv; }
}