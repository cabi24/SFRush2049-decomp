/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
OSTime dll_insert(OSTimer* t) {
    OSTimer* timep;
    OSTime tim;
    u32 savedMask = __osDisableInt();

    timep = __osTimerList->next;
    tim = t->value;
    for (; timep != __osTimerList && tim > timep->value; timep = timep->next) {
        tim -= timep->value;
    }

    t->value = tim;

    if (timep != __osTimerList) {
        timep->value -= tim;
    }

    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}
