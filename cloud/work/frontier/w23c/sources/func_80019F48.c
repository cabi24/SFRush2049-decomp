/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80019F48 - lib_1a660 (IDO 7.1). MusyX synth.c StartLayer (older revision than the GameCube decomp
 * doldecomp/ttyd libs/musyx: Rush passes the packed id|prio|key word, no portamento check, only the
 * macro case of the layer-type switch). Fetch layer table PACKED>>16 (func_80016F80 = dataGetLayer, count
 * into COUNT); for each layer whose key range holds KEY & 127: note = key + transpose clamped 0..127; if
 * func_80019ED0 (lib_1a660, locked) finds a voice already on that note, return it; else pan = layer pan - 64
 * + PANNING clamped (128 = surround), volume scaled by the layer volume / 127, priority = packed prio +
 * layer offset clamped 0..255; start the macro (func_80024988 = macStart, locked) and chain the voices: the
 * first becomes the result through func_8001ECE0 (vidMakeRoot), later ones are linked child/parent in
 * D_8004BEB8 (synthVoice). Returns the root voice id or 0xFFFFFFFF.
 * w23c levers (w21b best true=2235 strict=2453 -> 0):
 * - The key-range test is one && condition around the body (no `continue`): with the `||`/continue form the
 *   count reload is a coloured PRE web (v1) and the loop guard block changes (2235 -> 695).
 * - `layer = &layers[i]` per iteration from a separate `layers` result: retail's s1 is the strength-reduced
 *   pointer started in the loop preheader (the move from v0 after the guard), and `layers` is the 4-byte local
 *   that makes retail's frame 176 (60 -> 0).
 * - `switch (layer->id & 0xC000) { case 0: ... break; }` (MusyX's layer-type switch): its blocks make the
 *   callee-saved order previous, -1, i, channel, set, &D_8004BEB8, 416 (= retail s2..s7, fp); as an `if` i is
 *   decided last (fp) (210 -> 60 with the init order).
 * - `note = layer->transpose + (key & 127)` and `layer->volume * volume` operand orders; priority as one
 *   expression `((packed >> 8) & 255) + layer->priority` (the early `priority = packed >> 8` statement keeps
 *   packed and priority in one interfering web: a3/a0 vs retail v1/a1).
 * No devices, no unused locals, no pad arrays, no false prototypes, no implicit declarations. Types: the TU's
 * VoiceState_80019C8C (verbatim) for D_8004BEB8/func_8001ECE0; a private 12-byte Layer_80019F48 view
 * (MusyX LAYER: id, keyLow, keyHigh, transpose, volume, prioOffset, panning). */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState_80019C8C { u32 command00;u8 unknown04[12];u32 child,parent;u8 unknown18[12]; u32 flags;u32 age28;u8 unknown2C[2];u8 channel2E,unknown2F;u32 volume30;u8 unknown34[4];u32 panning38;u8 unknown3C[4];u32 words40[2];u8 status48,status49;u8 channel,set;u8 unknown4C[2]; u16 base_key,key;u8 unknown52[3];u8 channel55;u8 unknown56[10];u32 identifier; u8 unknown64[4];u16 loop68;u8 unknown6A[34];u32 glide;u8 unknown90[4];u32 pitch; u8 state98,depth99,flag9A;u8 unknown9B[34];u8 activeBD,groupBE,unknownBF;s8 cents;u8 original_key;u16 valueC2;u8 unknownC4[168];u32 word16C;u16 half170;u8 unknown172[6];u32 word178;u16 half17C;u8 unknown17E[34]; } VoiceState_80019C8C;
#pragma pack(0)
#pragma pack(1)
typedef struct Layer_80019F48 {
    u16 id;
    u8 low;
    u8 high;
    s8 transpose;
    u8 volume;
    s16 priority;
    u8 panning;
    u8 unknown09[3];
} Layer_80019F48;
#pragma pack(0)
extern VoiceState_80019C8C D_8004BEB8[];
extern u32 func_8001ECE0(VoiceState_80019C8C *);
extern void *func_80016F80(u16, u16 *);
extern u32 func_80019ED0(u8, u8, u8);
extern u32 func_80024988(u32, u16, u8, u8, u8, u8, u8, u16, u16, u8, u8);

u32 func_80019F48(u32 packed, u16 allocation, u8 key, u8 volume,
                 u8 panning, u8 channel, u8 set, u16 offset, u16 section, u8 group)
{
    u16 count;
    u32 result;
    u32 previous;
    u32 current;
    u32 nextPacked;
    int i;
    int note;
    int pan;
    int priority;
    Layer_80019F48 *layer;
    Layer_80019F48 *layers;
    u8 scaled;
    result = 0xFFFFFFFFU;
    layers = func_80016F80(packed >> 16, &count);
    if (layers != 0) {
        for (i = 0; i < count; i++) {
            layer = &layers[i];
            if (layer->id != 0xFFFF && layer->low <= (key & 127) && layer->high >= (key & 127)) {
            note = layer->transpose + (key & 127);
            note = note > 127 ? 127 : note < 0 ? 0 : note;
            current = func_80019ED0(note, channel, set);
            if (current != 0xFFFFFFFFU) return current;
            if ((layer->panning & 128) == 0) {
                pan = layer->panning - 64;
                pan += panning;
                pan = pan < 0 ? 0 : pan > 127 ? 127 : pan;
            } else pan = 128;
            scaled = (layer->volume * volume) / 127;
            priority = ((packed >> 8) & 255) + layer->priority;
            priority = priority > 255 ? 255 : priority < 0 ? 0 : priority;
            nextPacked = (packed & 255) | ((u32)priority << 8) | ((u32)layer->id << 16);
            switch (layer->id & 0xC000) {
            case 0:
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
                break;
            }
            }
        }
    }
    return result;
}
