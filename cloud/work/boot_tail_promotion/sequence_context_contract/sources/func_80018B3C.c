/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80018B3C.c; canonical native types. */
#include "../sequence_context.h"

void func_80018B3C(unsigned int value)
{
    if (D_8004BE80->timingStartF68) {
        D_8004BE80->currentF6C = D_8004BE80->timingStartF68;
        D_8004BE80->highF74 = value;
        D_8004BE80->lowF70 = 0;
        func_80018A30();
        func_8001897C();
    }
}
