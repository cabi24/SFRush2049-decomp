/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 fabsf(f32), modff(f32,f32*);
#pragma intrinsic(fabsf)
extern f32 D_80123B98,D_80123B9C,D_80123BA0,D_80123BA4,D_80123BA8,D_80123BAC,D_80123BB0,D_80123BB4,D_80123BB8,D_80123BBC,D_80123BC0,D_80123BC4;
f32 func_800A557C(f32 arg0)
{
  f32 sp30;
  f32 sp2C;
  f32 temp_f2;
  f32 temp_f6;
  float new_var;
  f32 fraction;
  f32 var_f0;
  f32 var_f14;
  f32 var_f16;
  if (D_80123B98 < fabsf(arg0))
  {
    return 0.0f;
  }
  if (fabsf(modff(arg0 * D_80123B9C, &sp2C)) >= 0.5f)
  {
    if (arg0 < 0.0f)
    {
      var_f0 = -1.0f;
    }
    else
    {
      var_f0 = 1.0f;
    }
    sp2C += var_f0;
  }
  fraction = modff(arg0, &sp30);
  new_var = (sp30 - (sp2C * D_80123BA0)) + fraction;
  var_f14 = new_var - (sp2C * D_80123BA4);
  if (fabsf(var_f14) < D_80123BA8)
  {
    var_f16 = 1.0f;
  }
  else
  {
    temp_f2 = var_f14 * var_f14;
    var_f14 += ((((D_80123BAC * temp_f2) + D_80123BB0) * temp_f2) + D_80123BB4) * temp_f2 * var_f14;
    temp_f6 = D_80123BB8 * temp_f2;
    var_f16 = ((((((temp_f6 + D_80123BBC) * temp_f2) + D_80123BC0) * temp_f2) + D_80123BC4) * temp_f2) + 1.0f;
  }
  if (((s32) sp2C) & 1)
  {
    return var_f16 / (-var_f14);
  }
  return var_f14 / var_f16;
}
