/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro helper reconstruction; only observed state layout is modeled. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x4A];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[4];
    u16 note50;
    u8 unknown52[0x6E];
    s8 detuneC0;
    u8 lastNoteC1;
} MacroState;
#pragma pack()
extern u8 func_8002193C(MacroState *, MacroCommand *);
extern void func_80020F4C(u8, u8, u8);
u8 func_8002279C(MacroState *state, MacroCommand *command)
{
    state->note50 = state->lastNoteC1 + (s8)(u8)(command->word[0] >> 8);
    state->note50 = (s16)state->note50 < 0 ? 0 : state->note50 > 127 ? 127 : state->note50;
    state->detuneC0 = command->word[0] >> 16;
    if (state->channel4A != 255) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C(state, command);
}
