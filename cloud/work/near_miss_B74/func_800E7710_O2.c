/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;
typedef struct OSMesgQueue {void *mtqueue,*fullqueue;int validCount,first,msgCount;void **msg;} OSMesgQueue;
extern OSMesgQueue D_80035458,D_801497D0;
extern void *D_80150F18[8],*D_801527E4[1];
extern s16 D_80150F58;
extern s8 D_8011194C;
extern u8 D_80111950,D_80149440[],D_80156CF0,D_80156D00,D_80156D10,D_80156D20;
extern void osCreateMesgQueue(OSMesgQueue *,void **,int),osSetEventMesgAlt(int,OSMesgQueue *,void *);
extern int osJamMesg(OSMesgQueue *,void *,int),osRecvMesg(OSMesgQueue *,void **,int),__osContBuildPacket(OSMesgQueue *,u8 *,u8 *);
void func_800E7710(void) {
 void *message;
 osCreateMesgQueue(&D_80035458,D_80150F18,8);
 D_80150F58=5;
 osSetEventMesgAlt(5,&D_80035458,&D_80150F58);
 if(!D_8011194C) {
  D_8011194C=1;
  osCreateMesgQueue(&D_801497D0,D_801527E4,1);
  osJamMesg(&D_801497D0,0,0);
 }
 osRecvMesg(&D_801497D0,&message,1);
 __osContBuildPacket(&D_80035458,&D_80111950,D_80149440);
 osJamMesg(&D_801497D0,0,0);
 D_80156CF0=0;D_80156D00=0;D_80156D10=0;D_80156D20=0;
}
