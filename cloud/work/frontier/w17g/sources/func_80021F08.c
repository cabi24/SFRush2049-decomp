/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021F08 (static, boot 0x80021F08, 96 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 9 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80021F08.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-medium-next/nonmatch/func_80021F08.c (Astra / BT05-medium-next).
 * w17g TU-context edits (codegen-neutral, re-proved): type MacroState->MacroState_80021F08.
 * Shaping devices: none. */
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
typedef struct MacroState_80021F08 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand *field1C;
    MacroCommand *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState_80021F08;
typedef struct MacroControl {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl;
#pragma pack(0)

extern MacroCommand *func_80016C20(u16);

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
