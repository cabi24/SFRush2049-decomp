typedef float f32;
extern f32 G0[3], T[];
extern f32 OUT[3];
extern unsigned char IX;
extern f32 R(void);
void p00(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] += s * c[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p01(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + s * c[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p02(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + c[0] * s; OUT[0] = pos[0]; OUT[1] = s; }
void p03(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = s * c[0] + pos[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p04(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] += c[0] * s; OUT[0] = pos[0]; OUT[1] = s; }
void p05(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + c[0] * (R() - 2.5f); OUT[0] = pos[0]; OUT[1] = s; }
void p06(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + (R() - 2.5f) * c[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p07(f32 *c) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] += (R() - 2.5f) * c[0]; OUT[0] = pos[0]; OUT[1] = s; }
