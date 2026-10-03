typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
void func_803930B8(void);
void func_80393004(void);
void func_80392F28(void);
extern s32 D_803BA804;
extern s32 D_803BA800;
extern s8 D_803BA84D;
extern s8 D_803BA854;
extern s8 D_803BA850;
extern s8 D_803BA80C;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s32 D_803BA818[];
extern s8 D_803B6554[][13];
extern s32 D_803BA7D4;

void func_803931C0(s32 mode)
{
    D_803BA804 = mode;
    switch (D_803BA804) {
    case 0:
        func_803930B8();
        func_80393004();
        D_803BA84D = 0;
        D_803BA854 = 0;
        D_803BA850 = 0;
        D_803BA80C = 0;
        break;
    case 1:
        if (D_803BA800 == 0 && D_803BA858 >= 29) {
            D_803BA858 = 0;
            while (D_803B6554[D_803BA880][D_803BA818[D_803BA880]] == 0) {
                if (++D_803BA818[D_803BA880] >= 13) {
                    D_803BA818[D_803BA880] = 0;
                }
            }
        }
        func_80392F28();
        break;
    }
    D_803BA7D4 = 1;
}
