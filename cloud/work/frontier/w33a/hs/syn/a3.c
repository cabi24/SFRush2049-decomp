extern int g(int); extern void h(int, char*); extern int sel;
static void fs(int f){ g(f); }
void top(void){ char buf[64]; if (sel==2) { char m[32]; fs(1); fs(2); fs(3); h(1,m);} h(0,buf);}
