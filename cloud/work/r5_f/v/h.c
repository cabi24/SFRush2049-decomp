typedef signed int s32;
typedef float f32;
s32 t1(s32 a, f32 *p, f32 *q, s32 *o, f32 w, s32 fl) {
    if (fl) return (s32)w;
    return p[0]+q[0];
}
s32 t2(s32 a, f32 *p, f32 *q, s32 *o, s32 fl, f32 w) {
    if (fl) return (s32)w;
    return p[0]+q[0];
}
