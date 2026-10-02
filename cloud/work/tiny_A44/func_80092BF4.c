/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Key64 {s32 slot;u8 pad[60];} Key64;
typedef struct Slot68 {u8 pad[60];u32 first,second;} Slot68;
extern Key64 D_80139334[];extern Slot68 D_8012E700[];
void func_80092BF4(s16 key,u32 *first,u32 *second) {
 s32 slot=D_80139334[key].slot;
 D_8012E700[(s16)slot].first=*first;
 D_8012E700[(s16)slot].second=*second;
}
