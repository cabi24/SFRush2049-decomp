/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018A30.c; canonical native types. */
#include "../sequence_context.h"

void func_80018A30(void)
{
    if (D_8004BE80->timingStartF68) {
        while (D_8004BE80->currentF6C->time != 0xFFFFFFFFU) {
            if (D_8004BE80->highF74 + D_8004BE80->half120 <
                D_8004BE80->currentF6C->time) break;
            func_80019A60(D_8004BE80->rate124 =
                D_8004BE80->currentF6C->value, D_8004BE78);
            D_8004BE80->currentF6C++;
        }
    }
}
