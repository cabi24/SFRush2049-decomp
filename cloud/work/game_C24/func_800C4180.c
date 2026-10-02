/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
extern const f32 D_80123F80;
extern f32 func_8008C768(f32,f32);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)
#define C24_ABS(x) fabsf(x)
void func_800C4180(f32 *m, f32 *out) {
 f32 x=m[6];
 if (C24_ABS(x)<D_80123F80 && C24_ABS(m[8])<D_80123F80) {
  *out=func_8008C768(m[2],m[0]);
 } else {
  *out=func_8008C768(-x,m[8]);
 }
}
