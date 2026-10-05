/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8004FA18;
extern SampleBuffer D_8004FA50[];
extern void func_80014C60(short *, unsigned int);

void func_8001C508(void)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004FA50[i].mode == 1) {
            func_80014C60(D_8004FA50[i].buffer, D_8004FA50[i].samples);
        }
    }
}
