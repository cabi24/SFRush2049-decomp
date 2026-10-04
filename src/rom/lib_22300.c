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
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021F68.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80021F84.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022160.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022324.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800223D0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002243C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002245C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800224B0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022514.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022580.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800225AC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800225DC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800225FC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022678.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002279C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022874.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022A40.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022A78.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022A98.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022C58.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022CD4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80022F24.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002300C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800230B0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023190.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002321C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800232A4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_800233B0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023520.s")
/* PROMOTED 2026-10-04 — func_80023544
 * Source:   cloud/matches/boot_tail/func_80023544.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80023544.c:func_80023544 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 func_800233B0(MacroState *, MacroCommand *, u32);
u8 func_80023544(MacroState *state, MacroCommand *command)
{
    return func_800233B0(state, command, 0);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023564.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023710.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023734.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023754.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_80023818.s")
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
