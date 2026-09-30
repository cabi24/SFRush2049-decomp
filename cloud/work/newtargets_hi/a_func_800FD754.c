typedef signed int s32;
void func_800FD754(float *src, float *dst, float sx, float sy, float sz) {
    s32 i;
    for (i = 0; i < 3; i++) {
        dst[i] = src[i] * sx;
        dst[i + 3] = src[i + 3] * sy;
        dst[i + 6] = src[i + 6] * sz;
    }
}
