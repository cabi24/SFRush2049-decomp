/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8002394C.c: file-local type names MacroCommand, MacroControl, MacroState suffixed _8002394C so several bodies share one ROM TU; no other change. */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_8002394C MacroState_8002394C;
typedef struct MacroControl_8002394C MacroControl_8002394C;
typedef struct MacroCommand_8002394C {
    u32 word[2];
} MacroCommand_8002394C;

extern void func_80023754(MacroState_8002394C *state, MacroControl_8002394C *control,
                         MacroCommand_8002394C *command, u32 flag);

u8 func_8002394C(MacroState_8002394C *state, MacroCommand_8002394C *command)
{
    func_80023754(state, (MacroControl_8002394C *)((u8 *)state + 0xE8),
                  command, 0x10000000);
    return 0;
}
