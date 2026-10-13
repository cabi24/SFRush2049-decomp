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
extern short D_80038390;
void func_80014E10(void)
{
    D_80038390 = 0;
}

/* PROMOTED 2026-10-11 — func_80014E1C
 * Source:   cloud/work/frontier/w18c/sources/func_80014E1C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014E1C.c:func_80014E1C (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct RelativeEntry { unsigned int next_offset; unsigned short id; } RelativeEntry;
void *func_80014E1C(unsigned short id, RelativeEntry *entry)
{
    while (entry->next_offset != 0xFFFFFFFFU) {
        if (entry->id == id) {
            return entry;
        }
        entry = (RelativeEntry *)((unsigned char *)entry + entry->next_offset);
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80014E64
 * Source:   cloud/work/frontier/w18c/sources/func_80014E64.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014E64.c:func_80014E64 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
struct ResourceOffsets;
struct RelativeEntry;
extern void *func_80014E1C(unsigned short, struct RelativeEntry *);
void *func_80014E64(unsigned short id, struct ResourceOffsets *resource)
{
    unsigned char *p = (unsigned char *)resource;
    p += ((unsigned int *)resource)[0];
    return func_80014E1C(id, (struct RelativeEntry *)p);
}

/* PROMOTED 2026-10-11 — func_80014E90
 * Source:   cloud/work/frontier/w18c/sources/func_80014E90.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014E90.c:func_80014E90 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *func_80014E90(unsigned short id, struct ResourceOffsets *resource)
{
    unsigned char *p = (unsigned char *)resource;
    p += ((unsigned int *)resource)[1];
    return func_80014E1C(id, (struct RelativeEntry *)p);
}

/* PROMOTED 2026-10-11 — func_80014EBC
 * Source:   cloud/work/frontier/w18c/sources/func_80014EBC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014EBC.c:func_80014EBC (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *func_80014EBC(unsigned short id, struct ResourceOffsets *resource)
{
    unsigned char *p = (unsigned char *)resource;
    p += ((unsigned int *)resource)[2];
    return func_80014E1C(id, (struct RelativeEntry *)p);
}

/* PROMOTED 2026-10-11 — func_80014EE8
 * Source:   cloud/work/frontier/w18c/sources/func_80014EE8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014EE8.c:func_80014EE8 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *func_80014EE8(unsigned short id, struct ResourceOffsets *resource)
{
    unsigned char *p = (unsigned char *)resource;
    p += ((unsigned int *)resource)[3];
    return func_80014E1C(id, (struct RelativeEntry *)p);
}

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

/* PROMOTED 2026-10-11 — func_80015318
 * Source:   cloud/work/frontier/w18c/sources/func_80015318.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80015318.c:func_80015318 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_800164D0(unsigned short, void *, unsigned short);
void func_80015318(unsigned short id, unsigned short *data)
{
    func_800164D0(id, data + 2, data[0]);
}

/* PROMOTED 2026-10-11 — func_80015348
 * Source:   cloud/work/frontier/w17g/sources/func_80015348.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80015348.c:func_80015348 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001661C(unsigned short);
void func_80015348(unsigned short id)
{
    func_8001661C(id);
}

/* PROMOTED 2026-10-11 — func_8001536C
 * Source:   cloud/work/frontier/w20b/sources/func_8001536C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_8001536C.c:func_8001536C (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct ResourceGroup { int size; u16 id; u16 type; u32 offsets[6]; } ResourceGroup;
extern u8 D_8002C630;
extern ResourceGroup *D_80038398[];
extern void func_80014F14(u16 *ids, ResourceOffsets *resources);
extern void func_80015228(u16 *ids, void *address, SampleRecord *records);
extern void func_80014F80(u16 *ids, ResourceOffsets *resources);
extern void func_80014FEC(u16 *ids, ResourceOffsets *resources);
extern void func_80015058(u16 *ids, ResourceOffsets *resources);
extern void func_80015318(unsigned short id, unsigned short *data);
int func_8001536C(u8 *groups, u16 id, void *address, SampleRecord *records, ResourceOffsets *resources)
{
    u8 *group;
    if (D_8002C630 != 0 && D_80038390 < 128) {
        group = groups;
        while (*(u32 *)group != 0xFFFFFFFF) {
            if (((ResourceGroup *)group)->id == id) {
                D_80038398[D_80038390] = (ResourceGroup *)group;
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[0]; p += 8; func_80014F14((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[1]; p += 8; func_80015228((u16 *)p, address, records); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[2]; p += 8; func_80014F80((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[3]; p += 8; func_80014FEC((u16 *)p, resources); }
                { u8 *p = group; p += ((ResourceGroup *)group)->offsets[4]; p += 8; func_80015058((u16 *)p, resources); }
                if (((ResourceGroup *)group)->type == 1) {
                    u8 *p = group; p += ((ResourceGroup *)group)->offsets[5]; p += 8; func_80015318(id, (u16 *)p);
                }
                D_80038390++;
                return 1;
            }
            group += ((ResourceGroup *)group)->size;
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_800154A4
 * Source:   cloud/work/frontier/w20b/sources/func_800154A4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_800154A4.c:func_800154A4 (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800150C8(u16 *ids);
extern void func_800152A8(u16 *ids);
extern void func_80015120(u16 *ids);
extern void func_80015178(u16 *ids);
extern void func_800151D0(u16 *ids);
extern void func_80015348(unsigned short id);
int func_800154A4(void)
{
    ResourceGroup *group;
    if (D_8002C630 != 0 && D_80038390 != 0) {
        group = D_80038398[--D_80038390];
        { u8 *p = (u8 *)group; p += group->offsets[0]; p += 8; func_800150C8((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[1]; p += 8; func_800152A8((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[2]; p += 8; func_80015120((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[3]; p += 8; func_80015178((u16 *)p); }
        { u8 *p = (u8 *)group; p += group->offsets[4]; p += 8; func_800151D0((u16 *)p); }
        if (group->type == 1) {
            func_80015348(group->id);
        }
        return 1;
    }
    return 0;
}

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

/* PROMOTED 2026-10-12 — func_8001558C
 * Source:   cloud/matches/boot_tail/func_8001558C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001558C.c:func_8001558C (score0); rom_tu.h context: cloud/work/frontier/w28p/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct { u32 metadata; u16 id; u16 type; u32 header[5]; u32 bank_a; u32 bank_b; u32 programs; } ResourceHeader;
#pragma pack(1)
typedef struct { u16 id; u8 data[130]; } Program;
#pragma pack()
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct Page_800178B0 Page_800178B0;
typedef struct MidiSetup_800178B0 MidiSetup_800178B0;
typedef struct Arr_800178B0 Arr_800178B0;
typedef struct PlayPara_800178B0 PlayPara_800178B0;
extern u32 func_800178B0(Page_800178B0 *, Page_800178B0 *, MidiSetup_800178B0 *, Arr_800178B0 *, PlayPara_800178B0 *);
int func_8001558C(u16 id, u16 program_id, u32 stream, u32 options, u8 unlocked)
{
    int i;
    ResourceHeader *resource;
    void *bank_a;
    void *bank_b;
    Program *program;
    int result;
    if (D_8002C630) {
        i = 0;
        result = id;
        for (; i < D_80038390; i++) {
            resource = (ResourceHeader *)D_80038398[i];
            if (resource->id == id) {
                if (resource->type == 0) {
                    bank_a = (u8 *)resource + resource->bank_a + 8;
                    bank_b = (u8 *)resource + resource->bank_b + 8;
                    program = (Program *)((u8 *)resource + resource->programs + 8);
                    for (; program->id != 0xffff; program++) {
                        if (program->id == program_id) {
                            if (unlocked) {
                                result = func_800178B0(bank_a, bank_b, (MidiSetup_800178B0 *)program, (Arr_800178B0 *)stream, (PlayPara_800178B0 *)options);
                            }
                            else {
                                func_80014594();
                                result = func_800178B0(bank_a, bank_b, (MidiSetup_800178B0 *)program, (Arr_800178B0 *)stream, (PlayPara_800178B0 *)options);
                                func_800145DC();
                            }
                            return result;
                        }
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

/* PROMOTED 2026-10-11 — func_800156E8
 * Source:   cloud/work/frontier/w17g/sources/func_800156E8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_800156E8.c:func_800156E8 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001558C(unsigned short, unsigned short, unsigned int, unsigned int, unsigned char);
int func_800156E8(unsigned short group, unsigned short item, unsigned int context, unsigned int option)
{
    return func_8001558C(group, item, context, option, 0);
}

