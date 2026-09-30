void func_8008B32C(float (*src)[3], float (*dst)[3], float s) { int i, j; for (j = 0; j < 3; j++) for (i = 0; i < 3; i++) dst[j][i] = src[j][i] * s; }
