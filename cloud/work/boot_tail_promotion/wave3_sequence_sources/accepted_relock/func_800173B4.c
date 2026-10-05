/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Standalone pool-score evidence for the canonical sequence-context spelling of
 * the accepted src/rom/lib_17dc0.c body (boot-tail wave 3 relock). */
#include "sequence_context.h"
SequenceNode *func_800173B4(void)
{
    SequenceNode *node;
    SequenceNode *head;
    node = D_80043EB0;
    if (node != 0) {
        D_80043EB0 = node->next;
        if (D_80043EB0 != 0) {
            D_80043EB0->previous = 0;
        }
        node->previous = 0;
        head = D_8004BE80->active;
        node->next = head;
        if (head != 0) {
            D_8004BE80->active->previous = node;
        }
        D_8004BE80->active = node;
    }
    return node;
}
