extern void use(float *a);
extern int g1, g2, g3;
int t(void) {
    float a[3];
    int x;
    int y;
    int z;
    float b[3];
    x = g1; y = g2; z = g3;
    use(a); use(b);
    if (x > 3) use(a);
    return x + y + z;
}
