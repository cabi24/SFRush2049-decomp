typedef float f32;
extern f32 G0[3], G1[3];
extern f32 OUT[3];
extern f32 R(void);
void pa(void) { f32 pos[3]; pos[0] = G0[0]; pos[0] += (R() - 2.5f) * G1[0]; OUT[0] = pos[0]; }
void pb(void) { f32 pos[3]; pos[0] = G0[0]; pos[0] = pos[0] + (R() - 2.5f) * G1[0]; OUT[0] = pos[0]; }
void pc(void) { f32 pos[3]; pos[0] = G0[0]; pos[0] = pos[0] + G1[0] * (R() - 2.5f); OUT[0] = pos[0]; }
void pd(void) { f32 pos[3]; pos[0] = G0[0]; pos[0] += G1[0] * (R() - 2.5f); OUT[0] = pos[0]; }
void pe(void) { f32 pos[3]; pos[0] = G0[0]; pos[0] = (R() - 2.5f) * G1[0] + pos[0]; OUT[0] = pos[0]; }
