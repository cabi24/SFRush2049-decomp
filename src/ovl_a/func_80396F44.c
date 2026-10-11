/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s16 D_8014A108;
extern s32 D_803BACE0[];

s32 func_80396F44(s32 count, s32 skip)
{
    s8 i;
    s32 found = 0;

    count = D_8014A108;
    for (i = 0; i < count; i++) {
        if (i != skip && D_803BACE0[i] == 3) {
            found = 1;
        }
    }
    return found;
}
