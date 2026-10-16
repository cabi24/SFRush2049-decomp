extern int g(int); extern void h(int, char*); extern int sel; extern void lk(void); extern void ul(void);
static int fs(){ int r; lk(); r = g(11); ul(); return r; }
void top(void){ int p; int old; char buf[64]; for (p=0;p<sel;p++) { old = fs(); h(p,buf); old = fs(); } }
