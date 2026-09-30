typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct { s32 pad[8]; } OSMesgQueue;
typedef void *OSMesg;
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
void func_8008A704(void)
{
  OSMesg m = 0;
  if (!D_8011194C) {
    D_8011194C = (s8) 1;
    osCreateMesgQueue(&D_801497D0, (OSMesg *) &D_801527E4, 1);
    osJamMesg(&D_801497D0, (OSMesg) 0, 0);
  }
  (void) osRecvMesg(&D_801497D0, &m, 1);
}
