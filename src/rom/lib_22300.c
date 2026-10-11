/* GENERATED ROM-aligned TU — segment 0x22300 (rom/lib_22300)
 * layout map 54d665d6b39f1b38a2856abd36b6c977ef6f9441dc6ab90962db9c986228773b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80021700
 * Source:   cloud/matches/boot_tail/func_80021700.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021700.c:func_80021700 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
int func_80021700(u32 id)
{
    VoiceState *voice;
    if (id != 0xFFFFFFFFU) {
        voice = &D_8004BEB8[id & 255];
        if (voice->id60 == id) {
            voice->flags24 |= 8;
            return 0;
        }
    }
    return -1;
}

/* PROMOTED 2026-10-11 — func_80021764
 * Source:   cloud/work/frontier/w19b/sources/func_80021764.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/sources/func_80021764.c:func_80021764 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern u8 D_80056160[][16];
extern u8 D_800561E0[];
struct MacroState;
int func_80021764(struct MacroState *state)
{
    u8 index;
    if (((VoiceState *)state)->id60 != 0xFFFFFFFFU && ((VoiceState *)state)->channel4A != 255) {
        index = ((VoiceState *)state)->id60 & 0xFF;
        if (((VoiceState *)state)->set4B == 255) {
            if (D_800561E0[index] == index) return 1;
        } else {
            if (D_80056160[((VoiceState *)state)->set4B][((VoiceState *)state)->channel4A] == index) return 1;
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_800217E4
 * Source:   cloud/work/frontier/w19b/sources/func_800217E4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/sources/func_800217E4.c:func_800217E4 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
void func_800217E4(VoiceState *voice)
{
    u8 index;
    if (voice->id60 != 0xFFFFFFFFU && voice->channel4A != 255) {
        index = voice->id60 & 0xFF;
        if (voice->set4B == 255) {
            D_800561E0[index] = index;
        } else {
            D_80056160[voice->set4B][voice->channel4A] = index;
        }
    }
}

/* PROMOTED 2026-10-11 — func_80021844
 * Source:   cloud/work/frontier/w19b/sources/func_80021844.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/sources/func_80021844.c:func_80021844 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
void func_80021844(VoiceState *voice)
{
    u8 index;
    if (voice->id60 != 0xFFFFFFFFU && voice->channel4A != 255) {
        index = voice->id60 & 0xFF;
        if (voice->set4B == 255) {
            if (D_800561E0[index] == index) D_800561E0[index] = 255;
        } else {
            if (D_80056160[voice->set4B][voice->channel4A] == index) D_80056160[voice->set4B][voice->channel4A] = 255;
        }
    }
}

/* PROMOTED 2026-10-05 — func_800218CC
 * Source:   cloud/matches/boot_tail/func_800218CC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800218CC.c:func_800218CC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_80056160[8][16];
extern u8 D_800561E0[32];
void func_800218CC(void)
{
    u32 row;
    u32 index;
    for (row = 0; row < 8; row++) {
        for (index = 0; index < 16; index++) {
            D_80056160[row][index] = 255;
        }
    }
    for (index = 0; index < 32; index++) {
        D_800561E0[index] = 255;
    }
}

typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
/* PROMOTED 2026-10-11 — func_8002193C
 * Source:   cloud/work/frontier/w19a/sources/func_8002193C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_8002193C.c:func_8002193C (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "types.h"
#pragma pack(1)
typedef struct MacroState_8002193C { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x38]; u32 identifier60; u8 unknown64[0x38]; u32 deadline9C; u32 baseA0; u32 clockA4; } MacroState_8002193C;
#pragma pack()
extern u8 func_8001467C(int);
extern u16 func_8001E790(void);
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
u8 func_8002193C(MacroState *state, MacroCommand *command)
{
    u32 time;
    u8 milliseconds;
    time = (u16)(command->word[1] >> 16);
    if (time != 0) {
        if ((u8)(command->word[0] >> 8) & 1) {
            if (((MacroState_8002193C *)state)->flags24 & 8) return 0;
            ((MacroState_8002193C *)state)->flags24 |= 4;
        } else {
            ((MacroState_8002193C *)state)->flags24 &= ~4U;
        }
        if ((u8)(command->word[0] >> 24) & 1) {
            if (!(((MacroState_8002193C *)state)->flags24 & 0x20) && !func_8001467C(((MacroState_8002193C *)state)->identifier60 & 255)) return 0;
            ((MacroState_8002193C *)state)->flags24 |= 0x100000;
        } else {
            ((MacroState_8002193C *)state)->flags24 &= ~0x100000U;
        }
        if ((u8)(command->word[0] >> 16) & 1) time = func_8001E790() % time;
        if (time != 0xFFFF) {
            milliseconds = (u8)(command->word[1] >> 8) & 1;
            if (milliseconds) func_8001E930(&time);
            else func_8001E940(&time, state);
            if (milliseconds) {
                ((MacroState_8002193C *)state)->deadline9C = ((MacroState_8002193C *)state)->clockA4 + time;
            } else {
                ((MacroState_8002193C *)state)->deadline9C = ((MacroState_8002193C *)state)->baseA0 + time;
                if (((MacroState_8002193C *)state)->deadline9C <= ((MacroState_8002193C *)state)->clockA4) {
                    ((MacroState_8002193C *)state)->baseA0 = ((MacroState_8002193C *)state)->deadline9C;
                    ((MacroState_8002193C *)state)->deadline9C = 0;
                }
            }
        } else {
            ((MacroState_8002193C *)state)->deadline9C = 0xFFFFFFFFU;
        }
        return ((MacroState_8002193C *)state)->deadline9C != 0;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_80021B9C
 * Source:   cloud/matches/boot_tail/func_80021B9C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021B9C.c:func_80021B9C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 func_8002193C(MacroState *, MacroCommand *);
u8 func_80021B9C(MacroState *state, MacroCommand *command)
{
    ((u8 *)command)[6] = 1;
    return func_8002193C(state, command);
}

/* PROMOTED 2026-10-04 — func_80021BC0
 * Source:   cloud/matches/boot_tail/func_80021BC0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021BC0.c:func_80021BC0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001EB10(MacroState *);
extern void func_8001F6EC(MacroState *);
u8 func_80021BC0(MacroState *state, MacroCommand *command)
{
    func_8001EB10(state);
    func_8001F6EC(state);
    return 1;
}

/* PROMOTED 2026-10-05 — func_80021BF0
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80021BF0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80021BF0.c:func_80021BF0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021BF0 { u32 field00; u32 field04; u32 field08; u32 field0C; u8 unknown10[0xC]; MacroCommand *field1C; MacroCommand *field20; u8 unknown24[0xA]; u8 field2E; } MacroState_80021BF0;
typedef struct MacroControl_80021BF0 { u32 field0; u32 field4; u32 field8; } MacroControl_80021BF0;
#pragma pack(0)
extern u8 func_80021BC0(MacroState *, MacroCommand *);
u8 func_80021BF0(MacroState_80021BF0 *state, MacroCommand *command)
{
    if (((command->word[0] >> 8) & 0xFF) == 0 || state->field08 == 0) {
        return func_80021BC0((MacroState *)state, command);
    }
    state->field00 = state->field08;
    state->field04 = state->field0C;
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021C58
 * Source:   cloud/work/frontier/w17g/sources/func_80021C58.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021C58.c:func_80021C58 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021C58 { MacroCommand *field00; MacroCommand *field04; MacroCommand *field08; MacroCommand *field0C; u8 unknown10[0x20]; u32 field30; u8 unknown34[0x16]; u8 field4A; u8 unknown4B[5]; u16 field50; } MacroState_80021C58;
#pragma pack(0)
extern MacroCommand *func_80016C20(u16);
u8 func_80021C58(MacroState_80021C58 *state, MacroCommand *command)
{
    MacroCommand *address;
    if (state->field50 >= (u8)(command->word[0] >> 8)) {
        if ((address = func_80016C20((u16)(command->word[0] >> 16))) != 0) {
            state->field00 = address;
            state->field04 = address + (command->word[1] & 0xFFFF);
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021CDC
 * Source:   cloud/work/frontier/w17g/sources/func_80021CDC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021CDC.c:func_80021CDC (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021CDC { MacroCommand *field00; MacroCommand *field04; MacroCommand *field08; MacroCommand *field0C; u8 unknown10[0x20]; u32 field30; u8 unknown34[0x16]; u8 field4A; u8 unknown4B[5]; u16 field50; } MacroState_80021CDC;
#pragma pack(0)
u8 func_80021CDC(MacroState_80021CDC *state, MacroCommand *command)
{
    MacroCommand *address;
    if ((state->field30 >> 16) >= (u8)(command->word[0] >> 8)) {
        if ((address = func_80016C20((u16)(command->word[0] >> 16))) != 0) {
            state->field00 = address;
            state->field04 = address + (command->word[1] & 0xFFFF);
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021D5C
 * Source:   cloud/work/frontier/w17g/sources/func_80021D5C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021D5C.c:func_80021D5C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021D5C { MacroCommand *field00; MacroCommand *field04; MacroCommand *field08; MacroCommand *field0C; u8 unknown10[0x20]; u32 field30; u8 unknown34[0x16]; u8 field4A; u8 unknown4B[5]; u16 field50; } MacroState_80021D5C;
#pragma pack(0)
extern u16 func_800214C8(MacroState_80021D5C *);
u8 func_80021D5C(MacroState_80021D5C *state, MacroCommand *command)
{
    MacroCommand *address;
    u8 modulation;
    if (state->field4A != 255) {
        modulation = func_800214C8(state) >> 7;
        if (modulation >= (u8)(command->word[0] >> 8)) {
            if ((address = func_80016C20((u16)(command->word[0] >> 16))) != 0) {
                state->field00 = address;
                state->field04 = address + (command->word[1] & 0xFFFF);
            }
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021DF8
 * Source:   cloud/work/frontier/w17g/sources/func_80021DF8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021DF8.c:func_80021DF8 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021DF8 { MacroCommand *field00; MacroCommand *field04; MacroCommand *field08; MacroCommand *field0C; u8 unknown10[0x20]; u32 field30; u8 unknown34[0x16]; u8 field4A; u8 unknown4B[5]; u16 field50; } MacroState_80021DF8;
#pragma pack(0)
extern u16 func_8001E790(void);
u8 func_80021DF8(MacroState_80021DF8 *state, MacroCommand *command)
{
    MacroCommand *address;
    if ((u8)func_8001E790() >= (u8)(command->word[0] >> 8)) {
        if ((address = func_80016C20((u16)(command->word[0] >> 16))) != 0) {
            state->field00 = address;
            state->field04 = address + (command->word[1] & 0xFFFF);
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021E74
 * Source:   cloud/work/frontier/w17g/sources/func_80021E74.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021E74.c:func_80021E74 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021E74 { MacroCommand *field00; MacroCommand *field04; MacroCommand *field08; MacroCommand *field0C; u8 unknown10[0x20]; u32 field30; u8 unknown34[0x16]; u8 field4A; u8 unknown4B[5]; u16 field50; } MacroState_80021E74;
#pragma pack(0)
u8 func_80021E74(MacroState_80021E74 *state, MacroCommand *command)
{
    MacroCommand *address;
    if ((address = func_80016C20(command->word[0] >> 16)) != 0) {
        state->field08 = state->field00;
        state->field0C = state->field04;
        state->field00 = address;
        state->field04 = address + (command->word[1] & 0xFFFF);
        return 0;
    }
    return func_80021BC0((MacroState *)state, command);
}

/* PROMOTED 2026-10-11 — func_80021F08
 * Source:   cloud/work/frontier/w17g/sources/func_80021F08.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80021F08.c:func_80021F08 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021F08 { u32 field00; u32 field04; u32 field08; u32 field0C; u8 unknown10[0xC]; MacroCommand *field1C; MacroCommand *field20; u8 unknown24[0xA]; u8 field2E; } MacroState_80021F08;
typedef struct MacroControl { u32 field0; u32 field4; u32 field8; } MacroControl;
#pragma pack(0)
u8 func_80021F08(MacroState_80021F08 *state, MacroCommand *command)
{
    MacroCommand *start;
    start = func_80016C20((command->word[0] >> 16) & 0xFFFF);
    if (start != 0) {
        state->field1C = start;
        state->field20 = start + (command->word[1] & 0xFFFF);
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_80021F68
 * Source:   cloud/work/boot_tail_promotion/sources/func_80021F68.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80021F68.c:func_80021F68 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021F68 { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_80021F68;
#pragma pack(0)
typedef struct MacroCommand_80021F68 { u32 word[2]; } MacroCommand_80021F68;
u8 func_80021F68(MacroState_80021F68 *state, MacroCommand_80021F68 *command)
{
    state->field1C = state->field20 = 0;
    return 0;
}

/* PROMOTED 2026-10-11 — func_80021F84
 * Source:   cloud/work/frontier/w19a/sources/func_80021F84.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_80021F84.c:func_80021F84 (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80021F84 { MacroCommand *start00; MacroCommand *current04; u8 unknown08[0x1C]; u32 flags24; u8 unknown28[0x38]; u32 identifier60; u8 unknown64[4]; u16 loop68; } MacroState_80021F84;
#pragma pack()
u8 func_80021F84(MacroState_80021F84 *state, MacroCommand *command)
{
    if (state->loop68 == 0) {
        if ((u8)(command->word[0] >> 16) & 1) {
            state->loop68 = func_8001E790() % (u16)(command->word[1] >> 16);
        } else {
            state->loop68 = command->word[1] >> 16;
        }
        if (state->loop68 == 0xFFFF) goto skip;
        ++state->loop68;
    } else if (state->loop68 == 0xFFFF) {
        goto skip;
    }
    if (--state->loop68 != 0) {
skip:
        if (((u8)(command->word[0] >> 8) & 1) != 0 &&
            !(state->flags24 & 0x40000000U) && (state->flags24 & 8)) {
            state->loop68 = 0;
            return 0;
        }
        if (((u8)(command->word[0] >> 24) & 1) &&
            !(state->flags24 & 0x20) && !func_8001467C(state->identifier60 & 255)) {
            state->loop68 = 0;
            return 0;
        }
        state->current04 = state->start00 + (command->word[1] & 0xFFFF);
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022160
 * Source:   cloud/work/frontier/w20b/sources/func_80022160.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_80022160.c:func_80022160 (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
#pragma pack(1)
typedef struct MacroState_80022160 { u8 unknown00[0x10]; u32 child10; u32 parent14; u8 unknown18[0x17]; u8 section2F; u32 volume30; u8 unknown34[4]; u32 panning38; u8 unknown3C[0xE]; u8 channel4A; u8 set4B; u8 external4C; u8 unknown4D; u16 originalNote4E; u8 unknown50[0x10]; u32 identifier60; u8 unknown64[0x56]; u16 allocationBA; u8 unknownBC; u8 activeBD; u8 groupBE; u8 unknownBF[0xE1]; } MacroState_80022160;
#pragma pack()
extern u32 func_80024988(u32 packed, u16 allocation, u8 key, u8 volume, u8 panning, u8 channel, u8 set, u16 offset, u16 section, u8 start, u8 group);
extern void func_8001B744(MacroState_80022160 *, MacroState_80022160 *);
u8 func_80022160(MacroState_80022160 *state, MacroCommand *command)
{
    s32 key;
    u32 macro;
    u32 child;
    macro = (u16)(command->word[0] >> 16) << 16 | ((command->word[1] >> 16) & 0xFF) << 8 | ((command->word[1] >> 24) & 0xFF);
    key = state->originalNote4E + (s8)(u8)(command->word[0] >> 8);
    key = key < 0 ? 0 : key > 127 ? 127 : key;
    if (state->external4C) key |= 128;
    state->activeBD = 1;
    child = func_80024988(macro, state->allocationBA, key,
        state->volume30 >> 16, state->panning38 >> 16, state->channel4A,
        state->set4B, command->word[1], state->section2F, 0, state->groupBE);
    state->activeBD = 0;
    if (child != 0xFFFFFFFFU) {
        ((MacroState_80022160 *)&D_8004BEB8[child & 255])->parent14 = state->identifier60;
        if (state->child10 != 0xFFFFFFFFU) {
            ((MacroState_80022160 *)&D_8004BEB8[child & 255])->child10 = state->child10;
            ((MacroState_80022160 *)&D_8004BEB8[state->child10 & 255])->parent14 = child;
        }
        state->child10 = child;
        if (state->external4C) func_8001B744((MacroState_80022160 *)&D_8004BEB8[child & 255], state);
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022324.s")
/* PROMOTED 2026-10-11 — func_800223D0
 * Source:   cloud/work/frontier/w18c/sources/func_800223D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_800223D0.c:func_800223D0 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800223D0 { u8 unknown00[0x28]; u32 field28; u16 field2C; u8 unknown2E[2]; u32 field30; u8 unknown34[4]; s32 field38; u8 unknown3C[0x14]; u16 field50; } MacroState_800223D0;
#pragma pack(0)
typedef struct MacroCommand_800223D0 { u32 word[2]; } MacroCommand_800223D0;
u8 func_800223D0(MacroState_800223D0 *state, MacroCommand_800223D0 *command)
{
    s16 delta;
    s32 value;
    delta = command->word[0] >> 16;
    value = (state->field28 >> 15) + delta;
    if (value < 0) {
        state->field28 = 0;
    } else if (value > 65535) {
        state->field28 = 0x7FFF8000;
    } else {
        state->field28 = (u32)value << 15;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_8002243C
 * Source:   cloud/work/boot_tail_promotion/sources/func_8002243C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8002243C.c:func_8002243C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_8002243C { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_8002243C;
#pragma pack(0)
typedef struct MacroCommand_8002243C { u32 word[2]; } MacroCommand_8002243C;
u8 func_8002243C(MacroState_8002243C *state, MacroCommand_8002243C *command)
{
    state->field28 = ((u16)(command->word[0] >> 16)) << 15;
    return 0;
}

/* PROMOTED 2026-10-04 — func_8002245C
 * Source:   cloud/work/boot_tail_promotion/sources/func_8002245C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_8002245C.c:func_8002245C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_8002245C { u8 unknown00[0x28]; u32 field28; u16 field2C; u8 unknown2E[2]; u32 field30; u8 unknown34[4]; s32 field38; u8 unknown3C[0x14]; u16 field50; } MacroState_8002245C;
#pragma pack(0)
typedef struct MacroCommand_8002245C { u32 word[2]; } MacroCommand_8002245C;
u8 func_8002245C(MacroState_8002245C *state, MacroCommand_8002245C *command)
{
    u32 divisor;
    divisor = command->word[1];
    if (divisor != 0) {
        state->field2C = (state->field28 >> 8) / divisor;
    } else {
        state->field2C = 0;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_800224B0
 * Source:   cloud/work/boot_tail_promotion/sources/func_800224B0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_800224B0.c:func_800224B0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800224B0 { u8 unknown00[0x28]; u32 field28; u16 field2C; u8 unknown2E[2]; u32 field30; u8 unknown34[4]; s32 field38; u8 unknown3C[0x14]; u16 field50; } MacroState_800224B0;
#pragma pack(0)
typedef struct MacroCommand_800224B0 { u32 word[2]; } MacroCommand_800224B0;
u8 func_800224B0(MacroState_800224B0 *state, MacroCommand_800224B0 *command)
{
    u32 value;
    value = (u16)(command->word[0] >> 16)
          + (((command->word[1] & 0xFFFF) * (state->field30 >> 16)) >> 7);
    if (value > 60000) {
        state->field28 = 0x75300000;
    } else {
        state->field28 = value << 15;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022514
 * Source:   cloud/work/frontier/w17g/sources/func_80022514.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80022514.c:func_80022514 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022514 { u32 field00; u32 field04; u32 field08; u32 field0C; u8 unknown10[0xC]; MacroCommand *field1C; MacroCommand *field20; u8 unknown24[0xA]; u8 field2E; } MacroState_80022514;
#pragma pack(0)
extern void func_8001EF8C(MacroState *, u8);
u8 func_80022514(MacroState_80022514 *state, MacroCommand *command)
{
    s32 delta;
    s32 value;
    delta = (s16)(command->word[0] >> 16);
    value = (s16)(state->field2E + delta);
    if (value < 0) {
        value = 0;
    } else {
        value = (s16)(value > 255 ? 255 : value);
    }
    func_8001EF8C((MacroState *)state, (u8)value);
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022580
 * Source:   cloud/work/frontier/w17g/sources/func_80022580.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80022580.c:func_80022580 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
u8 func_80022580(MacroState *state, MacroCommand *command)
{
    func_8001EF8C(state, (u8)(command->word[0] >> 8));
    return 0;
}

/* PROMOTED 2026-10-04 — func_800225AC
 * Source:   cloud/work/boot_tail_promotion/sources/func_800225AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_800225AC.c:func_800225AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800225AC { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_800225AC;
#pragma pack(0)
typedef struct MacroCommand_800225AC { u32 word[2]; } MacroCommand_800225AC;
extern u16 D_8004BE98[];
u8 func_800225AC(MacroState_800225AC *state, MacroCommand_800225AC *command)
{
    D_8004BE98[(u8)(command->word[0] >> 8)] = (u8)(command->word[0] >> 16);
    return 0;
}

/* PROMOTED 2026-10-04 — func_800225DC
 * Source:   cloud/work/boot_tail_promotion/sources/func_800225DC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_800225DC.c:func_800225DC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800225DC { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_800225DC;
#pragma pack(0)
typedef struct MacroCommand_800225DC { u32 word[2]; } MacroCommand_800225DC;
u8 func_800225DC(MacroState_800225DC *state, MacroCommand_800225DC *command)
{
    state->field6A = command->word[0] >> 16;
    state->field6B = command->word[0] >> 8;
    return 0;
}

/* PROMOTED 2026-10-05 — func_800225FC
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800225FC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800225FC.c:func_800225FC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800225FC { u8 unknown00[0x4A]; u8 channel4A; u8 set4B; u8 unknown4C[4]; u16 note50; u8 unknown52[0x6E]; s8 detuneC0; u8 lastNoteC1; } MacroState_800225FC;
#pragma pack()
extern void func_80020F4C(u8, u8, u8);
extern int func_80021764(MacroState *);
u8 func_800225FC(MacroState_800225FC *state, MacroCommand *command)
{
    state->note50 = (u8)(command->word[0] >> 8) & 0x7F;
    state->detuneC0 = command->word[0] >> 16;
    if (func_80021764((MacroState *)state)) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C((MacroState *)state, command);
}

/* PROMOTED 2026-10-05 — func_80022678
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80022678.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80022678.c:func_80022678 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022678 { u8 unknown00[0x4A]; u8 channel4A; u8 set4B; u8 unknown4C[2]; u16 originalNote4E; u16 note50; u8 unknown52[0x6E]; s8 detuneC0; } MacroState_80022678;
#pragma pack()
u8 func_80022678(MacroState_80022678 *state, MacroCommand *command)
{
    if ((u8)(command->word[0] >> 24) == 0) {
        state->note50 += (s8)(u8)(command->word[0] >> 8);
    } else {
        state->note50 = state->originalNote4E + (s8)(u8)(command->word[0] >> 8);
    }
    state->note50 = (s16)state->note50 < 0 ? 0 : state->note50 > 127 ? 127 : state->note50;
    state->detuneC0 = command->word[0] >> 16;
    if (func_80021764((MacroState *)state)) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C((MacroState *)state, command);
}

/* PROMOTED 2026-10-11 — func_8002279C
 * Source:   cloud/work/frontier/w17g/sources/func_8002279C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8002279C.c:func_8002279C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_8002279C { u8 unknown00[0x4A]; u8 channel4A; u8 set4B; u8 unknown4C[4]; u16 note50; u8 unknown52[0x6E]; s8 detuneC0; u8 lastNoteC1; } MacroState_8002279C;
#pragma pack()
u8 func_8002279C(MacroState_8002279C *state, MacroCommand *command)
{
    state->note50 = state->lastNoteC1 + (s8)(u8)(command->word[0] >> 8);
    state->note50 = (s16)state->note50 < 0 ? 0 : state->note50 > 127 ? 127 : state->note50;
    state->detuneC0 = command->word[0] >> 16;
    if (state->channel4A != 255) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C((MacroState *)state, command);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022874.s")
/* PROMOTED 2026-10-11 — func_80022A40
 * Source:   cloud/work/frontier/w17g/sources/func_80022A40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80022A40.c:func_80022A40 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022A40 { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState_80022A40;
#pragma pack(0)
extern void func_8001489C(u32, u8);
u8 func_80022A40(MacroState_80022A40 *state, MacroCommand *command)
{
    func_8001489C(state->field60 & 0xFF, 1);
    return 0;
}

/* PROMOTED 2026-10-04 — func_80022A78
 * Source:   cloud/work/boot_tail_promotion/sources/func_80022A78.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80022A78.c:func_80022A78 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022A78 { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_80022A78;
#pragma pack(0)
typedef struct MacroCommand_80022A78 { u32 word[2]; } MacroCommand_80022A78;
u8 func_80022A78(MacroState_80022A78 *state, MacroCommand_80022A78 *command)
{
    state->flags24 |= 0x80;
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022A98
 * Source:   cloud/work/frontier/w17g/sources/func_80022A98.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80022A98.c:func_80022A98 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022A98 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x44]; u32 currentTime6C; u32 period70; u8 unknown74[4]; u8 keyRange78; u8 centRange79; } MacroState_80022A98;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
u8 func_80022A98(MacroState_80022A98 *state, MacroCommand *command)
{
    u32 time;
    s8 keyRange;
    s8 centRange;
    if ((u8)(command->word[0] >> 24) & 3) {
        state->flags24 |= 0x8000;
    } else {
        state->flags24 &= ~0x8000;
    }
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, (MacroState *)state);
    }
    if (time) {
        state->flags24 |= 0x4000;
        state->period70 = time;
        keyRange = command->word[0] >> 8;
        centRange = command->word[0] >> 16;
        if (keyRange < 0) {
            if (centRange < 0) state->centRange79 = -centRange;
            else state->centRange79 = centRange;
            state->keyRange78 = -keyRange;
            state->currentTime6C = state->period70 / 2;
        } else {
            if (centRange < 0) {
                if (keyRange == 0) {
                    state->centRange79 = -centRange;
                    state->currentTime6C = state->period70 / 2;
                } else {
                    --keyRange;
                    state->centRange79 = 100 - centRange;
                    state->currentTime6C = 0;
                }
            } else {
                state->centRange79 = centRange;
                state->currentTime6C = 0;
            }
            state->keyRange78 = keyRange;
        }
    } else {
        state->flags24 &= ~0x4000;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_80022C58
 * Source:   cloud/work/boot_tail_promotion/sources/func_80022C58.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80022C58.c:func_80022C58 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MacroCommand_80022C58 { u32 word[2]; } MacroCommand_80022C58;
#pragma pack(1)
typedef struct MacroState_80022C58 { u32 field00; u32 field04; u32 field08; u32 field0C; u8 unknown10[0xC]; MacroCommand_80022C58 *field1C; MacroCommand_80022C58 *field20; u8 unknown24[0xA]; u8 field2E; } MacroState_80022C58;
typedef struct MacroControl_80022C58 { u32 field0; u32 field4; u32 field8; } MacroControl_80022C58;
#pragma pack(0)
extern void func_8001E930(u32 *);
u8 func_80022C58(MacroState_80022C58 *state, MacroCommand_80022C58 *command)
{
    u32 duration;
    u8 index;
    MacroControl_80022C58 *control;
    index = command->word[0] >> 8;
    duration = (u16)(command->word[0] >> 16);
    func_8001E930(&duration);
    control = (MacroControl_80022C58 *)((u8 *)state + 0x168) + index;
    if (control->field4 != 0) {
        control->field0 = 0;
    }
    control->field4 = duration;
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022CD4
 * Source:   cloud/work/frontier/w21a/ownslot/func_80022CD4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21a/ownslot/func_80022CD4.c:func_80022CD4 (score0); rom_tu.h context: cloud/work/frontier/w21i/ctx71_w21i.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct PitchState_80022CD4 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x28]; u16 note50; u8 unknown52[0xA]; u32 sample5C; u8 unknown60[0x60]; s8 detuneC0; } PitchState_80022CD4;
#pragma pack(0)
typedef struct PitchCommand_80022CD4 { u32 word[2]; } PitchCommand_80022CD4;
u8 func_80022CD4(PitchState_80022CD4 *state, PitchCommand_80022CD4 *command)
{
    u32 f;
    u32 of;
    u32 i;
    u32 frq;
    u32 ofrq;
    u32 no;
    static u16 kf[13] = {
        4096, 4339, 4597, 4871, 5160, 5467, 5792, 6137, 6502, 6888, 7298, 7732, 8192,
    };

    frq = (command->word[0] >> 8) & 0xFFFF;
    ofrq = state->sample5C & 0xFFFFFF;
    if (ofrq == frq) {
        state->note50 = (u8)(state->sample5C >> 24);
        state->detuneC0 = 0;
    } else if (ofrq < frq) {
        f = (frq << 12) / ofrq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        for (i = 11; i >= 0; i--) {
            if (f > kf[i]) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) + no * 12 + i;
        state->detuneC0 = ((f - kf[i]) * 100) / (kf[i + 1] - kf[i]);
    } else {
        f = (ofrq << 12) / frq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        for (i = 11; i >= 0; i--) {
            if (f > kf[i]) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) - no * 12 - i;
        state->detuneC0 = -(s8)(((f - kf[i]) * 100) / (kf[i + 1] - kf[i]));
    }
    state->flags24 |= 0x100;
    return 0;
}

/* PROMOTED 2026-10-11 — func_80022F24
 * Source:   cloud/work/frontier/w17g/sources/func_80022F24.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80022F24.c:func_80022F24 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80022F24 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x38]; u32 identifier60; } MacroState_80022F24;
typedef struct PackedEnvelope { u16 attack, decay, sustain, release; } PackedEnvelope;
#pragma pack()
typedef struct Envelope { u16 attack, decay, sustain, release; } Envelope;
extern void *func_80016E68(u16);
extern void func_800148F8(u32, const Envelope *);
u8 func_80022F24(MacroState_80022F24 *state, MacroCommand *command)
{
    Envelope envelope;
    PackedEnvelope *table;
    if ((table = func_80016E68(command->word[0] >> 8)) != 0) {
        envelope.attack = (table->attack >> 8) | (table->attack << 8);
        envelope.decay = (table->decay >> 8) | (table->decay << 8);
        envelope.sustain = (table->sustain >> 8) | (table->sustain << 8);
        envelope.release = (table->release >> 8) | (table->release << 8);
        func_800148F8(state->identifier60 & 0xFF, &envelope);
        state->flags24 |= 0x200;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_8002300C
 * Source:   cloud/work/frontier/w17g/sources/func_8002300C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8002300C.c:func_8002300C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MacroState_8002300C { u8 unknown00[0x40]; u32 sweepOffset40[2]; u8 sweepNumber48[2]; u8 unknown4A[0x32]; u32 sweepAddition7C[2]; u8 unknown84[0x2C]; u8 sweepCountB0[2]; } MacroState_8002300C;
extern u32 func_80014D30(u32);
u8 func_8002300C(MacroState_8002300C *state, MacroCommand *command, int index)
{
    s32 delta;
    state->sweepOffset40[index] = 0;
    state->sweepNumber48[index] = (u8)(command->word[0] >> 8);
    state->sweepCountB0[index] = state->sweepNumber48[index];
    delta = (s16)(command->word[0] >> 16);
    if (delta >= 0) {
        delta = (s32)func_80014D30(delta);
    } else {
        delta = (s32)(0U - func_80014D30(-delta));
    }
    state->sweepAddition7C[index] = (u32)delta << 16;
    command->word[0] = 0;
    return func_8002193C((MacroState *)state, command);
}

/* PROMOTED 2026-10-11 — func_800230B0
 * Source:   cloud/work/frontier/w20b/sources/func_800230B0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20b/sources/func_800230B0.c:func_800230B0 (score0); rom_tu.h context: cloud/work/frontier/w20b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState_800230B0 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x10]; u32 pan38; u32 panDelta3C; u8 unknown40[0x68]; u32 panTimeA8; u32 panTargetAC; } VoiceState_800230B0;
#pragma pack(0)
u8 func_800230B0(VoiceState_800230B0 *voice, MacroCommand *command)
{
    u32 unused0, unused1;
    u32 time;
    s8 offset;
    voice->flags24 |= 0x10000;
    time = (command->word[0] >> 16) & 65535;
    voice->panTimeA8 = time;
    func_8001E930(&voice->panTimeA8);
    offset = command->word[1] & 0xFF;
    voice->pan38 = ((command->word[0] >> 8) & 255) << 16;
    voice->panTargetAC = ((u32)offset << 16) + voice->pan38;
    if (voice->panTimeA8 != 0) {
        voice->panDelta3C = ((u32)offset << 16) / time;
    } else {
        voice->panDelta3C = (u32)offset << 16;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80023190
 * Source:   cloud/work/frontier/w19b/sources/func_80023190.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19b/sources/func_80023190.c:func_80023190 (score0); rom_tu.h context: cloud/work/frontier/w19b/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
u8 func_80023190(MacroState_800223D0 *state, MacroCommand *command)
{
    s32 value;
    s8 gain;
    value = ((u32)state->field50 << 16) - (((command->word[0] >> 16) & 0xFF) << 16);
    gain = command->word[0] >> 8;
    value = (s32)((u32)value * gain);
    value >>= 7;
    value += (u8)(command->word[0] >> 24) << 16;
    value = value < 0 ? 0 : (value > 0x7F0000 ? 0x7F0000 : value);
    state->field38 = value;
    return 0;
}

/* PROMOTED 2026-10-11 — func_8002321C
 * Source:   cloud/work/frontier/w17g/sources/func_8002321C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8002321C.c:func_8002321C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u32 func_8002321C(u32 volume, u16 curve)
{
    u8 *table;
    u32 high;
    u32 low;
    u32 difference;
    if (curve != 0) {
        if ((table = func_80016E68(curve)) != 0) {
            high = volume >> 16;
            low = volume & 0xFFFF;
            if (high < 127) {
                difference = low * (table[high + 1] - table[high]);
                volume = difference + (table[high] << 16);
            } else {
                volume = table[high] << 16;
            }
        }
    }
    return volume;
}

/* PROMOTED 2026-10-11 — func_800232A4
 * Source:   cloud/work/frontier/w18c/sources/func_800232A4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_800232A4.c:func_800232A4 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800232A4 { u8 unknown00[0x30]; u32 volume30; u32 originalVolume34; } MacroState_800232A4;
#pragma pack()
extern u32 func_8002321C(u32, u16);
u8 func_800232A4(MacroState_800232A4 *state, MacroCommand *command)
{
    u16 curve;
    if ((u8)(command->word[1] >> 8) == 0) {
        state->volume30 = ((u8)(command->word[0] >> 8) * state->volume30) >> 7;
    } else {
        state->volume30 = ((u8)(command->word[0] >> 8) * state->originalVolume34) >> 7;
    }
    state->volume30 += (u8)(command->word[0] >> 16) << 16;
    if (state->volume30 > 0x7F0000) {
        state->volume30 = 0x7F0000;
    }
    curve = (u8)command->word[1];
    curve |= (u8)(command->word[0] >> 24) << 8;
    state->volume30 = func_8002321C(state->volume30, curve);
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800233B0.s")
extern u8 func_800233B0(MacroState *, MacroCommand *, u32);
/* PROMOTED 2026-10-05 — func_80023520
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80023520.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80023520.c:func_80023520 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80023520 { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState_80023520;
#pragma pack(0)
u8 func_80023520(MacroState_80023520 *state, MacroCommand *command)
{
    return func_800233B0((MacroState *)state, command, state->field30);
}

/* PROMOTED 2026-10-04 — func_80023544
 * Source:   cloud/matches/boot_tail/func_80023544.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80023544.c:func_80023544 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023544(MacroState *state, MacroCommand *command)
{
    return func_800233B0(state, command, 0);
}

/* PROMOTED 2026-10-11 — func_80023564
 * Source:   cloud/work/frontier/w19a/sources/func_80023564.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_80023564.c:func_80023564 (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack()
extern u8 func_800225FC(MacroState_800225FC *state, MacroCommand *command);
u8 func_80023564(MacroState_800225FC *state, MacroCommand *command)
{
    u8 low;
    u8 high;
    u8 temporary;
    s16 lower;
    s16 upper;
    u8 detune;
    if ((u8)(command->word[1] >> 8) == 0) {
        low = command->word[0] >> 8;
        high = command->word[0] >> 24;
        if (low > high) {
            temporary = low;
            low = high;
            high = temporary;
        }
    } else {
        lower = state->note50 - (u8)(command->word[0] >> 8);
        upper = state->note50 + (u8)(command->word[0] >> 24);
        low = lower < 0 ? 0 : lower > 127 ? 127 : (u8)lower;
        high = upper < 0 ? 0 : upper > 127 ? 127 : (u8)upper;
    }
    if (command->word[1] & 0xFF) {
        detune = (func_8001E790() % 201) - 100;
    } else {
        detune = command->word[0] >> 16;
    }
    command->word[0] = ((low + (func_8001E790() % ((high - low) + 1))) * 256) | 0x19 | (detune << 16);
    command->word[1] = 0;
    func_800225FC(state, command);
    return 0;
}

/* PROMOTED 2026-10-04 — func_80023710
 * Source:   cloud/work/boot_tail_promotion/sources/func_80023710.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80023710.c:func_80023710 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80023710 { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_80023710;
#pragma pack(0)
typedef struct MacroCommand_80023710 { u32 word[2]; } MacroCommand_80023710;
u8 func_80023710(MacroState_80023710 *state, MacroCommand_80023710 *command)
{
    state->flags24 |= 0x40000;
    return 0;
}

/* PROMOTED 2026-10-04 — func_80023734
 * Source:   cloud/work/boot_tail_promotion/sources/func_80023734.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80023734.c:func_80023734 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_80023734 { u8 unknown00[0x1C]; u32 field1C; u32 field20; u32 flags24; u32 field28; u8 unknown2C[0x3E]; u8 field6A; u8 field6B; u8 unknown6C[0x2D]; u8 field99; u8 field9A; } MacroState_80023734;
#pragma pack(0)
typedef struct MacroCommand_80023734 { u32 word[2]; } MacroCommand_80023734;
u8 func_80023734(MacroState_80023734 *state, MacroCommand_80023734 *command)
{
    state->field99 = command->word[0] >> 8;
    state->field9A = command->word[0] >> 16;
    return 0;
}

typedef struct MacroState_80023818 MacroState_80023818;
typedef struct MacroControl_80023818 MacroControl_80023818;
typedef struct MacroCommand_80023818 { u32 word[2]; } MacroCommand_80023818;
extern void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control, MacroCommand_80023818 *command, u32 flag);
/* PROMOTED 2026-10-11 — func_80023754
 * Source:   cloud/work/frontier/w18c/sources/func_80023754.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80023754.c:func_80023754 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
struct MacroState_80023818 { u8 unknown00[0x24]; u32 flags24; };
typedef struct ControlEntry_80023754 { u8 controller; u8 combination; s16 scale; } ControlEntry_80023754;
struct MacroControl_80023818 { ControlEntry_80023754 entries[4]; u8 count; };
#pragma pack()
extern u8 func_80021548(u8);
void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control,
                  MacroCommand_80023818 *command, u32 flag)
{
    u8 index;
    u8 controller;
    u8 combination;
    combination = command->word[1] & 0xFF;
    if (!(state->flags24 & flag) || combination == 0) {
        state->flags24 |= flag;
        control->count = 0;
    }
    if (control->count < 4) {
        index = control->count++;
        controller = func_80021548(command->word[0] >> 8);
        control->entries[index].controller = controller;
        control->entries[index].combination = combination;
        control->entries[index].scale = ((s32)(s16)(command->word[0] >> 16) * 256) / 100;
    }
}

/* PROMOTED 2026-10-04 — func_80023818
 * Source:   cloud/work/boot_tail_promotion/sources/func_80023818.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80023818.c:func_80023818 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023818(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xC4),
                  command, 0x00200000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_80023844
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023844.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023844.c:func_80023844 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023844(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xD6),
                  command, 0x00400000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_80023870
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023870.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023870.c:func_80023870 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023870(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xFA),
                  command, 0x00800000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_8002389C
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_8002389C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_8002389C.c:func_8002389C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_8002389C(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0x11E),
                  command, 0x01000000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_800238C8
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_800238C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_800238C8.c:func_800238C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_800238C8(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0x130),
                  command, 0x08000000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_800238F4
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_800238F4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_800238F4.c:func_800238F4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_800238F4(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0x142),
                  command, 0x04000000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_80023920
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023920.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023920.c:func_80023920 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023920(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0x154),
                  command, 0x02000000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_8002394C
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_8002394C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_8002394C.c:func_8002394C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_8002394C(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xE8),
                  command, 0x10000000);
    return 0;
}

/* PROMOTED 2026-10-05 — func_80023978
 * Source:   cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023978.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_wrapper_contracts/sources/func_80023978.c:func_80023978 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 func_80023978(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0x10C),
                  command, 0x20000000);
    return 0;
}

/* PROMOTED 2026-10-11 — func_800239A4
 * Source:   cloud/work/frontier/w17g/sources/func_800239A4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_800239A4.c:func_800239A4 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroState_800239A4 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x22]; u8 channel4A; u8 set4B; u8 unknown4C[0x44]; u32 time90; u8 unknown94[4]; u8 mode98; } MacroState_800239A4;
#pragma pack()
extern void func_80020FDC(u8, u8, u8);
extern void func_80019BE4(MacroState_800239A4 *);
u8 func_800239A4(MacroState_800239A4 *state, MacroCommand *command)
{
    u32 time;
    state->mode98 = command->word[0] >> 16;
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, (MacroState *)state);
    }
    state->time90 = time;
    switch ((u8)(command->word[0] >> 8)) {
    case 0:
        state->flags24 &= ~0x800;
        state->flags24 |= 0x1000;
        if (state->channel4A != 255) func_80020FDC(state->channel4A, state->set4B, 0);
        break;
    case 1:
        if (!(state->flags24 & 0x800)) func_80019BE4(state);
        state->flags24 |= 0x1800;
        break;
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_80023AD4
 * Source:   cloud/work/frontier/w18c/sources/func_80023AD4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80023AD4.c:func_80023AD4 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceVariables_80023AD4 { u8 unknown000[0x180]; s16 variables180[16]; } VoiceVariables_80023AD4;
#pragma pack(0)
extern s16 D_8004BE78[];
extern u16 func_800215A8(VoiceState *voice, u8 controller);
s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index)
{
    s32 value;
    if (isController) {
        return func_800215A8((VoiceState *)voice, index);
    }
    index &= 31;
    value = index < 16 ? voice->variables180[index] : (&D_8004BE78[16])[index - 16];
    return value;
}

/* PROMOTED 2026-10-11 — func_80023B50
 * Source:   cloud/work/frontier/w18c/sources/func_80023B50.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80023B50.c:func_80023B50 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern void func_8002165C(VoiceState *voice, u8 controller, s16 value);
void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value)
{
    if (isController) {
        func_8002165C((VoiceState *)voice, index, value);
    } else {
        index &= 31;
        if (index < 16) {
            voice->variables180[index] = value;
        } else {
            (&D_8004BE78[16])[index - 16] = value;
        }
    }
}

/* PROMOTED 2026-10-11 — func_80023BDC
 * Source:   cloud/work/frontier/w20c/sources/func_80023BDC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_80023BDC.c:func_80023BDC (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index);
extern void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value);
u8 func_80023BDC(VoiceVariables_80023AD4 *voice, MacroCommand *command, u8 operation)
{
    s16 left;
    s16 right;
    s32 result;

    left = func_80023AD4(voice, (command->word[0] >> 24) & 255, command->word[1] & 255);
    if (operation == 4) {
        right = (s16)(command->word[1] >> 8);
    } else {
        right = func_80023AD4(voice, (command->word[1] >> 8) & 255, (command->word[1] >> 16) & 255);
    }
    switch (operation) {
    case 4:
    case 0: result = left + right; break;
    case 1: result = left - right; break;
    case 2: result = left * right; break;
    case 3: result = right != 0 ? left / right : 0; break;
    }
    func_80023B50(voice, (command->word[0] >> 8) & 255, (command->word[0] >> 16) & 255,
                 result < -32768 ? -32768 : result > 32767 ? 32767 : result);
    return 0;
}

/* PROMOTED 2026-10-11 — func_80023D70
 * Source:   cloud/work/frontier/w18c/sources/func_80023D70.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80023D70.c:func_80023D70 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value);
u8 func_80023D70(MacroState *state, MacroCommand *command)
{
    func_80023B50((VoiceVariables_80023AD4 *)state, command->word[0] >> 8, command->word[0] >> 16,
                  (s16)(command->word[1] & 0xFFFF));
    return 0;
}

/* PROMOTED 2026-10-11 — func_80023DB8
 * Source:   cloud/work/frontier/w18c/sources/func_80023DB8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80023DB8.c:func_80023DB8 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct MacroVoice_80023DB8 { MacroCommand *program; MacroCommand *current; } MacroVoice_80023DB8;
#pragma pack(0)
extern s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index);
u8 func_80023DB8(MacroVoice_80023DB8 *voice, MacroCommand *command, u8 comparison)
{
    s16 left;
    s16 right;
    u8 condition;
    left = func_80023AD4((VoiceVariables_80023AD4 *)voice, command->word[0] >> 8, command->word[0] >> 16);
    right = func_80023AD4((VoiceVariables_80023AD4 *)voice, command->word[0] >> 24, command->word[1] & 0xFF);
    switch (comparison) {
    case 0: condition = left == right; break;
    case 1: condition = left < right; break;
    }
    if ((command->word[1] >> 8) & 255) {
        condition = !condition;
    }
    if (condition) {
        voice->current = ((command->word[1] >> 16) & 65535) + voice->program;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023E9C.s")
/* PROMOTED 2026-10-11 — func_80024988
 * Source:   cloud/work/frontier/w19a/sources/func_80024988.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_80024988.c:func_80024988 (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
#pragma pack(1)
typedef struct MacroState_80024988 { MacroCommand *start00; MacroCommand *current04; MacroCommand *saved08; u8 unknown0C[4]; u32 child10; u32 parent14; u8 unknown18[12]; u32 flags24; u32 age28; u16 ageSpeed2C; u8 unknown2E[0x1E]; u8 external4C; u8 unknown4D; u16 originalNote4E; u16 note50; u8 volume52; u8 panning53; u8 channel54; u8 set55; u8 section56; u8 unknown57[9]; u32 identifier60; u16 macro64; u8 unknown66[0x36]; u32 deadline9C; u8 unknownA0[0x1A]; u16 allocationBA; u8 unknownBC[3]; u8 groupBF; u8 detuneC0; u8 unknownC1[0xDF]; } MacroState_80024988;
#pragma pack()
extern int func_8001F13C(u8, u8, u16, u8);
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

