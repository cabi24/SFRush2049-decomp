typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct {s8 active;u8 pad[23];} Record;
extern s32 D_801460F4;
extern Record *D_80144C48;
s32 func_800FE73C(void) {s32 i;for(i=0;i<D_801460F4;i++){if(D_80144C48[i].active)return 0;}return 1;}
