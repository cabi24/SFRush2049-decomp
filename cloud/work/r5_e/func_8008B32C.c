void func_8008B32C(float *src, float *dst, float s) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            dst[i * 3 + j] = src[i * 3 + j] * s;
        }
    }
}
