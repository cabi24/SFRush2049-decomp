/* GENERATED ROM-aligned TU — segment 0x1f5b0 (rom/lib_1f5b0)
 * layout map ae263abb1203148d19b634cda2c10e4ccdd9bfaa8069507b2028db9a263d7388; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001E9B0.s")
/* PROMOTED 2026-10-04 — func_8001EAA0
 * Source:   cloud/matches/boot_tail/func_8001EAA0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001EAA0.c:func_8001EAA0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; u32 unknown04; u32 key; int value; } SequenceNode;
#pragma pack(0)
extern SequenceNode *D_80050C50;
SequenceNode *func_8001EAA0(u32 key)
{
    SequenceNode *node;
    node = D_80050C50;
    while (node != 0) {
        if (key == node->key) return node;
        if (key < node->key) break;
        node = node->next;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_8001EAEC
 * Source:   cloud/matches/boot_tail/func_8001EAEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001EAEC.c:func_8001EAEC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_80050A4C;
unsigned int func_8001EAEC(void)
{
    unsigned int value;
    do { value = D_80050A4C++; } while (value == 0xFFFFFFFF);
    return value;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001EB10.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001ECE0.s")
/* PROMOTED 2026-10-04 — func_8001EDF4
 * Source:   cloud/matches/boot_tail/func_8001EDF4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001EDF4.c:func_8001EDF4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceNode *func_8001EAA0(u32 key);
int func_8001EDF4(u32 key)
{
    SequenceNode *node;
    if (key != 0xFFFFFFFF) {
        node = func_8001EAA0(key);
        if (node != 0) return node->value;
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001EE34.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001EE9C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001EF8C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F13C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F6EC.s")
/* PROMOTED 2026-10-04 — func_8001F7EC
 * Source:   cloud/matches/boot_tail/func_8001F7EC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001F7EC.c:func_8001F7EC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;
extern u8 D_8004FA18;
extern ChannelLink D_80050440[32];
extern u8 D_800504C0;
extern u8 D_800504C1;
void func_8001F7EC(void)
{
    u32 i;
    for (i = 0; i < D_8004FA18; i++) {
        D_80050440[i].previous = i - 1;
        D_80050440[i].next = i + 1;
        D_80050440[i].flags = 1;
    }
    D_80050440[0].previous = 255;
    D_80050440[D_8004FA18 - 1].next = 255;
    D_800504C0 = 0;
    D_800504C1 = D_8004FA18 - 1;
}

/* PROMOTED 2026-10-04 — func_8001F864
 * Source:   cloud/matches/boot_tail/func_8001F864.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001F864.c:func_8001F864 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001F7EC(void);
extern void func_8001EE34(void);
extern unsigned char D_800504C2, D_800504C3;
void func_8001F864(void)
{
    func_8001F7EC();
    func_8001EE34();
    D_800504C2 = 0;
    D_800504C3 = 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F898.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F9D0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001FA18.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001FAE4.s")
