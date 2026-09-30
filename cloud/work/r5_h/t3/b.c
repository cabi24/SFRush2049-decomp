typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct { u8 pad[3]; s8 f3; u8 pad4[2]; u8 f6; u8 pad7[13]; } E14;
extern E14 D_80156D38[];
extern s8 D_801234AC[];
extern unsigned char D_80140BDC;
/*@1: void func_800972C4(s32 a) || void func_800972C4(s32 a) */
{
  /*@2: s8 v; E14 *e; || E14 *e; s8 v; || s8 v, *p; E14 *e; || */
  /*@3: e = &D_80156D38[a]; || e = D_80156D38 + a; || */
  /*@4: v = D_801234AC[e->f6]; e->f3 = v; || v = D_801234AC[e->f6]; e->f3 = v; || p = &e->f3; v = D_801234AC[e->f6]; *p = v; */
  /*@5: if (v != 0 && a >= D_80140BDC) D_80140BDC = a + 1; || if (v != 0) { if (a >= D_80140BDC) D_80140BDC = a + 1; } */
}
