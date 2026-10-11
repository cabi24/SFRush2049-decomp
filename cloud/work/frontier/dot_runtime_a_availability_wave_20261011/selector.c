typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s8 D_803BA860;
extern u8 D_803BA7FC[];
extern u8 D_803BA7FE[];

void func_80392F28(void)
{
    s32 i;
    s32 flag;
    s8 level = D_803BA858;

    if (level < 12) {
        D_803BA880 = 0;
        flag = 1;
    } else if (level < 20) {
        D_803BA880 = 1;
        flag = 1;
    } else if (level < 24) {
        D_803BA880 = 2;
        flag = 1;
    } else if (level < 25) {
        D_803BA880 = 3;
        flag = 1;
    } else if (level < 29) {
        D_803BA880 = 4;
        flag = 1;
        D_803BA860 = level - 25;
    } else {
        D_803BA880 = 5;
        flag = 1;
    }
    for (i = 0; i < 2; i++) {
        D_803BA7FC[i] = flag;
        D_803BA7FE[i] = flag;
    }
}
