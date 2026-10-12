/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001A270 (lib_1a660, IDO 7.1): MusyX synthStartSound with StartKeymap inlined (D10 4745). w22c from w21b's
 * best_true0.c (code-identical, frame 104 vs 96). trace71 spill71: cfe local area 32 vs retail 28, i.e. one 4-byte
 * local too many. The keymap entry index is not a variable: `keymap[(u8)(key & 127)]` is CSE'd into the same web
 * the `u8 index` local had (same code), and the panning sum `pan_sum` (kept as a two-statement int so the add order
 * is retail's) takes the slot range index occupied, so the six slots end at -24 like retail's.
 * No devices, no unused locals. Keymap is the 8-byte packed MusyX entry (byte loads of priority need pack(1)). */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct Keymap {
    u16 id;
    s8 transpose;
    u8 panning;
    s16 priority;
    u8 unknown06[2];
} Keymap;
#pragma pack(0)
extern Keymap *func_80016EE0(u16);
extern u32 func_80019ED0(u8, u8, u8);
extern u32 func_80024988(u32, u16, u8, u8, u8, u8, u8, u16, u16, u8, u8);
extern u32 func_80019F48(u32, u16, u8, u8, u8, u8, u8, u16, u16, u8);
u32 func_8001A270(u32 packed, u8 key, u8 volume, u8 panning,
                 u8 channel, u8 set, u16 offset, u16 section, u8 group, s16 priorityOffset)
{
    Keymap *keymap;
    u16 id;
    int pan;
    int pan_sum;
    int priority;
    int note;
    id = packed >> 16;
    priority = (int)((packed >> 8) & 255) + priorityOffset;
    priority = priority < 0 ? 0 : priority > 255 ? 255 : priority;
    packed = (packed & 0xFFFF00FFU) | ((u32)priority << 8);
    switch (id & 0xC000) {
    case 0:
        priority = func_80019ED0(key, channel, set);
        if (priority != 0xFFFFFFFFU) return priority;
        return func_80024988(packed, packed >> 16, key, volume, panning, channel, set, offset, section, 1, group);
    case 0x4000:
        keymap = func_80016EE0(id);
        if (keymap != 0) {
            if ((pan = keymap[(u8)(key & 127)].id) != 0xFFFF) {
                if ((keymap[(u8)(key & 127)].panning & 128) == 0) {
                    pan_sum = keymap[key].panning - 64;
                    pan_sum += panning;
                    if (pan_sum < 0) panning = 0;
                    else if (pan_sum > 127) panning = 127;
                    else panning = pan_sum;
                } else panning = 128;
                note = (key & 127) + keymap[(u8)(key & 127)].transpose;
                note = note > 127 ? 127 : note < 0 ? 0 : note;
                priority += keymap[(u8)(key & 127)].priority;
                priority = priority > 255 ? 255 : priority < 0 ? 0 : priority;
                packed &= 255;
                packed |= (u32)priority << 8;
                packed |= (u32)pan << 16;
                if ((pan & 0xC000) == 0) {
                    priority = func_80019ED0(note, channel, set);
                    if (priority != 0xFFFFFFFFU) return priority;
                    return func_80024988(packed, id, note | (key & 128), volume, panning, channel, set, offset, section, 1, group);
                }
                return func_80019F48(packed, id, note | (key & 128), volume, panning, channel, set, offset, section, group);
            }
        }
        break;
    case 0x8000:
        return func_80019F48(packed, packed >> 16, key, volume, panning, channel, set, offset, section, group);
    }
    return 0xFFFFFFFFU;
}
