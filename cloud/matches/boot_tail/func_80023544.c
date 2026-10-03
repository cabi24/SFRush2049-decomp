/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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

extern u8 func_800233B0(MacroState *, MacroCommand *, u32);

u8 func_80023544(MacroState *state, MacroCommand *command)
{
    return func_800233B0(state, command, 0);
}
