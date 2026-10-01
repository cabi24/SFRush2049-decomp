typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
void func_8008E26C(u32,u32,s32,u32);
void sign_extend_call(u32 a,u32 b,s32 index,u32 flags) {func_8008E26C(a,b,(s32)((u32)index<<16)>>16,flags);}
