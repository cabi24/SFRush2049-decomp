/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021C58 (static, boot 0x80021C58, 132 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 21 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80021C58.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-conditions/nonmatch/func_80021C58.c (Astra / BT05-conditions).
 * w17g TU-context edits (codegen-neutral, re-proved): type MacroState->MacroState_80021C58.
 * Shaping devices: none. */
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
typedef struct MacroState_80021C58 {
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
} MacroState_80021C58;
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
