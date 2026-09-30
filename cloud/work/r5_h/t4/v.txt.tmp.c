typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
extern s8 D_801147C4;
extern s32 D_801551E8;
extern s32 D_801551EC;
extern s32 D_801551F0[];

void func_800B73E4(void)
{
  u32 i; s32 *p;
  if (D_801147C4 == 0) {
 D_801147C4 = 1;
 D_801551EC = D_801551E8 = 0;
 i = 0; while (i < 8) { D_801551F0[i++] = 0; }
}
}