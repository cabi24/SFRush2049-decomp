void func_8008B32C(float *src, float *dst, float s) { int i, j; for (i = 0; i < 3; i++, dst += 3, src += 3) { float *p = dst, *q = src; for (j = 0; j < 3; j++, p++, q++) { *p = *q * s; } } }
