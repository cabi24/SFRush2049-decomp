typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
    f32 loc[11]; f32 *p = loc;
    f32 x = v[0]; f32 y = v[1]; f32 len; f32 inv;
    loc[10] = v[2];
    len = sqrtf(x*x + y*y + loc[10]*loc[10]);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = loc[10]*inv;
    return len;
}