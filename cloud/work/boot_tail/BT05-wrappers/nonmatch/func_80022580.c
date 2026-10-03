/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: bounded source-form residual; see packet README. */
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
