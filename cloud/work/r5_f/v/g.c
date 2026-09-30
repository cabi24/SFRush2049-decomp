typedef signed int s32;
typedef float f32;
s32 t1(s32 a, f32 *p, s32 *o, f32 w) {
    o[0] = (s32)(p[0] * 65536.0f);
    o[1] = (s32)(w * 65536.0f);
    return 1;
}
