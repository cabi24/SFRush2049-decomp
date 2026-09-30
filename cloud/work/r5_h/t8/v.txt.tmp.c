typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct PartSlot {
  u8 pad0[0x24]; f32 f24; f32 f28; f32 f2C; u8 pad30[0x60]; f32 f90; s8 f94; u8 pad95[3];
} PartSlot;
extern s32 D_8011418C;
extern PartSlot D_80150B70[];
extern s32 D_80150B70x;
void math_utility(void *arg0, void *arg1);

void particle_position_set(s32 arg0)
{
PartSlot *p = &D_80150B70[((arg0 & 1) ? 0 : (arg0 & 2) ? 1 : (arg0 & 4) ? 2 : 3)];
 p->f94 = ((arg0 & 1) ? 0 : (arg0 & 2) ? 1 : (arg0 & 4) ? 2 : 3); p->f24 = 0; p->f28 = 0; p->f2C = 0;
 math_utility(&D_8011418C, p);
 p->f90 = 0;
}