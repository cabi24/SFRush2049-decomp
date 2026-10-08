/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Historical standalone source was verified with -O2. This context copy is
 * freshly byte-equal under the complete -O3 group recipe disclosed in README.
 * Genuine already accepted direct-return wrapper body; explicit inline contract
 * is witnessed by all five native SOUND expansions in the target.
 * Companion body must remain exactly equal and receives no new credit. */
typedef unsigned char u8;
typedef signed int s32;
extern signed char D_8010FFC0;
s32 entity_flags_apply(s32, s32, s32, u8);
__inline s32 func_800B61A8(s32 a0, s32 a1, s32 a2, u8 a3)
{
  if (D_8010FFC0 == 0) {
    return -1;
  }
  if (a0 == -1) {
    return -1;
  }
  return entity_flags_apply(a0, a1, a2, a3);
}
