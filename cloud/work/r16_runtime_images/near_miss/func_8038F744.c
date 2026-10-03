typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s8 D_803B65E4[];
extern s32 D_80156944;
extern s8 D_801164A1;

s8 func_8038F744(s32 index)
{
    s8 *entry = &D_803B65E4[index];
    s8 value = *entry;

    if (index == 7 && !(D_80156944 & 0x100)) {
        value = 0;
    }
    if (entry == &D_803B65E4[6]) {
        value = D_801164A1;
    }
    return value;
}
