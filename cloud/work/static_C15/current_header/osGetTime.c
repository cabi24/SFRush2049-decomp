/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern OSTime gViTimeAccumHi; extern u32 gViLastCount; extern u32 osGetCount(void);
OSTime osGetTime(void) {
    u32 tmptime;
    u32 elapseCount;
    OSTime currentCount;
    register u32 saveMask;


    saveMask = __osDisableInt();
    tmptime = osGetCount();
    elapseCount = tmptime - gViLastCount;
    currentCount = gViTimeAccumHi;
    __osRestoreInt(saveMask);
    return currentCount + elapseCount;
}
