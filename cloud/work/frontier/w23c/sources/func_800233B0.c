/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800233B0 - lib_22300 (IDO 7.1). Macro command: start a volume envelope (MusyX synthmacros.c envelope
 * command shape; behaviour only): the time field (command word 1 >> 16) goes to state+0xB4 and is converted by
 * func_8001E930 or func_8001E940 (by flag bit 8 of word 1), then func_8001E9A0 (time / 256) gives the step count
 * (at least 1); the target = word0 byte 1 scaled by the current volume (state+0x30) / 128 plus word0 byte 2 << 16,
 * clamped to 0x7F0000 and shaped by the curve (word1 low byte | word0 top byte << 8) through func_8002321C; the
 * target, per-step delta (target - START) / steps and START are stored and flag 0x20000 is set. Returns 0.
 * w23c (D10 BT05-larger-trio true=610): the curve is built as its sibling func_800232A4 (locked) does,
 * `curve = (u8)command->word[1]; curve |= (u8)(command->word[0] >> 24) << 8;` (the `|=` keeps the variable as
 * the first `or` operand and the word load in a1: 610 -> 0). No devices, no unused locals.
 * Types: the TU's MacroState/MacroCommand (the TU prototypes `u8 func_800233B0(MacroState *, MacroCommand *, u32)`
 * right after the slot); the voice fields are read through a private packed view MacroState_800233B0 by the cast
 * macro VOICE_233B0 (type view, no code; as func_8001C860's EM). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800233B0 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[8];
    u32 volume30;
    u8 unknown34[0x50];
    s32 delta84;
    u32 target88;
    u8 unknown8C[0x28];
    u32 timeB4;
} MacroState_800233B0;
#pragma pack()
extern void func_8001E930(u32 *);
extern void func_8001E940(u32 *, MacroState *);
extern u32 func_8001E9A0(u32);
extern u32 func_8002321C(u32, u16);

#define VOICE_233B0 ((MacroState_800233B0 *)state)
u8 func_800233B0(MacroState *state, MacroCommand *command, u32 start)
{
    u32 target;
    s32 milliseconds;
    u16 curve;
    VOICE_233B0->timeB4 = (u16)(command->word[1] >> 16);
    if ((u8)(command->word[1] >> 8) & 1) {
        func_8001E930(&VOICE_233B0->timeB4);
    } else {
        func_8001E940(&VOICE_233B0->timeB4, state);
    }
    milliseconds = func_8001E9A0(VOICE_233B0->timeB4);
    if (milliseconds == 0) milliseconds = 1;
    target = ((u8)(command->word[0] >> 8) * VOICE_233B0->volume30) >> 7;
    target += (u8)(command->word[0] >> 16) << 16;
    if (target > 0x7F0000) target = 0x7F0000;
    curve = (u8)command->word[1];
    curve |= (u8)(command->word[0] >> 24) << 8;
    target = func_8002321C(target, curve);
    VOICE_233B0->target88 = target;
    VOICE_233B0->delta84 = (s32)(target - start) / milliseconds;
    VOICE_233B0->volume30 = start;
    VOICE_233B0->flags24 |= 0x20000;
    return 0;
}
