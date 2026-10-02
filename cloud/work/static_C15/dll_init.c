/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern OSTime gViTimeAccumHi; extern u32 gViLastCount,gViRetraceCount;
void dll_init(void) {
    gViTimeAccumHi = 0;
    gViLastCount = 0;
    gViRetraceCount = 0;
    __osTimerList->next = __osTimerList->prev = __osTimerList;
    __osTimerList->interval = __osTimerList->value = 0;
    __osTimerList->mq = NULL;
    __osTimerList->msg = 0;
}
