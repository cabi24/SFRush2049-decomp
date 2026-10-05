/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;
typedef void *OSMesg;typedef struct OSMesgQueue OSMesgQueue;
typedef struct Controller {u8 other0[6];s8 present;u8 other7[5];u8 pak[112];s8 active,pending;u8 other126[646];} Controller;
extern s8 D_8011EAE4,D_8011194C;
extern OSMesgQueue D_801497D0,D_80035458;
extern OSMesg D_801527E4;
extern Controller D_80144030[];
void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
s32 osRecvMesg(OSMesgQueue *,OSMesg *,s32);s32 osJamMesg(OSMesgQueue *,OSMesg,s32);
s32 osMotorStart(OSMesgQueue *,void *,s32);s32 osMotorInit(void *,s32);
void check_mpath_save(void)
{
 OSMesg message;Controller *controller;void *pak;s32 index;
 D_8011EAE4=0;
 if(D_8011194C==0) {
  D_8011194C=1;
  osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
  osJamMesg(&D_801497D0,0,0);
 }
 osRecvMesg(&D_801497D0,&message,1);
 controller=D_80144030;
 for(index=0;index<4;index++,controller++) {
  pak=controller->pak;
  if(controller->present) {
   controller->pending=0;
   osMotorStart(&D_80035458,pak,index);
   if(osMotorInit(pak,0)==0)controller->active=0;
  }
 }
 osJamMesg(&D_801497D0,0,0);
}
