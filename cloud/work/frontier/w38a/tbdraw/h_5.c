/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
u8 object_type_byte2_get(void);
u8 object_type_byte3_get(void);
s16 object_bytes23_sum(void) { return (s16)object_type_byte2_get() + (s16)object_type_byte3_get(); }
