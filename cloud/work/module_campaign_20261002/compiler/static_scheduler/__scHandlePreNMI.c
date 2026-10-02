#include "scheduler_native.h"
/* Native function is original SDK __scYield, not pre-NMI handling. */
void __scHandlePreNMI(OSSched *scheduler) {
 if(scheduler->curRSPTask->type==1) {
  scheduler->curRSPTask->state|=0x10;
  osDpWait();
 } else {
 }
}
