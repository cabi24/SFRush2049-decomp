typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct {u8 present;u8 pad[15];} Entry;
extern Entry *D_801407FC;
s32 func_800D2FA8(s32,s32,s32 *,s32 *,s32,s32);
s32 func_800D3430(s32 index,s32 value,s32 *out_index,s32 *out_value,s32 flags) {
    if(flags && D_801407FC[index].present) {
        *out_index=index;
        *out_value=value;
        return 1;
    }
    return func_800D2FA8(index,value,out_index,out_value,flags,0);
}
