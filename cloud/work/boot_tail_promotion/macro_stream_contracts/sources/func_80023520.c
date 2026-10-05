/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Source-contract repair: shared helper pointers retain the accepted MacroState
 * and MacroCommand ABI. Packed types below are observed views of that same
 * runtime state; explicit pointer conversions do not alter layout or call ABI.
 * Original locked source is preserved. No promotion or coverage is claimed. */
/* Native macro-handler reconstruction. State/command ABI is evidenced by
 * the resident dispatcher; original opcode names and complete types are unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80023520 {
    u8 unknown00[0x30];
    u32 field30;
    u8 unknown34[0x2C];
    u32 field60;
} MacroState_80023520;
#pragma pack(0)

extern u8 func_800233B0(MacroState *, MacroCommand *, u32);

u8 func_80023520(MacroState_80023520 *state, MacroCommand *command)
{
    return func_800233B0((MacroState *)state, command, state->field30);
}
