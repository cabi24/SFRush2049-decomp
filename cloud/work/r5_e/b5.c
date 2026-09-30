void func_8008B32C(float (*src)[3], float (*dst)[3], float s) { int i, j; i = 0; do { j = 0; do { dst[i][j] = src[i][j] * s; j++; } while (j < 3); i++; } while (i < 3); }
