/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001D084(StateNode *);
int func_8001D4CC(StateNode *state)
{
    if (D_8002C630) {
        func_80014594();
        func_8001D084(state);
        func_800145DC();
        return 1;
    }
    return 0;
}
