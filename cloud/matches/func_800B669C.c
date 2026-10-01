/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 first,second; } Pair;
extern Pair D_80118E20;
void func_800B669C(u32 first,u32 second) {Pair *p=&D_80118E20;p->first=first;p->second=second;}
