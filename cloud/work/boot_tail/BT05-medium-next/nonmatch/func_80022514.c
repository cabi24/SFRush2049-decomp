/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: bounded source-form residual; see packet README. */
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
typedef struct MacroState {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand *field1C;
    MacroCommand *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState;
typedef struct MacroControl {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl;
#pragma pack(0)

extern void func_8001EF8C(MacroState *, u8);

u8 func_80022514(MacroState *state, MacroCommand *command)
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
    func_8001EF8C(state, (u8)value);
    return 0;
}
