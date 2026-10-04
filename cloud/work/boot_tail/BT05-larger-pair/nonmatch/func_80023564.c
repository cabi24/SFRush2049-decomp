/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native random-note command; signed16 relative bounds precede clamping. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x50];
    u16 note50;
} MacroState;
#pragma pack()
extern u16 func_8001E790(void);
extern u8 func_800225FC(MacroState *, MacroCommand *);
u8 func_80023564(MacroState *state, MacroCommand *command)
{
    u8 low;
    u8 high;
    u8 temporary;
    s16 lower;
    s16 upper;
    u8 detune;
    u32 randomDetune;
    randomDetune = command->word[1] & 0xFF;
    if ((u8)(command->word[1] >> 8) == 0) {
        low = command->word[0] >> 8;
        high = command->word[0] >> 24;
        if (low > high) {
            temporary = low;
            low = high;
            high = temporary;
        }
    } else {
        lower = state->note50 - (u8)(command->word[0] >> 8);
        upper = state->note50 + (u8)(command->word[0] >> 24);
        low = lower < 0 ? 0 : lower > 127 ? 127 : lower;
        high = upper < 0 ? 0 : upper > 127 ? 127 : upper;
    }
    if (randomDetune) {
        detune = (func_8001E790() % 201) - 100;
    } else {
        detune = command->word[0] >> 16;
    }
    command->word[0] = ((u8)detune << 16) | 0x19 |
        ((low + (func_8001E790() % ((high - low) + 1))) * 256);
    command->word[1] = 0;
    func_800225FC(state, command);
    return 0;
}
