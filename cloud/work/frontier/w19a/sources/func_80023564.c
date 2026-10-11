/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023564 - lib_22300 (IDO 7.1). Macro command "random note": pick a note range from
 * the command (word0 bits 8-15 / 24-31; absolute and swapped into order when word1 bits 8-15
 * are 0, else relative to the voice's current note +0x50 as signed 16-bit bounds clamped to
 * 0..127), a detune (random -100..100 via func_8001E790 when word1's low byte is set, else
 * word0 bits 16-23), then rewrite the command in place as a note-on (0x19 | note << 8 |
 * detune << 16, word1 = 0) with a random note in the range and run it (func_800225FC).
 * Returns 0. Reached through the macro dispatch table (no direct callers).
 * Types: the TU's MacroCommand (func_80021B9C's block), func_800225FC's packed state view
 * MacroState_800225FC and its prototype (verbatim, deduplicated by --prefix-dedup), and the
 * TU's func_8001E790 declaration (func_80021DF8's block).
 * w19a: from D10 BT05-larger-pair (5.3; 7.1 true=705): word1's low byte is tested where it
 * is used (no `randomDetune` local), the upper clamp narrows its arm (`(u8)lower`, retail's
 * in-arm andi), and the command word is built note-first. No shaping devices, no unused
 * locals. */
#include "types.h"
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800225FC { u8 unknown00[0x4A]; u8 channel4A; u8 set4B; u8 unknown4C[4]; u16 note50; u8 unknown52[0x6E]; s8 detuneC0; u8 lastNoteC1; } MacroState_800225FC;
#pragma pack()
extern u16 func_8001E790(void);
extern u8 func_800225FC(MacroState_800225FC *state, MacroCommand *command);
u8 func_80023564(MacroState_800225FC *state, MacroCommand *command)
{
    u8 low;
    u8 high;
    u8 temporary;
    s16 lower;
    s16 upper;
    u8 detune;
    if ((u8)(command->word[1] >> 8) == 0) {
        low = command->word[0] >> 8;
        high = command->word[0] >> 24;
        if (low > high) {
            temporary = low;
            low = high;
            high = temporary;
        }
    } else {
        lower = state->note50 - (u8)(command->word[0] >> 8);
        upper = state->note50 + (u8)(command->word[0] >> 24);
        low = lower < 0 ? 0 : lower > 127 ? 127 : (u8)lower;
        high = upper < 0 ? 0 : upper > 127 ? 127 : (u8)upper;
    }
    if (command->word[1] & 0xFF) {
        detune = (func_8001E790() % 201) - 100;
    } else {
        detune = command->word[0] >> 16;
    }
    command->word[0] = ((low + (func_8001E790() % ((high - low) + 1))) * 256) | 0x19 | (detune << 16);
    command->word[1] = 0;
    func_800225FC(state, command);
    return 0;
}
