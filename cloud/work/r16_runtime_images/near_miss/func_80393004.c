typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern u8 D_803BA7E0[];
extern u8 D_803BA7F0[];
extern s32 D_803BA830[];
extern s8 D_803B65A4;

void func_80393004(void)
{
    s32 i;
    s32 count;

    D_803BA7E0[0] = 1;
    D_803BA7F0[0] = 1;
    D_803BA7E0[1] = 1;
    D_803BA7F0[1] = 1;
    D_803BA7E0[2] = 0;
    count = D_803BA830[D_803B65A4];
    D_803BA7F0[2] = count < 1;
    for (i = 0; i < 8; i++) {
        D_803BA7E0[i + 3] = i < count;
        D_803BA7F0[i + 3] = i < count;
    }
}
