extern int g(int); extern void h(int, char*); extern int sel; extern void lk(void); extern void ul(void);
static int fs(){ lk(); return g(11); }
void top(void){ int p; char buf[64]; for (p=0;p<sel;p++) { fs(); h(p,buf); fs(); } }
