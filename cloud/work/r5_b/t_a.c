typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
    f32 len; f32 inv;
    f32 a; f32 b; f32 c; f32 d; f32 e; f32 f; f32 g; f32 h; f32 z = v[2]; f32 x = v[0]; f32 y = v[1];  f32 *pa = &a; f32 *pz = &z; f32 *pb = &b;f32 *pc = &c;f32 *pd = &d;f32 *pe = &e;f32 *pf = &f;f32 *pg = &g;f32 *ph = &h;
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}