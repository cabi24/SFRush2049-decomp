#include "scheduler_native.h"
/* Complete SDK scheduler event dispatcher; N64 adds frame timestamps and
 * task-queue scheduling, with actual five message IDs 666..670. */
void __scMain(void *arg) {
 OSMesg message;
 OSSched *scheduler=(OSSched *)arg;
 OSScClient *client;
 while(1) {
  osRecvMesg(&scheduler->cmdQueue,&message,OS_MESG_BLOCK);
  switch((s32)message) {
   case 666:
    __osScFrameTimeHi=osGetTime();
    __scHandleRetrace(scheduler);
    __scSchedule(scheduler);
    break;
   case 667:
    __scHandleRSP(scheduler);
    break;
   case 668:
    __osScFrameTimeResult=(s32)(osGetTime()-__osScRetraceTimeHi);
    __scHandleRDP(scheduler);
    break;
   case 669:
    __scSchedule(scheduler);
    break;
   case 670:
    for(client=scheduler->clientList;client!=0;client=client->next)
     osSendMesg(client->msgQueue,(OSMesg)&scheduler->priority,OS_MESG_BLOCK);
    break;
  }
 }
}
