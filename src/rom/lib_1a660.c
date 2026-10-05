/* GENERATED ROM-aligned TU — segment 0x1a660 (rom/lib_1a660)
 * layout map 91b72acb516b2291b9dbf568e33b80bbc7fb9c8413206c2c73c661eb5c01ac02; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019A60.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019AD4.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019ED0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019F48.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A270.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A5D8.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B1D0.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B4A4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B5F4.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B9F8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001BDB8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001BE14.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001C19C.s")
/* PROMOTED 2026-10-05 — func_8001C1D8
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_8001C1D8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_8001C1D8.c:func_8001C1D8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_voice.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
#pragma pack(0)
typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern ChannelState D_8004F300[32];
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

