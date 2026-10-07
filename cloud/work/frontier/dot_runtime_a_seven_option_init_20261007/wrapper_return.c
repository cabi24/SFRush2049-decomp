/* Main-image source-contract sidecar only. Never compiled into runtime A. */
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
extern s32 func_8008E26C(u32,u32,s16,u32);
s32 sign_extend_call(u32 value,u32 data,s32 index,u32 flags)
{
    return func_8008E26C(value,data,(s16)index,flags);
}
