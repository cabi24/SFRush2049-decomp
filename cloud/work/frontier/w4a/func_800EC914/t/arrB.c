typedef short s16; typedef int s32;
extern s16 A, B[], C; extern s16 X[], Y[];
extern signed char N;
void tf(void) {
    s32 i, j, index;
    A = 0; j = A; B[0] = j; C = j;
    for (i = 0; i < N; i++) {
        index = X[i];
        if (index == 2) { j = C; Y[j] = index; C = j + 1; }
        else { j = B[0]; Y[j] = index; B[0] = j + 1; }
    }
}
