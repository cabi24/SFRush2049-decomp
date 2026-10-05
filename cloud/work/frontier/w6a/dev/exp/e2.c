extern void use(int *);
extern void g(void);
extern int G[10];
void f1(int x)
{
    int a[2];
    use(a);
}
void f2(int x)
{
    int a[2];
    int y = G[x];
    use(a);
    G[0] = y;
}
void f3(int x)
{
    int a[2];
    int y = G[x], z = G[x+1], w = G[x+2];
    use(a);
    g();
    G[0] = y + z + w;
}
