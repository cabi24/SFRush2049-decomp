typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef signed int s32;
typedef struct { s8 b[5]; } E5;
extern s16 D_8014A108;
extern float D_80144DA8[];
extern s8 D_80144018[];
extern float D_80124618;
extern s8 D_80151AC0[];
extern E5 D_80151AC0e[3];

void func_800F7EB0(void)
{
 s32 i, j;
 for (i = 0; i < D_8014A108; i++) { D_80144018[i] = 0; D_80144DA8[i] = D_80124618; }
 for (j = 0; j < 15; j += 5) { D_80151AC0[j] = -1; for (i = 1; i < 5; i++) D_80151AC0[j + i] = -1; }
}