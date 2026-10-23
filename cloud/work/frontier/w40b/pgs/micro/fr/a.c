typedef int s32;
void g(s32 x);
void h(void);
void e1(void) {}
void e2(void) {}
void k1(s32 x) { g(x); }
void k2(s32 x) { g(x); }
void v1(void) { h(); }
void v2(void) { h(); }
s32 r1(s32 x) { s32 y; y = x + 1; g(y); return y; }
s32 r2(s32 x) { s32 y; y = x + 1; g(y); return y; }
void c0(void) { h(); h(); }
void c_e1(void) { h(); e1(); h(); }
void c_e2(void) { h(); e2(); h(); }
void c_k1(void) { h(); k1(5); h(); }
void c_k2(void) { h(); k2(5); h(); }
void c_v1(void) { h(); v1(); h(); }
void c_v2(void) { h(); v2(); h(); }
void c_r1(void) { h(); g(r1(5)); h(); }
void c_r2(void) { h(); g(r2(5)); h(); }
