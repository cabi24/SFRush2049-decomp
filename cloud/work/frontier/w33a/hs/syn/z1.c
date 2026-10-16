extern int g(int); extern void h(int, char*); extern int sel; extern void lk(void); extern void ul(void);
static int fs(f) int f; { int r; lk(); r = g(f); ul(); return r; }
static mid(){ char m[32]; fs(1); h(1,m); fs(2); h(2,m); fs(3); }
void top(void){ int p; char buf[64]; for (p=0;p<sel;p++) h(p,buf); if (sel==2) mid(); }
