/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018FEC.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

void func_80018FEC(unsigned int identifier)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].activeFC0 = 1;
        } else {
            D_80043EB8[index & 0x7FFFFFFFU].flagsFEE &= ~8;
        }
    }
}
