extern int g(int); extern void h(int, char*); extern int sel; extern int gw(char*,int);
static short ct(char *t, short x) { return x - (gw(t,-1)>>1); }
void top(void){ char buf[64]; if (sel==2) { char m[32]; h(ct(m,160),m); h(ct(m,10),m); h(ct(m,20),m); h(1,m);} h(0,buf);}
