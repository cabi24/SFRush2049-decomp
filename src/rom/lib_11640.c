/* GENERATED ROM-aligned TU — segment 0x11640 (rom/lib_11640)
 * layout map 5aa0eac4fd4c7b89e56ec8126930be97345b2a4934e8ca8af0a9af74f0efb716; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"
#include "boot_tail_audio_record.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80010A40.s")
/* PROMOTED 2026-10-04 — func_80010C68
 * Source:   cloud/matches/boot_tail/func_80010C68.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010C68.c:func_80010C68 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_80038220;
extern unsigned char D_800381F8[];
extern void *D_80038210[];
extern unsigned int D_8003828C;
extern unsigned char *D_800381F0;
extern unsigned char D_80038040[];
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void osCreateMesgQueue(OSMesgQueue *, void **, int);
extern int osAiSetFrequency(unsigned int);
extern void osCreateThread(OSThread *, int, void (*)(void *), void *, void *, int);
extern void osStartThread(OSThread *);
extern void func_80010A40(void *);
void func_80010C68(unsigned int *frequency)
{
    D_80038220 = 1;
    osCreateMesgQueue((OSMesgQueue *) D_800381F8, D_80038210, 4);
    osSetEventMesgAlt(6, (OSMesgQueue *) D_800381F8, &D_80038220);
    D_8003828C = osAiSetFrequency(*frequency);
    *frequency = D_8003828C;
    D_800381F0 = D_80038018(1024, 128);
    osCreateThread((OSThread *) D_80038040, 0, func_80010A40, 0,
                   D_800381F0 + 1024, 122);
    osStartThread((OSThread *) D_80038040);
}

/* PROMOTED 2026-10-04 — func_80010D3C
 * Source:   cloud/work/boot_tail_promotion/sources/func_80010D3C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80010D3C.c:func_80010D3C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80010D3C(unsigned int *frequency)
{
    D_8003828C = osAiSetFrequency(*frequency);
    *frequency = D_8003828C;
}

/* PROMOTED 2026-10-05 — func_80010D74
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80010D74.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80010D74.c:func_80010D74 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int osJamMesg(OSMesgQueue *, OSMesg, int);
extern void (*D_8003801C)(void *);
extern void *D_80038228[];
void func_80010D74(void)
{
    unsigned char command[1];
    command[0] = 255;
    osJamMesg((OSMesgQueue *)D_800381F8, command, 1);
    D_8003801C(D_80038228[0]);
    D_8003801C(D_800381F0);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80010DD8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80010E80.s")
/* PROMOTED 2026-10-04 — func_80011074
 * Source:   cloud/matches/boot_tail/func_80011074.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80011074.c:func_80011074 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_800382E8;
extern void (*D_8003801C)(void *);
extern void func_800118C0(void);
void func_80011074(void)
{
    if (D_800382E8 != 0) {
        func_800118C0();
        D_8003801C(D_800382E8);
        D_800382E8 = 0;
    }
}

/* PROMOTED 2026-10-04 — func_800110C4
 * Source:   cloud/matches/boot_tail/func_800110C4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800110C4.c:func_800110C4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_800382F0;
extern void func_80011074(void);
void func_800110C4(void)
{
    func_80011074();
    if (D_800382F0 != 0) {
        D_8003801C(D_800382F0);
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011104.s")
/* PROMOTED 2026-10-04 — func_8001144C
 * Source:   cloud/matches/boot_tail/func_8001144C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001144C.c:func_8001144C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 * Relocked: 2026-10-05 (wave 4) as the two-cell D_800382D8 table:
 *           cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_8001144C.c
 *           (score0 after reloc_addrs.us.txt names D_800382DC as D_800382D8+4)
 */
extern unsigned short *D_800382D8[2];
extern void *D_80038298;
extern void *D_800382E4;
void func_8001144C(void)
{
    D_8003801C(D_800382D8[0]);
    D_8003801C(D_800382D8[1]);
    D_8003801C(D_80038298);
    D_8003801C(D_800382E4);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_800114C0.s")
/* PROMOTED 2026-10-04 — func_80011848
 * Source:   cloud/matches/boot_tail/func_80011848.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80011848.c:func_80011848 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern volatile unsigned char D_800382CC;
extern void *D_8003802C;
void func_80011848(void)
{
    while (D_800382CC != 0) {
    }
    D_8003801C(D_8003802C);
}

/* PROMOTED 2026-10-05 — func_80011894
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80011894.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80011894.c:func_80011894 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_80038038;
extern unsigned int D_8003803C;
extern unsigned short D_80038028;
void func_80011894(void)
{
    unsigned int value;
    value = (unsigned int)D_8003802C;
    D_8003803C = value & ~15U;
    D_80038038 = value;
    D_80038028 = 0;
}

/* PROMOTED 2026-10-04 — func_800118C0
 * Source:   cloud/work/boot_tail_promotion/sources/func_800118C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_800118C0.c:func_800118C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern volatile unsigned char D_800382CD;
extern void (*D_80038004)(void);
extern void osYieldThread(void);
void func_800118C0(void)
{
    if (D_800382CD != 0) {
        D_80038004();
        osYieldThread();
        D_800382CD = 0;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011910.s")
/* PROMOTED 2026-10-04 — func_800119E0
 * Source:   cloud/matches/boot_tail/func_800119E0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800119E0.c:func_800119E0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short *D_800382D0;
extern unsigned short *D_800382D4;
extern void func_80011910(void *, unsigned short);
void func_800119E0(void)
{
    func_80011910(D_800382D0, *D_800382D4);
}

/* PROMOTED 2026-10-04 — func_80011A10
 * Source:   cloud/matches/boot_tail/func_80011A10.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80011A10.c:func_80011A10 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80011A10(AudioState *audio)
{
    audio->state = 0;
    audio->step = 0;
    audio->saved_value = 0.0f;
    audio->value = 0.0f;
    audio->count = audio->initial_count;
    audio->scale = 1.0f;
}

/* PROMOTED 2026-10-04 — func_80011A3C
 * Source:   cloud/matches/boot_tail/func_80011A3C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80011A3C.c:func_80011A3C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80011A3C(AudioState *audio)
{
    audio->state = 3;
    audio->step = 0;
    audio->saved_value = audio->value;
    audio->scale = 0.0f;
    audio->count = audio->release_count;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011A64.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011C1C.s")
/* PROMOTED 2026-10-05 — func_80011C84
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80011C84.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80011C84.c:func_80011C84 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
void func_80011C84(unsigned short index)
{
    AudioState *audio = &D_80038294[index];
    func_80011A10(audio);
    audio->active = 1;
}

/* PROMOTED 2026-10-04 — func_80011CD8
 * Source:   cloud/work/boot_tail_promotion/sources/func_80011CD8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80011CD8.c:func_80011CD8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern unsigned char D_800382B0[];
void func_80011CD8(void)
{
    if (D_800382CC != 0) {
        osRecvMesg((OSMesgQueue *)D_800382B0, 0, 1);
        D_800382CC = 0;
        osYieldThread();
    }
}

/* PROMOTED 2026-10-04 — func_80011D24
 * Source:   cloud/matches/boot_tail/func_80011D24.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80011D24.c:func_80011D24 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short D_80038028;
extern void (*D_80038010)(void *, unsigned short, void *);
void func_80011D24(void)
{
    if (D_80038028 != 0) {
        D_800382CC = 1;
        D_80038010(D_8003802C, D_80038028, D_800382B0);
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011D74.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011F60.s")
/* PROMOTED 2026-10-04 — func_800121BC
 * Source:   cloud/matches/boot_tail/func_800121BC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800121BC.c:func_800121BC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_8003833C;
extern void *D_80038338;
void func_800121BC(void)
{
    D_8003801C(D_8003833C);
    D_8003801C(D_80038338);
}

/* PROMOTED 2026-10-04 — func_80012200
 * Source:   cloud/matches/boot_tail/func_80012200.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80012200.c:func_80012200 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct AudioNode { struct AudioNode *next; unsigned char unknown04[12]; unsigned short countdown; } AudioNode;
extern AudioNode *D_80038344;
void func_80012200(void)
{
    AudioNode *node;
    for (node = D_80038344; node != 0; node = node->next) {
        if (node->countdown != 0) {
            --node->countdown;
        }
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80012234.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_800123A8.s")
/* PROMOTED 2026-10-04 — func_8001261C
 * Source:   cloud/matches/boot_tail/func_8001261C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001261C.c:func_8001261C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_80038350;
extern void *D_8003834C;
void func_8001261C(void)
{
    D_8003801C(D_80038350);
    D_8003801C(D_8003834C);
}

/* PROMOTED 2026-10-04 — func_80012660
 * Source:   cloud/matches/boot_tail/func_80012660.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80012660.c:func_80012660 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; } AudioCacheNode;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
AudioCacheNode *func_80012660(unsigned int tag)
{
    AudioCacheNode *node;
    if (D_8003835C != 0) {
        node = D_8003835C;
        D_8003835C = node->next;
        if (D_8003835C != 0) {
            D_8003835C->previous = 0;
        }
        if (D_80038358 == 0) {
            node->next = D_80038354;
            if (node->next != 0) {
                D_80038354->previous = node;
            }
            D_80038354 = node;
            D_80038358 = node;
        } else {
            node->next = 0;
            node->previous = D_80038358;
            D_80038358->next = node;
            D_80038358 = node;
        }
        return node;
    }
    node = D_80038354;
    node->next->previous = 0;
    D_80038354 = node->next;
    D_80038358->next = node;
    node->previous = D_80038358;
    node->next = 0;
    D_80038358 = node;
    return node;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80012730.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80012D18.s")
/* PROMOTED 2026-10-05 — func_80013964
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80013964.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80013964.c:func_80013964 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8003829E;
extern unsigned short D_800382E0[];
extern unsigned int D_80038034;
extern unsigned int D_80038030;
void func_80013964(void)
{
    unsigned int cursor;
    D_800382D4 = &D_800382E0[D_8003829E];
    D_800382D0 = D_800382D8[D_8003829E];
    cursor = (unsigned int)D_800382D0 + 16;
    D_80038034 = cursor & ~15U;
    D_80038030 = cursor;
    *D_800382D4 = 0;
    *D_800382D0 = 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_800139D4.s")
/* PROMOTED 2026-10-05 — func_80013C84
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80013C84.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80013C84.c:func_80013C84 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
extern unsigned short D_8003829C;
void func_80013C84(void)
{
    int i;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].active != 0) {
            D_80038294[i].position = D_80038294[i].current;
            D_80038294[i].current = (unsigned int) D_80038294[i].sample_position;
        }
    }
}

/* PROMOTED 2026-10-04 — func_80013D70
 * Source:   cloud/matches/boot_tail/func_80013D70.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80013D70.c:func_80013D70 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern unsigned int D_800382A0;
extern unsigned int D_800382A4;
extern void *D_80038228[];
extern void func_800198C8(void);
extern void func_8001B154(void);
extern void func_800139D4(void *, unsigned short);
int func_80013D70(unsigned short index, unsigned short count)
{
    if (D_8002C630 != 0) {
        if (--D_800382A0 == 0) {
            D_800382A0 = D_800382A4;
            func_800198C8();
            func_8001B154();
        }
    }
    func_800139D4(D_80038228[index], count);
    return 1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80013DEC.s")
/* PROMOTED 2026-10-04 — func_800140F8
 * Source:   cloud/matches/boot_tail/func_800140F8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800140F8.c:func_800140F8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short D_80038360;
extern void (*D_80038020)(void);
extern void (*D_80038024)(void);
extern void func_80013DEC(void);
extern void (*D_80038008)(void (*)(void));
void func_800140F8(void)
{
    D_80038360 = 0xFFFF;
    D_80038020 = 0;
    D_80038024 = 0;
    D_80038008(func_80013DEC);
}

/* PROMOTED 2026-10-04 — func_80014140
 * Source:   cloud/matches/boot_tail/func_80014140.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014140.c:func_80014140 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned short D_80038292;
extern volatile short D_80038362;
extern void (*D_8003800C)(void (*)(void));
void func_80014140(void)
{
    D_80038362 = D_80038292;
    while (D_80038362 > 0) {
    }
    D_8003800C(func_80013DEC);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014198.s")
/* PROMOTED 2026-10-04 — func_80014374
 * Source:   cloud/matches/boot_tail/func_80014374.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014374.c:func_80014374 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_80014374(unsigned int flags)
{
    unsigned char result;
    if (flags & 0x10000) {
        result = 2;
    } else if (flags & 0x20000) {
        result = 3;
    } else if (flags & 0x40000) {
        result = 4;
    } else if (flags & 0x80000) {
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

/* PROMOTED 2026-10-04 — func_800143C0
 * Source:   cloud/matches/boot_tail/func_800143C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800143C0.c:func_800143C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_80038290;
extern unsigned char D_80038291;
extern void func_80014550(void);
extern void func_80010C68(unsigned int *);
extern unsigned char func_80014374(unsigned int);
extern void func_80014198(unsigned int *, unsigned short, unsigned short, unsigned char);
int func_800143C0(unsigned int *frequency, unsigned short count, unsigned short size, unsigned int flags)
{
    D_80038291 = 0;
    func_80014550();
    D_80038290 = (flags & 0x100000) != 0;
    func_80010C68(frequency);
    func_80014198(frequency, count, size, func_80014374(flags));
    return 0;
}

/* PROMOTED 2026-10-04 — func_80014434
 * Source:   cloud/matches/boot_tail/func_80014434.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014434.c:func_80014434 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80010D3C(unsigned int *);
int func_80014434(unsigned int *frequency, unsigned short count, unsigned short size, unsigned int flags)
{
    func_80014550();
    func_80010D3C(frequency);
    func_80014198(frequency, count, size, func_80014374(flags));
    return 0;
}

/* PROMOTED 2026-10-04 — func_80014488
 * Source:   cloud/matches/boot_tail/func_80014488.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014488.c:func_80014488 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80014140(void);
extern void func_800110C4(void);
extern void func_80011848(void);
extern void func_800121BC(void);
extern void func_8001261C(void);
extern void func_8001144C(void);
extern void func_80010D74(void);
void func_80014488(void)
{
    if (D_8002C630 != 0) {
        func_80014140();
        func_800118C0();
        func_800110C4();
        func_80011848();
        func_800121BC();
        func_8001261C();
        func_8001144C();
        func_80010D74();
    }
}

/* PROMOTED 2026-10-04 — func_800144F0
 * Source:   cloud/matches/boot_tail/func_800144F0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800144F0.c:func_800144F0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800144F0(void)
{
    if (D_8002C630 != 0) {
        func_80014140();
        func_800118C0();
        func_800110C4();
        func_80011848();
        func_800121BC();
        func_8001261C();
        func_8001144C();
    }
}

/* PROMOTED 2026-10-04 — func_80014550
 * Source:   cloud/matches/boot_tail/func_80014550.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014550.c:func_80014550 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct MessageQueue MessageQueue;
extern MessageQueue D_80038368;
extern void *D_80038380;
void func_80014550(void)
{
    osCreateMesgQueue(&D_80038368, &D_80038380, 1);
    osJamMesg(&D_80038368, 0, 0);
}

/* PROMOTED 2026-10-04 — func_80014594
 * Source:   cloud/matches/boot_tail/func_80014594.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014594.c:func_80014594 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int D_8002C5DC;
void func_80014594(void)
{
    if (D_8002C5DC == 0) {
        osRecvMesg(&D_80038368, 0, 1);
    }
    D_8002C5DC++;
}

/* PROMOTED 2026-10-04 — func_800145DC
 * Source:   cloud/matches/boot_tail/func_800145DC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800145DC.c:func_800145DC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800145DC(void)
{
    if (D_8002C5DC > 0) {
        if (--D_8002C5DC == 0) {
            osJamMesg(&D_80038368, 0, 0);
        }
    }
}

/* PROMOTED 2026-10-04 — func_80014624
 * Source:   cloud/matches/boot_tail/func_80014624.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014624.c:func_80014624 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014624(void)
{
    osRecvMesg(&D_80038368, 0, 1);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014650.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_8001467C.s")
/* PROMOTED 2026-10-04 — func_800146AC
 * Source:   cloud/matches/boot_tail/func_800146AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800146AC.c:func_800146AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int func_800146AC(void)
{
    return 0;
}

/* PROMOTED 2026-10-05 — func_800146B4
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_800146B4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_800146B4.c:func_800146B4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
#pragma pack(1)
typedef struct SampleInfo_800146B4 { unsigned int frequency; void *data; unsigned int offset; unsigned int length; unsigned int loopStart; unsigned int loopLength; unsigned char format; } SampleInfo_800146B4;
#pragma pack()
extern void func_80011C1C(unsigned short, unsigned short);
void func_800146B4(unsigned int index, SampleInfo_800146B4 *sample, unsigned char reset)
{
    func_80011C1C(index, 0);
    if (reset) {
        D_80038294[index].initial_count = 0;
        D_80038294[index].value22 = 0;
        D_80038294[index].value24 = 1.0f;
        D_80038294[index].release_count = 20;
    }
    D_80038294[index].data = sample->data;
    D_80038294[index].sample_position = sample->offset;
    D_80038294[index].current = D_80038294[index].position = sample->offset;
    D_80038294[index].length = sample->length;
    D_80038294[index].loop_start = sample->loopStart;
    D_80038294[index].loop_length = sample->loopLength;
    D_80038294[index].format = sample->format;
    if (D_80038294[index].loop_length != 0) {
        D_80038294[index].tail = D_80038294[index].length -
            D_80038294[index].loop_length - D_80038294[index].loop_start;
        if (D_80038294[index].tail < 10) D_80038294[index].tail = 0;
    } else {
        D_80038294[index].tail = 0;
    }
    if (((unsigned long)sample->data & 0xF0000000UL) != 0x80000000UL) {
        D_80038294[index].flags |= 1;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_8001489C.s")
/* PROMOTED 2026-10-04 — func_800148F8
 * Source:   cloud/matches/boot_tail/func_800148F8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800148F8.c:func_800148F8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_800148F8(int index, __unaligned unsigned short *data)
{
    D_80038294[index].value20 = data[0];
    D_80038294[index].value22 = data[1];
    D_80038294[index].value24 = (float)(unsigned int)data[2] * (1.0 / 4096.0);
    D_80038294[index].release_count = data[3];
}

/* PROMOTED 2026-10-04 — func_800149BC
 * Source:   cloud/matches/boot_tail/func_800149BC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800149BC.c:func_800149BC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80011C84(unsigned short);
void func_800149BC(unsigned int index)
{
    func_80011C84((unsigned short)index);
}

/* PROMOTED 2026-10-05 — func_800149DC
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_800149DC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_800149DC.c:func_800149DC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
void func_800149DC(int index)
{
    D_80038294[index].pending = 0;
}

/* PROMOTED 2026-10-04 — func_80014A04
 * Source:   cloud/matches/boot_tail/func_80014A04.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014A04.c:func_80014A04 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014A04(int unused)
{
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014A0C.s")
/* PROMOTED 2026-10-05 — func_80014A74
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014A74.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014A74.c:func_80014A74 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
extern void func_8001E0E0(unsigned short *, unsigned short *, unsigned int, unsigned int, unsigned int, unsigned short *, unsigned int, unsigned short *);
void func_80014A74(int index, unsigned int volume, unsigned int pan, unsigned int span, unsigned int aux)
{
    AudioSlot *slot;
    slot = &D_80038294[index];
    func_8001E0E0(&slot->value42, &slot->value40, volume, pan, span,
                  &slot->value44, aux, &slot->value46);
}

/* PROMOTED 2026-10-05 — func_80014AF0
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c:func_80014AF0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
void func_80014AF0(int index)
{
    if (D_80038294[index].active) {
        D_80038294[index].active = 0;
        D_80038294[index].release_pending = 1;
    }
}

/* PROMOTED 2026-10-05 — func_80014B3C
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014B3C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014B3C.c:func_80014B3C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
void func_80014B3C(int index)
{
    if (D_80038294[index].active != 0) {
        D_80038294[index].release_count = 20;
        if (D_80038294[index].pending != 0) {
            D_80038294[index].pending = 0;
        } else {
            func_80011A3C(&D_80038294[index]);
        }
    }
}

/* PROMOTED 2026-10-05 — func_80014BB0
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014BB0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014BB0.c:func_80014BB0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
void func_80014BB0(int index)
{
    D_80038294[index].active = 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014BD8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014BF8.s")
/* PROMOTED 2026-10-05 — func_80014C18
 * Source:   cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014C18.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014C18.c:func_80014C18 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "boot_tail_audio_record.h"
unsigned int func_80014C18(int index)
{
    return D_80038294[index].position;
}

/* PROMOTED 2026-10-04 — func_80014C40
 * Source:   cloud/matches/boot_tail/func_80014C40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014C40.c:func_80014C40 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osWritebackDCache(void *, int);
void func_80014C40(void *buffer, unsigned int samples)
{
    osWritebackDCache(buffer, samples * 2);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014C60.s")
/* PROMOTED 2026-10-04 — func_80014CAC
 * Source:   cloud/matches/boot_tail/func_80014CAC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014CAC.c:func_80014CAC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *(*D_80038014)(void *);
void *func_80014CAC(void *address)
{
    unsigned int region;
    region = (unsigned int)address & 0xFF000000;
    if (region != 0x80000000 && region != 0xB0000000) {
        address = D_80038014(address);
    }
    return address;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014CF4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014CFC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014D08.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80014D14.s")
/* PROMOTED 2026-10-04 — func_80014D1C
 * Source:   cloud/matches/boot_tail/func_80014D1C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014D1C.c:func_80014D1C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014D1C(void)
{
    D_80038291 = 1;
}

