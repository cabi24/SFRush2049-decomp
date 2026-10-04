/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native timed macro reconstruction; external helpers remain declared only. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    MacroCommand *start00;
    MacroCommand *current04;
    u8 unknown08[0x1C];
    u32 flags24;
    u8 unknown28[0x38];
    u32 identifier60;
    u8 unknown64[4];
    u16 loop68;
} MacroState;
#pragma pack()
extern u16 func_8001E790(void);
extern u8 func_8001467C(u32);
u8 func_80021F84(MacroState *state, MacroCommand *command)
{
    if (state->loop68 == 0) {
        if ((u8)(command->word[0] >> 16) & 1) {
            state->loop68 = func_8001E790() % (u16)(command->word[1] >> 16);
        } else {
            state->loop68 = command->word[1] >> 16;
        }
        if (state->loop68 == 0xFFFF) goto skip;
        ++state->loop68;
    } else if (state->loop68 == 0xFFFF) {
        goto skip;
    }
    if (--state->loop68 != 0) {
skip:
        if (((u8)(command->word[0] >> 8) & 1) != 0 &&
            !(state->flags24 & 0x40000000U) && (state->flags24 & 8)) {
            state->loop68 = 0;
        } else if (((u8)(command->word[0] >> 24) & 1) &&
                   !(state->flags24 & 0x20) && !func_8001467C(state->identifier60 & 255)) {
            state->loop68 = 0;
        } else {
            state->current04 = state->start00 + (u16)command->word[1];
        }
    }
    return 0;
}
