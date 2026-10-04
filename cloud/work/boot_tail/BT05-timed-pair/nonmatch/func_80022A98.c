/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native timed macro reconstruction; external helpers remain declared only. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x44];
    u32 currentTime6C;
    u32 period70;
    u8 unknown74[4];
    u8 keyRange78;
    u8 centRange79;
} MacroState;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
u8 func_80022A98(MacroState *state, MacroCommand *command)
{
    u32 time;
    s8 keyRange;
    s8 centRange;
    if ((u8)(command->word[0] >> 24) & 3) {
        state->flags24 |= 0x8000;
    } else {
        state->flags24 &= ~0x8000;
    }
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, state);
    }
    if (time) {
        state->flags24 |= 0x4000;
        state->period70 = time;
        keyRange = command->word[0] >> 8;
        centRange = command->word[0] >> 16;
        if (keyRange < 0) {
            if (centRange < 0) state->centRange79 = -centRange;
            else state->centRange79 = centRange;
            state->keyRange78 = -keyRange;
            state->currentTime6C = state->period70 / 2;
        } else {
            if (centRange < 0) {
                if (keyRange == 0) {
                    state->centRange79 = -centRange;
                    state->currentTime6C = state->period70 / 2;
                } else {
                    --keyRange;
                    state->centRange79 = 100 - centRange;
                    state->currentTime6C = 0;
                }
            } else {
                state->centRange79 = centRange;
                state->currentTime6C = 0;
            }
            state->keyRange78 = keyRange;
        }
    } else {
        state->flags24 &= ~0x4000;
    }
    return 0;
}
