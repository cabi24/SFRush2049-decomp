typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
void func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; volatile f32 z = v[2]; volatile f32 pad[4];
    f32 len = sqrtf(x*x + y*y + z*z);
    if (len > D_8012394C) {
        f32 inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
    else len = 0.0f;
}