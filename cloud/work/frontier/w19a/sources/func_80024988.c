/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80024988 - lib_22300 (IDO 7.1). Start a macro on a new voice (MusyX macStart family):
 * PACKED = macro id << 16 | priority << 8 | voice limit. Look the macro program up
 * (func_80016C20; none -> -1), allocate a voice (func_8001F13C(priority, limit, ALLOCATION,
 * external = KEY bit 7); -1 -> -1), reset it (func_8001EB10), set flags (keep 0x10, add 2; 1
 * when its sample channel is active, func_8001467C), clear the deadline; an external voice
 * (KEY bit 7) gets its own channel (func_80020820(index, 255)), else CHANNEL/SET; then the
 * macro id, allocation, age 0x75300000 with speed 1024, program start and current = start +
 * OFFSET, original note / note = KEY (7 bits), detune 0, VOLUME, PANNING, SECTION, no saved
 * return or child/parent (-1), GROUP, and the identifier macro << 16 | key << 8 | index.
 * Queue it (func_8001EF8C(priority)); START runs it now (func_8001ECE0). Returns the
 * identifier (or func_8001ECE0's result). No direct callers in the static image.
 * Types: the TU's VoiceState D_8004BEB8 (func_80021700's block) addressed through a packed
 * private view MacroState_80024988 (D10's MacroState; same 0x1A0 size) by one cast; the TU's
 * MacroState / MacroCommand and its func_8001EB10 / func_80016C20 / func_8001EF8C prototypes
 * (casts to MacroState * at the two calls); other callees as their locked definitions.
 * w19a: from D10 BT05-core-pair (5.3; 7.1 true=825): `u16 macro`, no `external` local (KEY bit 7
 * tested where used), locals declared state, macro, priority, program, index (retail's slots),
 * SECTION stored before the saved/child/parent words. Shaping device (disclosed): the voice
 * limit is passed as `packed & 255` - retail copies PACKED and narrows the copy (`move a1,s0;
 * andi a1,a1,0xff`); a bare `packed` / `(u8)packed` narrows straight from s0 (true=420).
 * No unused locals. */
#include "types.h"
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80024988 {
    MacroCommand *start00;
    MacroCommand *current04;
    MacroCommand *saved08;
    u8 unknown0C[4];
    u32 child10;
    u32 parent14;
    u8 unknown18[12];
    u32 flags24;
    u32 age28;
    u16 ageSpeed2C;
    u8 unknown2E[0x1E];
    u8 external4C;
    u8 unknown4D;
    u16 originalNote4E;
    u16 note50;
    u8 volume52;
    u8 panning53;
    u8 channel54;
    u8 set55;
    u8 section56;
    u8 unknown57[9];
    u32 identifier60;
    u16 macro64;
    u8 unknown66[0x36];
    u32 deadline9C;
    u8 unknownA0[0x1A];
    u16 allocationBA;
    u8 unknownBC[3];
    u8 groupBF;
    u8 detuneC0;
    u8 unknownC1[0xDF];
} MacroState_80024988;
#pragma pack()
extern void func_8001EB10(MacroState *);
extern MacroCommand *func_80016C20(u16);
extern void func_8001EF8C(MacroState *, u8);
extern int func_8001F13C(u8, u8, u16, u8);
extern u8 func_8001467C(int);
extern void func_80020820(u8, u8);
extern u32 func_8001ECE0(MacroState_80024988 *);
u32 func_80024988(u32 packed, u16 allocation, u8 key, u8 volume,
                 u8 panning, u8 channel, u8 set, u16 offset,
                 u16 section, u8 start, u8 group)
{
    MacroState_80024988 *state;
    u16 macro;
    u8 priority;
    MacroCommand *program;
    int index;
    macro = packed >> 16;
    program = func_80016C20(macro);
    if (program != 0) {
        priority = packed >> 8;
        index = func_8001F13C(priority, packed & 255, allocation, (key & 128) ? 1 : 0);
        if (index != -1) {
            state = (MacroState_80024988 *)&D_8004BEB8[index];
            func_8001EB10((MacroState *)state);
            state->flags24 = (state->flags24 & 0x10) | 2;
            if (func_8001467C(index)) state->flags24 |= 1;
            state->deadline9C = 0;
            if (key & 128) {
                state->external4C = 1;
                key &= 127;
                func_80020820(index, 255);
                state->channel54 = index;
                state->set55 = 255;
            } else {
                state->external4C = 0;
                state->channel54 = channel;
                state->set55 = set;
            }
            state->macro64 = macro;
            state->allocationBA = allocation;
            state->age28 = 0x75300000U;
            state->ageSpeed2C = 1024;
            state->start00 = program;
            state->current04 = program + offset;
            state->originalNote4E = key;
            state->note50 = key;
            state->detuneC0 = 0;
            state->volume52 = volume;
            state->panning53 = panning;
            state->section56 = section;
            state->saved08 = 0;
            state->child10 = 0xFFFFFFFFU;
            state->parent14 = 0xFFFFFFFFU;
            state->groupBF = group;
            state->identifier60 = (macro << 16) | ((u32)key << 8) | (u32)index;
            func_8001EF8C((MacroState *)state, priority);
            if (start) return func_8001ECE0(state);
            return state->identifier60;
        }
    }
    return 0xFFFFFFFFU;
}
