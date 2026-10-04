/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct Keymap {
    u16 id;
    u32 packed;
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
u32 func_8001A270(u32 parameters, u8 key, u8 volume, u8 panning,
                 u8 channel, u8 set, u16 offset, u16 section, u8 group, s16 priorityOffset)
{
    u16 id;
    u32 packed;
    Keymap *keymap;
    u32 result;
    int priority;
    int pan;
    int note;
    u8 index;
    packed = parameters;
    id = packed >> 16;
    priority = ((packed >> 8) & 255) + priorityOffset;
    priority = priority < 0 ? 0 : priority > 255 ? 255 : priority;
    packed = (packed & 0xFFFF00FFU) | ((u32)priority << 8);
    switch (id & 0xC000) {
    case 0:
        result = func_80019ED0(key, channel, set);
        if (result != 0xFFFFFFFFU) return result;
        return func_80024988(packed, packed >> 16, key, volume, panning, channel, set, offset, section, 1, group);
    case 0x4000:
        keymap = func_80016EE0(id);
        if (keymap != 0) {
            packed &= 255;
            index = key & 127;
            if (keymap[index].id != 0xFFFF) {
                if ((keymap[index].panning & 128) == 0) {
                    pan = keymap[key].panning - 64;
                    pan += panning;
                    if (pan < 0) panning = 0;
                    else if (pan > 127) panning = 127;
                    else panning = pan;
                } else panning = 128;
                note = (key & 127) + keymap[index].transpose;
                note = note > 127 ? 127 : note < 0 ? 0 : note;
                priority += keymap[index].priority;
                priority = priority > 255 ? 255 : priority < 0 ? 0 : priority;
                packed |= ((u32)priority << 8) | ((u32)keymap[index].id << 16);
                if ((keymap[index].id & 0xC000) == 0) {
                    result = func_80019ED0(note, channel, set);
                    if (result != 0xFFFFFFFFU) return result;
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
