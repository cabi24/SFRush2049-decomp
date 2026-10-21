extern void ext(float); extern float ext2(float, float);
extern void func_8038B100(float*, int);
extern void func_8038B200(float*, int);
extern void func_8038B300(float*, int);
extern void func_8038B400(float*, int);
extern void func_8038B500(float*, int);
void func_8038B100(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 2.0f); p[i+1] = ext2(p[i], 2.5f); if (p[2] > 2.0f) ext(p[3]); }  }
void func_8038B200(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 3.0f); p[i+1] = ext2(p[i], 3.5f); if (p[2] > 3.0f) ext(p[3]); }  }
void func_8038B300(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 4.0f); p[i+1] = ext2(p[i], 4.5f); if (p[2] > 4.0f) ext(p[3]); }  }
void func_8038B400(float *p, int c) { int i; for (i = 0; i < c; i++) { ext(p[i] * 5.0f); p[i+1] = ext2(p[i], 5.5f); if (p[2] > 5.0f) ext(p[3]); } func_8038B100(p, c);func_8038B200(p, c);func_8038B300(p, c); }
