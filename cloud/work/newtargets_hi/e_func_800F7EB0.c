typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef struct { s8 b[5]; } E5;
extern s16 D_8014A108;
extern float D_80144DA8[];
extern s8 D_80144018[];
extern float D_80124618;
extern E5 D_80151AC0[3];
void func_800F7EB0(void) {
    s32 i;
    s32 j;
    for (i = 0; i < D_8014A108; i++) {
        D_80144DA8[i] = D_80124618;
        D_80144018[i] = 0;
    }
    for (j = 0; j < 3; j++) {
        s32 k;
        for (k = 0; k < 5; k++) D_80151AC0[j].b[k] = -1;
    }
}
