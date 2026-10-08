/* Accepted func_800B61A8 body, copied without semantic changes from
 * src/blob/groups/frontier_car_checkpoints/func_800B61A8.c at dea99f09.
 * This is read-only genuine kept context, not a new claim.
 * Its implementation/prototype also documents the real SOUND expansion in
 * accepted func_800F8EC8, another native caller of race_countdown_display.
 */
typedef int s32;
extern signed char D_8010FFC0;
unsigned int entity_flags_apply(unsigned int, unsigned int, unsigned int,
                               unsigned char);
__inline s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3)
{
  unsigned int new_var;
  if (D_8010FFC0 == 0)
  {
    return -1;
  }
  if (arg0 == (-1))
  {
    return -1;
  }
  new_var = entity_flags_apply(arg0, arg1, arg2, arg3);
  return new_var;
}
