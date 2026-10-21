typedef float f32;
extern void use(f32 *);
extern void usei(int *);
extern int K;
void g(void)
{
    int a;
    int b;
    usei(&a);
    if (K) {
        f32 x, y, z;
        use(&x); use(&y); use(&z);
    }
    if (K == 2) {
        f32 x, y, z;
        use(&x); use(&y); use(&z);
    }
    usei(&b);
}
