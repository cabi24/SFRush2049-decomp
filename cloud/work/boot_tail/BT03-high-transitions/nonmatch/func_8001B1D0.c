/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct Preset {
    u16 identifier;
    u16 high;
    u8 low;
    u8 middle;
    u8 default_first;
    u8 default_second;
    u8 flags;
    u8 parameter;
    u8 unknown0A[2];
} Preset;
#pragma pack(0)
extern Preset *func_80017040(u16);
extern u32 func_8001A270(u32, u8, u8, u8, u8, u8, u16, u16, u8, s16);
u32 func_8001B1D0(u16 identifier, u8 first, u8 second)
{
    Preset *preset;
    u32 result;
    result = 0xFFFFFFFFU;
    preset = func_80017040(identifier);
    if (preset != 0) {
        if (first == 255) first = preset->default_first;
        if (second == 255) second = preset->default_second;
        result = func_8001A270(((u32)preset->high << 16) |
            ((u32)preset->middle << 8) | preset->low,
            preset->flags | 128, first, second, 255, 255, 0, 255,
            preset->parameter, 0);
    }
    return result;
}
