/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
extern s8 D_80149B60;
u8 object_type_byte2_get(void);
u8 object_type_byte3_get(void);
s8 func_800B3D0C(void);
s16 object_bytes23_sum(void) { return object_type_byte2_get() + object_type_byte3_get(); }
