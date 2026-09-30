typedef struct P { short a; short b; char body[18]; } P;
extern void f2(float *m, char *b);
extern void g(void);
void t(float *mat, P *poly, float *x) {
    f2(mat, poly->body);
    g();
    f2(mat, poly->body);
    g();
    x[0] = mat[0];
}
