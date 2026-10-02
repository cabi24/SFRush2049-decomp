/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Record {u32 pad0[2];s32 id;u8 pad12[2];u8 paused;u8 pad15[45];} Record;
typedef struct OSMesgQueue OSMesgQueue;extern OSMesgQueue D_80142728;
extern u32 D_80146104;extern Record *D_80110270;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
void game_timer_pause(s32 token) {
 Record *record;
 osRecvMesg(&D_80142728,0,1);
 if(token==-1)record=0;
 else {record=&D_80110270[token&D_80146104];if(record->id!=token)record=0;}
 if(record)record->paused=1;
 osJamMesg(&D_80142728,0,0);
}
