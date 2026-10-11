/* GENERATED ROM-aligned TU — segment 0x1cf90 (rom/lib_1cf90)
 * layout map 49b03b3d75a02558e10ab39c8693394c31f03cc7f18aec15a71756604b34fc1e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — func_8001C390
 * Source:   cloud/work/frontier/w18c/sources/func_8001C390.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001C390.c:func_8001C390 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8004FA18;
extern SampleBuffer D_8004FA50[];
void func_8001C390(void)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        D_8004FA50[i].mode = 0;
    }
}

/* PROMOTED 2026-10-04 — func_8001C3CC
 * Source:   cloud/matches/boot_tail/func_8001C3CC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001C3CC.c:func_8001C3CC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
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

/* PROMOTED 2026-10-05 — func_8001C508
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001C508.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001C508.c:func_8001C508 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8004FA18;
extern void func_80014C60(short *, unsigned int);
void func_8001C508(void)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004FA50[i].mode == 1) {
            func_80014C60(D_8004FA50[i].buffer, D_8004FA50[i].samples);
        }
    }
}

/* PROMOTED 2026-10-11 — func_8001C580
 * Source:   cloud/work/frontier/w18c/sources/func_8001C580.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001C580.c:func_8001C580 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct SampleInfo_800146B4 { unsigned int frequency; void *data; unsigned int offset; unsigned int length; unsigned int loopStart; unsigned int loopLength; unsigned char format; } SampleInfo_800146B4;
#pragma pack()
extern unsigned char D_8002C630;
extern int D_8004F800;
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001F898(unsigned char);
extern void func_800146B4(unsigned int, SampleInfo_800146B4 *, unsigned char);
extern void func_80014A0C(int, unsigned short);
extern void func_80014A74(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void func_800149BC(unsigned int);
int func_8001C580(unsigned char request, short *buffer, unsigned int samples, unsigned int frequency,
                  unsigned char volume, unsigned char pan, unsigned char span, unsigned char aux,
                  SampleCallback callback, unsigned int context)
{
    int index;
    SampleInfo_800146B4 info;
    float ratio;
    if (D_8002C630) {
        func_80014594();
        index = func_8001F898(request);
        if (index != -1) {
            D_8004FA50[index].callback = callback;
            D_8004FA50[index].mode = 1;
            D_8004FA50[index].context = context;
            D_8004FA50[index].position = 0;
            info.frequency = frequency | 0x400000;
            info.offset = 0;
            info.loopStart = 0;
            info.format = 0;
            D_8004FA50[index].samples = samples;
            info.length = samples;
            info.loopLength = samples;
            D_8004FA50[index].buffer = buffer;
            info.data = buffer;
            func_800146B4(index, &info, 1);
            ratio = (float)frequency / (float)D_8004F800;
            func_80014A0C(index, (unsigned int)(ratio * 4096.0f));
            func_80014A74(index, volume << 16, pan << 16, span << 16, aux << 16);
            func_800149BC(index);
        }
        func_800145DC();
        return index;
    }
    return -1;
}

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

/* PROMOTED 2026-10-11 — func_8001C77C
 * Source:   cloud/work/frontier/w17g/sources/func_8001C77C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001C77C.c:func_8001C77C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80014A74(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void func_8001C77C(unsigned int channel, unsigned char first, unsigned char second, unsigned char third, unsigned char fourth)
{
    if (D_8002C630 && channel != 0xFFFFFFFF) {
        func_80014594();
        func_80014A74(channel, first << 16, second << 16, third << 16, fourth << 16);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-05 — func_8001C7F4
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001C7F4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001C7F4.c:func_8001C7F4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001F954(unsigned int);
void func_8001C7F4(unsigned int channel)
{
    if (D_8002C630) {
        func_80014594();
        if (D_8004FA50[channel].mode == 1) {
            D_8004FA50[channel].mode = 0;
            func_8001F954(channel);
        }
        func_800145DC();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001C860.s")
/* PROMOTED 2026-10-11 — func_8001CC9C
 * Source:   cloud/work/frontier/w17g/sources/func_8001CC9C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001CC9C.c:func_8001CC9C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_8001CC9C(unsigned char value)
{ if (value > 127) return 127; return value; }

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

/* PROMOTED 2026-10-11 — func_8001CCDC
 * Source:   cloud/work/frontier/w17g/sources/func_8001CCDC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001CCDC.c:func_8001CCDC (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 func_8001CC9C(u8);
extern u16 func_8001CCC0(u32);
extern int func_8001B7C0(u32, u8);
extern int func_8001B29C(u32, u8);
extern int func_8001B3A0(u32, u8);
extern int func_8001B4A4(u32, u16);
void func_8001CCDC(StateNode *em, float vol, float xPan, float yPan,
                   float zPan, float doppler)
{
    u32 identifier;
    identifier = em->identifier34;
    if (em->flags08 & 0x100000) {
        func_8001B7C0(identifier, func_8001CC9C((u32)((em->fade * vol) * 127.0f)));
    } else {
        func_8001B7C0(identifier, func_8001CC9C((u32)(vol * 127.0f)));
    }
    func_8001B29C(identifier, func_8001CC9C((u32)((1.0f + xPan) * 64.0f)));
    func_8001B3A0(identifier, func_8001CC9C((u32)((1.0f - zPan) * 64.0f)));
    func_8001B4A4(identifier, func_8001CCC0(doppler * 8192.0f));
}

/* PROMOTED 2026-10-04 — func_8001D084
 * Source:   cloud/matches/boot_tail/func_8001D084.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D084.c:func_8001D084 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

typedef struct Vector { float x, y, z; } Vector;
/* PROMOTED 2026-10-11 — func_8001D0F0
 * Source:   cloud/work/frontier/w18c/sources/func_8001D0F0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001D0F0.c:func_8001D0F0 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct Emitter_8001D0F0 { u8 unknown00[12]; Vector position; Vector velocity; unsigned int unknown24; float gain28; float current2C; } Emitter_8001D0F0;
extern u8 D_8002C630;
int func_8001D0F0(Emitter_8001D0F0 *state, const Vector *position, const Vector *velocity, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->gain28 = (float)level / 127.0f;
        if (state->gain28 < state->current2C) state->current2C = state->gain28;
        func_800145DC();
        return 1;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_8001D1C0
 * Source:   cloud/work/frontier/w17g/sources/func_8001D1C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001D1C0.c:func_8001D1C0 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_8001D1C0(unsigned int *state)
{
    if (D_8002C630) return (state[2] & 0x10000) != 0;
    return 0;
}

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

/* PROMOTED 2026-10-05 — func_8001D4CC
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001D4CC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001D4CC.c:func_8001D4CC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern void func_8001D084(StateNode *);
int func_8001D4CC(StateNode *state)
{
    if (D_8002C630) {
        func_80014594();
        func_8001D084(state);
        func_800145DC();
        return 1;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_8001D518
 * Source:   cloud/matches/boot_tail/func_8001D518.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D518.c:func_8001D518 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
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

/* PROMOTED 2026-10-11 — func_8001D660
 * Source:   cloud/work/frontier/w17g/sources/func_8001D660.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001D660.c:func_8001D660 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SpatialState_8001D660 { u8 unknown00[12]; Vector position; Vector velocity; Vector forward; Vector side; Vector up; float inverse[12]; u8 unknown78[12]; float gain84; } SpatialState_8001D660;
extern void func_8001D5C0(SpatialState *);
int func_8001D660(SpatialState_8001D660 *state, const Vector *position, const Vector *velocity,
                  const Vector *forward, const Vector *up, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->forward = *forward;
        state->up.x = -up->x;
        state->up.y = -up->y;
        state->up.z = -up->z;
        func_8001D5C0((SpatialState *)state);
        state->gain84 = (float)level / 127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_8001D764
 * Source:   cloud/work/frontier/w18c/sources/func_8001D764.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001D764.c:func_8001D764 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SpatialState_8001D764 { LinkNode *next, *previous; u32 flags08; Vector position, velocity, forward, side, up; float inverse[12]; float first78, second7C, third80, gain84; } SpatialState_8001D764;
extern LinkNode *D_8004FD54;
int func_8001D764(SpatialState_8001D764 *state, const Vector *position, const Vector *velocity,
                  const Vector *forward, const Vector *up, float first, float second,
                  float third, u32 flags, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        if ((state->next = D_8004FD54) != 0) D_8004FD54->previous = (LinkNode *)state;
        state->previous = 0;
        D_8004FD54 = (LinkNode *)state;
        state->position = *position;
        state->velocity = *velocity;
        state->forward = *forward;
        state->up.x = -up->x;
        state->up.y = -up->y;
        state->up.z = -up->z;
        state->first78 = first;
        state->second7C = second;
        state->third80 = third;
        func_8001D5C0((SpatialState *)state);
        state->flags08 = flags;
        state->gain84 = (float)level / 127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_8001D8B0
 * Source:   cloud/matches/boot_tail/func_8001D8B0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001D8B0.c:func_8001D8B0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-06 — func_8001D944
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001D944.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001D944.c:func_8001D944 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_pr177.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct SmallNode { struct SmallNode *next; float distance; StateNode *object; } SmallNode;
typedef struct LargeNode { struct LargeNode *next; float distance, first, second, third, fourth; StateNode *object; } LargeNode;
typedef struct EmitGroup { unsigned int identifier; LargeNode *large; SmallNode *small; } EmitGroup;
extern EmitGroup D_8004FDA0[32];
extern unsigned char D_8004FF20;
extern LargeNode D_8004FF28[32];
extern unsigned char D_800502A8;
extern SmallNode D_800502B0[32];
extern unsigned char D_80050430;
void func_8001D944(StateNode *object, float distance)
{
    long i;
    SmallNode *current;
    SmallNode *previous;
    for (i = 0; i < D_8004FF20; ++i) {
        if (object->group == D_8004FDA0[i].identifier) break;
    }
    if (i == D_8004FF20) {
        D_8004FDA0[i].large = 0;
        D_8004FDA0[i].small = 0;
        D_8004FDA0[i].identifier = object->group;
        ++D_8004FF20;
    }
    previous = 0;
    for (current = D_8004FDA0[i].small; current != 0; current = current->next) {
        if (current->distance > distance) break;
        previous = current;
    }
    if (previous == 0) D_8004FDA0[i].small = &D_800502B0[D_80050430];
    else previous->next = &D_800502B0[D_80050430];
    D_800502B0[D_80050430].next = current;
    D_800502B0[D_80050430].object = object;
    D_800502B0[D_80050430++].distance = distance;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001DA74.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1cf90/func_8001DC08.s")
/* PROMOTED 2026-10-05 — func_8001DDE0
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001DDE0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001DDE0.c:func_8001DDE0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
extern const float D_8002D90C;
extern void func_8001D928(void);
extern void func_8001C860(StateNode *, float *, float *, float *, float *, float *);
extern int func_8001DA74(StateNode *, float, float, float, float, float);
extern unsigned int func_8001B1D0(unsigned short, unsigned char, unsigned char);
extern unsigned int func_800201D0(unsigned int);
extern void func_8001D944(StateNode *, float);
extern int func_8001B8C4(unsigned int);
extern void func_8001CCDC(StateNode *, float, float, float, float, float);
extern void func_8001DC08(void);
void func_8001DDE0(void)
{
    StateNode *emitter;
    StateNode *next;
    float volume;
    float x_pan;
    float y_pan;
    float z_pan;
    float pitch;

    func_8001D928();
    emitter = D_8004FD50;
    for (; emitter != 0; emitter = next) {
        next = emitter->next;
        if (emitter->flags08 & 0x40000) {
            func_8001D084(emitter);
            continue;
        }
        if (emitter->flags08 & 0x20001) {
            func_8001C860(emitter, &volume, &pitch, &x_pan, &y_pan, &z_pan);
        }
        if (!(emitter->flags08 & 0x80000)) {
            if (emitter->flags08 & 0x20000) {
                if (volume == 0.0f && (emitter->flags08 & 4)) {
                    emitter->flags08 |= 0x80000;
                    emitter->flags08 &= ~0x20000;
                    goto found_emitter;
                }
                if (emitter->flags08 & 1) {
                    if (func_8001DA74(emitter, volume, x_pan, y_pan, z_pan, pitch)) {
                        continue;
                    }
                } else {
                    if ((emitter->identifier34 = func_8001B1D0(emitter->sound_id, 127, 64)) == 0xFFFFFFFFU) {
                        if (!(emitter->flags08 & 2)) {
                            emitter->flags08 |= 0x40000;
                            emitter->flags08 &= ~0x20000;
                        } else {
                            continue;
                        }
                    }
                }
            } else if ((emitter->identifier34 = func_800201D0(emitter->identifier34)) == 0xFFFFFFFFU) {
                if (emitter->flags08 & 2) {
                    emitter->flags08 |= 0x20000;
                } else {
                    emitter->flags08 |= 0x40000;
                }
            }
found_emitter:
            if (emitter->identifier34 != 0xFFFFFFFFU) {
                if (emitter->flags08 & 1) {
                    func_8001D944(emitter, volume);
                }
                if (volume == 0.0f && (emitter->flags08 & 4)) {
                    func_8001B8C4(emitter->identifier34);
                    emitter->identifier34 = 0xFFFFFFFFU;
                    if (emitter->flags08 & 2) {
                        emitter->flags08 |= 0x80000;
                    } else {
                        emitter->flags08 |= 0x40000;
                    }
                } else {
                    func_8001CCDC(emitter, volume, x_pan, y_pan, z_pan, pitch);
                }
            }
            if (emitter->flags08 & 0x100000) {
                emitter->fade += D_8002D90C;
                if (emitter->fade >= 1.0f) {
                    emitter->flags08 &= ~0x100000;
                }
            }
        } else if (volume != 0.0f) {
            emitter->flags08 &= ~0x80000;
            emitter->flags08 |= 0x20000;
        }
    }
    func_8001DC08();
}

/* PROMOTED 2026-10-05 — func_8001E0C0
 * Source:   cloud/work/boot_tail_promotion/sources/func_8001E0C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8001E0C0.c:func_8001E0C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_sample_contract.h"
void func_8001E0C0(void)
{
    D_8004FD50 = 0;
    D_8004FD54 = 0;
}

/* PROMOTED 2026-10-11 — func_8001E0D4
 * Source:   cloud/work/frontier/w17g/sources/func_8001E0D4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001E0D4.c:func_8001E0D4 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_8001E0D4(void)
{
}

