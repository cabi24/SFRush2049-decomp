/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
/* Native function is original SDK __scYield, not pre-NMI handling. */
void __scHandlePreNMI(OSSched *scheduler) {
 if(scheduler->curRSPTask->type==1) {
  scheduler->curRSPTask->state|=0x10;
  osDpWait();
 } else {
 }
}
