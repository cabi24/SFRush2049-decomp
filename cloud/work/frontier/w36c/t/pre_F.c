extern short n;
extern int st[];
extern void f0(int), f1(int), f2(int), f3(int), f4(int), g(char *, int), h(int);
void helper(int p);
void test(void)
{
    int i;
    for (i = 0; i < n; i++) {
        switch (st[i]) {
        case 0: f0(i); break;
        case 1: f1(i); break;
        case 2: f2(i); break;
        case 3: f3(i); break;
        case 4: f4(i); break;
        case 5: helper(i); break;
        case 6: helper(i); break;
        }
        do {} while (0);
    }
}
void helper(int p)
{
    int y = 3;
    char str1[40];
    g(str1, p + 1);
    h(y);
}

