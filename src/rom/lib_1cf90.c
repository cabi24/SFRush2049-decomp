/* GENERATED ROM-aligned TU — segment 0x1cf90 (rom/lib_1cf90)
 * layout map 49b03b3d75a02558e10ab39c8693394c31f03cc7f18aec15a71756604b34fc1e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C390.s")
/* PROMOTED 2026-10-04 — func_8001C3CC
 * Source:   cloud/matches/boot_tail/func_8001C3CC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001C3CC.c:func_8001C3CC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef int (*SampleCallback)(short *, u32, short *, u32, u32);
typedef struct SampleBuffer { u8 mode; u8 unknown01[3]; SampleCallback callback; short *buffer; u32 samples; u32 position; u32 context; } SampleBuffer;
extern u8 D_8004FA18;
extern SampleBuffer D_8004FA50[];
extern u32 func_80014C18(int);
extern void func_80014C40(short *, u32);
void func_8001C3CC(void)
{
    int i;
    u32 position;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004FA50[i].mode == 1) {
            position = func_80014C18(i);
            if (position != D_8004FA50[i].position) {
                if (position > D_8004FA50[i].position) {
                    if (D_8004FA50[i].callback(D_8004FA50[i].buffer + D_8004FA50[i].position,
                        position - D_8004FA50[i].position, 0, 0, D_8004FA50[i].context)) {
                        func_80014C40(D_8004FA50[i].buffer + D_8004FA50[i].position,
                            position - D_8004FA50[i].position);
                    }
                } else {
                    if (D_8004FA50[i].callback(D_8004FA50[i].buffer + D_8004FA50[i].position,
                        D_8004FA50[i].samples - D_8004FA50[i].position,
                        D_8004FA50[i].buffer, position, D_8004FA50[i].context)) {
                        func_80014C40(D_8004FA50[i].buffer + D_8004FA50[i].position,
                            D_8004FA50[i].samples - D_8004FA50[i].position);
                        func_80014C40(D_8004FA50[i].buffer, position);
                    }
                }
            }
            D_8004FA50[i].position = position;
        }
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C508.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C580.s")
/* PROMOTED 2026-10-04 — func_8001C770
 * Source:   cloud/matches/boot_tail/func_8001C770.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001C770.c:func_8001C770 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned int func_8001C770(unsigned int count)
{
    return count * 2 + 8;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C77C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C7F4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C860.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001CC9C.s")
/* PROMOTED 2026-10-04 — func_8001CCC0
 * Source:   cloud/matches/boot_tail/func_8001CCC0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001CCC0.c:func_8001CCC0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_8001CCC0(unsigned int value)
{
    if (value > 16383) {
        return 16383;
    }
    return value;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001CCDC.s")
/* PROMOTED 2026-10-04 — func_8001D084
 * Source:   cloud/matches/boot_tail/func_8001D084.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D084.c:func_8001D084 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StateNode { struct StateNode *next; struct StateNode *previous; u32 flags08; u8 unknown0C[40]; u32 identifier34; } StateNode;
extern StateNode *D_8004FD50;
extern int func_8001B8C4(u32);
void func_8001D084(StateNode *state)
{
    if (state->next != 0) state->next->previous = state->previous;
    if (state->previous != 0) state->previous->next = state->next;
    else D_8004FD50 = state->next;
    state->flags08 &= 0xFFFF;
    if (state->identifier34 != 0xFFFFFFFF) func_8001B8C4(state->identifier34);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D0F0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D1C0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D1F4.s")
/* PROMOTED 2026-10-04 — func_8001D3F0
 * Source:   cloud/matches/boot_tail/func_8001D3F0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D3F0.c:func_8001D3F0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_8002C630;
extern int func_8001D1F4(void *, const void *, const void *, float, float, u32, u16, u32, u8, u8);
int func_8001D3F0(void *state, const void *first, const void *second, float fourth,
                 float fifth, u32 flags, u16 identifier, u8 eighth, u8 ninth)
{
    if (D_8002C630) {
        return func_8001D1F4(state, first, second, fourth, fifth, flags,
                            identifier, identifier | 0x80000000, eighth, ninth);
    }
    return -1;
}

/* PROMOTED 2026-10-04 — func_8001D460
 * Source:   cloud/matches/boot_tail/func_8001D460.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D460.c:func_8001D460 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_8001D460(void *state, const void *first, const void *second, float fourth,
                 float fifth, u32 flags, u16 identifier, u16 eighth, u8 ninth, u8 tenth)
{
    if (D_8002C630) {
        return func_8001D1F4(state, first, second, fourth, fifth, flags,
                            identifier, eighth, ninth, tenth);
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D4CC.s")
/* PROMOTED 2026-10-04 — func_8001D518
 * Source:   cloud/matches/boot_tail/func_8001D518.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D518.c:func_8001D518 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct StatePrefix { unsigned char unknown00[8]; unsigned int flags08; unsigned char unknown0C[40]; unsigned int identifier34; } StatePrefix;
unsigned int func_8001D518(StatePrefix *state)
{
    unsigned int result;
    result = 0xFFFFFFFF;
    if (D_8002C630) {
        func_80014594();
        if (state->flags08 & 0x10000) result = state->identifier34;
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001D578
 * Source:   cloud/matches/boot_tail/func_8001D578.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D578.c:func_8001D578 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001D4CC(StateNode *);
void func_8001D578(void)
{
    StateNode *state;
    StateNode *next;
    state = D_8004FD50;
    while (state != 0) {
        next = state->next;
        func_8001D4CC(state);
        state = next;
    }
}

/* PROMOTED 2026-10-04 — func_8001D5C0
 * Source:   cloud/matches/boot_tail/func_8001D5C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D5C0.c:func_8001D5C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct Vector { float x, y, z; } Vector;
typedef struct SpatialState { unsigned char unknown00[12]; Vector position; unsigned char unknown18[12]; Vector forward; Vector side; Vector up; float inverse[12]; } SpatialState;
extern void func_80024D04(Vector *, const Vector *, const Vector *);
extern void func_80024D74(float *, const float *);
void func_8001D5C0(SpatialState *state)
{
    float matrix[12];
    func_80024D04(&state->side, &state->up, &state->forward);
    matrix[0] = state->side.x;
    matrix[3] = state->side.y;
    matrix[6] = state->side.z;
    matrix[1] = state->up.x;
    matrix[4] = state->up.y;
    matrix[7] = state->up.z;
    matrix[2] = state->forward.x;
    matrix[5] = state->forward.y;
    matrix[8] = state->forward.z;
    matrix[9] = state->position.x;
    matrix[10] = state->position.y;
    matrix[11] = state->position.z;
    func_80024D74(state->inverse, matrix);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D660.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D764.s")
/* PROMOTED 2026-10-04 — func_8001D8B0
 * Source:   cloud/matches/boot_tail/func_8001D8B0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D8B0.c:func_8001D8B0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct LinkNode { struct LinkNode *next; struct LinkNode *previous; } LinkNode;
extern LinkNode *D_8004FD54;
int func_8001D8B0(LinkNode *node)
{
    if (D_8002C630) {
        func_80014594();
        if (node->next != 0) node->next->previous = node->previous;
        if (node->previous != 0) node->previous->next = node->next;
        else D_8004FD54 = node->next;
        func_800145DC();
        return 1;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_8001D928
 * Source:   cloud/matches/boot_tail/func_8001D928.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D928.c:func_8001D928 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8004FF20, D_800502A8, D_80050430;
void func_8001D928(void)
{
    D_8004FF20 = 0;
    D_800502A8 = 0;
    D_80050430 = 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001D944.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001DA74.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001DC08.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001DDE0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001E0C0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001E0D4.s")
