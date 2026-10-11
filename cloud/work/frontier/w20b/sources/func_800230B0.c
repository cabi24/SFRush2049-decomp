/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800230B0 - lib_22300 (IDO 7.1). Macro command "pan sweep": sets voice flag 0x10000, takes the
 * sweep time from word[0] bits 16..31 (stored at +0xA8, then converted in place by func_8001E930), the
 * start pan from word[0] bits 8..15 (<< 16, +0x38), a signed 8-bit pan offset from word[1]; target =
 * start + (offset << 16) (+0xAC), per-tick delta = (offset << 16) / time, or the whole offset when the
 * converted time is 0 (+0x3C). Returns 0.
 * w20b levers: (1) `offset = word[1] & 0xFF` BEFORE the start-pan store (retail loads word[1] right after
 * word[0]; after it, as1 gives the 7-row schedule w18c/w19b had at the right frame); (2) frame.
 * UNUSED LOCALS (disclosed, exact frame residual): `unused0`, `unused1` declared first. Retail frame 40;
 * without them 32 (arg area 16, ra +20, time spilled at +28); two 4-byte slots -> 40. Under 7.1 locals
 * declared before the used ones take slots (declared after them they do not: w19b's "frame never moves"
 * probes). A used local in their place (`delta`) changes the target sum's operand order (true=10).
 * Types: the TU's MacroCommand; private packed VoiceState_800230B0 view (the TU's VoiceState lacks these
 * fields). No other devices. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct VoiceState_800230B0 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x10];
    u32 pan38;
    u32 panDelta3C;
    u8 unknown40[0x68];
    u32 panTimeA8;
    u32 panTargetAC;
} VoiceState_800230B0;
#pragma pack(0)
extern void func_8001E930(u32 *);

u8 func_800230B0(VoiceState_800230B0 *voice, MacroCommand *command)
{
    u32 unused0, unused1;
    u32 time;
    s8 offset;
    voice->flags24 |= 0x10000;
    time = (command->word[0] >> 16) & 65535;
    voice->panTimeA8 = time;
    func_8001E930(&voice->panTimeA8);
    offset = command->word[1] & 0xFF;
    voice->pan38 = ((command->word[0] >> 8) & 255) << 16;
    voice->panTargetAC = ((u32)offset << 16) + voice->pan38;
    if (voice->panTimeA8 != 0) {
        voice->panDelta3C = ((u32)offset << 16) / time;
    } else {
        voice->panDelta3C = (u32)offset << 16;
    }
    return 0;
}
