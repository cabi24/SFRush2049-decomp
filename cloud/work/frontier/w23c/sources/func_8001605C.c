/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001605C - lib_16320 (IDO 7.1). MusyX synthdata.c dataInsertSDir (older revision, as the GameCube decomp
 * doldecomp/ttyd libs/musyx MUSY_VERSION >= 2.0.2 branch without the duplicate-id scan; shape only): register the
 * sample directory RECORDS (terminated by identifier 0xFFFF) with sample data BASE in D_800385A8[D_800385A0]
 * (dataSmpSDirs, max 8): count the records, then with interrupts off (func_80014594/func_800145DC =
 * hwDisableIrq/hwEnableIrq) store records/base/count, clear every record's reference count, bump the directory
 * count. Returns 1 when registered or already present, 0 when the table is full.
 * w23c (from D10 BT03-low-registry, true=125): locals in MusyX's order (i, s, n, j: index, entry, count, i) give
 * retail's homes (entry -8, count -10), and the clearing loop increments the record pointer in the for header
 * (`for (i = 0; i < count; ++i, ++entry)`), which puts the pointer add before the store as retail. No devices,
 * no unused locals. Types: the TU's boot_tail_sample_contract.h (SampleRecord, RegisteredSamples). */
#include "boot_tail_sample_contract.h"
typedef unsigned short u16;
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_8001605C(SampleRecord *records, void *base)
{
    int index;
    SampleRecord *entry;
    u16 count;
    u16 i;
    for (index = 0; index < D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    if (index == D_800385A0) {
        if (D_800385A0 < 8) {
            count = 0;
            entry = records;
            while (entry->identifier != 0xFFFF) {
                ++count;
                ++entry;
            }
            entry = records;
            func_80014594();
            D_800385A8[D_800385A0].records = records;
            D_800385A8[D_800385A0].base = base;
            D_800385A8[D_800385A0].count = count;
            for (i = 0; i < count; ++i, ++entry) {
                entry->references = 0;
            }
            ++D_800385A0;
            func_800145DC();
            return 1;
        }
        return 0;
    }
    return 1;
}
