/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Source-contract repair: shared helper pointers retain the accepted MacroState
 * and MacroCommand ABI. Packed types below are observed views of that same
 * runtime state; explicit pointer conversions do not alter layout or call ABI.
 * Original locked source is preserved. No promotion or coverage is claimed. */
/* Native macro helper reconstruction; only observed state layout is modeled. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800225FC {
    u8 unknown00[0x4A];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[4];
    u16 note50;
    u8 unknown52[0x6E];
    s8 detuneC0;
    u8 lastNoteC1;
} MacroState_800225FC;
#pragma pack()
extern u8 func_8002193C(MacroState *, MacroCommand *);
extern void func_80020F4C(u8, u8, u8);
extern int func_80021764(MacroState *);
u8 func_800225FC(MacroState_800225FC *state, MacroCommand *command)
{
    state->note50 = (u8)(command->word[0] >> 8) & 0x7F;
    state->detuneC0 = command->word[0] >> 16;
    if (func_80021764((MacroState *)state)) {
        func_80020F4C(state->channel4A, state->set4B, state->note50);
    }
    command->word[0] = 4;
    return func_8002193C((MacroState *)state, command);
}
