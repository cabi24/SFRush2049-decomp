/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
f32 func_8009D444(f32 x,f32 y) {return sqrtf(x*x+y*y);}
