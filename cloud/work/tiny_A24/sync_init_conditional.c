typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
typedef void *OSMesg;
typedef struct {void *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
s32 osJamMesg(OSMesgQueue *,OSMesg,s32);
extern s8 D_801147C0,D_80149DA0;
extern OSMesgQueue D_801461D0;
extern OSMesg D_801461FC;
void sync_init_conditional(void) {if(!D_801147C0){D_801147C0=1;osCreateMesgQueue(&D_801461D0,&D_801461FC,1);osJamMesg(&D_801461D0,NULL,0);}D_80149DA0=-1;}
