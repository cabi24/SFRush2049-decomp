typedef float f32;
extern f32 G0[3], G1[3], T[];
extern f32 OUT[3];
extern unsigned char IX;
extern int k;
void p00(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + (f32)(s * G1[0]); OUT[0] = pos[0]; OUT[1] = s; }
void p01(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = (s * G1[0]) + pos[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p02(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] - -(s * G1[0]); OUT[0] = pos[0]; OUT[1] = s; }
void p03(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + s * G1[0] * 1.0f; OUT[0] = pos[0]; OUT[1] = s; }
void p04(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + (s * G1[0] + 0.0f); OUT[0] = pos[0]; OUT[1] = s; }
void p05(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = +pos[0] + s * G1[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p06(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = (pos[0]) + (s) * (G1[0]); OUT[0] = pos[0]; OUT[1] = s; }
void p07(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + s * *&G1[0]; OUT[0] = pos[0]; OUT[1] = s; }
void p08(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + s * G1[k]; OUT[0] = pos[0]; OUT[1] = s; }
void p09(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = s * G1[0] + pos[0] ; OUT[0] = pos[0]; OUT[1] = s; }
void p10(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; { f32 *p = &pos[0]; *p = *p + s * G1[0]; } OUT[0] = pos[0]; OUT[1] = s; }
void p11(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; { f32 *p = pos; p[0] += s * G1[0]; } OUT[0] = pos[0]; OUT[1] = s; }
