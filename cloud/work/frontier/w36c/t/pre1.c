extern short n;
extern int st[];
extern void f0(int), f1(int), g(int, int);
void test(void)
{
    int i;
    for (i = 0; i < n; i++) {
        switch (st[i]) {
        case 0: f0(i); break;
        case 1: f1(i); break;
        case 5: g(i, i + 1); break;
        }
    }
}
