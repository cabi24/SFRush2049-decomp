typedef int s32; typedef short s16;
void g(s32 *x);
void gg(s32 x);
s32 hh(s32 x);
void h(void);
void k1(s32 x) { gg(x); }
void k2(s32 x) { gg(x); }
static void k3(s32 x) { gg(x); }
s32 r1(s32 x) { return hh(x); }
s32 r2(s32 x) { return hh(x); }
s16 q1(s16 x) { return hh(x); }
s32 l1(s32 x) { s32 y; y = hh(x); return y; }
void c0(void) { s32 a[4]; h(); g(a); h(); }
void ck1(void) { s32 a[4]; h(); k1(3); g(a); h(); }
void ck2(void) { s32 a[4]; h(); k2(3); g(a); h(); }
void ck3(void) { s32 a[4]; h(); k3(3); g(a); h(); }
void cr1(void) { s32 a[4]; h(); a[0] = r1(3); g(a); h(); }
void cr2(void) { s32 a[4]; h(); a[0] = r2(3); g(a); h(); }
void cq1(void) { s32 a[4]; h(); a[0] = q1(3); g(a); h(); }
void cl1(void) { s32 a[4]; h(); a[0] = l1(3); g(a); h(); }
