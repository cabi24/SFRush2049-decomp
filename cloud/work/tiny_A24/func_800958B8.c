typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct {s8 active;u8 pad1[3];u8 marked;u8 pad5[19];} Record;
extern s32 D_801460F4;
extern Record *D_80144C48;
extern u8 D_8011028C;
void func_800958B8(void) {s32 i;for(i=0;i<D_801460F4;i++){if(D_80144C48[i].active)D_80144C48[i].marked=1;}D_8011028C=1;}
