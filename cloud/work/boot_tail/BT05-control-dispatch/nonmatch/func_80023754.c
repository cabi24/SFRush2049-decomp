/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Append a bounded controller entry; external translator is declared only. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
} MacroState;
typedef struct ControlEntry {
    u8 controller;
    u8 combination;
    s16 scale;
} ControlEntry;
typedef struct MacroControl {
    ControlEntry entries[4];
    u8 count;
} MacroControl;
#pragma pack()
extern u8 func_80021548(u8);
void func_80023754(MacroState *state, MacroControl *control,
                  MacroCommand *command, u32 flag)
{
    u8 combination;
    u8 index;
    combination = command->word[1];
    if (!(state->flags24 & flag) || combination == 0) {
        state->flags24 |= flag;
        control->count = 0;
    }
    if (control->count < 4) {
        index = control->count++;
        control->entries[index].controller = func_80021548(command->word[0] >> 8);
        control->entries[index].combination = combination;
        control->entries[index].scale = ((s32)(s16)(command->word[0] >> 16) * 256) / 100;
    }
}
