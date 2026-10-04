/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro reconstruction; unknown object ranges are not stack padding. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x22];
    u8 channel4A;
    u8 set4B;
    u8 unknown4C[0x44];
    u32 time90;
    u8 unknown94[4];
    u8 mode98;
} MacroState;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
extern void func_80020FDC(u8, u8, u8);
extern void func_80019BE4(MacroState *);
u8 func_800239A4(MacroState *state, MacroCommand *command)
{
    u32 time;
    state->mode98 = command->word[0] >> 16;
    time = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&time);
    } else {
        func_8001E940(&time, state);
    }
    state->time90 = time;
    switch ((u8)(command->word[0] >> 8)) {
    case 0:
        state->flags24 &= ~0x800;
        state->flags24 |= 0x1000;
        if (state->channel4A != 255) func_80020FDC(state->channel4A, state->set4B, 0);
        break;
    case 1:
        if (!(state->flags24 & 0x800)) func_80019BE4(state);
        state->flags24 |= 0x1800;
        break;
    }
    return 0;
}
