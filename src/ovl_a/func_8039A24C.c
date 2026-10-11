/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef int s32;
s32 func_800A361C(s32 arg0);
s32 func_800A35F8(s32 arg0);
extern s32 D_803B46B4;

s32 func_8039A24C(s32 arg0)
{
    if (func_800A361C(arg0) || (D_803B46B4 == 2 && func_800A35F8(arg0))) {
        return 1;
    }
    return 0;
}
