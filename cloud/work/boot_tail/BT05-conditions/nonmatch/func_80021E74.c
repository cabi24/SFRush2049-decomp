/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: bounded conditional/callee-lowering research. */
/* Native conditional macro transfer. Public MusyX names are source-family
 * context only; the packed N64 layout and byte status follow native evidence.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand {
    u32 word[2];
} MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    MacroCommand *field00;
    MacroCommand *field04;
    MacroCommand *field08;
    MacroCommand *field0C;
    u8 unknown10[0x20];
    u32 field30;
    u8 unknown34[0x16];
    u8 field4A;
    u8 unknown4B[5];
    u16 field50;
} MacroState;
#pragma pack(0)
extern MacroCommand *func_80016C20(u16);

extern u8 func_80021BC0(MacroState *, MacroCommand *);

u8 func_80021E74(MacroState *state, MacroCommand *command)
{
    MacroCommand *address;
    if ((address = func_80016C20(command->word[0] >> 16)) != 0) {
        state->field08 = state->field00;
        state->field0C = state->field04;
        state->field00 = address;
        state->field04 = address + (command->word[1] & 0xFFFF);
        return 0;
    }
    return func_80021BC0(state, command);
}
