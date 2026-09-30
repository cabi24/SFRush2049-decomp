typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 p0[148]; u8 idx; u8 p1[3]; } A152;
typedef struct { u8 p[860]; u8 f860; u8 f861; u8 q[90]; } B952;
extern s16 D_80151AD0;
extern A152 D_80150B70[];
extern float D_801106B4[];
extern B952 D_80152818[];
extern u8 D_80110680[];
void func_800E9234(B952 *b);
void func_800FAD50(B952 *b);
void render_post_process(void) {
    s32 i;
    B952 *b;
    for (i = 0; i < D_80151AD0; i++) {
        b = &D_80152818[D_80150B70[i].idx];
        b->f860 = i;
        func_800E9234(b);
        b->f861 = 7;
        func_800FAD50(b);
        D_801106B4[i] = 0.0f;
    }
    for (i = 0; i < 6; i++) {
        D_80110680[i] = 0;
    }
}
