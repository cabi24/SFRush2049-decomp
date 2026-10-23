typedef int s32;
void g(s32 *x);
void h(void);
void e1(void) {}
void e2(void) {}
void c0(void) { s32 a[4]; h(); g(a); h(); }
void c1(void) { s32 a[4]; h(); e1(); g(a); h(); }
void c4(void) { s32 a[4]; h(); e1(); g(a); e1(); h(); e1(); e1(); }
void c4k(void) { s32 a[4]; h(); e2(); g(a); e2(); h(); e2(); e2(); }
