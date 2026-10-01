/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
void dll_update(void) {
    OSTimer* t;
    u32 count;
    u32 elapsed_cycles;


    if (__osTimerList->next == __osTimerList) {
        return;
    }
    for (;;) {
        t = __osTimerList->next;

        if (t == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }

        count = osGetCount();
        elapsed_cycles = count - __osTimerCounter;
        __osTimerCounter = count;

        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            dll_reschedule(t->value);
            break;
        }

        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = NULL;
        t->prev = NULL;

        if (t->mq != NULL) {
            osJamMesg(t->mq, t->msg, OS_MESG_NOBLOCK);
        }

    __ProfDone:

        if (t->interval != 0) {
            t->value = t->interval;
            dll_insert(t);
        }
    }
}
