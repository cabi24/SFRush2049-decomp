/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021E74 (static, boot 0x80021E74, 148 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 16 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80021E74.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-conditions/nonmatch/func_80021E74.c (Astra / BT05-conditions).
 * w17g TU-context edits (codegen-neutral, re-proved): this file's private struct view is renamed MacroState_80021E74
 * (the TU already defines a different MacroState); func_80021BC0 keeps the TU's prototype `extern u8 func_80021BC0(MacroState *, MacroCommand *);` with the TU's MacroState typedef copied verbatim, and the call casts `state`.
 * Shaping devices: the (MacroState *) argument cast above (type view only, no code).
 * Previously listed shaping devices: none. */
/* Native conditional macro transfer. Public MusyX names are source-family
 * context only; the packed N64 layout and byte status follow native evidence.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;
#pragma pack(1)
typedef struct MacroState_80021E74 {
    MacroCommand *field00;
    MacroCommand *field04;
    MacroCommand *field08;
    MacroCommand *field0C;
    u8 unknown10[0x20];
    u32 field30;
    u8 unknown34[0x16];
    u8 field4A;
    u8 unknown4B[5];
    u16 field50;
} MacroState_80021E74;
#pragma pack(0)
extern MacroCommand *func_80016C20(u16);


typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
extern u8 func_80021BC0(MacroState *, MacroCommand *);
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
