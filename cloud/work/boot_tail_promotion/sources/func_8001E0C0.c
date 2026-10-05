/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
extern StateNode *D_8004FD50;
extern LinkNode *D_8004FD54;
void func_8001E0C0(void)
{
    D_8004FD50 = 0;
    D_8004FD54 = 0;
}
