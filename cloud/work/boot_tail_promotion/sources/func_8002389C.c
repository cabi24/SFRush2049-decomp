/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8002389C.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _8002389C so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_8002389C MacroState_8002389C;
typedef struct MacroControl_8002389C MacroControl_8002389C;
typedef struct MacroCommand_8002389C {
    u32 word[2];
} MacroCommand_8002389C;

extern void func_80023754(MacroState_8002389C *state, MacroControl_8002389C *control,
                         MacroCommand_8002389C *command, u32 flag);

u8 func_8002389C(MacroState_8002389C *state, MacroCommand_8002389C *command)
{
    func_80023754(state, (MacroControl_8002389C *)((u8 *)state + 0x11E),
                  command, 0x01000000);
    return 0;
}
