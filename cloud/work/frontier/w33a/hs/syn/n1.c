extern int g(int); extern void h(int, char*); extern int sel;
static int fs(int f){ int old; old=g(f); return old;}
static void mid(void){ char m[32]; fs(1); fs(2); fs(3); h(1,m);}
void top(void){ char buf[64]; if (sel==2) mid(); h(0,buf);}
