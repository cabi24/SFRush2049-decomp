/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern SampleBuffer D_8004FA50[];
extern void func_8001F954(unsigned int);
void func_8001C7F4(unsigned int channel)
{
    if (D_8002C630) {
        func_80014594();
        if (D_8004FA50[channel].mode == 1) {
            D_8004FA50[channel].mode = 0;
            func_8001F954(channel);
        }
        func_800145DC();
    }
}
