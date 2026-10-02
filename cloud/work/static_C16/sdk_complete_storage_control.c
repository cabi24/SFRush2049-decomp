/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
OSTime gViTimeAccumHi;
u32 gViLastCount;
u32 gViRetraceCount;
u32 __osTimerCounter;
OSTimer __osBaseTimer;
OSTimer* __osTimerList = &__osBaseTimer;
void dll_init(void) {
    gViTimeAccumHi = 0;
    gViLastCount = 0;
    gViRetraceCount = 0;
    __osTimerList->next = __osTimerList->prev = __osTimerList;
    __osTimerList->interval = __osTimerList->value = 0;
    __osTimerList->mq = NULL;
    __osTimerList->msg = 0;
}
