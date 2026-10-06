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
s16 object_bytes23_sum(void) { s16 r; u8 a; a = object_type_byte2_get(); r = a + object_type_byte3_get(); return r; }
s16 object_bytes_sum_global(void) { s8 g; u8 a; u8 b; a = object_type_byte3_get(); b = object_type_byte2_get(); g = D_80149B60; return (s16)(g + b + a); }
