/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u8 f0, f1, ascent, descent; } FontHdr;
extern FontHdr *D_801497F0;
void sound_update_channel(s32 force);
u8 object_type_byte2_get(void);
u8 object_type_byte3_get(void);
static u32 asc(void) { sound_update_channel(0); return D_801497F0->ascent; }
static u32 des(void) { sound_update_channel(0); return D_801497F0->descent; }
s16 object_bytes23_sum(void) { return asc() + des(); }
