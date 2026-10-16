extern void h(int, char*); extern int sel;
static void mid(void){ char m[32]; h(1,m);}
void top(void){ char buf[64]; if (sel==2) mid(); h(0,buf);}
