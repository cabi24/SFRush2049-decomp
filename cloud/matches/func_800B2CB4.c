/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct {u16 id,slot;u8 pad4[28];} Record;
typedef struct {u16 mode;u8 pad2[22];} Dest;
extern u16 D_8015267C;
extern Record *D_801525EC;
extern Dest *D_801497F8;
void func_800B2CB4(u32 id) {s32 i;Record *p=D_801525EC;for(i=0;i<D_8015267C;i++,p++){if(p->id==id)D_801497F8[p->slot].mode=15;}}
