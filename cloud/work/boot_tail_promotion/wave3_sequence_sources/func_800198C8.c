/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800198C8.c; canonical native types.
 * Wave 3 copy: includes the production include/sequence_context.h. */
#include "sequence_context.h"

void func_800198C8(void)
{
    int i;
    unsigned char first, second, third;
    if (D_8004F808) {
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].activeFC0) {
                D_8004BE80 = &D_80043EB8[i];
                D_8004BE78 = i;
                D_8004BE7C = func_8001BDB8(D_80043EB8[i].channelFC4);
                func_80018A30();
                func_8001897C();
                func_80018AEC();
                first = func_80018634();
                second = func_80017D38();
                func_8001824C();
                func_80018448();
                func_800180A0();
                third = func_80018184();
                func_80017540();
                if (!first && !second && !third) {
                    D_80043EB8[i].activeFC0 = 0;
                    D_80043EB8[i].inactiveFC1 = 1;
                }
            }
        }
    }
}
