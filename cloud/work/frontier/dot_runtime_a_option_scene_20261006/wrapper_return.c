/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef short s16; typedef int s32; typedef unsigned int u32;
extern s32 func_8008E26C(u32,u32,s16,u32);
s32 sign_extend_call(u32 value, u32 data, s32 index, u32 flags)
{
    return func_8008E26C(value, data, (s16)index, flags);
}
