/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u32 osGetCount(void); extern void __osSetCompare(u32);
void dll_reschedule(OSTime tim) {
    OSTime NewTime;
    u32 savedMask;


    savedMask = __osDisableInt();
    __osTimerCounter = osGetCount();
    NewTime = __osTimerCounter + tim;
    __osSetCompare(NewTime);
    __osRestoreInt(savedMask);
}
