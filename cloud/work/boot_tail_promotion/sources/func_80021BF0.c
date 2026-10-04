/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80021BF0.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _80021BF0 so several bodies share one ROM TU; no other change. */
/* Native macro-handler reconstruction. Field names and complete state layout
 * remain unknown; no authenticated arcade counterpart is available here.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand_80021BF0 {
    u32 word[2];
} MacroCommand_80021BF0;
#pragma pack(1)
typedef struct MacroState_80021BF0 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand_80021BF0 *field1C;
    MacroCommand_80021BF0 *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState_80021BF0;
typedef struct MacroControl_80021BF0 {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl_80021BF0;
#pragma pack(0)

extern u8 func_80021BC0(MacroState_80021BF0 *, MacroCommand_80021BF0 *);

u8 func_80021BF0(MacroState_80021BF0 *state, MacroCommand_80021BF0 *command)
{
    if (((command->word[0] >> 8) & 0xFF) == 0 || state->field08 == 0) {
        return func_80021BC0(state, command);
    }
    state->field00 = state->field08;
    state->field04 = state->field0C;
    return 0;
}
