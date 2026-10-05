/* GENERATED ROM-aligned TU — segment 0x16320 (rom/lib_16320)
 * layout map 231669c219ff8d17283ad97903995c8f4ca7b9a1708f46d32621282f1fe514f5; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015720.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_800158D8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015A0C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015C0C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015D68.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015F28.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_8001605C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_800161A0.s")
/* PROMOTED 2026-10-04 — func_800162AC
 * Source:   cloud/matches/boot_tail/func_800162AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800162AC.c:func_800162AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
extern void func_80014CFC(void **, void **);
int func_800162AC(u16 id, SampleRecord *records)
{
    u32 index;
    void *descriptor;
    for (index = 0; index < (u32)D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    while (records->identifier != 0xFFFF) {
        if (records->identifier == id) {
            if (records->references == 0) {
                records->data = D_800385A8[index].base + records->offset;
                descriptor = records->descriptor;
                func_80014CFC(&descriptor, &records->data);
            }
            ++records->references;
            break;
        }
        ++records;
    }
    return 1;
}

/* PROMOTED 2026-10-05 — func_800163A8
 * Source:   cloud/work/boot_tail_promotion/sources/func_800163A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_800163A8.c:func_800163A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
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

/* PROMOTED 2026-10-04 — func_800164D0
 * Source:   cloud/matches/boot_tail/func_800164D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800164D0.c:func_800164D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct { unsigned char unknown_00[9]; unsigned char state; unsigned char unknown_0A[2]; } Descriptor;
#pragma pack(1)
typedef struct { u16 id; u16 count; Descriptor *descriptors; } RecordFields;
#pragma pack()
typedef union { RecordFields fields; unsigned int words[2]; } RecordStorage;
extern int D_80042228;
extern RecordStorage D_80042230[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_800164D0(u16 id, Descriptor *descriptors, u16 count)
{
    int i;
    for (i = 0; i < D_80042228 && ((RecordFields *)&D_80042230[i])->id != id; i++) {
    }
    if (i == D_80042228 && D_80042228 < 128) {
        func_80014594();
        ((RecordFields *)&D_80042230[D_80042228])->id = id;
        ((RecordFields *)&D_80042230[D_80042228])->count = count;
        ((RecordFields *)&D_80042230[D_80042228])->descriptors = descriptors;
        for (i = 0; i < count; i++) {
            descriptors->state = 31;
            descriptors++;
        }
        D_80042228++;
        func_800145DC();
        return 1;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_8001661C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_8001671C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016998.s")
/* PROMOTED 2026-10-04 — func_80016BF8
 * Source:   cloud/matches/boot_tail/func_80016BF8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80016BF8.c:func_80016BF8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct PackedKey { unsigned char unknown00[4]; unsigned short key; } PackedKey;
#pragma pack()
int func_80016BF8(const PackedKey *left, const PackedKey *right)
{
    return left->key - right->key;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016C20.s")
/* PROMOTED 2026-10-04 — func_80016CE0
 * Source:   cloud/matches/boot_tail/func_80016CE0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80016CE0.c:func_80016CE0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_80016CE0(const unsigned short *left, const unsigned short *right)
{
    return *left - *right;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016CF0.s")
/* PROMOTED 2026-10-04 — func_80016E40
 * Source:   cloud/matches/boot_tail/func_80016E40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80016E40.c:func_80016E40 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
int func_80016E40(const PackedKey *left, const PackedKey *right)
{
    return left->key - right->key;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016E68.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016EE0.s")
/* PROMOTED 2026-10-04 — func_80016F58
 * Source:   cloud/matches/boot_tail/func_80016F58.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80016F58.c:func_80016F58 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
int func_80016F58(const PackedKey *left, const PackedKey *right)
{
    return left->key - right->key;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80016F80.s")
/* PROMOTED 2026-10-04 — func_80017018
 * Source:   cloud/matches/boot_tail/func_80017018.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017018.c:func_80017018 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct PackedLeadingKey { unsigned short key; } PackedLeadingKey;
#pragma pack()
int func_80017018(const PackedLeadingKey *left, const PackedLeadingKey *right)
{
    return left->key - right->key;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80017040.s")
/* PROMOTED 2026-10-05 — func_80017108
 * Source:   cloud/matches/boot_tail/func_80017108.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017108.c:func_80017108 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
extern int D_80038608;
extern int D_8003C610;
extern int D_8003CE18;
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern void func_80014CF4(void);
void func_80017108(void)
{
    int i;
    D_800385A0 = 0;
    D_80038608 = 0;
    D_8003C610 = 0;
    D_8003CE18 = 0;
    D_80042228 = 0;
    D_8003DA20 = 0;
    for (i = 0; i < 512; ++i) {
        D_8003DA28[i].count = 0;
        D_8003DA28[i].first = 0;
    }
    func_80014CF4();
}

