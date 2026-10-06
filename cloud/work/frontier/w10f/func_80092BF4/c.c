typedef short s16;
typedef int s32;
typedef struct { s32 j; s32 pad[15]; } E64;
extern E64 D_80139334[];
extern s32 D_8012E700[][17];
void func_80092BF4(s16 idx, s32 *a1, s32 *a2) {
    s32 j = D_80139334[idx].j;
    D_8012E700[(s16)j][15] = *a1;
    D_8012E700[(s16)j][16] = *a2;
}
