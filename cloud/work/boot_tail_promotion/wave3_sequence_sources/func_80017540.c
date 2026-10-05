/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80017540.c; canonical native types.
 * Wave 3 copy: includes the production include/sequence_context.h. */
#include "sequence_context.h"

void func_80017540(void)
{
    SequenceNode *entry;
    SequenceNode *next;
    entry = D_8004BE80->pending;
    while (entry != 0) {
        next = entry->next;
        if (func_800201D0(entry->identifier) == 0xFFFFFFFFU) func_80017470(entry);
        entry = next;
    }
}
