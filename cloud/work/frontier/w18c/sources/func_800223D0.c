/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800223D0 - lib_22300 (IDO 7.1). Macro command handler "add to pitch/position":
 * the signed 16-bit operand in word0 bits 16-31 is added to state->field28's whole part
 * (field28 is 17.15 fixed point, unaligned, packed state); the sum is clamped to 0..65535
 * and stored back as value << 15. Returns 0 (continue).
 * w18c: the operand is an s16 local (`delta = word0 >> 16`): IDO 7.1 narrows it in its own
 * register (srl/sll/sra on v0), as retail. D10's (s16)(parameter >> 16) into an s32 local gives
 * fresh temporaries (true=70). No shaping devices.
 * Types: the TU's view of this state is MacroState_8002245C (same fields, pack(1)); this file
 * keeps a private copy under its own name, as the neighbouring promoted handlers do. */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
#pragma pack(1)
typedef struct MacroState_800223D0 { u8 unknown00[0x28]; u32 field28; u16 field2C; u8 unknown2E[2]; u32 field30; u8 unknown34[4]; s32 field38; u8 unknown3C[0x14]; u16 field50; } MacroState_800223D0;
#pragma pack(0)
typedef struct MacroCommand_800223D0 { u32 word[2]; } MacroCommand_800223D0;

u8 func_800223D0(MacroState_800223D0 *state, MacroCommand_800223D0 *command)
{
    s16 delta;
    s32 value;
    delta = command->word[0] >> 16;
    value = (state->field28 >> 15) + delta;
    if (value < 0) {
        state->field28 = 0;
    } else if (value > 65535) {
        state->field28 = 0x7FFF8000;
    } else {
        state->field28 = (u32)value << 15;
    }
    return 0;
}
