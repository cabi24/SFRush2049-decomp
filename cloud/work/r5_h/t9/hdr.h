typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct Y { u8 pad[0x38]; s32 f38; u8 name[11]; } Y;
typedef struct X { u8 p0[8]; void **f8; u8 p1[0x15]; u8 name[11]; Y **f2C; } X;
s32 func_800A1910(u8 *a, u8 *b, s32 n);
extern void *memcpy(void *, void *, unsigned);
s32 format_string_parse();
void slot_state_lookup(void **a, u32 b, s32 c);
