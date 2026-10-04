/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80022C58.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _80022C58 so several bodies share one ROM TU; no other change. */
/* Native macro-handler reconstruction. Field names and complete state layout
 * remain unknown; no authenticated arcade counterpart is available here.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand_80022C58 {
    u32 word[2];
} MacroCommand_80022C58;
#pragma pack(1)
typedef struct MacroState_80022C58 {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u8 unknown10[0xC];
    MacroCommand_80022C58 *field1C;
    MacroCommand_80022C58 *field20;
    u8 unknown24[0xA];
    u8 field2E;
} MacroState_80022C58;
typedef struct MacroControl_80022C58 {
    u32 field0;
    u32 field4;
    u32 field8;
} MacroControl_80022C58;
#pragma pack(0)

extern void func_8001E930(u32 *);

u8 func_80022C58(MacroState_80022C58 *state, MacroCommand_80022C58 *command)
{
    u32 duration;
    u8 index;
    MacroControl_80022C58 *control;
    index = command->word[0] >> 8;
    duration = (u16)(command->word[0] >> 16);
    func_8001E930(&duration);
    control = (MacroControl_80022C58 *)((u8 *)state + 0x168) + index;
    if (control->field4 != 0) {
        control->field0 = 0;
    }
    control->field4 = duration;
    return 0;
}
