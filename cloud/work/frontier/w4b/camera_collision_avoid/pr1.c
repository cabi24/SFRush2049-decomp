typedef float f32;
void g(f32 *p);
void pr(f32 *a, f32 *b) {
    f32 p[3];
    p[0] = b[1] * a[2] - b[2] * a[1];
    p[1] = b[2] * a[0] - b[0] * a[2];
    p[2] = b[0] * a[1] - b[1] * a[0];
    g(p);
}
