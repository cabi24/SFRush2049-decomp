typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
void func_8008E0B8(f32 *v) {
    f32 len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    f32 inv;
    if (len <= D_8012394C) {
        len = 0.0f;
    } else {
        inv = 1.0f / len;
        v[0] *= inv; v[1] *= inv; v[2] *= inv;
    }
}
