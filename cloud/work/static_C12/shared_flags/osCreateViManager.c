/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
void osCreateViManager(OSThread* t, OSPri pri) {
    register u32 saveMask;


    saveMask = __osDisableInt();

    if (t == NULL) {
        t = __osRunningThread;
    }

    if (t->priority != pri) {
        t->priority = pri;

        if (t != __osRunningThread && t->state != OS_STATE_STOPPED) {
            dll_remove(t->queue, t);
            __osEnqueueThread(t->queue, t);
        }

        if (__osRunningThread->priority < ((OSThread *)__osActiveQueue)->priority) {
            __osRunningThread->state = OS_STATE_RUNNABLE;
            __osCleanupThread((OSThread **) &__osActiveQueue);
        }
    }

    __osRestoreInt(saveMask);
}
