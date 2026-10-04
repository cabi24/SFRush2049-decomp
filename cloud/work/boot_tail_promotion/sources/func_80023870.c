/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80023870.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _80023870 so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_80023870 MacroState_80023870;
typedef struct MacroControl_80023870 MacroControl_80023870;
typedef struct MacroCommand_80023870 {
    u32 word[2];
} MacroCommand_80023870;

extern void func_80023754(MacroState_80023870 *state, MacroControl_80023870 *control,
                         MacroCommand_80023870 *command, u32 flag);

u8 func_80023870(MacroState_80023870 *state, MacroCommand_80023870 *command)
{
    func_80023754(state, (MacroControl_80023870 *)((u8 *)state + 0xFA),
                  command, 0x00800000);
    return 0;
}
