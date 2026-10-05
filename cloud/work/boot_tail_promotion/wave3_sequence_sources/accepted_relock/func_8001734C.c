/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Standalone pool-score evidence for the canonical sequence-context spelling of
 * the accepted src/rom/lib_17dc0.c body (boot-tail wave 3 relock). */
#include "sequence_context.h"
extern int func_8001FA18(unsigned int);
void func_8001734C(SequenceContext *context)
{
    SequenceNode *entry;
    entry = context->active;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
    entry = context->pending;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
}
