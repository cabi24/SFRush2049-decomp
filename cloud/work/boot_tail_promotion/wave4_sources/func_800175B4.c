/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800175B4.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

extern unsigned int D_8004BE84;
unsigned int func_800175B4(unsigned int slot)
{
    unsigned int identifier;
    int i;
    do {
        identifier = D_8004BE84++;
        D_8004BE84 &= 0x7FFFFFFFU;
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].inactiveFC1 == 0 &&
                D_80043EB8[i].identifier == identifier) {
                identifier = 0xFFFFFFFFU;
                break;
            }
        }
    } while (identifier == 0xFFFFFFFFU);
    D_80043EB8[slot].identifier = identifier;
    return identifier;
}
