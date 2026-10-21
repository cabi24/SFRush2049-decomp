extern int g;
extern void a(void), b(int,int), c(float), d(void);
int f(void *p)
{
    c(0.0f);
    switch (g) { case 1: a(); break; case 2: b(1,1); break; }
    c(-1.0f);
    return 1;
}
