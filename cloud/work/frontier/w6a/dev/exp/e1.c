extern void use(void *);
extern int g;
void f1(int x)
{
    int a;
    unsigned short b;
    unsigned int c, d;
    int arr1[2];
    use(arr1);
    if (x) {
        unsigned short e;
        use(&e);
    } else {
        int q[3];
        use(q);
    }
    {
        float uv[3];
        use(uv);
    }
    a = g; b = g; c = g; d = g;
    g = a + b + c + d;
}
