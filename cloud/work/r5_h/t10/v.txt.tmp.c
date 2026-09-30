typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct { s32 pad[8]; } OSMesgQueue;
typedef void *OSMesg;
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
extern OSMesgQueue D_80153E68;
extern OSMesg D_80153EF0[];
typedef struct { s8 f0; u8 p[11]; OSMesgQueue *f12; } TS;
extern TS D_80153F10;
s32 func_8008ABE4();

void task_complete_signal(void *arg0)
{
 osCreateMesgQueue(&D_80153E68, D_80153EF0, 1);
 D_80153F10.f0 = 0;
for (;;) {
 osRecvMesg(&D_80153E68, 0, 1);
 if (func_8008ABE4() == 0) {
 D_80153F10.f0 = 0;
 osJamMesg(D_80153F10.f12, 0, 1);
 }
 }
 return;
}