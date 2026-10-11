/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80021F84 - lib_22300 (IDO 7.1). Macro "loop" command: the loop counter at +0x68 starts
 * at word1 bits 16-31 (or a random count below it, word0 bit 16; func_8001E790) plus one; 0xFFFF
 * loops forever. While the counter has not run out, jump back to command index word1 bits 0-15
 * (current = start + index), unless the jump is conditional and its condition fails: word0
 * bit 8 = only while the voice is not releasing (flags24 & 8 without 0x40000000), bit 24 = only
 * while its sample is still playing (flag 0x20 or func_8001467C); a failed condition clears the
 * counter. Returns 0 (continue). Reached through the macro dispatch table.
 * Types: the TU's MacroCommand (func_80021B9C's block); the voice through a packed private view
 * MacroState_80021F84 (D10's; retail's lwl/lwr/swl/swr); func_8001E790 as the TU declares it and
 * func_8001467C as its locked definition (lib_11640).
 * w19a: from D10 BT05-timed-pair (5.3; 7.1 true=1125): each refusal returns 0 where it clears the
 * counter (retail's per-path `move v0,zero`). Shaping device (disclosed): the jump index is
 * `command->word[1] & 0xFFFF` - retail loads the word and masks it (andi); `(u16)word[1]` folds
 * to lhu under 7.1 (true=745), as w18c's func_80023D70 / func_80023754. D10's `goto skip` into
 * the countdown branch kept (shared jump path for the forever loop). No unused locals. */
#include "types.h"
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_80021F84 {
    MacroCommand *start00;
    MacroCommand *current04;
    u8 unknown08[0x1C];
    u32 flags24;
    u8 unknown28[0x38];
    u32 identifier60;
    u8 unknown64[4];
    u16 loop68;
} MacroState_80021F84;
#pragma pack()
extern u16 func_8001E790(void);
extern u8 func_8001467C(int);
u8 func_80021F84(MacroState_80021F84 *state, MacroCommand *command)
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
            return 0;
        }
        if (((u8)(command->word[0] >> 24) & 1) &&
            !(state->flags24 & 0x20) && !func_8001467C(state->identifier60 & 255)) {
            state->loop68 = 0;
            return 0;
        }
        state->current04 = state->start00 + (command->word[1] & 0xFFFF);
    }
    return 0;
}
