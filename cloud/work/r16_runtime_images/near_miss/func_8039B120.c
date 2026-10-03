typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[4]; u8 u4; u8 pad5[3]; s8 b8; s8 b9; } Cfg;
extern Cfg *D_803BAD90;
extern s8 D_80154640;
extern s8 D_803B3424;
extern u8 D_803B3428;
extern u8 D_80150E88[];
extern u8 D_80150DDB[];
extern u8 D_80150F7C[];
extern s8 D_80156994;

void func_8039B120(void)
{
    D_803B3424 = !(D_803BAD90->b9 >= D_80154640) && D_803BAD90->u4 != 0;
    D_803B3428 = D_803BAD90->u4;
    if (D_803BAD90->b8 > 0) {
        D_80150E88[1] = 1;
        D_80150DDB[1] = 1;
        D_80150F7C[1] = 1;
    }
    if (D_803BAD90->b8 >= 2 && D_80156994 != 0) {
        D_80150E88[2] = 1;
        D_80150DDB[2] = 1;
        D_80150F7C[2] = 1;
    }
    if (D_803BAD90->b8 == 3) {
        D_80150E88[3] = 1;
        D_80150F7C[3] = 1;
    }
    if (D_80156994 == 0 && D_803BAD90->b8 == 2) {
        D_803B3424 = 0;
    }
}
