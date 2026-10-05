typedef float f32; typedef int s32;
void g(f32 *p);
f32 cosf(f32);
void pr(f32 *o, f32 *a, f32 *b, s32 x, f32 *u, f32 ang) {
    f32 p[3];
    p[0] = b[1] * a[2] - b[2] * a[1];
    p[1] = b[2] * a[0] - b[0] * a[2];
    p[2] = b[0] * a[1] - b[1] * a[0];
    p[0] = o[0] + p[0];
    p[1] = o[1] + p[1];
    p[2] = o[2] + p[2];
    g(u);
    u[0] = cosf(ang);
    g(p);
}
