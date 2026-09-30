typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct PadCfg { u8 p0[4]; void *p4; u8 p8[0x12]; s8 f1A; u8 p1b[0xD]; s32 f28; } PadCfg;
extern s32 D_80149D98;
extern s32 D_80117358;
extern void Input_ApplyPadConfig(void *);

s32 state_update_global(PadCfg *arg0)
{
s32 t = (D_80149D98 != 0);
 s32 v = arg0->f1A;
 if (t != v) { arg0->f1A = t; Input_ApplyPadConfig(arg0); v = arg0->f1A; }
 if (!v) { arg0->p4 = &D_80117358; Input_ApplyPadConfig(arg0); arg0->f28 = 0; }
 return 1;
}