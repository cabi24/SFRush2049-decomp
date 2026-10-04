/* GENERATED ROM-aligned TU — segment 0x21210 (rom/lib_21210)
 * layout map 7db48fae6e80338f4674ce14cd1283cee2dbc149f9f1dacaebf5704c974b2257; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020610.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_800206AC.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020A04.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020DA8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020F4C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020F98.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80020FDC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_80021028.s")
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
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_800215A8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21210/func_8002165C.s")
