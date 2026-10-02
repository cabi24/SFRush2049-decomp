/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_80098A54(f32 *v) {
 f32 x=v[0],y=v[1],z=v[2];f32 length=sqrtf(z*z+(x*x+y*y)),factor;
 if(length==0.0f){v[1]=v[2]=0.0f;v[0]=1.0f;}
 else {factor=1.0f/length;v[0]=x*factor;v[1]=y*factor;v[2]=z*factor;}
 return length;
}
