typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef struct {u32 word0;void *pointer4;u8 pad8[18];s8 flag1A;u8 pad1B[13];u32 word28;} Record;
extern u32 D_80149D98,D_80117358;
void Input_ApplyPadConfig(void *);
s32 state_update_global(Record *p) {u32 desired=D_80149D98!=0;if(desired!=p->flag1A){p->flag1A=desired;Input_ApplyPadConfig(p);}if(p->flag1A)return 1;p->pointer4=&D_80117358;Input_ApplyPadConfig(p);p->word28=0;return 1;}
