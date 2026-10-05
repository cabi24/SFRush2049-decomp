/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018E6C.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

void func_80018E6C(void)
{
    int i;
    for (i = 0; i < 8; i++) {
        func_80018D40(D_80043EB8[i].identifier);
    }
}
