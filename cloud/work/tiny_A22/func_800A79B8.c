typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {u8 pad00[26];s16 field1A;u32 field1C;u8 pad20[36];} Record;
extern Record D_8012E700[];
void func_800A79B8(s32 index,s32 halfword,u32 word) {Record *r=&D_8012E700[index];r->field1A=halfword;r->field1C=word;}
