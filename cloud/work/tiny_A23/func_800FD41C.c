typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {u8 pad[636];u32 count;} Clock;
extern Clock D_8002E8E8;
extern u32 D_80111958;
extern f32 D_8002AFB8;
f32 func_800FD41C(void) {return (f32)(D_8002E8E8.count-D_80111958)*D_8002AFB8;}
