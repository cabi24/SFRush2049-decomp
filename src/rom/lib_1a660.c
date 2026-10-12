/* GENERATED ROM-aligned TU — segment 0x1a660 (rom/lib_1a660)
 * layout map 91b72acb516b2291b9dbf568e33b80bbc7fb9c8413206c2c73c661eb5c01ac02; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — func_80019A60
 * Source:   cloud/work/frontier/w18c/sources/func_80019A60.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80019A60.c:func_80019A60 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8004FA20[];
void func_80019A60(unsigned int value, unsigned char index)
{
    if (index == 255) {
        index = 8;
    }
    D_8004FA20[index] = (value << 3) * 1536 / 240;
}

/* PROMOTED 2026-10-04 — func_80019AA8
 * Source:   cloud/matches/boot_tail/func_80019AA8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019AA8.c:func_80019AA8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8004FA20[];
unsigned int func_80019AA8(unsigned char *state)
{
    return D_8004FA20[state[75] == 255 ? 8 : state[75]];
}

/* PROMOTED 2026-10-11 — func_80019AD4
 * Source:   cloud/work/frontier/w17g/sources/func_80019AD4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80019AD4.c:func_80019AD4 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct RampState { u8 unknown00[36]; u32 flags24; u8 unknown28[100]; u32 start8C; u32 end90; u32 current94; } RampState;
#pragma pack(0)
extern u32 D_8004BE90;
u32 func_80019AD4(RampState *state, u32 target)
{
    s32 duration;
    u32 previous;
    u32 start;
    u32 end;
    if (state->flags24 & 0x800) {
        end = state->end90;
        start = state->start8C;
        duration = (end - start) >> 8;
        if (duration > 0) {
            previous = state->current94;
            state->current94 += (s32)(D_8004BE90 * (u32)((s32)(target - state->current94) >> 8)) / duration;
            if ((previous < target && state->current94 < target) ||
                (previous > target && state->current94 > target)) {
                target = state->current94;
                state->start8C = start + D_8004BE90;
            } else {
                state->start8C = end;
            }
        }
    }
    return target;
}

/* PROMOTED 2026-10-04 — func_80019BE4
 * Source:   cloud/matches/boot_tail/func_80019BE4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019BE4.c:func_80019BE4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags; u8 unknown28[34]; u8 channel; u8 set; u8 unknown4C[64]; u32 current8C; u32 stored90; u32 fixed94; u8 mode98; u8 unknown99[40]; u8 lastC1; } VoiceState;
#pragma pack(0)
extern void func_80020FDC(u8, u8, u8);
void func_80019BE4(VoiceState *state)
{
    if (!(state->flags & 0x80000)) {
        if (state->mode98 == 1) {
            if (!(state->flags & 0x2000)) state->current8C = 0;
            else state->current8C = state->stored90;
        } else {
            state->current8C = state->stored90;
        }
        state->fixed94 = (u32)state->lastC1 << 16;
    }
    if (state->channel != 255) func_80020FDC(state->channel, state->set, 1);
}

/* PROMOTED 2026-10-04 — func_80019C8C
 * Source:   cloud/work/boot_tail_promotion/sources/func_80019C8C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80019C8C.c:func_80019C8C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
/* Array stride 0x1A0; expose the selector at +0x55 and value at +0xC2
 * without changing the field names used by the accepted body below.
 * Wave 4 names the fields func_8001C1D8 initializes, carved from the
 * unknown ranges; every offset and the 0x1A0 size are unchanged. */
typedef struct VoiceState_80019C8C { u32 command00;u8 unknown04[12];u32 child,parent;u8 unknown18[12]; u32 flags;u32 age28;u8 unknown2C[2];u8 channel2E,unknown2F;u32 volume30;u8 unknown34[4];u32 panning38;u8 unknown3C[4];u32 words40[2];u8 status48,status49;u8 channel,set;u8 unknown4C[2]; u16 base_key,key;u8 unknown52[3];u8 channel55;u8 unknown56[10];u32 identifier; u8 unknown64[4];u16 loop68;u8 unknown6A[34];u32 glide;u8 unknown90[4];u32 pitch; u8 state98,depth99,flag9A;u8 unknown9B[34];u8 activeBD,groupBE,unknownBF;s8 cents;u8 original_key;u16 valueC2;u8 unknownC4[168];u32 word16C;u16 half170;u8 unknown172[6];u32 word178;u16 half17C;u8 unknown17E[34]; } VoiceState_80019C8C;
#pragma pack(0)
extern VoiceState_80019C8C D_8004BEB8[];
extern u8 D_8004FA18;
extern u8 func_8001467C(u32);
extern void func_8001EB10(VoiceState_80019C8C *);
extern u32 func_8001ECE0(VoiceState_80019C8C *);
extern void func_800217E4(VoiceState_80019C8C *);
extern void func_80020F4C(u8,u8,u8);
u32 func_80019C8C(u8 key,u8 channel,u8 set)
{
    u32 i,result,previous;
    VoiceState_80019C8C *state,*last;
    result=0xFFFFFFFFU;
    for (i=0,state=D_8004BEB8;i<D_8004FA18;i++,state++) {
        if (state->identifier!=0xFFFFFFFFU && state->channel==channel && state->set==set &&
            (state->flags&16) && (!(state->flags&8) || (state->flags&0x40000000)) &&
            func_8001467C(i)) {
            last=state;
            state->pitch=((u32)state->key<<16)+(state->cents*65536)/100;
            state->original_key=state->key;
            state->key=key+(state->key&255)-(u8)state->base_key;
            state->base_key=key;
            state->cents=0;
            state->glide=0;
            state->flags|=0x80800;
            func_8001EB10(&D_8004BEB8[i]);
            if (result==0xFFFFFFFFU) {
                state->child=0xFFFFFFFFU;
                state->parent=0xFFFFFFFFU;
                result=func_8001ECE0(&D_8004BEB8[i]);
                previous=state->identifier;
            } else {
                D_8004BEB8[(previous&255)].child=state->identifier;
                state->parent=previous;
                previous=state->identifier;
            }
        }
    }
    if (result!=0xFFFFFFFFU) {
        func_800217E4(last);
        func_80020F4C(last->channel,last->set,(u8)last->key);
    }
    return result;
}

/* PROMOTED 2026-10-11 — func_80019ED0
 * Source:   cloud/work/frontier/w18c/sources/func_80019ED0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80019ED0.c:func_80019ED0 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 func_80021028(u8, u8);
extern u32 func_80019C8C(u8, u8, u8);
u32 func_80019ED0(u8 key, u8 channel, u8 set)
{
    u32 identifier;
    if (channel != 255 && func_80021028(channel, set) != 0) {
        identifier = func_80019C8C(key & 127, channel, set);
        if (identifier != 0xFFFFFFFFU) {
            return identifier;
        }
    }
    return 0xFFFFFFFFU;
}

/* PROMOTED 2026-10-11 — func_80019F48
 * Source:   cloud/work/frontier/w23c/sources/func_80019F48.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w23c/sources/func_80019F48.c:func_80019F48 (score0); rom_tu.h context: cloud/work/frontier/w23c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
#pragma pack(1)
typedef struct Layer_80019F48 { u16 id; u8 low; u8 high; s8 transpose; u8 volume; s16 priority; u8 panning; u8 unknown09[3]; } Layer_80019F48;
#pragma pack(0)
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

/* PROMOTED 2026-10-11 — func_8001A270
 * Source:   cloud/work/frontier/w22c/sources/func_8001A270.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w22c/sources/func_8001A270.c:func_8001A270 (score0); rom_tu.h context: cloud/work/frontier/w22c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct Keymap { u16 id; s8 transpose; u8 panning; s16 priority; u8 unknown06[2]; } Keymap;
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

/* PROMOTED 2026-10-11 — func_8001A5D8
 * Source:   cloud/work/frontier/w18c/sources/func_8001A5D8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001A5D8.c:func_8001A5D8 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern ChannelState D_8004F300[32];
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A658.s")
/* PROMOTED 2026-10-04 — func_8001B154
 * Source:   cloud/matches/boot_tail/func_8001B154.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001B154.c:func_8001B154 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern s32 D_8004F808;
extern s32 D_8004F800;
extern u32 D_8004BE90;
extern void func_8001A658(void);
void func_8001B154(void)
{
    if (D_8004F808 != 0) {
        D_8004BE90 = (u32)((s32)((u32)D_8004F808 * 32000U) / D_8004F800) << 3;
        func_8001A658();
    }
}

/* PROMOTED 2026-10-11 — func_8001B1D0
 * Source:   cloud/work/frontier/w17g/sources/func_8001B1D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001B1D0.c:func_8001B1D0 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct Preset { u16 identifier; u16 high; u8 low; u8 middle; u8 default_first; u8 default_second; u8 flags; u8 parameter; u8 unknown0A[2]; } Preset;
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

/* PROMOTED 2026-10-05 — func_8001B29C
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B29C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B29C.c:func_8001B29C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern int func_8001EDF4(u32);
extern void func_80020610(u8, u8, u8, u8);
int func_8001B29C(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            result = 0;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(10, index, D_8004BEB8[index].channel55, value);
            } else {
                func_80020610(10, index, D_8004BEB8[index].set, value);
            }
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}

/* PROMOTED 2026-10-05 — func_8001B3A0
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B3A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B3A0.c:func_8001B3A0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
int func_8001B3A0(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            result = 0;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(131, index, D_8004BEB8[index].channel55, value);
            } else {
                func_80020610(131, index, D_8004BEB8[index].set, value);
            }
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}

/* PROMOTED 2026-10-11 — func_8001B4A4
 * Source:   cloud/work/frontier/w18c/sources/func_8001B4A4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001B4A4.c:func_8001B4A4 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
int func_8001B4A4(u32 identifier, u16 value)
{
    int result;
    u32 index;
    u8 low;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            low = value & 127;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(132, index, D_8004BEB8[index].channel55, value >> 7);
                func_80020610(133, index, D_8004BEB8[index].channel55, low);
            } else {
                func_80020610(132, index, D_8004BEB8[index].set, value >> 7);
                func_80020610(133, index, D_8004BEB8[index].set, low);
            }
            result = 0;
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}

/* PROMOTED 2026-10-11 — func_8001B5F4
 * Source:   cloud/work/frontier/w18c/sources/func_8001B5F4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001B5F4.c:func_8001B5F4 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
int func_8001B5F4(u32 identifier, u16 value)
{
    int result;
    u32 index;
    u8 low;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            low = value & 127;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(128, index, D_8004BEB8[index].channel55, value >> 7);
                func_80020610(129, index, D_8004BEB8[index].channel55, low);
            } else {
                func_80020610(128, index, D_8004BEB8[index].set, value >> 7);
                func_80020610(129, index, D_8004BEB8[index].set, low);
            }
            result = 0;
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001B744
 * Source:   cloud/matches/boot_tail/func_8001B744.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001B744.c:func_8001B744 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
struct VoiceState;
extern void func_80020DA8(u8, struct VoiceState *, struct VoiceState *);
void func_8001B744(struct VoiceState *destination, struct VoiceState *source)
{
    func_80020DA8(7, destination, source);
    func_80020DA8(10, destination, source);
    func_80020DA8(91, destination, source);
    func_80020DA8(128, destination, source);
    func_80020DA8(132, destination, source);
}

/* PROMOTED 2026-10-05 — func_8001B7C0
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B7C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B7C0.c:func_8001B7C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
int func_8001B7C0(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier == identifier) {
            result = 0;
            if (D_8004BEB8[index].flags & 2) {
                func_80020610(7, index, D_8004BEB8[index].channel55, value);
            } else {
                func_80020610(7, index, D_8004BEB8[index].set, value);
            }
            identifier = D_8004BEB8[index].child;
        } else {
            return result;
        }
    }
    return result;
}

/* PROMOTED 2026-10-05 — func_8001B8C4
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B8C4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B8C4.c:func_8001B8C4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern u8 D_8002C630;
int func_8001B8C4(u32 key)
{
    u32 index;
    int result;
    result = -1;
    if (D_8002C630) {
        key = func_8001EDF4(key);
        while (key != 0xFFFFFFFFU) {
            index = key & 255;
            if (D_8004BEB8[index].identifier == key) {
                D_8004BEB8[index].flags |= 8;
                result = 0;
            }
            key = D_8004BEB8[index].child;
        }
    }
    return result;
}

/* PROMOTED 2026-10-05 — func_8001B968
 * Source:   cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B968.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/voice_lists/sources/func_8001B968.c:func_8001B968 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
u16 func_8001B968(u32 key)
{
    int identifier;
    identifier = func_8001EDF4(key);
    if (identifier != -1) {
        if (D_8004BEB8[identifier & 255].identifier == (u32)identifier &&
            !(D_8004BEB8[identifier & 255].flags & 2)) {
            return D_8004BEB8[identifier & 255].valueC2;
        }
    }
    return 0;
}

/* PROMOTED 2026-10-11 — func_8001B9F8
 * Source:   cloud/work/frontier/w20c/sources/func_8001B9F8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_8001B9F8.c:func_8001B9F8 (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MasterFader { u32 volume; u32 target; s32 delta; u32 time; u32 seqId; u8 type; u8 seqMode; u8 unknown16[2]; u32 pauseVolume; u32 pauseTarget; s32 pauseDelta; u32 pauseTime; } MasterFader;
extern void func_8001E930(u32 *);
void func_8001B9F8(u8 volume, u16 time, u8 vGroup, u8 seqMode, u32 seqId)
{
    u32 ltime;
    u32 i;
    u8 type;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (vGroup) {
    case 255:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 0 || ((MasterFader *)D_8004F300)[i].type == 1) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 252:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 2 || ((MasterFader *)D_8004F300)[i].type == 3) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 250: type = 2; goto setup_type;
    case 251: type = 3; goto setup_type;
    case 253: type = 0; goto setup_type;
    case 254: type = 1; goto setup_type;
    setup_type:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == type) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    default:
        i = vGroup;
        ((MasterFader *)D_8004F300)[i].target = volume << 16;
        ((MasterFader *)D_8004F300)[i].time = ltime;
        ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
        ((MasterFader *)D_8004F300)[i].seqMode = seqMode;
        ((MasterFader *)D_8004F300)[i].seqId = seqId;
        return;
    }
}

/* PROMOTED 2026-10-11 — func_8001BDB8
 * Source:   cloud/work/frontier/w18c/sources/func_8001BDB8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001BDB8.c:func_8001BDB8 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_8001BDB8(u8 index)
{
    ChannelState *state;
    state = &D_8004F300[index];
    if (state->kind14 != 4 && state->pending0C != 0 && (int)state->flags08 < 0) return 1;
    return 0;
}

/* PROMOTED 2026-10-11 — func_8001BE14
 * Source:   cloud/work/frontier/w20c/sources/func_8001BE14.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_8001BE14.c:func_8001BE14 (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_8001BE14(u8 volume, u16 time, u8 group)
{
    u32 i;
    u32 ltime;
    u8 type;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (group) {
    case 255:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 0 || ((MasterFader *)D_8004F300)[i].type == 1) {
                ((MasterFader *)D_8004F300)[i].pauseTarget = volume << 16;
                ((MasterFader *)D_8004F300)[i].pauseTime = ltime;
                ((MasterFader *)D_8004F300)[i].pauseDelta = (s32)(((MasterFader *)D_8004F300)[i].pauseTarget - ((MasterFader *)D_8004F300)[i].pauseVolume) / time;
            }
        }
        return;
    case 252:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 2 || ((MasterFader *)D_8004F300)[i].type == 3) {
                ((MasterFader *)D_8004F300)[i].pauseTarget = volume << 16;
                ((MasterFader *)D_8004F300)[i].pauseTime = ltime;
                ((MasterFader *)D_8004F300)[i].pauseDelta = (s32)(((MasterFader *)D_8004F300)[i].pauseTarget - ((MasterFader *)D_8004F300)[i].pauseVolume) / time;
            }
        }
        return;
    case 250:
        type = 2;
        goto setup_type;
    case 251:
        type = 3;
        goto setup_type;
    case 253:
        type = 0;
        goto setup_type;
    case 254:
        type = 1;
        goto setup_type;
    setup_type:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == type) {
                ((MasterFader *)D_8004F300)[i].pauseTarget = volume << 16;
                ((MasterFader *)D_8004F300)[i].pauseTime = ltime;
                ((MasterFader *)D_8004F300)[i].pauseDelta = (s32)(((MasterFader *)D_8004F300)[i].pauseTarget - ((MasterFader *)D_8004F300)[i].pauseVolume) / time;
            }
        }
        return;
    default:
        i = group;
        ((MasterFader *)D_8004F300)[i].pauseTarget = volume << 16;
        ((MasterFader *)D_8004F300)[i].pauseTime = ltime;
        ((MasterFader *)D_8004F300)[i].pauseDelta = (s32)(((MasterFader *)D_8004F300)[i].pauseTarget - ((MasterFader *)D_8004F300)[i].pauseVolume) / time;
        return;
    }
}

/* PROMOTED 2026-10-11 — func_8001C19C
 * Source:   cloud/work/frontier/w18c/sources/func_8001C19C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_8001C19C.c:func_8001C19C (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern unsigned char D_8004F314[][40];
void func_8001C19C(unsigned char channel, unsigned char value)
{
    if (D_8002C630) {
        D_8004F314[channel][0] = value;
    }
}

/* PROMOTED 2026-10-05 — func_8001C1D8
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_8001C1D8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_8001C1D8.c:func_8001C1D8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_voice.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
extern short D_8004BE98[16];
extern u32 D_8004F804;
extern u8 D_8004F2F8;
extern void func_80019A60(u32,u8);
extern void func_8001E9B0(void);
extern void func_8001F864(void);
extern void func_800218CC(void);
void func_8001C1D8(u32 configuration)
{
    int i;
    D_8004F800 = configuration;
    D_8004F804 = 10240;
    func_80019A60(120,255);
    D_8004F2F8 = 0;
    for (i=0;i<32;i++) {
        D_8004BEB8[i].identifier = 0xFFFFFFFFU;
        D_8004BEB8[i].command00 = 0;
        D_8004BEB8[i].flags = 0;
        D_8004BEB8[i].age28 = 0;
        D_8004BEB8[i].channel2E = 0;
        D_8004BEB8[i].loop68 = 0;
        D_8004BEB8[i].channel = 255;
        D_8004BEB8[i].volume30 = 0;
        D_8004BEB8[i].depth99 = 128;
        D_8004BEB8[i].flag9A = 0;
        D_8004BEB8[i].panning38 = 0x3F0000;
        D_8004BEB8[i].words40[0] = 0;
        D_8004BEB8[i].words40[1] = 0;
        D_8004BEB8[i].status48 = 0;
        D_8004BEB8[i].status49 = 0;
        D_8004BEB8[i].activeBD = 0;
        D_8004BEB8[i].groupBE = 23;
        D_8004BEB8[i].word16C = 0;
        D_8004BEB8[i].half170 = 0;
        D_8004BEB8[i].word178 = 0;
        D_8004BEB8[i].half17C = 0;
        D_8004BEB8[i].glide = 100;
        D_8004BEB8[i].state98 = 0;
    }
    for (i=0;i<32;i++) {
        D_8004F300[i].word00 = 0;
        D_8004F300[i].word04 = 0;
        D_8004F300[i].pending0C = 0;
        D_8004F300[i].kind14 = 4;
        D_8004F300[i].value18 = 0x7F0000;
    }
    D_8004F300[31].kind14 = 1;
    for (i=0;i<8;i++) D_8004F300[i+23].kind14 = 0;
    D_8004F300[21].word00 = 0x7F0000;
    D_8004F300[22].word00 = 0x7F0000;
    func_8001E9B0();
    func_8001F864();
    for (i=0;i<16;i++) {
        D_8004BE98[i] = 0;
    }
    func_800218CC();
}

