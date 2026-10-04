/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800238F4.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _800238F4 so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_800238F4 MacroState_800238F4;
typedef struct MacroControl_800238F4 MacroControl_800238F4;
typedef struct MacroCommand_800238F4 {
    u32 word[2];
} MacroCommand_800238F4;

extern void func_80023754(MacroState_800238F4 *state, MacroControl_800238F4 *control,
                         MacroCommand_800238F4 *command, u32 flag);

u8 func_800238F4(MacroState_800238F4 *state, MacroCommand_800238F4 *command)
{
    func_80023754(state, (MacroControl_800238F4 *)((u8 *)state + 0x142),
                  command, 0x04000000);
    return 0;
}
