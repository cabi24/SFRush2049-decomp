typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct { s32 a, b, c, d; } S;
extern s8 D_801147C4;
extern s32 D_801551E8;
extern s32 D_801551EC;
extern S D_801551F0[];

void func_800B73E4(void)
{
  s32 i; S *s;
  if (D_801147C4 == 0) {
 D_801147C4 = 1;
 D_801551EC = 0;
 D_801551E8 = 0;
 i = 0; do { D_801551F0[i].a = 0; D_801551F0[i].b = 0; D_801551F0[i].c = 0; D_801551F0[i].d = 0; i++; } while (i < 2);
}
}