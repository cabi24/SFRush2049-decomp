/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018F20.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

void func_80018F20(unsigned int identifier, unsigned short value)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].valueFC2 = value;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].valueFEC = value;
            D_80043EB8[index].flagsFEE |= 0x20;
        }
    }
}
