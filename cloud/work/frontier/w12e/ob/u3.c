/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Bank Bank;
extern Bank *D_801497F0;
extern s8 D_80149B60;
void sound_update_channel(s32 force);
u8 object_type_byte2_get(void);
u8 object_type_byte3_get(void);
s16 object_bytes23_sum(void) { s32 a; s32 b; u8 first; first=object_type_byte2_get(); a = first + object_type_byte3_get(); if (b) {} if (a) {} return (s16)a; }
s16 object_bytes_sum_global(void) {
 u8 first;
 sound_update_channel(0);
 first = *((u8 *)D_801497F0 + 3);
 sound_update_channel(0);
 return (s16)(D_80149B60 + *((u8 *)D_801497F0 + 2) + first);
}
