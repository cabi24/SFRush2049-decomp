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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021764.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800217E4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021844.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800218CC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002193C.s")
/* PROMOTED 2026-10-04 — func_80021B9C
 * Source:   cloud/matches/boot_tail/func_80021B9C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021B9C.c:func_80021B9C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021BF0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021C58.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021CDC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021D5C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021DF8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021E74.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021F08.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021F84.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022160.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022324.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800223D0.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022514.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022580.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800225FC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022678.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002279C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022874.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022A40.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022A98.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022CD4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022F24.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002300C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800230B0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023190.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002321C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800232A4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800233B0.s")
extern u8 func_800233B0(MacroState *, MacroCommand *, u32);
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023520.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023564.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023754.s")
/* PROMOTED 2026-10-04 — func_80023818
 * Source:   cloud/work/boot_tail_promotion/sources/func_80023818.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80023818.c:func_80023818 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MacroState_80023818 MacroState_80023818;
typedef struct MacroControl_80023818 MacroControl_80023818;
typedef struct MacroCommand_80023818 { u32 word[2]; } MacroCommand_80023818;
extern void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control, MacroCommand_80023818 *command, u32 flag);
u8 func_80023818(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xC4),
                  command, 0x00200000);
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023844.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023870.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002389C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800238C8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800238F4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023920.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002394C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023978.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800239A4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023AD4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023B50.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023BDC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023D70.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023DB8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023E9C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80024988.s")
