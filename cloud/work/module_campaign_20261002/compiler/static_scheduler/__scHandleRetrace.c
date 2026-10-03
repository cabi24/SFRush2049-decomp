#include "scheduler_native.h"
/* SDK __scHandleRetrace adapted to N64 pending-swap and notification path.
 * Removed task-drain block and its unused donor locals are not imported. */
void __scHandleRetrace(OSSched *scheduler) {
 OSScClient *client;
 scheduler->retraceCount++;
 if(__osScPendingSwap && scheduler->retraceCount-__osScSwapCount>=2U) {
  osViSetMode((void *)(u32)__osScPendingSwap);
  display_mode_tick();
  __osScSwapCount=scheduler->retraceCount;
  __osScPendingSwap=0;
 }
 for(client=scheduler->clientList;client!=0;client=client->next)
  osJamMesg(client->msgQueue,(OSMesg)scheduler,OS_MESG_NOBLOCK);
}
