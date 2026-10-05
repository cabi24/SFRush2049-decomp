/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800171C0.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

extern SequenceNode D_800426B0[256];
void func_800171C0(void)
{
    SequenceNode *previous;
    int i;
    previous = 0;
    D_80043EB0 = D_800426B0;
    for (i = 0; i < 256; i++) {
        D_800426B0[i].previous = previous;
        if (previous != 0) {
            previous->next = &D_800426B0[i];
        }
        previous = &D_800426B0[i];
    }
    previous->next = 0;
}
