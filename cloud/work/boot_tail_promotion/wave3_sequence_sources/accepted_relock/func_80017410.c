/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Standalone pool-score evidence for the canonical sequence-context spelling of
 * the accepted src/rom/lib_17dc0.c body (boot-tail wave 3 relock). */
#include "sequence_context.h"
void func_80017410(SequenceNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    node->next = D_80043EB0;
    if (node->next != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}
