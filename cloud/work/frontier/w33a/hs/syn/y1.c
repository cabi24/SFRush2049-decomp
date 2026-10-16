extern int g(int); extern void h(int, char*); extern int sel; extern void lk(void); extern void ul(void);
static int fs(f) int f; { int r; lk(); r = g(f); ul(); return r; }
static void other(){ char q[8]; fs(4); h(2,q); fs(5); }
static void mid(){ char m[32]; fs(1); h(1,m); fs(2); h(2,m); fs(3); }
static int top(int a){ int p; char buf[64]; switch (sel) { case 1: other(); break; case 2: mid(); break; case 3: for (p=0;p<sel;p++) h(p,buf); break;} return 1; }
int (*tab[])(int) = { top };
