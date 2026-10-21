typedef float f32;
extern volatile f32 G;
extern f32 H;
extern void use(f32);
extern int K;
void f1(void) { f32 v; v = G; }
void f2(void) { G; }
void f3(void) { f32 v; v = G * 1.0f; }
void f4(void) { f32 v; v = G; if (K) use(v); }
void f5(int k) { f32 v; switch (k) { case 1: v = G; H = 2.0f; break; case 2: v = 3.0f * G; goto s; default: v = G * 2.0f; s: H = v; break; } }
void f6(void) { f32 v; v = G + 0.0f; }
void f7(void) { f32 v; v = (f32)(double)G; }
