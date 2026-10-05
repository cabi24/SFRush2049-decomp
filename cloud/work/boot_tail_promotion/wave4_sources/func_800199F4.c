/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800199F4.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

extern void func_800171C0(void);
extern void func_800175A8(void);
void func_800199F4(void)
{
    unsigned int index;
    for (index = 0; index < 8; index++) {
        D_80043EB8[index].activeFC0 = 0;
        D_80043EB8[index].inactiveFC1 = 1;
    }
    func_800171C0();
    func_800175A8();
}
