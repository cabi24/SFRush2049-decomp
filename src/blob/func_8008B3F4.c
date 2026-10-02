/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
extern f32 D_80123884;f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_8008B3F4(f32 value) {if(value<D_80123884)value=D_80123884;return 1.0f/sqrtf(value);}
