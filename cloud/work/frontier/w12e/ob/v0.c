/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Bank Bank;
extern Bank *D_801497F0;
extern s8 D_80149B60;
extern s32 D_80149B08;
void sound_update_channel(s32 force);
u8 object_type_byte2_get(void);
u8 object_type_byte3_get(void);
void func_80096288(s32 arg0, s32 arg1, s32 arg2)
{
  s32 t;
  if (arg0) {}
  if (arg1) {}
  t = !arg2;
  if (arg2 != 0)
  {
    if (t) {}
    if (t) {}
  }
  if (t) {}
  if (0) { switch (arg0 + arg1) { case 1: D_80149B08 = 1; break; case 2: D_80149B08 = 2; break; case 3: D_80149B08 = 3; break; } }
}
s16 object_bytes23_sum(void) { u8 first; first=object_type_byte2_get(); return (s16)(first + object_type_byte3_get()); }
s16 object_bytes_sum_global(void) {
 u8 first;
 sound_update_channel(0);
 first = *((u8 *)D_801497F0 + 3);
 sound_update_channel(0);
 return (s16)(D_80149B60 + *((u8 *)D_801497F0 + 2) + first);
}
