/* 9/30 words differ: arg3 lands in s0 (target: t0 copy), v copy v1 vs t1 */
void func_800ADCE0(u8 *arg0, s32 arg1, s16 *arg2, s32 arg3)
{
  s16 *hi;
  u32 w;
  u32 cnt;
  u32 v;

  hi = arg2 + arg1 - 1;
  while (arg1 > 0) {
    w = (arg0[0] << 8) + arg0[1];
    arg0 += 2;
    cnt = w >> 13;
    v = w & 0x1FFF;
    arg1 = arg1 - cnt - 1;
    while ((s32) cnt >= 0) {
      cnt -= 1;
      if (v == arg3) {
        *arg2++ = v++;
      } else {
        *hi-- = v++;
      }
    }
  }
}
