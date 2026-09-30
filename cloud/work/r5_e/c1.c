void func_8008B32C(float (*src)[3], float (*dst)[3], float s) { int i, j; float *p, *q; for (i = 0; i < 3; i++) { p = dst[i]; q = src[i]; for (j = 0; j < 3; j++) { *p++ = *q++ * s; } } }
