extern int g; extern void a(void), b(int,int), c(float);
int f(void *p)
{
    c(0.0f);
    switch (g) { case 0: a(); break; case 1: b(1,1); a(); break; } 
    c(-1.0f);
    return 1;
}
