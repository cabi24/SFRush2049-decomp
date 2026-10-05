/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation; compile with -Iinclude. */
#include "boot_tail_sample_contract.h"
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
extern void func_80014D08(void *, unsigned int);
extern int func_800161A0(SampleRecord *);
int func_800163A8(unsigned short id)
{
    SampleRecord *resource;
    int used;
    int found;
    unsigned int i;
    found = 0;
    for (i = 0; i < (unsigned int)D_800385A0; i++) {
        used = 0;
        for (resource = D_800385A8[i].records; resource->identifier != 0xffff; resource++) {
            if (resource->identifier == id) {
                found = 1;
                if (--resource->references == 0) {
                    func_80014D08(resource->descriptor, resource->offset);
                }
            }
            if (resource->references != 0) {
                used = 1;
            }
        }
        if (found) {
            if (!used) {
                func_800161A0(D_800385A8[i].records);
            }
            return 1;
        }
    }
    return 0;
}
