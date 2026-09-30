extern void use(float *a, float *b, float *c, int *x, int *y);
extern int g1, g2;
int t(void) {
    float a[3];
    int x;
    float b[3];
    int y;
    float c[3];
    x = g1; y = g2;
    use(a, b, c, &x, &y);
    return x + y;
}
