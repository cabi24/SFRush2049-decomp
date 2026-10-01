typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {u8 pad[20];s16 field14;u8 tail[46];} Record;
extern Record D_8012E700[];
void func_80090770(s16 index,s16 value) {D_8012E700[index].field14=value;}
