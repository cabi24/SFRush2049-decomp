/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022514 (static, boot 0x80022514, 108 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 23 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80022514.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-medium-next/nonmatch/func_80022514.c (Astra / BT05-medium-next).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_80022514
 * (the TU already defines a different MacroState); func_8001EF8C keeps the TU's prototype `extern void func_8001EF8C(MacroState *, u8);` with the TU's MacroState typedef copied verbatim, and the call casts `state`.
 * Shaping devices: the (MacroState *) argument cast above (type view only, no code).
 * Previously listed shaping devices: none. */
/* Native macro-handler reconstruction. Field names and complete state layout
 * remain unknown; no authenticated arcade counterpart is available here.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;
#pragma pack(1)
typedef struct MacroState_80022514 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand *field1C;
    MacroCommand *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState_80022514;
typedef struct MacroControl {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl;
#pragma pack(0)


typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
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
