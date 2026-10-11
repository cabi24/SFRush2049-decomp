/* GENERATED ROM-aligned TU — segment 0x16320 (rom/lib_16320)
 * layout map 231669c219ff8d17283ad97903995c8f4ca7b9a1708f46d32621282f1fe514f5; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — func_80015720
 * Source:   cloud/work/frontier/w19b/relocname/func_80015720.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_80015720.c:func_80015720 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003C618;
#pragma pack()
typedef union { RecordFields_8003C618 fields; unsigned int words[2]; } RecordStorage_8003C618;
extern int D_8003C610;
extern RecordStorage_8003C618 D_8003C618[];
extern void func_80014594(void);
extern void func_800145DC(void);
int func_80015720(u16 id, void *payload)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && ((RecordFields_8003C618 *)&D_8003C618[i])->id < id; i++) {
    }
    if (i < D_8003C610) {
        if (((RecordFields_8003C618 *)&D_8003C618[i])->id != id) {
            if (D_8003C610 < 2048) {
                for (j = D_8003C610 - 1; j >= i; j--) {
                    D_8003C618[j + 1] = D_8003C618[j];
                }
                D_8003C610++;
            } else {
                func_800145DC();
                return 0;
            }
        } else {
            ((RecordFields_8003C618 *)&D_8003C618[i])->references++;
            func_800145DC();
            return 0;
        }
    } else {
        if (D_8003C610 < 2048) {
            D_8003C610++;
        } else {
            func_800145DC();
            return 0;
        }
    }
    ((RecordFields_8003C618 *)&D_8003C618[i])->payload = payload;
    ((RecordFields_8003C618 *)&D_8003C618[i])->id = id;
    ((RecordFields_8003C618 *)&D_8003C618[i])->references = 1;
    func_800145DC();
    return 1;
}

/* PROMOTED 2026-10-11 — func_800158D8
 * Source:   cloud/work/frontier/w19b/relocname/func_800158D8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_800158D8.c:func_800158D8 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
int func_800158D8(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003C610 && ((RecordFields_8003C618 *)&D_8003C618[i])->id != id; i++) {
    }
    if (i != D_8003C610) {
        if (--((RecordFields_8003C618 *)&D_8003C618[i])->references == 0) {
            for (j = i + 1; j < D_8003C610; j++) {
                D_8003C618[j - 1] = D_8003C618[j];
            }
            D_8003C610--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015A0C.s")
/* PROMOTED 2026-10-11 — func_80015C0C
 * Source:   cloud/work/frontier/w19b/relocname/func_80015C0C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_80015C0C.c:func_80015C0C (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct { unsigned int payload; u16 id; u16 type; u16 references; u16 metadata; } RecordFields_8003CE20;
#pragma pack()
extern int D_8003CE18;
typedef union { RecordFields_8003CE20 fields; unsigned int words[3]; } RecordStorage_8003CE20;
extern RecordStorage_8003CE20 D_8003CE20[];
int func_80015C0C(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_8003CE18 && ((RecordFields_8003CE20 *)&D_8003CE20[i])->id != id; i++) {
    }
    if (i != D_8003CE18) {
        if (--((RecordFields_8003CE20 *)&D_8003CE20[i])->references == 0) {
            for (j = i + 1; j < D_8003CE18; j++) {
                D_8003CE20[j - 1] = D_8003CE20[j];
            }
            D_8003CE18--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_80015D68.s")
/* PROMOTED 2026-10-11 — func_80015F28
 * Source:   cloud/work/frontier/w19b/relocname/func_80015F28.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_80015F28.c:func_80015F28 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct { unsigned int payload; u16 id; u16 references; } RecordFields_80038610;
#pragma pack()
extern int D_80038608;
typedef union { RecordFields_80038610 fields; unsigned int words[2]; } RecordStorage_80038610;
extern RecordStorage_80038610 D_80038610[];
int func_80015F28(u16 id)
{
    int i;
    int j;
    func_80014594();
    for (i = 0; i < D_80038608 && ((RecordFields_80038610 *)&D_80038610[i])->id != id; i++) {
    }
    if (i != D_80038608) {
        if (--((RecordFields_80038610 *)&D_80038610[i])->references == 0) {
            for (j = i + 1; j < D_80038608; j++) {
                D_80038610[j - 1] = D_80038610[j];
            }
            D_80038608--;
            func_800145DC();
            return 1;
        }
    }
    func_800145DC();
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_8001605C.s")
/* PROMOTED 2026-10-11 — func_800161A0
 * Source:   cloud/work/frontier/w19b/relocname/func_800161A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_800161A0.c:func_800161A0 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern int D_800385A0;
extern RegisteredSamples D_800385A8[];
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

/* PROMOTED 2026-10-11 — func_8001661C
 * Source:   cloud/work/frontier/w19b/relocname/func_8001661C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/relocname/func_8001661C.c:func_8001661C (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_relocname.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
int func_8001661C(u16 id)
{
    int i;
    int j;
    for (i = 0; i < D_80042228 && ((RecordFields *)&D_80042230[i])->id != id; i++) {
    }
    if (i != D_80042228) {
        func_80014594();
        for (j = i + 1; j < D_80042228; j++) {
            D_80042230[j - 1] = D_80042230[j];
        }
        D_80042228--;
        func_800145DC();
        return 1;
    }
    return 0;
}

/* Macro main table record, hoisted before its first user (w21b; was in the func_80017108 block). */
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
/* PROMOTED 2026-10-11 — func_8001671C
 * Source:   cloud/work/frontier/w21b/sources/func_8001671C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21b/sources/func_8001671C.c:func_8001671C (score0); rom_tu.h context: cloud/work/frontier/w21b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003E228;
#pragma pack()
typedef union { RecordFields_8003E228 fields; unsigned int words[2]; } RecordStorage_8003E228;
extern int D_8003DA20;
extern ResourceRange D_8003DA28[512];
extern RecordStorage_8003E228 D_8003E228[];
int func_8001671C(u16 id, void *payload)
{
    long main;
    long pos;
    long base;
    long i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count == 0) {
        pos = base = D_8003DA28[main].first = D_8003DA20;
    } else {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && ((RecordFields_8003E228 *)&D_8003E228[base + i])->id < id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            pos = base + i;
            if (((RecordFields_8003E228 *)&D_8003E228[pos])->id == id) {
                ((RecordFields_8003E228 *)&D_8003E228[pos])->references++;
                func_800145DC();
                return 0;
            }
        } else {
            pos = base + i;
        }
    }
    if (D_8003DA20 < 2048) {
        for (i = 0; i < 512; ++i) {
            if (D_8003DA28[i].first > base) {
                D_8003DA28[i].first++;
            }
        }
        i = D_8003DA20 - 1;
        for (; i >= pos; --i) {
            D_8003E228[i + 1] = D_8003E228[i];
        }
        ((RecordFields_8003E228 *)&D_8003E228[pos])->id = id;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->payload = payload;
        ((RecordFields_8003E228 *)&D_8003E228[pos])->references = 1;
        D_8003DA28[main].count++;
        D_8003DA20++;
        func_800145DC();
        return 1;
    }
    func_800145DC();
    return 0;
}

/* PROMOTED 2026-10-11 — func_80016998
 * Source:   cloud/work/frontier/w21b/sources/func_80016998.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21b/sources/func_80016998.c:func_80016998 (score0); rom_tu.h context: cloud/work/frontier/w21b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
#pragma pack(1)
#pragma pack()
int func_80016998(u16 id)
{
    long main;
    long base;
    long i;
    func_80014594();
    main = id >> 6;
    if (D_8003DA28[main].count != 0) {
        base = D_8003DA28[main].first;
        for (i = 0; i < D_8003DA28[main].count && ((RecordFields_8003E228 *)&D_8003E228[base + i])->id != id; ++i) {
        }
        if (i < D_8003DA28[main].count) {
            if (--((RecordFields_8003E228 *)&D_8003E228[base + i])->references == 0) {
                for (i = base + i + 1; i < D_8003DA20; ++i) {
                    D_8003E228[i - 1] = D_8003E228[i];
                }
                for (i = 0; i < 512; ++i) {
                    if (D_8003DA28[i].first > base) {
                        --D_8003DA28[i].first;
                    }
                }
                --D_8003DA28[main].count;
                --D_8003DA20;
            }
        }
    }
    func_800145DC();
    return 0;
}

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

/* PROMOTED 2026-10-11 — func_80016E68
 * Source:   cloud/work/frontier/w20b/ownbss/func_80016E68.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/ownbss/func_80016E68.c:func_80016E68 (score0); rom_tu.h context: cloud/work/frontier/w21d/ctx71_ownbss.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
#pragma pack(1)
#pragma pack()
int func_80016E40(const PackedKey *left, const PackedKey *right);
void *func_8001E864(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
void *func_80016E68(u16 id)
{
    static RecordFields_80038610 key_80038610;
    static RecordFields_80038610 *result_80038610;

    key_80038610.id = id;
    if ((result_80038610 = func_8001E864(&key_80038610, D_80038610, D_80038608, sizeof(RecordFields_80038610),
                                (int (*)(const void *, const void *))func_80016E40)) != 0) {
        return (void *)result_80038610->payload;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80016EE0
 * Source:   cloud/work/frontier/w20b/ownbss/func_80016EE0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/ownbss/func_80016EE0.c:func_80016EE0 (score0); rom_tu.h context: cloud/work/frontier/w21d/ctx71_ownbss.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
#pragma pack(1)
#pragma pack()
void *func_80016EE0(u16 id)
{
    static RecordFields_8003C618 key_8003C618;
    static RecordFields_8003C618 *result_8003C618;

    key_8003C618.id = id;
    if ((result_8003C618 = func_8001E864(&key_8003C618, D_8003C618, D_8003C610, sizeof(RecordFields_8003C618),
                                (int (*)(const void *, const void *))func_80016E40)) != 0) {
        return result_8003C618->payload;
    }
    return 0;
}

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

/* PROMOTED 2026-10-11 — func_80016F80
 * Source:   cloud/work/frontier/w20b/ownbss/func_80016F80.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/ownbss/func_80016F80.c:func_80016F80 (score0); rom_tu.h context: cloud/work/frontier/w21d/ctx71_ownbss.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
#pragma pack(1)
#pragma pack()
int func_80016F58(const PackedKey *left, const PackedKey *right);
void *func_80016F80(u16 id, u16 *type)
{
    static RecordFields_8003CE20 key_8003CE20;
    static RecordFields_8003CE20 *result_8003CE20;

    key_8003CE20.id = id;
    if ((result_8003CE20 = func_8001E864(&key_8003CE20, D_8003CE20, D_8003CE18, sizeof(RecordFields_8003CE20),
                                (int (*)(const void *, const void *))func_80016F58)) != 0) {
        *type = result_8003CE20->type;
        return (void *)result_8003CE20->payload;
    }
    return 0;
}

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

