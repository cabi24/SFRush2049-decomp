/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 fabsf(f32), modff(f32,f32*);
#pragma intrinsic(fabsf)
extern f32 D_80123B98,D_80123B9C,D_80123BA0,D_80123BA4,D_80123BA8,D_80123BAC,D_80123BB0,D_80123BB4,D_80123BB8,D_80123BBC,D_80123BC0,D_80123BC4;
f32 func_800A557C(f32 x) {
 f32 rounded,whole,fraction,remainder,squared,numerator,denominator;
 if(D_80123B98<fabsf(x)) return 0.0f;
 fraction=modff(x*D_80123B9C,&rounded);
 if(fabsf(fraction)>=0.5f) {
  if(x<0.0f) rounded-=1.0f;
  else rounded+=1.0f;
 }
 fraction=modff(x,&whole);
 remainder=((whole-rounded*D_80123BA0)+fraction)-rounded*D_80123BA4;
 if(fabsf(remainder)<D_80123BA8) denominator=1.0f;
 else {
  squared=remainder*remainder;
  numerator=(((D_80123BAC*squared+D_80123BB0)*squared+D_80123BB4)*squared)*remainder;
  denominator=((((D_80123BB8*squared+D_80123BBC)*squared+D_80123BC0)*squared+D_80123BC4)*squared)+1.0f;
  remainder+=numerator;
 }
 if((s32)rounded&1) return denominator/(-remainder);
 return remainder/denominator;
}
