extern void use(void *);
typedef struct { unsigned int w0, w1; } Gfx;
extern Gfx *gp;
extern int G[10];
extern float F[10];
void t1(void) { int a; use(&a); }
void t2(void) { int a; use(&a); { Gfx *_g = gp++; _g->w0 = 1; _g->w1 = 2; } }
void t3(void) { int a; use(&a); if ((G[0] < 2) != 0) G[1] = 3; }
void t4(void) { int a; use(&a); { Gfx *_g = gp++; _g->w0 = ((unsigned)((short)F[0]) & 0x3ff) << 14 | ((unsigned)((short)F[1]) & 0x3ff) << 2; _g->w1 = 0; } }
void t5(void) { int a; use(&a); { Gfx *_g = gp++; _g->w0 = ((unsigned)((int)F[0]) & 0x3ff) << 14 | ((unsigned)((int)F[1]) & 0x3ff) << 2; _g->w1 = 0; } }
void t6(void) { int a; use(&a); { Gfx *_g = gp++; _g->w0 = ((unsigned)((int)((float)G[0] * 4.0f)) & 0xfff) << 12; _g->w1 = 0; } }
void t7(void) { int a; use(&a); G[2] = 128000 / (G[0] - G[1]); }
