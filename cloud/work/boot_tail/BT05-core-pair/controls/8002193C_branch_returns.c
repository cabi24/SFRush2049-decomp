/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native core macro reconstruction; genuine helper and storage contracts. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x38];
    u32 identifier60;
    u8 unknown64[0x38];
    u32 deadline9C;
    u32 baseA0;
    u32 clockA4;
} MacroState;
#pragma pack()
extern u8 func_8001467C(u32);
extern u16 func_8001E790(void);
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
u8 func_8002193C(MacroState *state, MacroCommand *command)
{
    u32 time;
    u8 milliseconds;
    time = (u16)(command->word[1] >> 16);
    if (time != 0) {
    if ((u8)(command->word[0] >> 8) & 1) {
        if (state->flags24 & 8) return 0;
        state->flags24 |= 4;
    } else {
        state->flags24 &= ~4U;
    }
    if ((u8)(command->word[0] >> 24) & 1) {
        if (!(state->flags24 & 0x20) && !func_8001467C(state->identifier60 & 255)) return 0;
        state->flags24 |= 0x100000;
    } else {
        state->flags24 &= ~0x100000U;
    }
    if ((u8)(command->word[0] >> 16) & 1) time = func_8001E790() % time;
    if (time != 0xFFFF) {
        milliseconds = (u8)(command->word[1] >> 8) & 1;
        if (milliseconds) func_8001E930(&time);
        else func_8001E940(&time, state);
        if (milliseconds) {
            state->deadline9C = state->clockA4 + time;
            return state->deadline9C != 0;
        } else {
            state->deadline9C = state->baseA0 + time;
            if (state->deadline9C <= state->clockA4) {
                state->baseA0 = state->deadline9C;
                state->deadline9C = 0;
            }
            return state->deadline9C != 0;
        }
    } else {
        state->deadline9C = 0xFFFFFFFFU;
        }
    }
    return 0;
}
