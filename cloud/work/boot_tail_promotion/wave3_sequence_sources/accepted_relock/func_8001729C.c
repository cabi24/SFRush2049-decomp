/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Standalone pool-score evidence for the canonical sequence-context spelling of
 * the accepted src/rom/lib_17dc0.c body (boot-tail wave 3 relock). */
#include "sequence_context.h"
void func_8001729C(SequenceContext *state)
{
    SequenceNode *node;
    node = state->active;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->active;
        state->active = 0;
    }
    node = state->pending;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->pending;
        state->pending = 0;
    }
}
