/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"

void dll_remove(register OSThread** queue, register OSThread* t) {
    register OSThread* pred;
    register OSThread* succ;

    pred = (OSThread*)queue;
    succ = pred->next;

    while (succ != NULL) {
        if (succ == t) {
            pred->next = t->next;

            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
