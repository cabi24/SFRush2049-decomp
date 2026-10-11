/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { u8 pad[164]; } Rec164;
extern Rec164 D_803B3460[];
extern Rec164 *D_803B4278;
extern f32 D_803B427C;
extern s32 D_803B4280;

void func_8038FF90(s32 index)
{
    D_803B4278 = &D_803B3460[index];
    D_803B427C = 0.0f;
    D_803B4280 = 0;
}
