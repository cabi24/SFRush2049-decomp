extern int g; extern void a(void), b(int,int), c(float);
int f(void *p)
{
    c(0.0f);
    if (g == 0) a(); else if (g == 1) { b(1,1); a(); }
    c(-1.0f);
    return 1;
}
