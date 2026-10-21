extern void use(int *);
void f(int c) {
    int a, b;
    use(&a); use(&b);
    if (c) {
        int x, y, z;
        use(&x); use(&y); use(&z);
    } else {
        int p, q;
        use(&p); use(&q);
    }
    {
        int u, v, w;
        use(&u); use(&v); use(&w);
    }
}
