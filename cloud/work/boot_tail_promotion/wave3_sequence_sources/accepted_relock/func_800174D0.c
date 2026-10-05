/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Standalone pool-score evidence for the canonical sequence-context spelling of
 * the accepted src/rom/lib_17dc0.c body (boot-tail wave 3 relock). */
#include "sequence_context.h"
void func_800174D0(SequenceNode *node)
{
    SequenceNode *head;
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    head = D_8004BE80->pending;
    node->next = head;
    if (head != 0) {
        D_8004BE80->pending->previous = node;
    }
    node->previous = 0;
    D_8004BE80->pending = node;
}
