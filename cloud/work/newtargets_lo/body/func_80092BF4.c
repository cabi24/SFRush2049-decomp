typedef struct { s32 j; u8 pad[60]; } A;
typedef struct { u8 pad[60]; s32 x; s32 y; } B;
extern A D_80139334[];
extern B D_8012E700[];
void func_80092BF4(s16 idx, s32 *a1, s32 *a2) {
    s16 j = D_80139334[idx].j;
    D_8012E700[j].x = *a1;
    j = D_80139334[idx].j;
    D_8012E700[j].y = *a2;
}
