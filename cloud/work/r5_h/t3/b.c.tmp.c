typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct { u8 pad[3]; s8 f3; u8 pad4[2]; u8 f6; u8 pad7[13]; } E14;
extern E14 D_80156D38[];
extern s8 D_801234AC[];
extern unsigned char D_80140BDC;
void func_800972C4(s32 a)
{
  E14 *e; s8 v;
  e = D_80156D38 + a;
  v = D_801234AC[e->f6]; e->f3 = v;
  if (v != 0 && a >= D_80140BDC) D_80140BDC = a + 1;
}
