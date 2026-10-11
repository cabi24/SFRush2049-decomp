/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001A5D8 - lib_1a660 (IDO 7.1). Pitch conversion for a voice: KEY is 8.16 fixed point
 * (key >> 16 = note, low 16 bits = fraction). The whole-note value is
 * func_8001E50C(note, state->pitch5C) (lib_1ece0, unmatched) shifted to 16.16; a nonzero fraction
 * interpolates linearly towards func_8001E440(whole) (lib_1ece0, locked `u16 (u16)`):
 * result += (next - whole) * fraction.
 * w18c: no `whole` local (retail recomputes result >> 16 for both uses); locals declared
 * fraction-then-result, which gives retail's spill slots (result 0x20, fraction 0x24; the
 * other order swaps them: true=0 strict=16). No shaping devices beyond declaration order.
 * Prior: D10 BT03-high-routing (5.3, 19 words; 7.1 true=260). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoicePrefix { u8 unknown00[92]; u32 pitch5C; } VoicePrefix;
#pragma pack(0)
extern u32 func_8001E50C(u8, u32);
extern u16 func_8001E440(u16);
u32 func_8001A5D8(VoicePrefix *state, u32 key)
{
    u32 fraction;
    u32 result;
    result = func_8001E50C(key >> 16, state->pitch5C) << 16;
    fraction = key & 65535;
    if (fraction != 0) {
        result += (func_8001E440(result >> 16) - (result >> 16)) * fraction;
    }
    return result;
}
