/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native fixed-point scaling; unsigned low-word arithmetic is intentional. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x30];
    u32 volume30;
    u32 originalVolume34;
} MacroState;
#pragma pack()
extern u32 func_8002321C(u32, u16);
u8 func_800232A4(MacroState *state, MacroCommand *command)
{
    u16 curve;
    if ((u8)(command->word[1] >> 8) == 0) {
        state->volume30 = ((u8)(command->word[0] >> 8) * state->volume30) >> 7;
    } else {
        state->volume30 = ((u8)(command->word[0] >> 8) * state->originalVolume34) >> 7;
    }
    state->volume30 += (u8)(command->word[0] >> 16) << 16;
    if (state->volume30 > 0x7F0000) {
        state->volume30 = 0x7F0000;
    }
    curve = ((u8)(command->word[0] >> 24) << 8) | (u8)command->word[1];
    state->volume30 = func_8002321C(state->volume30, curve);
    return 0;
}
