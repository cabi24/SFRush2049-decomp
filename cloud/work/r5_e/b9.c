void func_8008B32C(float (*src)[3], float (*dst)[3], float s) { int j, i; for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) dst[i][j] = src[i][j] * s; }
