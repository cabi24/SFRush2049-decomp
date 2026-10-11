/* GENERATED ROM-aligned TU — segment 0x21210 (rom/lib_21210)
 * layout map 7db48fae6e80338f4674ce14cd1283cee2dbc149f9f1dacaebf5704c974b2257; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED (w17g dry run) — func_80020610
 * Source:   cloud/work/frontier/w17g/sources/func_80020610.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
extern unsigned char D_80050D00[][16][134];
extern unsigned char D_80055000[][134];
void func_80020610(unsigned char controller, unsigned char channel,
                   unsigned char set, unsigned char value)
{
    if (set != 255) {
        D_80050D00[set][channel][controller] = value & 127;
    } else {
        D_80055000[channel][controller] = value & 127;
    }
}

/* PROMOTED (w17g dry run) — func_800206AC
 * Source:   cloud/work/frontier/w17g/sources/func_800206AC.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);
void func_800206AC(u8 controller, u8 channel, u8 set, u16 value)
{
    if (controller < 64) {
        func_80020610(controller & 31, channel, set, value >> 7);
        func_80020610((controller & 31) + 32, channel, set, value & 127);
    } else if (controller == 128 || controller == 129) {
        func_80020610(controller & 254, channel, set, value >> 7);
        func_80020610((controller & 254) + 1, channel, set, value & 127);
    } else if (controller == 132 || controller == 133) {
        func_80020610(controller & 254, channel, set, value >> 7);
        func_80020610((controller & 254) + 1, channel, set, value & 127);
    } else {
        func_80020610(controller, channel, set, value >> 7);
    }
}

/* PROMOTED 2026-10-04 — func_80020820
 * Source:   cloud/matches/boot_tail/func_80020820.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020820.c:func_80020820 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_80050D00[][16][134];
extern u8 D_80055000[][134];
extern void func_80020610(u8, u8, u8, u8);
extern void func_80020F4C(u8, u8, u8);
extern void func_80020FDC(u8, u8, u8);
void func_80020820(u8 channel, u8 set)
{
    u32 i;
    if (set != 255) {
        for (i = 0; i < 134; i++) {
            D_80050D00[set][channel][i] = 0;
        }
    } else {
        for (i = 0; i < 134; i++) {
            D_80055000[channel][i] = 0;
        }
    }
    func_80020610(7, channel, set, 127);
    func_80020610(10, channel, set, 64);
    func_80020610(128, channel, set, 64);
    func_80020610(129, channel, set, 0);
    func_80020610(64, channel, set, 0);
    func_80020610(65, channel, set, 0);
    func_80020610(91, channel, set, 0);
    func_80020610(131, channel, set, 0);
    func_80020610(132, channel, set, 64);
    func_80020610(133, channel, set, 0);
    func_80020F4C(channel, set, 255);
    func_80020FDC(channel, set, 0);
}

/* PROMOTED (w17g dry run) — func_80020A04
 * Source:   cloud/work/frontier/w17g/sources/func_80020A04.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
u16 func_80020A04(u8 controller, u8 channel, u8 set)
{
    u8 *pair;
    if (set != 255) {
        if (controller < 64) {
            pair = &D_80050D00[set][channel][controller & 31];
            return (pair[0] << 7) | pair[32];
        } else if (controller == 128 || controller == 129) {
            pair = &D_80050D00[set][channel][controller & 254];
            return (pair[0] << 7) | pair[1];
        } else if (controller == 132 || controller == 133) {
            pair = &D_80050D00[set][channel][controller & 254];
            return (pair[0] << 7) | pair[1];
        } else if (controller < 70) {
            return D_80050D00[set][channel][controller] < 64 ? 0 : 16383;
        } else if (controller >= 96 && controller <= 101) {
            return 0;
        } else {
            return D_80050D00[set][channel][controller] << 7;
        }
    } else {
        if (controller < 64) {
            pair = &D_80055000[channel][controller & 31];
            return (pair[0] << 7) | pair[32];
        } else if (controller == 128 || controller == 129) {
            pair = &D_80055000[channel][controller & 254];
            return (pair[0] << 7) | pair[1];
        } else if (controller == 132 || controller == 133) {
            pair = &D_80055000[channel][controller & 254];
            return (pair[0] << 7) | pair[1];
        } else if (controller < 70) {
            return D_80055000[channel][controller] < 64 ? 0 : 16383;
        } else if (controller >= 96 && controller <= 101) {
            return 0;
        } else {
            return D_80055000[channel][controller] << 7;
        }
    }
}

/* PROMOTED (w17g dry run) — func_80020DA8
 * Source:   cloud/work/frontier/w17g/sources/func_80020DA8.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
#pragma pack(1)
typedef struct VoicePrefix { u8 unknown00[96]; u32 identifier60; } VoicePrefix;
#pragma pack()
void func_80020DA8(u8 controller, VoicePrefix *destination, VoicePrefix *source)
{
    u32 dst;
    u32 src;
    u32 index;
    u8 *dstdata;
    u8 *srcdata;
    dst = destination->identifier60 & 255;
    src = source->identifier60 & 255;
    if (controller < 64) {
        index = controller & 31;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[32] = srcdata[32];
    } else if (controller == 128 || controller == 129) {
        index = controller & 254;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[1] = srcdata[1];
    } else if (controller == 132 || controller == 133) {
        index = controller & 254;
        dstdata = &D_80055000[dst][index];
        srcdata = &D_80055000[src][index];
        dstdata[0] = srcdata[0];
        dstdata[1] = srcdata[1];
    } else {
        D_80055000[dst][controller] = D_80055000[src][controller];
    }
}

/* PROMOTED (w17g dry run) — func_80020F4C
 * Source:   cloud/work/frontier/w17g/sources/func_80020F4C.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
extern unsigned char D_80050C60[][16];
extern unsigned char D_80050CE0[];
void func_80020F4C(unsigned char channel, unsigned char set,
                   unsigned char value)
{
    if (set != 255) {
        D_80050C60[set][channel] = value;
    } else {
        D_80050CE0[channel] = value;
    }
}

/* PROMOTED (w17g dry run) — func_80020F98
 * Source:   cloud/work/frontier/w17g/sources/func_80020F98.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
unsigned char func_80020F98(unsigned char channel, unsigned char set)
{
    if (set != 255) {
        return D_80050C60[set][channel];
    } else {
        return D_80050CE0[channel];
    }
}

/* PROMOTED (w17g dry run) — func_80020FDC
 * Source:   cloud/work/frontier/w17g/sources/func_80020FDC.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
extern unsigned char D_800560C0[][16];
extern unsigned char D_80056140[];
void func_80020FDC(unsigned char channel, unsigned char set,
                   unsigned char value)
{
    if (set != 255) {
        D_800560C0[set][channel] = value;
    } else {
        D_80056140[channel] = value;
    }
}

/* PROMOTED (w17g dry run) — func_80021028
 * Source:   cloud/work/frontier/w17g/sources/func_80021028.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
unsigned char func_80021028(unsigned char channel, unsigned char set)
{
    if (set != 255) {
        return D_800560C0[set][channel];
    } else {
        return D_80056140[channel];
    }
}

/* PROMOTED 2026-10-04 — func_8002106C
 * Source:   cloud/matches/boot_tail/func_8002106C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8002106C.c:func_8002106C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct ControlSource { u8 controller; u8 combine; u16 scale; } ControlSource;
typedef struct ControlInput { ControlSource source[4]; u8 count; u8 unknown11; } ControlInput;
typedef struct VoiceControls { u8 unknown00[196]; ControlInput input[9]; } VoiceControls;
#pragma pack(0)
void func_8002106C(VoiceControls *voice)
{
    voice->input[0].source[0].controller = 7;
    voice->input[0].source[0].combine = 0;
    voice->input[0].source[0].scale = 256;
    voice->input[0].count = 1;
    voice->input[1].source[0].controller = 10;
    voice->input[1].source[0].combine = 0;
    voice->input[1].source[0].scale = 256;
    voice->input[1].count = 1;
    voice->input[2].source[0].controller = 131;
    voice->input[2].source[0].combine = 0;
    voice->input[2].source[0].scale = 256;
    voice->input[2].count = 1;
    voice->input[3].source[0].controller = 128;
    voice->input[3].source[0].combine = 0;
    voice->input[3].source[0].scale = 256;
    voice->input[3].count = 1;
    voice->input[5].source[0].controller = 1;
    voice->input[5].source[0].combine = 0;
    voice->input[5].source[0].scale = 256;
    voice->input[5].count = 1;
    voice->input[6].source[0].controller = 64;
    voice->input[6].source[0].combine = 0;
    voice->input[6].source[0].scale = 256;
    voice->input[6].count = 1;
    voice->input[7].source[0].controller = 65;
    voice->input[7].source[0].combine = 0;
    voice->input[7].source[0].scale = 256;
    voice->input[7].count = 1;
    voice->input[8].source[0].controller = 91;
    voice->input[8].source[0].combine = 0;
    voice->input[8].source[0].scale = 256;
    voice->input[8].count = 1;
    voice->input[4].source[0].controller = 132;
    voice->input[4].source[0].combine = 0;
    voice->input[4].source[0].scale = 256;
    voice->input[4].count = 1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80021150.s")
/* PROMOTED 2026-10-04 — func_80021428
 * Source:   cloud/matches/boot_tail/func_80021428.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021428.c:func_80021428 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short func_80021150(void *voice, void *control);
unsigned short func_80021428(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 196);
}

/* PROMOTED 2026-10-04 — func_80021448
 * Source:   cloud/matches/boot_tail/func_80021448.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021448.c:func_80021448 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_80021448(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 214);
}

/* PROMOTED 2026-10-04 — func_80021468
 * Source:   cloud/matches/boot_tail/func_80021468.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021468.c:func_80021468 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_80021468(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 232);
}

/* PROMOTED 2026-10-04 — func_80021488
 * Source:   cloud/matches/boot_tail/func_80021488.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021488.c:func_80021488 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_80021488(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 250);
}

/* PROMOTED 2026-10-04 — func_800214A8
 * Source:   cloud/matches/boot_tail/func_800214A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800214A8.c:func_800214A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_800214A8(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 268);
}

/* PROMOTED 2026-10-04 — func_800214C8
 * Source:   cloud/matches/boot_tail/func_800214C8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800214C8.c:func_800214C8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_800214C8(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 286);
}

/* PROMOTED 2026-10-04 — func_800214E8
 * Source:   cloud/matches/boot_tail/func_800214E8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800214E8.c:func_800214E8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_800214E8(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 304);
}

/* PROMOTED 2026-10-04 — func_80021508
 * Source:   cloud/matches/boot_tail/func_80021508.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021508.c:func_80021508 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_80021508(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 322);
}

/* PROMOTED 2026-10-04 — func_80021528
 * Source:   cloud/matches/boot_tail/func_80021528.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80021528.c:func_80021528 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned short func_80021528(void *voice)
{
    return func_80021150(voice, (unsigned char *)voice + 340);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80021548.s")
/* PROMOTED (w17g dry run) — func_800215A8
 * Source:   cloud/work/frontier/w17g/sources/func_800215A8.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags24; u8 unknown28[34]; u8 channel4A; u8 set4B; u8 unknown4C[20]; u32 id60; u8 unknown64[268]; s16 lfo170; u8 unknown172[10]; s16 lfo17C; u8 unknown17E[34]; } VoiceState;
#pragma pack(0)
extern u8 func_80021548(u8 controller);
extern u16 func_80020A04(u8 controller, u8 channel, u8 set);
u16 func_800215A8(VoiceState *voice, u8 controller)
{
    u16 value;
    switch (func_80021548(controller)) {
    case 160:
        value = voice->lfo170 * 2 + 8192;
        break;
    case 161:
        value = voice->lfo17C * 2 + 8192;
        break;
    default:
        value = voice->channel4A != 255
            ? func_80020A04(controller, voice->channel4A, voice->set4B) : 0;
        break;
    }
    return value;
}

/* PROMOTED (w17g dry run) — func_8002165C
 * Source:   cloud/work/frontier/w17g/sources/func_8002165C.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
#pragma pack(1)
#pragma pack(0)
extern void func_800206AC(u8 controller, u8 channel, u8 set, u16 value);
void func_8002165C(VoiceState *voice, u8 controller, s16 value)
{
    value = value < 0 ? 0 : value > 16383 ? 16383 : value;
    switch (func_80021548(controller)) {
    case 160:
    case 161:
        break;
    default:
        if (voice->channel4A != 255) {
            func_800206AC(controller, voice->channel4A, voice->set4B, value);
        }
        break;
    }
}

