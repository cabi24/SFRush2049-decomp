/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022580 (static, boot 0x80022580, 44 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 2 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_80022580.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-wrappers/nonmatch/func_80022580.c (Astra / BT05-wrappers).
 * Shaping devices: none. */
/* Native macro-handler reconstruction. State/command ABI is evidenced by
 * the resident dispatcher; original opcode names and complete types are unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x30];
    u32 field30;
    u8 unknown34[0x2C];
    u32 field60;
} MacroState;
#pragma pack(0)
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;
extern void func_8001EF8C(MacroState *, u8);

u8 func_80022580(MacroState *state, MacroCommand *command)
{
    func_8001EF8C(state, (u8)(command->word[0] >> 8));
    return 0;
}
