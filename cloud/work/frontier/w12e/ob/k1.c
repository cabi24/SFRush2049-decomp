/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Bank Bank;
extern Bank *D_801497F0;
extern s8 D_80149B60;
void sound_update_channel();
s16 object_bytes23_sum(void) { u8 first; sound_update_channel(0, 0, 0); first = *((u8 *)D_801497F0 + 2); sound_update_channel(0, 0, 0); return (s16)(first + *((u8 *)D_801497F0 + 3)); }
s16 object_bytes_sum_global(void) {
 u8 first;
 sound_update_channel(0);
 first = *((u8 *)D_801497F0 + 3);
 sound_update_channel(0);
 return (s16)(D_80149B60 + *((u8 *)D_801497F0 + 2) + first);
}
