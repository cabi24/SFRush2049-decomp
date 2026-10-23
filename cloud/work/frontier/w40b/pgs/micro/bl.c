typedef int s32;
void g(s32 *x);
void h(void);
void c1(s32 q) { s32 a[2]; s32 b[2]; h(); g(a); if (q) { s32 c[2]; g(c); } else { s32 d[2]; g(d); } g(b); }
