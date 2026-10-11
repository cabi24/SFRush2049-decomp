/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800161A0 (IDO 7.1): code identical; relocation-name split only (w19b). Retail reaches a label that splat
 * placed at a compiler-made address (base-8/-12 loop end pointer or base+4 field). With the reloc_addrs.us.txt
 * entries in cloud/work/frontier/w19b/RESULTS.md (re-split target) this source scores
 * LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical. Prior: D10/BT03 (and w18c relocname/ for 1661C/158D8/15F28). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#include "boot_tail_sample_contract.h"
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_800161A0(SampleRecord *records)
{
    int index;
    int following;
    for (index = 0; index < D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    if (index != D_800385A0) {
        func_80014594();
        for (following = index + 1; following < D_800385A0; ++following) {
            D_800385A8[following - 1] = D_800385A8[following];
        }
        --D_800385A0;
        func_800145DC();
        return 1;
    }
    return 0;
}
