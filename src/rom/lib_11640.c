/* GENERATED ROM-aligned TU — segment 0x11640 (rom/lib_11640)
 * layout map 5aa0eac4fd4c7b89e56ec8126930be97345b2a4934e8ca8af0a9af74f0efb716; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"
/* ROM_OWNED_RODATA_BEGIN func_80011910 */
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11640/func_80011910.bss.s")
/* ROM_OWNED_RODATA_END func_80011910 */
#include "boot_tail_audio_record.h"

/* PROMOTED 2026-10-05 — func_80010A40
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80010A40.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80010A40.c:func_80010A40 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_80038290;
extern unsigned char D_80038291;
extern volatile unsigned char D_80038288;
extern unsigned short D_80038292;
extern unsigned int D_8003828C;
extern void *D_80038228[];
extern unsigned char D_800381F8[];
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void bzero(void *, int);
extern void osWritebackDCache(void *, int);
extern int osAiSetNextBuffer(void *, unsigned int);
extern int osRecvMesg(OSMesgQueue *, void **, int);
void func_80010A40(void *argument)
{
    void *message;
    int i;
    unsigned int stop;
    if (D_80038290 != 0) {
        D_80038292 = 24;
    } else {
        D_80038292 = 16;
    }
    if (D_8003828C <= 22050) {
        D_80038292 >>= 1;
    }
    D_80038228[0] = D_80038018(D_80038292 * 768, 128);
    for (i = 1; i < D_80038292; i++) {
        D_80038228[i] = (unsigned char *)D_80038228[i - 1] + 768;
    }
    bzero(D_80038228[0], D_80038292 * 768);
    osWritebackDCache(D_80038228[0], D_80038292 * 768);
    D_80038288 = 0;
    osAiSetNextBuffer(D_80038228[0], 768);
    stop = 0;
    do {
        osRecvMesg((OSMesgQueue *)D_800381F8, &message, 1);
        if (D_80038291 == 0) {
            switch (*(unsigned char *) message) {
            case 1:
                D_80038288 = (D_80038288 + 1) % D_80038292;
                osAiSetNextBuffer(D_80038228[D_80038288], 768);
                break;
            case 255:
                stop = 1;
                break;
            }
        }
    } while (stop == 0);
}

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
    osSetEventMesg(6, (OSMesgQueue *) D_800381F8, &D_80038220);
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
extern int osSendMesg(OSMesgQueue *, OSMesg, int);
extern void (*D_8003801C)(void *);
extern void *D_80038228[];
void func_80010D74(void)
{
    unsigned char command[1];
    command[0] = 255;
    osSendMesg((OSMesgQueue *)D_800381F8, command, 1);
    D_8003801C(D_80038228[0]);
    D_8003801C(D_800381F0);
}

/* PROMOTED 2026-10-12 — func_80010DD8
 * Source:   cloud/matches/boot_tail/func_80010DD8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010DD8.c:func_80010DD8 (score0); rom_tu.h context: cloud/work/frontier/w28p/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_800382F4;
extern void *D_800382E8;
extern void *D_800382F0;
void func_80010DD8(unsigned int rate, unsigned short duration)
{
    unsigned int count;
    if (duration != 0) {
        count = duration * rate / 1000;
        count = 192 - count % 192 + count;
        D_800382F4 = count;
        D_800382E8 = 0;
        D_800382F0 = D_80038018(count * 2, 0);
    } else {
        D_800382F0 = 0;
    }
}

/* PROMOTED 2026-10-11 — func_80010E80
 * Source:   cloud/work/frontier/w22c/sources/func_80010E80.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w22c/sources/func_80010E80.c:func_80010E80 (score0); rom_tu.h context: cloud/work/frontier/w22c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct AudioConfiguration_80010E80 { unsigned short duration; unsigned short delays[8]; unsigned char gains[8]; unsigned char count; unsigned char feedback; unsigned char mode; unsigned char unknown1D; short filter[4]; } AudioConfiguration_80010E80;
typedef struct AudioDelayState_80010E80 { void *buffer; unsigned int length; unsigned short count; unsigned short feedback; unsigned int delays[8]; unsigned short gains[8]; unsigned int unknown3C; short filter[4]; } AudioDelayState_80010E80;
extern void *D_800382F0;
extern void *D_800382E8;
extern int D_800382EC;
extern void func_80014594(void);
extern void func_800145DC(void);
void func_80010E80(void *configuration)
{
    unsigned int count;
    AudioDelayState_80010E80 *state;
    unsigned int i;
    if (D_800382F0 != 0) {
        count = ((AudioConfiguration_80010E80 *)configuration)->duration * D_8003828C / 1000;
        count = 192 - count % 192 + count;
        state = D_80038018(72, 0);
        state->buffer = D_800382F0;
        i = count * 2;
        bzero(state->buffer, i);
        osWritebackDCache(state->buffer, i);
        state->length = count;
        state->feedback = ((AudioConfiguration_80010E80 *)configuration)->feedback << 8;
        state->count = ((AudioConfiguration_80010E80 *)configuration)->count;
        for (i = 0; i < ((AudioConfiguration_80010E80 *)configuration)->count; i++) {
            state->delays[i] = (((AudioConfiguration_80010E80 *)configuration)->delays[i] * D_8003828C / 1000 + 3) & 0xFFFC;
            state->gains[i] = ((AudioConfiguration_80010E80 *)configuration)->gains[i] << 8;
        }
        if (((AudioConfiguration_80010E80 *)configuration)->mode == 1) {
            for (i = 0; i < 4; i++) {
                state->filter[i] = ((AudioConfiguration_80010E80 *)configuration)->filter[i];
            }
        } else {
            state->filter[0] = 0;
            state->filter[1] = 0;
            state->filter[2] = 0;
            state->filter[3] = 32767;
        }
        osWritebackDCache(state, 72);
        func_80014594();
        D_800382EC = 0;
        D_800382E8 = state;
        func_800145DC();
    }
}

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

/* PROMOTED 2026-10-11 — func_80011104
 * Source:   cloud/work/frontier/w20c/sources/func_80011104.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_80011104.c:func_80011104 (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_80038298;
extern unsigned short D_8003829C;
extern unsigned char D_8003829E;
extern float D_8002D8B8;
extern float D_8002D8BC;
extern unsigned short D_800382CE;
extern unsigned short *D_800382D8[2];
extern unsigned short D_800382E0[2];
extern void *D_800382E4;
void func_80011104(unsigned short count, unsigned int frequency)
{
    unsigned short i;
    D_8003829C = count;
    D_80038298 = D_80038018(D_8003829C * sizeof(AudioState), 0);
    D_80038294 = D_80038298;
    D_8003829E = 0;
    for (i = 0; i < D_8003829C; i++) {
        ((AudioState *)D_80038298)[i].active = 0;
        ((AudioState *)D_80038298)[i].release_pending = 0;
        ((AudioState *)D_80038298)[i].unknown60 = 255;
    }
    if (D_80038290 == 0) {
        D_800382CE = (unsigned short) (((float)frequency / 50.0f / 192.0f) * D_8002D8B8) + 1;
    } else {
        D_800382CE = (unsigned short) (((float)frequency / 25.0f / 192.0f) * D_8002D8BC) + 1;
    }
    D_800382D8[0] = D_80038018(D_800382CE * 2576U, 0);
    D_800382D8[1] = D_80038018(D_800382CE * 2576U, 0);
    D_800382E0[1] = 0;
    D_800382E0[0] = 0;
    D_800382E4 = D_80038018(664, 0);
    bzero(D_800382E4, 664);
    osWritebackDCache(D_800382E4, 664);
}

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

/* PROMOTED 2026-10-11 — func_800114C0
 * Source:   cloud/work/frontier/w20c/sources/func_800114C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_800114C0.c:func_800114C0 (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "PR/os_message.h"
extern float D_8002D8C0;
extern float D_8002D8C4;
extern float D_8002D8C8;
extern unsigned short D_800382A8;
extern unsigned int D_800382AC;
extern void *D_8003802C;
extern unsigned char D_800382B0[];
extern OSMesg D_800382C8;
extern volatile unsigned char D_800382CC;
extern unsigned short D_8002C5D0[];
extern void func_80011F60(int);
extern void func_800123A8(unsigned short);
void func_800114C0(unsigned short count, unsigned int frequency, unsigned char mode)
{
    if (D_80038290 == 0) {
        D_800382A8 = count * ((unsigned short) (((float)frequency / 50.0f * D_8002D8C0) / 192.0f) + 1);
    } else {
        D_800382A8 = count * ((unsigned short) (((float)frequency / 25.0f * D_8002D8C4) / 192.0f) + 1);
    }
    D_800382AC = 784;
    if (D_800382AC < 192) {
        D_800382AC = 192;
    }
    D_800382AC = (unsigned int) ((float)(D_800382A8 * D_800382AC) * D_8002D8C8);
    D_800382AC = ((((D_800382AC + 63) >> 6) * 40) + 15) & 0xFFF0;
    D_8003802C = D_80038018(D_800382A8 * 2 * 12U, 0);
    osCreateMesgQueue((OSMesgQueue *)D_800382B0, &D_800382C8, 1);
    D_800382CC = 0;
    func_80011F60(D_800382A8);
    func_800123A8((unsigned short)(D_8002C5D0[mode] * count));
}

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

/* PROMOTED 2026-10-11 — func_80011A64
 * Source:   cloud/work/frontier/w21a/sources/func_80011A64.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21a/sources/func_80011A64.c:func_80011A64 (score0); rom_tu.h context: /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/ctx_w21ac.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80011A64(AudioState *audio, unsigned short samples, unsigned char *active)
{
    unsigned int increment;
    float ratio;
    increment = ((samples * 32000U) / D_8003828C) << 11;
    if (audio->pending == 0 && audio->state != 3) {
        audio->state = 3;
        audio->step = 0;
        audio->saved_value = audio->value;
        audio->scale = 0.0f;
        audio->count = audio->release_count;
    } else if ((audio->step >> 16) >= audio->count) {
        switch (audio->state) {
        case 0:
            audio->state = 1;
            audio->step = 0;
            audio->saved_value = 1.0f;
            audio->scale = audio->value24;
            audio->count = audio->value22;
            break;
        case 1:
            audio->state = 2;
            audio->saved_value = audio->value24;
            audio->scale = audio->value24;
            audio->value = audio->value24;
            return;
        case 3:
            *active = 0;
            audio->value = 0.0f;
            return;
        }
    }
    if (audio->count != 0) {
        ratio = audio->step / (audio->count * 65536.0f);
    } else {
        ratio = 1.0f;
    }
    audio->value = (audio->scale - audio->saved_value) * ratio + audio->saved_value;
    audio->step += increment;
}

/* PROMOTED 2026-10-11 — func_80011C1C
 * Source:   cloud/work/frontier/w17g/sources/func_80011C1C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80011C1C.c:func_80011C1C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80011C1C(unsigned short index, unsigned short flags)
{
    AudioState *audio;
    audio = &D_80038294[index];
    audio->active = 0;
    audio->flags = flags;
    audio->rate = 4096;
    audio->pending = 1;
    audio->unknown18[0] = 0;
    audio->unknown18[1] = 0;
    audio->unknown18[2] = 0;
    audio->unknown18[3] = 0;
    audio->value40 = 0;
    audio->value42 = 0;
    audio->value44 = 0;
    audio->value46 = 0;
}

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

/* PROMOTED 2026-10-11 — func_80011D74
 * Source:   cloud/work/frontier/w21a/sources/func_80011D74.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21a/sources/func_80011D74.c:func_80011D74 (score0); rom_tu.h context: /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/ctx_w21ac.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern double func_8001E688(double);
void func_80011D74(AudioState *audio, unsigned short samples)
{
    double increment;
    double loop_end;
    increment = (audio->rate * samples) * 0.000244140625;
    if (audio->loop_length != 0 && (audio->pending != 0 || audio->tail == 0)
        && func_8001E688(audio->sample_position) <= audio->loop_start + audio->loop_length) {
        loop_end = audio->loop_start + audio->loop_length;
        if (audio->sample_position + increment < loop_end) {
            audio->sample_position += increment;
        } else {
            increment -= loop_end - audio->sample_position;
            increment -= func_8001E688(increment / audio->loop_length) * audio->loop_length;
            audio->sample_position = audio->loop_start + increment;
        }
    } else {
        if (audio->sample_position + increment >= audio->length) {
            audio->sample_position = audio->length - 1;
        } else {
            audio->sample_position += increment;
        }
    }
}

typedef struct AudioNode { struct AudioNode *next; struct AudioNode *previous; unsigned char *buffer; unsigned int unknown0C; unsigned short countdown; unsigned short unknown12; } AudioNode;
extern AudioNode *D_80038344;
/* PROMOTED 2026-10-11 — func_80011F60
 * Source:   cloud/work/frontier/w18c/sources/func_80011F60.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80011F60.c:func_80011F60 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osInvalDCache(void *, int);
extern AudioNode *D_80038338;
extern unsigned char *D_8003833C;
extern unsigned int D_80038340;
extern AudioNode *D_80038348;
void func_80011F60(int count)
{
    int i;
    D_80038338 = D_80038018(count * sizeof(AudioNode), 0);
    D_8003833C = D_80038018(count * 256, 0);
    osInvalDCache(D_8003833C, count * 256);
    D_80038344 = 0;
    D_80038348 = D_80038338;
    D_80038338[0].buffer = D_8003833C;
    D_80038338[0].previous = 0;
    for (i = 1; i < count; i++) {
        D_80038338[i].previous = &D_80038338[i - 1];
        D_80038338[i - 1].next = &D_80038338[i];
        D_80038338[i].buffer = &D_8003833C[i * 256];
    }
    D_80038338[i - 1].next = 0;
    D_80038340 = 0;
}

/* PROMOTED 2026-10-04 — func_800121BC
 * Source:   cloud/matches/boot_tail/func_800121BC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800121BC.c:func_800121BC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char *D_8003833C;
extern AudioNode *D_80038338;
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
void func_80012200(void)
{
    AudioNode *node;
    for (node = D_80038344; node != 0; node = node->next) {
        if (node->countdown != 0) {
            --node->countdown;
        }
    }
}

/* PROMOTED 2026-10-12 — func_80012234
 * Source:   cloud/matches/boot_tail/func_80012234.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80012234.c:func_80012234 (score0); rom_tu.h context: cloud/work/frontier/w28p/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct DmaEntry_80012234 { int length; unsigned int source; unsigned char *destination; } DmaEntry_80012234;
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
unsigned char *func_80012234(unsigned int source)
{
    AudioNode *node;
    for (node = D_80038344; node != 0; node = node->next) {
        if (source == node->unknown0C) {
            node->countdown = 2;
            return node->buffer;
        }
    }
    if (D_80038348 != 0) {
        node = D_80038348;
        D_80038348 = node->next;
        if (D_80038348 != 0) {
            D_80038348->previous = 0;
        }
        if (D_80038344 != 0) {
            D_80038344->previous = node;
        }
        node->next = D_80038344;
        D_80038344 = node;
    } else {
        for (node = D_80038344; node != 0; node = node->next) {
            if (node->countdown == 0) {
                break;
            }
        }
    }
    node->countdown = 2;
    node->unknown0C = source;
    D_80038038 += 12;
    D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].source = source;
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].length = 0x100;
    ((DmaEntry_80012234 *)D_8003802C)[D_80038028].destination = node->buffer;
    D_80038028++;
    return node->buffer;
}

typedef struct AudioCacheNode { struct AudioCacheNode *next; struct AudioCacheNode *previous; unsigned int unknown08; unsigned int unknown0C; unsigned int unknown10; unsigned char *buffer; } AudioCacheNode;
extern AudioCacheNode *D_80038354;
extern AudioCacheNode *D_80038358;
extern AudioCacheNode *D_8003835C;
/* PROMOTED 2026-10-11 — func_800123A8
 * Source:   cloud/work/frontier/w18c/sources/func_800123A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_800123A8.c:func_800123A8 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern AudioCacheNode *D_80038350;
extern unsigned char *D_8003834C;
void func_800123A8(unsigned short count)
{
    int i;
    D_8003834C = D_80038018(count * 1536, 0);
    osInvalDCache(D_8003834C, count * 1536);
    D_80038350 = D_80038018(count * sizeof(AudioCacheNode), 0);
    D_80038354 = 0;
    D_80038358 = 0;
    D_8003835C = D_80038350;
    D_80038350[0].previous = 0;
    D_80038350[0].buffer = D_8003834C;
    for (i = 1; i < count; i++) {
        D_80038350[i - 1].next = &D_80038350[i];
        D_80038350[i].previous = &D_80038350[i - 1];
        D_80038350[i].buffer = &D_8003834C[i * 1536];
    }
    D_80038350[i - 1].next = 0;
}

/* PROMOTED 2026-10-04 — func_8001261C
 * Source:   cloud/matches/boot_tail/func_8001261C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001261C.c:func_8001261C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern AudioCacheNode *D_80038350;
extern unsigned char *D_8003834C;
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

/* PROMOTED 2026-10-11 — func_80012730
 * Source:   cloud/work/frontier/w21c/sources/func_80012730.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w21c/sources/func_80012730.c:func_80012730 (score0); rom_tu.h context: /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/ctx_w21ac.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "types.h"
typedef struct DmaEntry_80012730 { int length; unsigned int source; unsigned char *destination; } DmaEntry_80012730;
typedef struct Fetch_80012730 { unsigned char *first; unsigned char *second; short first_length; short second_length; } Fetch_80012730;
extern AudioCacheNode *func_80012660(unsigned int tag);
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
void func_80012730(Fetch_80012730 *fetch, unsigned int addr, unsigned int length, unsigned int tag)
{
    unsigned int size;
    AudioCacheNode *previous;
    AudioCacheNode *node;
    unsigned char *pointers[2];
    unsigned int lengths[2];
    unsigned int starts[2];
    unsigned short count;
    unsigned int offset;
    unsigned int end;
    unsigned int low;

    end = addr + length;
    low = addr - 0x600;
    count = 0;
    for (node = D_80038358; node != NULL; node = previous) {
        previous = node->previous;
        if (tag != node->unknown0C || node->unknown08 >= end || low >= node->unknown08) {
            continue;
        }
        if (node->unknown10 != 0x600 && node->unknown08 + node->unknown10 < addr + length) {
            if (addr + length >= node->unknown08 + 0x600) {
                size = 0x600 - node->unknown10;
            } else {
                size = (addr + length - node->unknown08 - node->unknown10 + 0xFF) & ~0xFF;
            }
            D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08 + node->unknown10;
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = size;
            ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer + node->unknown10;
            D_80038028++;
            node->unknown10 += size;
        }
        if (node != D_80038358) {
            if (node->next != NULL) {
                node->next->previous = node->previous;
            }
            if (node->previous != NULL) {
                node->previous->next = node->next;
            } else {
                D_80038354 = node->next;
            }
            D_80038358->next = node;
            node->previous = D_80038358;
            node->next = NULL;
            D_80038358 = node;
        }
        if (addr < node->unknown08) {
            pointers[count] = node->buffer;
            starts[count] = node->unknown08;
            lengths[count] = length - node->unknown08 + addr;
            length -= lengths[count];
            count++;
        } else if (node->unknown08 + node->unknown10 < addr + length) {
            pointers[count] = node->buffer + addr - node->unknown08;
            starts[count] = addr;
            lengths[count] = node->unknown08 + node->unknown10 - addr;
            length -= lengths[count];
            count++;
            addr = node->unknown08 + node->unknown10;
        } else {
            pointers[count] = node->buffer + addr - node->unknown08;
            starts[count] = addr;
            lengths[count] = length;
            length = 0;
            count++;
            break;
        }
    }
    if (length != 0) {
        offset = (addr - tag) % 1536;
        do {
            node = func_80012660(tag);
            node->unknown0C = tag;
            node->unknown08 = addr - offset;
            if (length + offset > 0x600) {
                node->unknown10 = 0x600;
                D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
                addr = addr - offset + 0x600;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = 0x600;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer;
                D_80038028++;
                pointers[count] = node->buffer + offset;
                starts[count] = node->unknown08 + offset;
                lengths[count] = 0x600 - offset;
                length -= 0x600 - offset;
                count++;
            } else {
                node->unknown10 = (length + offset + 0xFF) & ~0xFF;
                D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].source = node->unknown08;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].length = node->unknown10;
                ((DmaEntry_80012730 *)D_8003802C)[D_80038028].destination = node->buffer;
                D_80038028++;
                pointers[count] = node->buffer + offset;
                starts[count] = node->unknown08 + offset;
                lengths[count] = length;
                length = 0;
                count++;
            }
            offset = 0;
        } while (length != 0);
    }
    if (count == 1) {
        fetch->first = pointers[0];
        fetch->first_length = lengths[0];
        fetch->second_length = 0;
        return;
    }
    if (starts[0] < starts[1]) {
        fetch->first = pointers[0];
        fetch->second = pointers[1];
        fetch->first_length = lengths[0];
        fetch->second_length = lengths[1];
        return;
    }
    fetch->first = pointers[1];
    fetch->second = pointers[0];
    fetch->first_length = lengths[1];
    fetch->second_length = lengths[0];
}

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

/* PROMOTED 2026-10-11 — func_800139D4
 * Source:   cloud/work/frontier/w19a/sources/func_800139D4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w19a/sources/func_800139D4.c:func_800139D4 (score0); rom_tu.h context: cloud/work/frontier/w19a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct AudioLoop { unsigned int unknown00; unsigned int length; } AudioLoop;
typedef struct AudioCommand { unsigned char unknown00[44]; unsigned short status; unsigned char unknown2E[22]; void *output; unsigned char unknown48[8]; } AudioCommand;
typedef struct AudioBlock { unsigned short count; unsigned short sample; unsigned int changed; void *work; AudioLoop *loop; AudioCommand commands[32]; } AudioBlock;
extern unsigned short D_8003829C;
extern unsigned short D_800382CE;
extern int D_800382EC;
extern void func_80012D18(AudioState *, unsigned short, unsigned char);
void func_800139D4(void *output, unsigned short samples)
{
    unsigned short i;
    unsigned char active;
    unsigned int changed;
    active = 0;
    changed = 0;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].release_pending != 0) {
            D_80038294[i].release_pending = 0;
            changed |= 1U << D_80038294[i].unknown60;
        }
        if (D_80038294[i].active != 0) {
            func_80012D18(&D_80038294[i], samples, active);
            active++;
        }
    }
    if (((AudioBlock *)D_800382D0)[*D_800382D4].count == 0) {
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[0].status = 0;
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[0].output = output;
    } else {
        ((AudioBlock *)D_800382D0)[*D_800382D4].commands[((AudioBlock *)D_800382D0)[*D_800382D4].count - 1].output = output;
    }
    ((AudioBlock *)D_800382D0)[*D_800382D4].work = D_800382E4;
    ((AudioBlock *)D_800382D0)[*D_800382D4].changed = changed;
    ((AudioBlock *)D_800382D0)[*D_800382D4].loop = D_800382E8;
    if (((AudioBlock *)D_800382D0)[*D_800382D4].loop != 0) {
        ((AudioBlock *)D_800382D0)[*D_800382D4].sample = D_800382EC / 192;
        D_800382EC += 192;
        if (D_800382EC == ((AudioLoop *)D_800382E8)->length) {
            D_800382EC = 0;
        }
    }
    (*D_800382D4)++;
    if (*D_800382D4 < D_800382CE) {
        D_80038034 = (unsigned int)((AudioBlock *)D_800382D0)[*D_800382D4].commands & ~15U;
        D_80038030 = (unsigned int)((AudioBlock *)D_800382D0)[*D_800382D4].commands;
        ((AudioBlock *)D_800382D0)[*D_800382D4].count = 0;
    }
}

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

/* PROMOTED 2026-10-11 — func_80013DEC
 * Source:   cloud/work/frontier/w20c/sources/func_80013DEC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w20c/sources/func_80013DEC.c:func_80013DEC (score0); rom_tu.h context: cloud/work/frontier/w20c/ctx71_w20c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void (*D_80038020)(void);
extern void (*D_80038024)(void);
extern unsigned short *D_800382D8[];
extern unsigned short D_80038360;
extern volatile short D_80038362;
extern unsigned int __osDisableInt(void);
extern void __osRestoreInt(unsigned int);
extern void func_80014624(void);
extern void func_80014650(void);
extern void func_8001DDE0(void);
extern void func_80011CD8(void);
extern void func_80011894(void);
extern void func_80012200(void);
extern void func_80013964(void);
extern void func_80013C84(void);
extern int func_80013D70(unsigned short, unsigned short);
extern void func_80011D24(void);
extern void func_8001C508(void);
extern void func_800119E0(void);
extern void func_8001C3CC(void);
void func_80013DEC(void)
{
    unsigned short index;
    unsigned short offset;
    unsigned short count;
    unsigned short target;
    unsigned int mask;
    if (D_80038024 != 0) {
        D_80038024();
    }
    func_80014624();
    mask = __osDisableInt();
    if (D_80038288 == 0) {
        target = D_80038292 - 1;
    } else {
        target = D_80038288 - 1;
    }
    __osRestoreInt(mask);
    if (D_8002C630 != 0) {
        func_8001DDE0();
    }
    if (D_80038360 != 0xFFFF) {
        func_80011CD8();
        func_80011894();
        if (D_80038020 != 0) {
            D_80038020();
        }
        func_80012200();
        func_80013964();
        if (D_80038360 != target) {
            func_80013C84();
            offset = 0;
            count = 0;
            if (D_80038360 < target) {
                for (index = D_80038360; index < target && count < D_800382CE; offset += 192, count++) {
                    func_80013D70(index++, offset);
                }
            } else {
                for (index = D_80038360; index < D_80038292 && count < D_800382CE; offset += 192, count++) {
                    func_80013D70(index++, offset);
                }
                for (index = 0; index < target && count < D_800382CE; offset += 192, count++) {
                    func_80013D70(index++, offset);
                }
            }
            func_80011D24();
            D_80038362 -= count;
            D_80038360 = index;
        }
        func_8001C508();
        D_800382D4 = &D_800382E0[D_8003829E ^ 1];
        D_800382D0 = D_800382D8[D_8003829E ^ 1];
        func_800118C0();
        func_800119E0();
        D_8003829E ^= 1;
        if (D_8002C630 != 0) {
            func_8001C3CC();
        }
    } else {
        D_80038360 = target;
    }
    func_80014650();
}

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

/* PROMOTED 2026-10-11 — func_80014198
 * Source:   cloud/work/frontier/w17g/sources/func_80014198.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014198.c:func_80014198 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct AudioConfiguration { unsigned int frequency; unsigned char mode; unsigned char sample_bits; unsigned char unknown06[256]; signed char text[256]; } AudioConfiguration;
extern unsigned int D_8004F808;
extern AudioConfiguration D_8004F810;
extern signed char D_8002D890[];
extern signed char D_8002D8A4[];
extern void func_80011104(unsigned short, unsigned int);
extern void func_800114C0(unsigned short, unsigned int, unsigned char);
extern void func_80010DD8(unsigned int, unsigned short);
extern void func_800140F8(void);
void func_80014198(unsigned int *frequency, unsigned short count,
                   unsigned short size, unsigned char mode)
{
    unsigned char i;
    float frame_samples;
    if (count > 32) {
        count = 32;
    }
    D_800382CD = 0;
    func_80011104(count, *frequency);
    func_800114C0(count, *frequency, mode);
    func_80010DD8(*frequency, size);
    func_800140F8();
    frame_samples = (float)*frequency / 60.0f;
    D_800382A4 = (unsigned int)(frame_samples / 192.0f + 0.5f);
    D_800382A0 = D_800382A4;
    D_8004F808 = D_800382A4 * 192;
    D_8004F810.frequency = *frequency;
    D_8004F810.sample_bits = 16;
    D_8004F810.mode = 1;
    for (i = 0; D_8002D890[i] != 0; i++) {
        D_8004F810.text[i] = D_8002D8A4[i];
    }
    D_8004F810.text[i] = 0;
}

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
    osSendMesg(&D_80038368, 0, 0);
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
            osSendMesg(&D_80038368, 0, 0);
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

/* PROMOTED 2026-10-11 — func_80014650
 * Source:   cloud/work/frontier/w16g/sources/func_80014650.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/func_80014650.c:func_80014650 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define NULL ((void *)0)
#define OS_MESG_NOBLOCK 0
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void func_80014650(void) {
    osSendMesg(&D_80038368, NULL, OS_MESG_NOBLOCK);
}

/* PROMOTED 2026-10-11 — func_8001467C
 * Source:   cloud/work/frontier/w17g/sources/func_8001467C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001467C.c:func_8001467C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned char func_8001467C(int index)
{
    return D_80038294[index].active != 0;
}

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

/* PROMOTED 2026-10-11 — func_8001489C
 * Source:   cloud/work/frontier/w17g/sources/func_8001489C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001489C.c:func_8001489C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_8001489C(int index, unsigned char release)
{
    D_80038294[index].release_count = 20;
    if (release) {
        func_80011A3C(&D_80038294[index]);
    }
}

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

/* PROMOTED 2026-10-11 — func_80014A0C
 * Source:   cloud/work/frontier/w17g/sources/func_80014A0C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014A0C.c:func_80014A0C (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014A0C(int index, unsigned short rate)
{
    if (rate <= 8192) {
        D_80038294[index].rate = rate;
    } else {
        D_80038294[index].rate = 8192;
    }
}

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

/* PROMOTED 2026-10-11 — func_80014BD8
 * Source:   cloud/work/frontier/w17g/sources/func_80014BD8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014BD8.c:func_80014BD8 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80010E80(void *state);
void func_80014BD8(void *state)
{
    func_80010E80(state);
}

/* PROMOTED 2026-10-11 — func_80014BF8
 * Source:   cloud/work/frontier/w17g/sources/func_80014BF8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014BF8.c:func_80014BF8 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014BF8(void)
{
    func_80011074();
}

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

/* PROMOTED 2026-10-11 — func_80014C60
 * Source:   cloud/work/frontier/w18c/sources/func_80014C60.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w18c/sources/func_80014C60.c:func_80014C60 (score0); rom_tu.h context: cloud/work/frontier/w18c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014C60(short *buffer, unsigned int samples)
{
    buffer[samples + 0] = buffer[0];
    buffer[samples + 1] = buffer[1];
    buffer[samples + 2] = buffer[2];
    buffer[samples + 3] = buffer[3];
    osWritebackDCache(&buffer[samples], 8);
}

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

/* PROMOTED 2026-10-11 — func_80014CF4
 * Source:   cloud/work/frontier/w17g/sources/func_80014CF4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014CF4.c:func_80014CF4 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014CF4(void)
{
}

/* PROMOTED 2026-10-11 — func_80014CFC
 * Source:   cloud/work/frontier/w17g/sources/func_80014CFC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014CFC.c:func_80014CFC (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014CFC(void **descriptor, void **data)
{
}

/* PROMOTED 2026-10-11 — func_80014D08
 * Source:   cloud/work/frontier/w17g/sources/func_80014D08.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014D08.c:func_80014D08 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014D08(void **descriptor, void *data)
{
}

/* PROMOTED 2026-10-11 — func_80014D14
 * Source:   cloud/work/frontier/w17g/sources/func_80014D14.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_80014D14.c:func_80014D14 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80014D14(void)
{
}

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

