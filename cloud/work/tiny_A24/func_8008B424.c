typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
extern f32 D_80123888;
f32 func_8008B424(f32 *p) {f32 x=p[0],y=p[1],z=p[2];f32 square=z*z+(x*x+y*y);if(square<D_80123888)square=D_80123888;return 1.0f/sqrtf(square);}
