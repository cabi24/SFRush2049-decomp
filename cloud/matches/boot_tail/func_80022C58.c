/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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

extern void func_8001E930(u32 *);

u8 func_80022C58(MacroState *state, MacroCommand *command)
{
    u32 duration;
    u8 index;
    MacroControl *control;
    index = command->word[0] >> 8;
    duration = (u16)(command->word[0] >> 16);
    func_8001E930(&duration);
    control = (MacroControl *)((u8 *)state + 0x168) + index;
    if (control->field4 != 0) {
        control->field0 = 0;
    }
    control->field4 = duration;
    return 0;
}
