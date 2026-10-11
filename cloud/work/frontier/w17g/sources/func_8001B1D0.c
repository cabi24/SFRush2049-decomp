/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001B1D0 (static, boot 0x8001B1D0, 204 B, segment 0x1a660 -> src/rom/lib_1a660.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 48 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_transition_8001B1D0.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-transitions/nonmatch/func_8001B1D0.c (Astra / BT03-high-transitions).
 * Shaping devices: none. */
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
