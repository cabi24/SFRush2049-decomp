typedef struct { int t, o; float a; float m[9]; float p[3]; unsigned char al, r[3]; } R;
extern R B[23];
extern void g(int), h(float *);
void f(void)
{
    int i;
    for (i = 0; i < 23; i++) { if (B[i].o == -1) { g(B[i].t); } }
    g(B[17].o);
    h(B[17].m);
    B[17].p[0] = 0.0f;
    B[17].p[1] = 115.0f;
    B[17].p[2] = 200.0f;
    B[17].p[0] *= 1.0f / 1;
    B[17].p[1] *= 1.0f / 1;
    B[17].p[2] *= 1.0f / 1;
    g(B[18].o);
}
