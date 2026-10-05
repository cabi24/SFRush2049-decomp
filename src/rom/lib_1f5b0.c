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
/* The used/free sequence lists share a 16-byte doubly linked node.
 * Voice records have a 0x1A0 stride; parent, node and reset word occupy
 * previously opaque fields at +0x14, +0x18 and +0x28 respectively.
 * Keep these declarations before any future early list-helper promotion. */
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[32]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
#pragma pack(0)
extern SequenceNode *D_80050C50;
extern VoiceState D_8004BEB8[];
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

/* PROMOTED 2026-10-05 — func_8001EB10
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001EB10.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001EB10.c:func_8001EB10 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern SequenceNode *D_80050C50, *D_80050C54;
extern void func_80021844(VoiceState *);
void func_8001EB10(VoiceState *state)
{
    func_80021844(state);
    if (state->identifier60 != 0xFFFFFFFFU) {
        if (state->parent_identifier != 0xFFFFFFFFU) {
            D_8004BEB8[(state->parent_identifier & 255)].next_identifier=state->next_identifier;
            if (state->next_identifier != 0xFFFFFFFFU) {
                D_8004BEB8[(state->next_identifier & 255)].parent_identifier=state->parent_identifier;
            }
        } else if (state->next_identifier != 0xFFFFFFFFU) {
            state->entry18->value=state->next_identifier;
            D_8004BEB8[(state->next_identifier & 255)].parent_identifier=0xFFFFFFFFU;
            D_8004BEB8[(state->next_identifier & 255)].entry18=state->entry18;
        } else {
            if (state->entry18->previous != 0) {
                state->entry18->previous->next=state->entry18->next;
            } else {
                D_80050C50=state->entry18->next;
            }
            if (state->entry18->next != 0) {
                state->entry18->next->previous=state->entry18->previous;
            }
            state->entry18->next=D_80050C54;
            if (D_80050C54 != 0) D_80050C54->previous=state->entry18;
            state->entry18->previous=0;
            D_80050C54=state->entry18;
        }
    }
}

/* PROMOTED 2026-10-05 — func_8001ECE0
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001ECE0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001ECE0.c:func_8001ECE0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern u32 func_8001EAEC(void);
u32 func_8001ECE0(VoiceState *state)
{
    SequenceNode *current;
    SequenceNode *previous;
    SequenceNode *node;
    u32 key;
    key = func_8001EAEC();
    previous = 0;
    for (current = D_80050C50; current != 0; current = current->next) {
        if (key < current->key) break;
        if (key == current->key) key = func_8001EAEC();
        previous = current;
    }
    node = D_80050C54;
    if (node == 0) return 0xFFFFFFFFU;
    if ((D_80050C54 = D_80050C54->next) != 0)
        D_80050C54->previous = 0;
    if (previous == 0) D_80050C50 = node;
    else previous->next = node;
    node->previous = previous;
    node->next = current;
    if (current != 0) current->previous = node;
    node->key = key;
    node->value = state->identifier60;
    state->entry18 = node;
    return key;
}

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
/* PROMOTED 2026-10-04 — func_8001EE9C
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001EE9C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001EE9C.c:func_8001EE9C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoicePrefix_8001EE9C { u8 unknown00[46]; u8 channel2E; u8 unknown2F[49]; u32 identifier60; } VoicePrefix_8001EE9C;
#pragma pack(0)
typedef struct ChannelLink_8001EE9C { u8 previous, next; u16 active; } ChannelLink_8001EE9C;
typedef struct GroupLink_8001EE9C { u16 next, previous; } GroupLink_8001EE9C;
extern ChannelLink_8001EE9C D_800504C8[32];
extern u8 D_80050548[256];
extern GroupLink_8001EE9C D_80050648[256];
extern u16 D_80050A48;
void func_8001EE9C(VoicePrefix_8001EE9C *state)
{
    ChannelLink_8001EE9C *link;
    GroupLink_8001EE9C *group;
    link = &D_800504C8[state->identifier60 & 255];
    if (link->active == 1) {
        if (link->previous != 255) D_800504C8[link->previous].next = link->next;
        else D_80050548[state->channel2E] = link->next;
        if (link->next != 255) D_800504C8[link->next].previous = link->previous;
        else if (link->previous == 255) {
            group = &D_80050648[state->channel2E];
            if (group->previous != 65535) D_80050648[group->previous].next = group->next;
            else D_80050A48 = group->next;
            if (group->next != 65535) D_80050648[group->next].previous = group->previous;
        }
        link->active = 0;
    }
}

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

/* PROMOTED 2026-10-04 — func_8001F898
 * Source:   cloud/matches/boot_tail/func_8001F898.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001F898.c:func_8001F898 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern int func_8001F13C(u8, u8, u16, u8);
extern void func_8001EB10(VoiceState *);
extern u8 func_8001467C(int);
extern void func_80014AF0(int);
extern void func_8001EF8C(VoiceState *, u8);
int func_8001F898(u8 channel)
{
    int index;
    VoiceState *state;
    index = func_8001F13C(channel, 255, 65535, 1);
    if (index != -1) {
        state = &D_8004BEB8[index];
        state->activeBD = 1;
        state->external4C = 1;
        func_8001EB10(state);
        state->identifier60 = (u32)index | 0xFFFFFF00U;
        if (func_8001467C(index)) func_80014AF0(index);
        state->command00 = 0;
        func_8001EF8C(state, channel);
    }
    return index;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s")
/* PROMOTED 2026-10-05 — func_8001F9D0
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001F9D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001F9D0.c:func_8001F9D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern void func_8001F6EC(VoiceState *);
void func_8001F9D0(VoiceState *state)
{
    func_8001EB10(state);
    state->flags24 &= ~3U;
    state->value28 = 0;
    func_8001F6EC(state);
}

/* PROMOTED 2026-10-04 — func_8001FA18
 * Source:   cloud/matches/boot_tail/func_8001FA18.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FA18.c:func_8001FA18 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern u8 D_8002C630;
extern int func_8001EDF4(u32);
extern void func_8001F9D0(VoiceState *);
extern void func_80014B3C(int);
int func_8001FA18(u32 key)
{
    int result;
    u32 index;
    u32 next;
    result = -1;
    if (D_8002C630) {
        key = func_8001EDF4(key);
        while (key != 0xFFFFFFFFU) {
            index = key & 255;
            next = D_8004BEB8[index].next_identifier;
            if (key == D_8004BEB8[index].identifier60) {
                result = 0;
                if (D_8004BEB8[index].command00 != 0) {
                    func_8001F9D0(&D_8004BEB8[index]);
                }
                func_80014B3C(index);
            }
            key = next;
        }
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FAE4
 * Source:   cloud/matches/boot_tail/func_8001FAE4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FAE4.c:func_8001FAE4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
void func_8001FAE4(u8 preserve_external)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004BEB8[i].command00 != 0) {
            if (!preserve_external ||
                (preserve_external && !D_8004BEB8[i].external4C)) {
                func_8001F9D0(&D_8004BEB8[i]);
                D_8004BEB8[i].command00 = 0;
                D_8004BEB8[i].flags24 &= ~3U;
            } else {
                continue;
            }
        }
        func_80014B3C(i);
    }
}

