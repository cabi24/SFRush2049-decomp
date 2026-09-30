typedef float M[3][3];
void func_8008B32C(M src, M dst, float s) { int i, j; for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) dst[i][j] = src[i][j] * s; }
