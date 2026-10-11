/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800232A4 - lib_22300 (IDO 7.1). Macro command "set volume": scale the voice volume (8.16)
 * by the command's byte operand / 128 - from the current volume when word1 bits 8-15 are 0,
 * else from the original volume (+0x34) - add the second byte operand as a whole step, clamp to
 * 0x7F0000, and map it through the curve CURVE = (word0 >> 24) << 8 | (word1 & 0xFF) with
 * func_8002321C (locked, same TU). Returns 0 (continue).
 * w18c: CURVE is built in two statements (`curve = (u8)word[1]; curve |= ... << 8`): the u16
 * variable then lives in a1 and is narrowed in its register, as retail; D10's one-expression
 * form folds the low byte to `lbu 7(a3)` (true=445). No shaping devices.
 * Types: private packed view MacroState_800232A4 (volume30, originalVolume34; the TU's
 * MacroState has neither); MacroCommand is the TU's typedef (verbatim).
 * Prior: D10 BT05-larger-pair (5.3, 7 words). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800232A4 {
    u8 unknown00[0x30];
    u32 volume30;
    u32 originalVolume34;
} MacroState_800232A4;
#pragma pack()
extern u32 func_8002321C(u32, u16);

u8 func_800232A4(MacroState_800232A4 *state, MacroCommand *command)
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
    curve = (u8)command->word[1];
    curve |= (u8)(command->word[0] >> 24) << 8;
    state->volume30 = func_8002321C(state->volume30, curve);
    return 0;
}
