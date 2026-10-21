/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern f32 D_80152748;
s32 func_80095198(f32 x,f32 delta) {f32 sum=x+delta;if(D_80152748<x)sum-=14400.0f;return sum<D_80152748;}
