/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Source-contract repair: shared helper pointers retain the accepted MacroState
 * and MacroCommand ABI. Packed types below are observed views of that same
 * runtime state; explicit pointer conversions do not alter layout or call ABI.
 * Original locked source is preserved. No promotion or coverage is claimed. */
/* Native macro-handler reconstruction. Field names and complete state layout
 * remain unknown; no authenticated arcade counterpart is available here.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80021BF0 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand *field1C;
    MacroCommand *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState_80021BF0;
typedef struct MacroControl_80021BF0 {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl_80021BF0;
#pragma pack(0)

extern u8 func_80021BC0(MacroState *, MacroCommand *);

u8 func_80021BF0(MacroState_80021BF0 *state, MacroCommand *command)
{
    if (((command->word[0] >> 8) & 0xFF) == 0 || state->field08 == 0) {
        return func_80021BC0((MacroState *)state, command);
    }
    state->field00 = state->field08;
    state->field04 = state->field0C;
    return 0;
}
