/* GENERATED ROM-aligned TU — segment 0x17dc0 (rom/lib_17dc0)
 * layout map 8baa7d85ab5236a98d6d20c261b8e0ad26e5b810082eeb29bf486ec2aa49ae83; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"
#include "sequence_context.h"

/* PROMOTED 2026-10-05 — func_800171C0
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800171C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800171C0.c:func_800171C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceNode D_800426B0[256];
void func_800171C0(void)
{
    SequenceNode *previous;
    int i;
    previous = 0;
    D_80043EB0 = D_800426B0;
    for (i = 0; i < 256; i++) {
        D_800426B0[i].previous = previous;
        if (previous != 0) {
            previous->next = &D_800426B0[i];
        }
        previous = &D_800426B0[i];
    }
    previous->next = 0;
}

/* PROMOTED 2026-10-04 — func_8001729C
 * Source:   cloud/matches/boot_tail/func_8001729C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001729C.c:func_8001729C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceNode *D_80043EB0;
extern SequenceContext *D_8004BE80;
void func_8001729C(SequenceContext *state)
{
    SequenceNode *node;
    node = state->active;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->active;
        state->active = 0;
    }
    node = state->pending;
    if (node != 0) {
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->pending;
        state->pending = 0;
    }
}

/* PROMOTED 2026-10-04 — func_8001734C
 * Source:   cloud/matches/boot_tail/func_8001734C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001734C.c:func_8001734C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001FA18(u32);
void func_8001734C(SequenceContext *context)
{
    SequenceNode *entry;
    entry = context->active;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
    entry = context->pending;
    while (entry != 0) {
        func_8001FA18(entry->identifier);
        entry = entry->next;
    }
}

/* PROMOTED 2026-10-04 — func_800173B4
 * Source:   cloud/matches/boot_tail/func_800173B4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800173B4.c:func_800173B4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
SequenceNode *func_800173B4(void)
{
    SequenceNode *node;
    SequenceNode *head;
    node = D_80043EB0;
    if (node != 0) {
        D_80043EB0 = node->next;
        if (D_80043EB0 != 0) {
            D_80043EB0->previous = 0;
        }
        node->previous = 0;
        head = D_8004BE80->active;
        node->next = head;
        if (head != 0) {
            D_8004BE80->active->previous = node;
        }
        D_8004BE80->active = node;
    }
    return node;
}

/* PROMOTED 2026-10-04 — func_80017410
 * Source:   cloud/matches/boot_tail/func_80017410.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017410.c:func_80017410 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017410(SequenceNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    node->next = D_80043EB0;
    if (node->next != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}

/* PROMOTED 2026-10-04 — func_80017470
 * Source:   cloud/matches/boot_tail/func_80017470.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017470.c:func_80017470 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017470(SequenceNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->pending = node->next;
    }
    node->next = D_80043EB0;
    if (node->next != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}

/* PROMOTED 2026-10-04 — func_800174D0
 * Source:   cloud/matches/boot_tail/func_800174D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800174D0.c:func_800174D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800174D0(SequenceNode *node)
{
    SequenceNode *head;
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->active = node->next;
    }
    head = D_8004BE80->pending;
    node->next = head;
    if (head != 0) {
        D_8004BE80->pending->previous = node;
    }
    node->previous = 0;
    D_8004BE80->pending = node;
}

/* PROMOTED 2026-10-05 — func_80017540
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80017540.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80017540.c:func_80017540 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80017540(void)
{
    SequenceNode *entry;
    SequenceNode *next;
    entry = D_8004BE80->pending;
    while (entry != 0) {
        next = entry->next;
        if (func_800201D0(entry->identifier) == 0xFFFFFFFFU) func_80017470(entry);
        entry = next;
    }
}

/* PROMOTED 2026-10-04 — func_800175A8
 * Source:   cloud/matches/boot_tail/func_800175A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800175A8.c:func_800175A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8004BE84;
void func_800175A8(void)
{
    D_8004BE84 = 0;
}

/* PROMOTED 2026-10-05 — func_800175B4
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800175B4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800175B4.c:func_800175B4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned int func_800175B4(unsigned int slot)
{
    unsigned int identifier;
    int i;
    do {
        identifier = D_8004BE84++;
        D_8004BE84 &= 0x7FFFFFFFU;
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].inactiveFC1 == 0 &&
                D_80043EB8[i].identifier == identifier) {
                identifier = 0xFFFFFFFFU;
                break;
            }
        }
    } while (identifier == 0xFFFFFFFFU);
    D_80043EB8[slot].identifier = identifier;
    return identifier;
}

/* PROMOTED 2026-10-04 — func_80017644
 * Source:   cloud/matches/boot_tail/func_80017644.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80017644.c:func_80017644 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern SequenceContext D_80043EB8[8];
u32 func_80017644(u32 identifier)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (D_80043EB8[i].inactiveFC1 == 0 &&
            D_80043EB8[i].identifier == (identifier & 0x7FFFFFFFU)) {
            return (identifier & 0x80000000U) | i;
        }
    }
    return 0xFFFFFFFFU;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80017720.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_800177EC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80017824.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_8001785C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_800178B0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80017D38.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_800180A0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018184.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_8001824C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018448.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018634.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_8001897C.s")
/* PROMOTED 2026-10-05 — func_80018A30
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018A30.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018A30.c:func_80018A30 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018A30(void)
{
    if (D_8004BE80->timingStartF68) {
        while (D_8004BE80->currentF6C->time != 0xFFFFFFFFU) {
            if (D_8004BE80->highF74 + D_8004BE80->half120 <
                D_8004BE80->currentF6C->time) break;
            func_80019A60(D_8004BE80->rate124 =
                D_8004BE80->currentF6C->value, D_8004BE78);
            D_8004BE80->currentF6C++;
        }
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018AEC.s")
/* PROMOTED 2026-10-05 — func_80018B3C
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018B3C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018B3C.c:func_80018B3C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018B3C(unsigned int value)
{
    if (D_8004BE80->timingStartF68) {
        D_8004BE80->currentF6C = D_8004BE80->timingStartF68;
        D_8004BE80->highF74 = value;
        D_8004BE80->lowF70 = 0;
        func_80018A30();
        func_8001897C();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018B8C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018BEC.s")
/* PROMOTED 2026-10-05 — func_80018C2C
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018C2C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018C2C.c:func_80018C2C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018C2C(unsigned int identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].flagsFEE |= 8;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018D00
 * Source:   cloud/matches/boot_tail/func_80018D00.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018D00.c:func_80018D00 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80018C2C(unsigned int);
void func_80018D00(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018C2C(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018D40
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018D40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80018D40.c:func_80018D40 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018D40(unsigned int identifier)
{
    identifier = func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if ((identifier & 0x80000000U) == 0) {
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].activeFC0 = 0;
                func_8001734C(&D_80043EB8[identifier]);
                func_8001729C(&D_80043EB8[identifier]);
            }
            D_80043EB8[identifier].inactiveFC1 = 1;
        } else {
            identifier &= 0x7FFFFFFFU;
            if (D_80043EB8[identifier].activeFC0 && !D_80043EB8[identifier].inactiveFC1) {
                D_80043EB8[identifier].pendingFF0 = 0;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018E2C
 * Source:   cloud/matches/boot_tail/func_80018E2C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018E2C.c:func_80018E2C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018D40(unsigned int);
void func_80018E2C(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018D40(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018E6C
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018E6C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018E6C.c:func_80018E6C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018E6C(void)
{
    int i;
    for (i = 0; i < 8; i++) {
        func_80018D40(D_80043EB8[i].identifier);
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80018EB4.s")
/* PROMOTED 2026-10-05 — func_80018F20
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018F20.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018F20.c:func_80018F20 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018F20(unsigned int identifier, unsigned short value)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].valueFC2 = value;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].valueFEC = value;
            D_80043EB8[index].flagsFEE |= 0x20;
        }
    }
}

/* PROMOTED 2026-10-04 — func_80018FA4
 * Source:   cloud/matches/boot_tail/func_80018FA4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80018FA4.c:func_80018FA4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018F20(unsigned int, unsigned short);
void func_80018FA4(unsigned int identifier, unsigned short value)
{
    if (D_8002C630) {
        func_80014594();
        func_80018F20(identifier, value);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80018FEC
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80018FEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80018FEC.c:func_80018FEC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80018FEC(unsigned int identifier)
{
    unsigned int index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].activeFC0 = 1;
        } else {
            D_80043EB8[index & 0x7FFFFFFFU].flagsFEE &= ~8;
        }
    }
}

/* PROMOTED 2026-10-04 — func_8001906C
 * Source:   cloud/matches/boot_tail/func_8001906C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001906C.c:func_8001906C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018FEC(unsigned int);
void func_8001906C(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018FEC(identifier);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_800190AC
 * Source:   cloud/matches/boot_tail/func_800190AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800190AC.c:func_800190AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u32 func_80017644(u32);
void func_800190AC(u32 identifier, u32 first, u32 second)
{
    u32 index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].first110 = first;
            D_80043EB8[index].second114 = second;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].firstFE4 = first;
            D_80043EB8[index].secondFE8 = second;
            D_80043EB8[index].flagsFEE |= 0x10;
        }
    }
}

/* PROMOTED 2026-10-04 — func_80019144
 * Source:   cloud/matches/boot_tail/func_80019144.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019144.c:func_80019144 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800190AC(unsigned int, unsigned int, unsigned int);
void func_80019144(unsigned int identifier, unsigned int first, unsigned int second)
{
    if (D_8002C630) {
        func_80014594();
        func_800190AC(identifier, first, second);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_80019194
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80019194.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_80019194.c:func_80019194 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80019194(unsigned char channel,unsigned short duration,unsigned int identifier,unsigned char flags)
{
    unsigned int original;
    int i;
    original=identifier;
    identifier=func_80017644(identifier);
    if (identifier != 0xFFFFFFFFU) {
        if (!(identifier & 0x80000000U)) {
            func_8001B9F8(channel,duration,D_80043EB8[identifier].channelFC4,flags,original);
            for (i=0;i<64;i++) {
                if (D_80043EB8[identifier].channels528[i] != D_80043EB8[identifier].channelFC4) {
                    func_8001B9F8(channel,duration,D_80043EB8[identifier].channels528[i],0,0);
                }
            }
        } else {
            identifier &= 0x7FFFFFFFU;
            switch (flags & 15) {
            case 0: D_80043EB8[identifier].valueFE0=channel; break;
            case 1: D_80043EB8[identifier].pendingFF0=0; break;
            case 2:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 8;
                break;
            case 3:
                D_80043EB8[identifier].valueFE0=channel;
                D_80043EB8[identifier].flagsFEE |= 128;
                break;
            }
        }
    }
}

/* PROMOTED 2026-10-04 — func_80019370
 * Source:   cloud/matches/boot_tail/func_80019370.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019370.c:func_80019370 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80019194(unsigned char, unsigned short, unsigned int, unsigned char);
void func_80019370(unsigned char value, unsigned short duration, unsigned int identifier, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_80019194(value, duration, identifier, mode);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_800193C8
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800193C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800193C8.c:func_800193C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_800193C8(unsigned int identifier)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) return (*(SequenceContext *)((unsigned int)D_80043EB8 +
                index * sizeof(SequenceContext))).channelFC4;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80019420.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_80019490.s")
/* PROMOTED 2026-10-04 — func_80019850
 * Source:   cloud/matches/boot_tail/func_80019850.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019850.c:func_80019850 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80019490(void *, unsigned int *, unsigned char);
void func_80019850(void *state, unsigned int *result)
{
    if (D_8002C630) {
        func_80014594();
        func_80019490(state, result, 0);
        func_800145DC();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/func_8001989C.s")
/* PROMOTED 2026-10-05 — func_800198C8
 * Source:   cloud/work/boot_tail_promotion/wave3_sequence_sources/func_800198C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave3_sequence_sources/func_800198C8.c:func_800198C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3_seq.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-05 — func_800199F4
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800199F4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800199F4.c:func_800199F4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800171C0(void);
extern void func_800175A8(void);
void func_800199F4(void)
{
    unsigned int index;
    for (index = 0; index < 8; index++) {
        D_80043EB8[index].activeFC0 = 0;
        D_80043EB8[index].inactiveFC1 = 1;
    }
    func_800171C0();
    func_800175A8();
}

