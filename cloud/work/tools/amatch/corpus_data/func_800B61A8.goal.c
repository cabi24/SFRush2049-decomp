/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed int s32;
extern signed char D_8010FFC0;
s32 entity_flags_apply(s32, s32, s32, u8);
s32 func_800B61A8(s32 a0, s32 a1, s32 a2, u8 a3)
{
  if (D_8010FFC0 == 0) {
    return -1;
  }
  if (a0 == -1) {
    return -1;
  }
  return entity_flags_apply(a0, a1, a2, a3);
}
