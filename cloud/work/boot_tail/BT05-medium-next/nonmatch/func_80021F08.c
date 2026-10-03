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

extern MacroCommand *func_80016C20(u16);

u8 func_80021F08(MacroState *state, MacroCommand *command)
{
    MacroCommand *start;
    start = func_80016C20((command->word[0] >> 16) & 0xFFFF);
    if (start != 0) {
        state->field1C = start;
        state->field20 = start + (command->word[1] & 0xFFFF);
    }
    return 0;
}
