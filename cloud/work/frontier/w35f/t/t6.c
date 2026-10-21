extern void ext(float *);
void f(float *p, float *q, float *v, int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[1] = 15.0f;
        q[0] = p[0] + 15.0f*p[1];
        q[1] = p[2] + 15.0f*p[3];
        q[2] = p[4] + 15.0f*p[5];
        ext(q);
        v[0] = 2.0f; v[2] = 2.0f; v[3] = 2.0f;
        q[3] = p[6] * 2.0f;
        ext(v);
        q[4] = p[7] > 10.0f ? 1.0f : 3.0f;
    }
}
