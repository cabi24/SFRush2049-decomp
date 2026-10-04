/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800238C8.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _800238C8 so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_800238C8 MacroState_800238C8;
typedef struct MacroControl_800238C8 MacroControl_800238C8;
typedef struct MacroCommand_800238C8 {
    u32 word[2];
} MacroCommand_800238C8;

extern void func_80023754(MacroState_800238C8 *state, MacroControl_800238C8 *control,
                         MacroCommand_800238C8 *command, u32 flag);

u8 func_800238C8(MacroState_800238C8 *state, MacroCommand_800238C8 *command)
{
    func_80023754(state, (MacroControl_800238C8 *)((u8 *)state + 0x130),
                  command, 0x08000000);
    return 0;
}
