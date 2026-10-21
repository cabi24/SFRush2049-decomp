void a(float *p) { p[0] = 10.0f; p[1] = 10.0f; p[2] = 10.0f; }
void b(float *p) { p[0] = 10.0f; p[1] = 10.00f; p[2] = 1e1f; }
void c(float *p) { p[0] = 10.f; p[1] = 10.0F; p[2] = 10.0e0f; }
void d(float *p) { p[0] = .5f; p[1] = 0.5f; p[2] = 0.50f; p[3] = 5e-1f; }
