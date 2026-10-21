extern void ext(float *);
extern void ext2(float *, float *);
void f(float *p, float *q, int n) {
    int i;
    float v1[3];
    for (i = 0; i < n; i++) {
        ext2(p, v1);
        v1[1] = 15.0f;
        ext(v1);
        q[0] = p[0] + 15.0f*p[1];
        q[1] = p[2] + 15.0f*p[3];
        q[2] = p[4] + 15.0f*p[5];
        ext(q);
    }
}
