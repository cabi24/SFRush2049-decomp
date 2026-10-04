/* GENERATED ROM-aligned TU — segment 0x15a10 (rom/lib_15a10)
 * layout map 6642e6a192ea8a14e7ca410824561e61c14c8896ea423af8459b04cbb4b1a59d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80014E10
 * Source:   cloud/matches/boot_tail/func_80014E10.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014E10.c:func_80014E10 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short D_80038390;
void func_80014E10(void)
{
    D_80038390 = 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80014E1C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80014E64.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80014E90.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80014EBC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80014EE8.s")
/* PROMOTED 2026-10-04 — func_80014F14
 * Source:   cloud/matches/boot_tail/func_80014F14.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014F14.c:func_80014F14 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct ResourceOffsets { unsigned int offsets[4]; } ResourceOffsets;
extern void *func_80014E64(u16, ResourceOffsets *);
extern int func_8001671C(u16, void *);
void func_80014F14(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    unsigned char *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014E64(id, resources);
        if (entry != 0) {
            func_8001671C(*ids, entry + 8);
        }
        ++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80014F80
 * Source:   cloud/matches/boot_tail/func_80014F80.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014F80.c:func_80014F80 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *func_80014E90(u16, ResourceOffsets *);
extern int func_80015D68(u16, void *);
void func_80014F80(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    unsigned char *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014E90(id, resources);
        if (entry != 0) {
            func_80015D68(*ids, entry + 8);
        }
        ++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80014FEC
 * Source:   cloud/matches/boot_tail/func_80014FEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014FEC.c:func_80014FEC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *func_80014EBC(u16, ResourceOffsets *);
extern int func_80015720(u16, void *);
void func_80014FEC(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    unsigned char *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014EBC(id, resources);
        if (entry != 0) {
            func_80015720(*ids, entry + 8);
        }
        ++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80015058
 * Source:   cloud/matches/boot_tail/func_80015058.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80015058.c:func_80015058 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct ResourceEntry { unsigned int next_offset; u16 id; u16 unknown06; u16 unknown08; u16 parameter; } ResourceEntry;
extern void *func_80014EE8(u16, ResourceOffsets *);
extern int func_80015A0C(u16, void *, u16);
void func_80015058(u16 *ids, ResourceOffsets *resources)
{
    u16 id;
    ResourceEntry *entry;
    while ((id = *ids) != 0xFFFF) {
        entry = func_80014EE8(id, resources);
        if (entry != 0) {
            func_80015A0C(*ids, (unsigned char *)entry + 12, entry->parameter);
        }
        ++ids;
    }
}

/* PROMOTED 2026-10-04 — func_800150C8
 * Source:   cloud/matches/boot_tail/func_800150C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800150C8.c:func_800150C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_80016998(u16);
void func_800150C8(u16 *ids)
{
    u16 id;
    id = *ids;
    while (id != 0xFFFF) {
        func_80016998(id);
        id = *++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80015120
 * Source:   cloud/matches/boot_tail/func_80015120.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80015120.c:func_80015120 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_80015F28(u16);
void func_80015120(u16 *ids)
{
    u16 id;
    id = *ids;
    while (id != 0xFFFF) {
        func_80015F28(id);
        id = *++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80015178
 * Source:   cloud/matches/boot_tail/func_80015178.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80015178.c:func_80015178 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_800158D8(u16);
void func_80015178(u16 *ids)
{
    u16 id;
    id = *ids;
    while (id != 0xFFFF) {
        func_800158D8(id);
        id = *++ids;
    }
}

/* PROMOTED 2026-10-04 — func_800151D0
 * Source:   cloud/matches/boot_tail/func_800151D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800151D0.c:func_800151D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_80015C0C(u16);
void func_800151D0(u16 *ids)
{
    u16 id;
    id = *ids;
    while (id != 0xFFFF) {
        func_80015C0C(id);
        id = *++ids;
    }
}

/* PROMOTED 2026-10-04 — func_80015228
 * Source:   cloud/matches/boot_tail/func_80015228.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80015228.c:func_80015228 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SampleRecord { u16 identifier; u16 references; u32 offset; void *data; u8 descriptor[16]; } SampleRecord;
extern void *func_80014CAC(void *);
extern int func_8001605C(SampleRecord *, void *);
extern int func_800162AC(u16, SampleRecord *);
void func_80015228(u16 *ids, void *address, SampleRecord *records)
{
    u16 id;
    if (func_8001605C(records, func_80014CAC(address))) {
        while ((id = *ids) != 0xFFFF) {
            func_800162AC(*ids++, records);
        }
    }
}

/* PROMOTED 2026-10-04 — func_800152A8
 * Source:   cloud/matches/boot_tail/func_800152A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800152A8.c:func_800152A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_800163A8(u16);
void func_800152A8(u16 *ids)
{
    u16 *end;
    end = ids;
    while (*end != 0xFFFF) {
        ++end;
    }
    --end;
    while (end >= ids) {
        func_800163A8(*end);
        --end;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80015318.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_80015348.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_8001536C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_800154A4.s")
/* PROMOTED 2026-10-04 — func_80015574
 * Source:   cloud/matches/boot_tail/func_80015574.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80015574.c:func_80015574 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_80015574(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_8001558C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_15a10/func_800156E8.s")
