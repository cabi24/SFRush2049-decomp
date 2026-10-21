typedef float f32;
extern volatile f32 G;
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
void h1(void) { f32 v; v = G * 2.0f; }
void h2(void) { f32 v; v = fabsf(G); }
void h3(void) { f32 v; v = -G; }
void h4(void) { f32 v; v = G; v = v; }
void h5(void) { f32 v; v = (f32)G; }
void h6(void) { double v; v = G; }
void h7(void) { f32 v; v = G; (void)v; }
void h8(int k) { f32 v; if (k) v = G; }
