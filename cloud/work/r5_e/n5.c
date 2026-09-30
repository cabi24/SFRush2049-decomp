void func_8008B32C(float (*src)[3], float (*dst)[3], float s) { int i, j; for (i = 0; i < 3; i++) { float *p = dst[i]; float *q = src[i]; for (j = 0; j < 3; j++) { *p = *q * s; p++; q++; } } }
