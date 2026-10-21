extern volatile float gFS;
extern float gG;
void f1(float *p, int c) { float vel; if (c) { vel = gFS; p[1] -= gG * gFS; } else { vel = 2.0f * gFS; p[0] += vel; } }
void f2(float *p, int c) { if (c) { gFS; p[1] -= gG * gFS; } }
void f3(float *p, int c) { float vel; if (c) { vel = gFS; p[1] -= gG * gFS; p[2] = vel; } }
void f4(float *p, int c) { float vel; switch (c) { case 3: vel = gFS; p[1] -= gG * gFS; break; case 1: vel = gFS * 2.0f; goto L; default: vel = gFS; L: p[0] = p[2] * vel; break; } }
