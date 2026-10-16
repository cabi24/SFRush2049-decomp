extern int g(int); extern void h(int, char*); extern int sel; extern void lk(void); extern void ul(void);
static int fs(){ int old; lk(); old = g(11); ul(); return old; }
void top(void){ int p; char buf[64]; for (p=0;p<sel;p++) { fs(); h(p,buf); fs(); } }
