/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro helper reconstruction; only observed state layout is modeled. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
typedef struct MacroState {
    u8 unknown00[0x40];
    u32 sweepOffset40[2];
    u8 sweepNumber48[2];
    u8 unknown4A[0x32];
    u32 sweepAddition7C[2];
    u8 unknown84[0x2C];
    u8 sweepCountB0[2];
} MacroState;
extern u32 func_80014D30(u32);
extern u8 func_8002193C(MacroState *, MacroCommand *);
u8 func_8002300C(MacroState *state, MacroCommand *command, int index)
{
    s32 delta;
    state->sweepOffset40[index] = 0;
    state->sweepNumber48[index] = (u8)(command->word[0] >> 8);
    state->sweepCountB0[index] = state->sweepNumber48[index];
    delta = (s16)(command->word[0] >> 16);
    if (delta >= 0) {
        delta = (s32)func_80014D30(delta);
    } else {
        delta = (s32)(0U - func_80014D30(-delta));
    }
    state->sweepAddition7C[index] = (u32)delta << 16;
    command->word[0] = 0;
    return func_8002193C(state, command);
}
