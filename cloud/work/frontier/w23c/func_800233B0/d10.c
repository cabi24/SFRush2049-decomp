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
    u8 unknown28[8];
    u32 volume30;
    u8 unknown34[0x50];
    s32 delta84;
    u32 target88;
    u8 unknown8C[0x28];
    u32 timeB4;
} MacroState;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
extern u32 func_8001E9A0(u32);
extern u32 func_8002321C(u32, u16);
u8 func_800233B0(MacroState *state, MacroCommand *command, u32 start)
{
    u32 target;
    s32 milliseconds;
    u16 curve;
    state->timeB4 = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&state->timeB4);
    } else {
        func_8001E940(&state->timeB4, state);
    }
    milliseconds = func_8001E9A0(state->timeB4);
    if (milliseconds == 0) milliseconds = 1;
    target = ((u8)(command->word[0] >> 8) * state->volume30) >> 7;
    target += (u8)(command->word[0] >> 16) << 16;
    if (target > 0x7F0000) target = 0x7F0000;
    curve = ((u8)(command->word[0] >> 24) << 8) | (u8)command->word[1];
    target = func_8002321C(target, curve);
    state->target88 = target;
    state->delta84 = (s32)(target - start) / milliseconds;
    state->volume30 = start;
    state->flags24 |= 0x20000;
    return 0;
}
