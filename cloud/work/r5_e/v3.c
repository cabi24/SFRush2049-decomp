void func_8008B32C(float *src, float *dst, float s) { int i, j; for (i = 0; i < 3; i++) { float *a = src + i*3; float *b = dst + i*3; for (j = 0; j < 3; j++) { *b++ = *a++ * s; } } }
