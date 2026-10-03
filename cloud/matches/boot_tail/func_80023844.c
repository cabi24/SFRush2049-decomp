/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState MacroState;
typedef struct MacroControl MacroControl;
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;

extern void func_80023754(MacroState *state, MacroControl *control,
                         MacroCommand *command, u32 flag);

u8 func_80023844(MacroState *state, MacroCommand *command)
{
    func_80023754(state, (MacroControl *)((u8 *)state + 0xD6),
                  command, 0x00400000);
    return 0;
}
