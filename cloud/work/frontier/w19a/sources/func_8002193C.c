/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002193C - lib_22300 (IDO 7.1). Macro "wait" command core: wait word1 bits 16-31 ticks
 * (0 = do not wait: returns 0). word0 bit 8 makes the wait cancellable (flags24 |= 4; refused
 * when the voice is already releasing, flag 8), bit 24 ties it to the voice's sample (flag
 * 0x100000; refused unless flag 0x20 or the sample channel is still active, func_8001467C),
 * bit 16 randomises the length (func_8001E790() % time). 0xFFFF waits forever (deadline -1);
 * otherwise the length is converted (word1 bit 8: milliseconds, func_8001E930; else ticks,
 * func_8001E940) and the deadline set from the clock (+0xA4) or the running base (+0xA0; a
 * deadline already in the past advances the base and expires at once). Returns deadline != 0.
 * Callers: func_80021B9C, func_800225FC, func_80022678, func_8002279C (lib_22300, locked).
 * Types: the TU's MacroState / MacroCommand (func_80021B9C's block; that block also declares this
 * function's prototype with them, so the TU edit `tools/tu_edits.py` hoists those two typedef
 * lines above this slot - proven byte-identical alone). The fields are read through a packed
 * private view MacroState_8002193C (retail's lwl/lwr) by a cast at each use (type view, no
 * code); func_8001467C / func_8001E930 / func_8001E940 as the TU and the locked definitions
 * declare them.
 * w19a: from D10 BT05-core-pair (5.3; 7.1 true=1080): no `result` local - `return
 * deadline9C != 0` at the end of the wait branch and `return 0` after it (retail duplicates the
 * compare-and-return into each path). No shaping devices, no unused locals. */
#include "types.h"
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_8002193C { u8 unknown00[0x24]; u32 flags24; u8 unknown28[0x38]; u32 identifier60; u8 unknown64[0x38]; u32 deadline9C; u32 baseA0; u32 clockA4; } MacroState_8002193C;
#pragma pack()
extern u8 func_8001467C(int);
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
            if (((MacroState_8002193C *)state)->flags24 & 8) return 0;
            ((MacroState_8002193C *)state)->flags24 |= 4;
        } else {
            ((MacroState_8002193C *)state)->flags24 &= ~4U;
        }
        if ((u8)(command->word[0] >> 24) & 1) {
            if (!(((MacroState_8002193C *)state)->flags24 & 0x20) && !func_8001467C(((MacroState_8002193C *)state)->identifier60 & 255)) return 0;
            ((MacroState_8002193C *)state)->flags24 |= 0x100000;
        } else {
            ((MacroState_8002193C *)state)->flags24 &= ~0x100000U;
        }
        if ((u8)(command->word[0] >> 16) & 1) time = func_8001E790() % time;
        if (time != 0xFFFF) {
            milliseconds = (u8)(command->word[1] >> 8) & 1;
            if (milliseconds) func_8001E930(&time);
            else func_8001E940(&time, state);
            if (milliseconds) {
                ((MacroState_8002193C *)state)->deadline9C = ((MacroState_8002193C *)state)->clockA4 + time;
            } else {
                ((MacroState_8002193C *)state)->deadline9C = ((MacroState_8002193C *)state)->baseA0 + time;
                if (((MacroState_8002193C *)state)->deadline9C <= ((MacroState_8002193C *)state)->clockA4) {
                    ((MacroState_8002193C *)state)->baseA0 = ((MacroState_8002193C *)state)->deadline9C;
                    ((MacroState_8002193C *)state)->deadline9C = 0;
                }
            }
        } else {
            ((MacroState_8002193C *)state)->deadline9C = 0xFFFFFFFFU;
        }
        return ((MacroState_8002193C *)state)->deadline9C != 0;
    }
    return 0;
}
