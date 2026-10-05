/* GENERATED ROM-aligned TU — segment 0x207b0 (rom/lib_207b0)
 * layout map a4f8c8e3046756fa727553750689364dc71ce2d0413805cd6440e6752a219abe; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_8001FBB0.s")
/* PROMOTED 2026-10-04 — func_8001FD2C
 * Source:   cloud/matches/boot_tail/func_8001FD2C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FD2C.c:func_8001FD2C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next10; u8 unknown14[16]; u32 flags24; u8 unknown28[3]; u8 channel2B; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[28]; u8 channel4B; u8 external4C; u8 unknown4D[8]; u8 channel55; u8 unknown56[10]; u32 identifier60; u8 unknown64[316]; } VoiceState;
#pragma pack(0)
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern VoiceState D_8004BEB8[];
extern int func_8001EDF4(u32);
extern void func_80020610(u8, u8, u8, u8);
int func_8001FD2C(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        identifier = func_8001EDF4(identifier);
        while (identifier != 0xFFFFFFFFU) {
            index = identifier & 255;
            if (D_8004BEB8[index].identifier60 == identifier) {
                result = 0;
                if (D_8004BEB8[index].flags24 & 2) {
                    func_80020610(91, index, D_8004BEB8[index].channel55, value);
                } else {
                    func_80020610(91, index, D_8004BEB8[index].channel4B, value);
                }
                identifier = D_8004BEB8[index].next10;
            } else {
                func_800145DC();
                return result;
            }
        }
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FE58
 * Source:   cloud/matches/boot_tail/func_8001FE58.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FE58.c:func_8001FE58 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern int func_8001B8C4(unsigned int);
int func_8001FE58(unsigned int identifier)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B8C4(identifier);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FEA4
 * Source:   cloud/matches/boot_tail/func_8001FEA4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FEA4.c:func_8001FEA4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B7C0(unsigned int, unsigned char);
int func_8001FEA4(unsigned int identifier, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B7C0(identifier, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FEF8
 * Source:   cloud/matches/boot_tail/func_8001FEF8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FEF8.c:func_8001FEF8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B5F4(unsigned int, unsigned short);
int func_8001FEF8(unsigned int identifier, unsigned short value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B5F4(identifier, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FF4C
 * Source:   cloud/matches/boot_tail/func_8001FF4C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FF4C.c:func_8001FF4C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B3A0(unsigned int, unsigned char);
int func_8001FF4C(unsigned int identifier, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B3A0(identifier, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FFA0
 * Source:   cloud/matches/boot_tail/func_8001FFA0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FFA0.c:func_8001FFA0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B4A4(unsigned int, unsigned short);
int func_8001FFA0(unsigned int identifier, unsigned short value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B4A4(identifier, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_8001FFF4
 * Source:   cloud/matches/boot_tail/func_8001FFF4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001FFF4.c:func_8001FFF4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B29C(unsigned int, unsigned char);
int func_8001FFF4(unsigned int identifier, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B29C(identifier, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_80020048
 * Source:   cloud/matches/boot_tail/func_80020048.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020048.c:func_80020048 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_80020048(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        identifier = func_8001EDF4(identifier);
        while (identifier != 0xFFFFFFFFU) {
            index = identifier & 255;
            if (D_8004BEB8[index].identifier60 == identifier) {
                result = 0;
                if (D_8004BEB8[index].flags24 & 2) {
                    func_80020610(64, index, D_8004BEB8[index].channel55, value);
                } else {
                    func_80020610(64, index, D_8004BEB8[index].channel4B, value);
                }
                identifier = D_8004BEB8[index].next10;
            } else {
                func_800145DC();
                return result;
            }
        }
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_80020174
 * Source:   cloud/matches/boot_tail/func_80020174.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020174.c:func_80020174 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001B1D0(unsigned short, unsigned char, unsigned char);
int func_80020174(unsigned short item, unsigned char channel, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B1D0(item, channel, value);
        func_800145DC();
    }
    return result;
}

/* PROMOTED 2026-10-05 — func_800201D0
 * Source:   cloud/matches/boot_tail/func_800201D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800201D0.c:func_800201D0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int func_8001EDF4(unsigned int);
unsigned int func_800201D0(unsigned int value)
{
    if (func_8001EDF4(value) != -1) {
        return value;
    }
    return 0xFFFFFFFF;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_80020200.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_8002021C.s")
/* PROMOTED 2026-10-04 — func_80020274
 * Source:   cloud/matches/boot_tail/func_80020274.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020274.c:func_80020274 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80018E6C(void);
extern void func_8001D578(void);
extern void func_8001FAE4(unsigned char);
void func_80020274(void)
{
    if (D_8002C630) {
        func_80014594();
        func_80018E6C();
        func_8001D578();
        func_8001FAE4(0);
        func_800145DC();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_800202C4.s")
/* PROMOTED 2026-10-04 — func_80020370
 * Source:   cloud/matches/boot_tail/func_80020370.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020370.c:func_80020370 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char *func_80017040(unsigned short);
extern void func_8001C19C(unsigned char, unsigned char);
void func_80020370(unsigned short identifier, unsigned char channel)
{
    unsigned char *state;
    if (D_8002C630) {
        func_80014594();
        state = func_80017040(identifier);
        if (state != 0) {
            if (channel != 254) {
                state[9] = channel;
                func_8001C19C(channel, 3);
            } else state[9] = 31;
        }
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_800203EC
 * Source:   cloud/matches/boot_tail/func_800203EC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800203EC.c:func_800203EC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001BE14(unsigned char, unsigned short, unsigned char);
void func_800203EC(unsigned char channel, unsigned short duration, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_8001BE14(channel, duration, mode);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_8002043C
 * Source:   cloud/matches/boot_tail/func_8002043C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8002043C.c:func_8002043C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001B9F8(unsigned char, unsigned short, unsigned char, unsigned char, unsigned int);
void func_8002043C(unsigned char channel, unsigned short duration, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_8001B9F8(channel, duration, mode, 0, 0);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_80020494
 * Source:   cloud/matches/boot_tail/func_80020494.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020494.c:func_80020494 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001B9F8(u8, u16, u8, u8, unsigned int);
void func_80020494(u8 channel, u16 duration, u8 first, u8 second)
{
    if (D_8002C630) {
        func_80014594();
        if (first) func_8001B9F8(channel, duration, 21, 0, 0);
        if (second) func_8001B9F8(channel, duration, 22, 0, 0);
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_80020518
 * Source:   cloud/matches/boot_tail/func_80020518.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020518.c:func_80020518 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8004F2F8;
void func_80020518(unsigned char value)
{
    D_8004F2F8 = value;
}

/* PROMOTED 2026-10-04 — func_80020528
 * Source:   cloud/matches/boot_tail/func_80020528.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020528.c:func_80020528 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80014BD8(void *);
void func_80020528(void *state)
{
    if (D_8002C630) func_80014BD8(state);
}

/* PROMOTED 2026-10-04 — func_80020558
 * Source:   cloud/matches/boot_tail/func_80020558.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80020558.c:func_80020558 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80014BF8(void);
void func_80020558(void)
{
    if (D_8002C630) {
        func_80014594();
        func_80014BF8();
        func_800145DC();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_80020598.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_800205E4.s")
