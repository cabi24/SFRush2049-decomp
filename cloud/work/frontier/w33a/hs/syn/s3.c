extern int g(int); extern void h(int, char*); extern int sel;
static int fs(){ int old; old = g(1); return old; }
void top(void){ char buf[64]; if (sel==2) { char m[32]; fs(); fs(); fs(); h(1,m);} h(0,buf);}
