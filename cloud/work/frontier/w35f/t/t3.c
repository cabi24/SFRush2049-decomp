extern float gFS;
extern float gA[4];
float gB;
void f1(float *p) { p[0] -= gFS; }
void f2(float *p, int c) { if (c) { p[0] -= gFS; } else { p[1] += gFS; } }
float f3(float *p) { return p[0] * gA[1] + p[1]; }
float f4(float *p, int c) { if (c) { p[0] -= gB; } else { p[1] += gB; } return gB; }
