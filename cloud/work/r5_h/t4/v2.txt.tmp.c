typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
extern s8 D_801147C4;
extern s32 D_801551E8[];

void func_800B73E4(void)
{
  u32 i; s32 *p;
  if (!D_801147C4) {
 D_801147C4 = 1;
 D_801551E8[1] = 0;
 D_801551E8[0] = 0;
 for (p = D_801551E8 + 2; p < D_801551E8 + 10; p++) *p = 0;
}
}