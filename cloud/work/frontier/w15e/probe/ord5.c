typedef float f32;
extern f32 G0[3], G1[3], T[];
extern f32 OUT[3];
extern unsigned char IX;
void pa(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] += s * G1[0]; OUT[0] = pos[0]; OUT[1] = s; }
void pb(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + s * G1[0]; OUT[0] = pos[0]; OUT[1] = s; }
void pc(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = pos[0] + G1[0] * s; OUT[0] = pos[0]; OUT[1] = s; }
void pd(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = s * G1[0] + pos[0]; OUT[0] = pos[0]; OUT[1] = s; }
void pe(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] = G1[0] * s + pos[0]; OUT[0] = pos[0]; OUT[1] = s; }
void pf(void) { f32 pos[3]; f32 s = T[IX]; pos[0] = G0[0]; pos[0] += G1[0] * s; OUT[0] = pos[0]; OUT[1] = s; }
