/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018D40.c; canonical native types. */
#include "../sequence_context.h"

void func_80018D40(unsigned int identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
            D_80043EB8[identifier].inactiveFC1 = 1;
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].pendingFF0 = 0;
            }
        }
    }
}
