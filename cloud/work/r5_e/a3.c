void func_8008B32C(float *src, float *dst, float s) { int i, j; float *p, *q; for (i = 0; i < 3; i++, dst += 3, src += 3) { p = dst; q = src; for (j = 0; j < 3; j++) { *p++ = *q++ * s; } } }
