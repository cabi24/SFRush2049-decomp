void func_8008B32C(float *src, float *dst, float s) { int i, j; for (i = 0; i < 3; i++) { for (j = 0; j < 3; j++) { dst[j] = src[j] * s; } src += 3; dst += 3; } }
