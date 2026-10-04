/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct Layer {
    u16 id;
    u8 low;
    u8 high;
    s8 transpose;
    u8 volume;
    s16 priority;
    u8 panning;
    u8 unknown09[3];
} Layer;
typedef struct Voice {
    u8 unknown00[16];
    u32 child;
    u32 parent;
    u8 unknown18[392];
} Voice;
#pragma pack(0)
extern Layer *func_80016F80(u16, u16 *);
extern u32 func_80019ED0(u8, u8, u8);
extern u32 func_80024988(u32, u16, u8, u8, u8, u8, u8, u16, u16, u8, u8);
extern u32 func_8001ECE0(Voice *);
extern Voice D_8004BEB8[];
u32 func_80019F48(u32 packed, u16 allocation, u8 key, u8 volume,
                 u8 panning, u8 channel, u8 set, u16 offset, u16 section, u8 group)
{
    Layer *layer;
    u16 count;
    u32 result;
    u32 previous;
    u32 current;
    u32 nextPacked;
    int i;
    u8 masked;
    int note;
    int pan;
    int priority;
    u8 scaled;
    result = 0xFFFFFFFFU;
    layer = func_80016F80(packed >> 16, &count);
    if (layer != 0) {
        for (i = 0; i < count; i++, layer++) {
            masked = key & 127;
            if (layer->id == 0xFFFF || layer->low > masked || layer->high < masked) continue;
            note = masked + layer->transpose;
            note = note > 127 ? 127 : note < 0 ? 0 : note;
            current = func_80019ED0(note, channel, set);
            if (current != 0xFFFFFFFFU) return current;
            priority = (packed >> 8) & 255;
            if ((layer->panning & 128) == 0) {
                pan = layer->panning - 64;
                pan += panning;
                pan = pan < 0 ? 0 : pan > 127 ? 127 : pan;
            } else pan = 128;
            scaled = (volume * layer->volume) / 127;
            priority += layer->priority;
            priority = priority > 255 ? 255 : priority < 0 ? 0 : priority;
            nextPacked = (packed & 255) | ((u32)priority << 8) | ((u32)layer->id << 16);
            if ((layer->id & 0xC000) == 0) {
                if (result == 0xFFFFFFFFU) {
                    previous = func_80024988(nextPacked, allocation, note | (key & 128), scaled,
                        pan, channel, set, offset, section, 0, group);
                    if (previous != 0xFFFFFFFFU) result = func_8001ECE0(&D_8004BEB8[previous & 255]);
                } else {
                    current = func_80024988(nextPacked, allocation, note | (key & 128), scaled,
                        pan, channel, set, offset, section, 0, group);
                    if (current != 0xFFFFFFFFU) {
                        D_8004BEB8[previous & 255].child = current;
                        D_8004BEB8[current & 255].parent = previous;
                        previous = current;
                    }
                }
            }
        }
    }
    return result;
}
