extern void h(int, char*); extern int sel;
static void mid(int a){ char m[32]; h(a,m);}
void top(void){ char buf[64]; if (sel==2) mid(3); h(0,buf);}
