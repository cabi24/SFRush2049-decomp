/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern f32 D_80123E04,D_80123E08;
f32 func_800BC21C(f32 *vector) {
 f32 x=vector[0],z=vector[2],result=x;
 if(z>0.0f) {if(z<400.0f)result=x/z;else result=x*D_80123E04;}
 else if(z<0.0f) {if(z>-400.0f)result=-x/z;else result=x*D_80123E08;}
 return result;
}
