extern short n;
extern int st[];
extern void f0(int), f1(int), g(char *, int), h(int);
extern char *tx[];
void helper(int p);
void test(void)
{
    int i;
    for (i = 0; i < n; i++) {
        switch (st[i]) {
        case 0: f0(i); break;
        case 1: f1(i); break;
        case 5: helper(i); break;
        }
    }
}
void helper(int p)
{
    int y = 3;
    char str1[40];
    g(str1, p + 1);
    h(y);
}
