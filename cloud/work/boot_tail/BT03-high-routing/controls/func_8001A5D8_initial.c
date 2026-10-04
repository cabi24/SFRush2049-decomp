/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
    u32 result;
    u32 fraction;
    result = func_8001E50C((key >> 16) & 255, state->pitch5C) << 16;
    fraction = key & 65535;
    if (fraction != 0) {
        u32 whole;
        whole = result >> 16;
        result += (func_8001E440(whole) - whole) * fraction;
    }
    return result;
}
