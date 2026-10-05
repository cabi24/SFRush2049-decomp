typedef short s16; typedef int s32;
extern s16 A, B, C[]; extern s16 X[], Y[];
extern signed char N;
void tf(void) {
    s32 i, j, index;
    A = 0; j = A; B = j; C[0] = j;
    for (i = 0; i < N; i++) {
        index = X[i];
        if (index == 2) { j = C[0]; Y[j] = index; C[0] = j + 1; }
        else { j = B; Y[j] = index; B = j + 1; }
    }
}
