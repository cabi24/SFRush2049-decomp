extern volatile float gFS;
extern float gG;
void f1(float *p) { p[0] -= gFS; }
void f2(float *p, int c) { if (c) { p[0] -= gFS; } else { p[1] += gFS; } }
void f3(float *p) { p[0] = gG * gFS; p[1] = gFS * p[1]; p[2] = gFS + gG; }
