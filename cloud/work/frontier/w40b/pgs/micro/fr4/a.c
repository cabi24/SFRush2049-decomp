typedef int s32;
void g(s32 *x);
void h(void);
void hb(s32 *b);
void k1(void) { s32 b[8]; hb(b); }
void c0(void) { s32 a[4]; h(); g(a); h(); }
void c1(void) { s32 a[4]; h(); k1(); g(a); h(); }
void c2(void) { s32 a[4]; s32 z[16]; h(); g(a); g(z); h(); }
