/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80023920.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _80023920 so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_80023920 MacroState_80023920;
typedef struct MacroControl_80023920 MacroControl_80023920;
typedef struct MacroCommand_80023920 {
    u32 word[2];
} MacroCommand_80023920;

extern void func_80023754(MacroState_80023920 *state, MacroControl_80023920 *control,
                         MacroCommand_80023920 *command, u32 flag);

u8 func_80023920(MacroState_80023920 *state, MacroCommand_80023920 *command)
{
    func_80023754(state, (MacroControl_80023920 *)((u8 *)state + 0x154),
                  command, 0x02000000);
    return 0;
}
