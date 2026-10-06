typedef short s16;
typedef int s32;
typedef struct { s32 j; s32 pad[15]; } E64;
typedef struct { s32 pad[15]; s32 x; s32 y; } E68;
extern E64 D_80139334[];
extern char D_8012E700[];
void func_80092BF4(s16 idx, s32 *a1, s32 *a2) {
    s32 j = D_80139334[idx].j;
    ((E68 *)(D_8012E700 + (s16)j * sizeof(E68)))->x = *a1;
    ((E68 *)(D_8012E700 + (s16)j * sizeof(E68)))->y = *a2;
}
