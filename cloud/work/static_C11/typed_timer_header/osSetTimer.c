/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
int osSetTimer(OSTimer* t, OSTime countdown, OSTime interval, OSMesgQueue* mq, OSMesg msg) {
    OSTime time;


    t->next = NULL;
    t->prev = NULL;
    t->interval = interval;
    t->value = (countdown != 0) ? countdown : interval;
    t->mq = mq;
    t->msg = msg;

    time = dll_insert(t);
    if (((OSTimer *)__osTimerList)->next == t) {
        dll_reschedule(time);
    }

    return 0;
}
