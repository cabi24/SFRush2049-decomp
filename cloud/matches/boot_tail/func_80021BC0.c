/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro-handler reconstruction. State/command ABI is evidenced by
 * the resident dispatcher; original opcode names and complete types are unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x30];
    u32 field30;
    u8 unknown34[0x2C];
    u32 field60;
} MacroState;
#pragma pack(0)
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;

extern void func_8001EB10(MacroState *);
extern void func_8001F6EC(MacroState *);

u8 func_80021BC0(MacroState *state, MacroCommand *command)
{
    func_8001EB10(state);
    func_8001F6EC(state);
    return 1;
}
