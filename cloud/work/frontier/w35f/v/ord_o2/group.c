typedef float f32;
extern void ext(float); extern float ext2(float, float);
void func_8038B100(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 2.0f); p[i+1] = ext2(p[i], 2.5f); if (p[2] > 2.0f) ext(p[3]); } }
void func_8038B200(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 3.0f); p[i+1] = ext2(p[i], 3.5f); if (p[2] > 3.0f) ext(p[3]); } }
void func_8038B300(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 4.0f); p[i+1] = ext2(p[i], 4.5f); if (p[2] > 4.0f) ext(p[3]); } }
