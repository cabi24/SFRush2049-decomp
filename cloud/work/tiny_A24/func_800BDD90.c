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
f32 func_800BDD90(f32 *p) {f32 y=p[1],x=p[0];f32 length=sqrtf(y*y+x*x);f32 inverse;if(length==0.0f){p[0]=1.0f;p[1]=0.0f;}else{inverse=1.0f/length;p[0]=x*inverse;p[1]=y*inverse;}return length;}
