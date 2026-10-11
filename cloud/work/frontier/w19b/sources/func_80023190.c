/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023190 - lib_22300 (IDO 7.1). Macro command handler: scales the distance between the
 * voice's 16-bit field50 (big-endian byte pair) and the command's byte 2 (both << 16) by the signed
 * byte-1 gain (>> 7), adds byte 3 << 16, clamps to [0, 0x7F0000] and stores it in field38; returns 0.
 * w19b: the command word is NOT a local: written as `command->word[0]` each time, uopt CSEs the one
 * `lw v0` and the s8 `gain = command->word[0] >> 8` becomes a coloured variable narrowed in place
 * (`srl a2,v0,8; sll a2,a2,24; sra a2,a2,24`); a `parameter` local gives ring temps (w18c true=160).
 * The clamp is one nested conditional expression: it gives retail's extra `b` over the empty else
 * (if/else forms: true=160..980). Statement order: value before gain (gain first: true=345).
 * Typedefs: the TU's MacroState_800223D0 (packed) / MacroCommand verbatim. No devices. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct MacroState_800223D0 { u8 unknown00[0x28]; u32 field28; u16 field2C; u8 unknown2E[2]; u32 field30; u8 unknown34[4]; s32 field38; u8 unknown3C[0x14]; u16 field50; } MacroState_800223D0;
#pragma pack(0)

u8 func_80023190(MacroState_800223D0 *state, MacroCommand *command)
{
    s32 value;
    s8 gain;
    value = ((u32)state->field50 << 16) - (((command->word[0] >> 16) & 0xFF) << 16);
    gain = command->word[0] >> 8;
    value = (s32)((u32)value * gain);
    value >>= 7;
    value += (u8)(command->word[0] >> 24) << 16;
    value = value < 0 ? 0 : (value > 0x7F0000 ? 0x7F0000 : value);
    state->field38 = value;
    return 0;
}
