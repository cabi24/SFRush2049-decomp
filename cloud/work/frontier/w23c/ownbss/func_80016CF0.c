/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80016CF0 - lib_16320 (IDO 7.1). MusyX synthdata.c dataGetSample (older revision: no ref_cnt test, no
 * extraData): search every registered sample directory (D_800385A8[0..D_800385A0), func_8001E864 = sndBSearch,
 * comparator func_80016CE0 = smpcmp) for sample ID and fill the sample info SAMPLE (info, address, offset 0, loop,
 * length & 0xFFFFFF, loop length, compression type = length >> 24); returns 0, or -1 when not found.
 * w23c: MusyX's function-local statics key, result, sheader ARE retail's D_80042648 (key.id), D_80042664,
 * D_80042668 (code identical with them; D10 BT03-low-sample-pair true=1235). Needs lib_16320's owned .bss block grown
 * to 0x80042630 (RESULTS.md "ownbss"). No devices, no unused locals. Types: boot_tail_sample_contract.h's
 * SampleRecord/RegisteredSamples (the TU's); private packed views of the sample header (unaligned in the record:
 * retail reads it with lwl/lwr) and of the output sample info. */
#include "boot_tail_sample_contract.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SampleHeader_80016CF0 { u32 info; u32 length; u32 loopOffset; u32 loopLength; } SampleHeader_80016CF0;
typedef struct SampleInfo_80016CF0 { u32 info; void *addr; void *offset; u32 length; u32 loop; u32 loopLength; u8 compType; } SampleInfo_80016CF0;
#pragma pack()
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
int func_80016CE0(const unsigned short *left, const unsigned short *right);
void *func_8001E864(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
int func_80016CF0(u16 id, SampleInfo_80016CF0 *sample)
{
    static SampleRecord key_80016CF0;
    static SampleRecord *result_80016CF0;
    static SampleHeader_80016CF0 *header_80016CF0;
    long i;

    key_80016CF0.identifier = id;
    for (i = 0; i < D_800385A0; ++i) {
        if ((result_80016CF0 = func_8001E864(&key_80016CF0, D_800385A8[i].records, D_800385A8[i].count, sizeof(SampleRecord),
                                    (int (*)(const void *, const void *))func_80016CE0)) != 0) {
            header_80016CF0 = (SampleHeader_80016CF0 *)result_80016CF0->descriptor;
            sample->info = header_80016CF0->info;
            sample->addr = result_80016CF0->data;
            sample->offset = 0;
            sample->loop = header_80016CF0->loopOffset;
            sample->length = header_80016CF0->length & 0xFFFFFF;
            sample->loopLength = header_80016CF0->loopLength;
            sample->compType = header_80016CF0->length >> 24;
            return 0;
        }
    }
    return -1;
}
