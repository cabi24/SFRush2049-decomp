/* GENERATED ROM-aligned TU — segment 0x25bb0 (rom/lib_25bb0)
 * layout map f5baec304e2af6053d0e085c8e4b667c97ff73771969d95aeccac477c5f6c448; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* Shared 0x1228 stream record. Named members replace only observed ranges;
 * signed state/scale, volatile busy, SDK queues and callback ABI are retained.
 * Hoisted before the first slot that uses it (boot-tail wave 4:
 * func_800251A8); text unchanged. */
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;

/* PROMOTED 2026-10-04 — func_80024FB0
 * Source:   cloud/matches/boot_tail/func_80024FB0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80024FB0.c:func_80024FB0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StreamState { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState;
extern unsigned char D_80038290;
void func_80024FB0(StreamState *stream)
{
    if (D_80038290 != 0) {
        stream->scale *= 2;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80024FD4.s")
/* PROMOTED 2026-10-04 — func_8002506C
 * Source:   cloud/matches/boot_tail/func_8002506C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8002506C.c:func_8002506C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80010714(void *, void *, unsigned int);
extern int osJamMesg(OSMesgQueue *, void *, int);
void func_8002506C(void *source, void *destination, unsigned int count,
                   OSMesgQueue *queue)
{
    func_80010714(destination, source, count);
    osJamMesg(queue, 0, 1);
}

/* PROMOTED 2026-10-04 — func_800250AC
 * Source:   cloud/matches/boot_tail/func_800250AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800250AC.c:func_800250AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern int osJamMesg(OSMesgQueue *, OSMesg, int);
extern OSMesgQueue D_800586A8;
extern OSMesg D_800586C0;
void func_800250AC(void)
{
    osCreateMesgQueue(&D_800586A8, &D_800586C0, 1);
    osJamMesg(&D_800586A8, 0, 0);
}

/* PROMOTED 2026-10-05 — func_800250F0
 * Source:   cloud/matches/boot_tail/func_800250F0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800250F0.c:func_800250F0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int osRecvMesg(OSMesgQueue *, void **, int);
int func_800250F0(void)
{
    osRecvMesg(&D_800586A8, 0, 1);
    return 0;
}

/* PROMOTED 2026-10-05 — func_80025120
 * Source:   cloud/matches/boot_tail/func_80025120.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80025120.c:func_80025120 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80025120(int token)
{
    osJamMesg(&D_800586A8, 0, 0);
}

/* PROMOTED 2026-10-05 — func_80025150
 * Source:   cloud/matches/boot_tail/func_80025150.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80025150.c:func_80025150 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80025150(void)
{
    osRecvMesg(&D_800586A8, 0, 1);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_8002517C.s")
/* PROMOTED 2026-10-05 — func_800251A8
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_800251A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_800251A8.c:func_800251A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_hoisted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern StreamState_80025264 D_80056230[2];
extern unsigned int D_80058680;
unsigned int func_800251A8(int selected)
{
    StreamState_80025264 *stream;
    int i;

    for (;;) {
        for (i = 0; i < 2; i++) {
            stream = &D_80056230[i];
            if (stream->busy && i != selected && stream->token == D_80058680) {
                break;
            }
        }
        if (i == 2) {
            break;
        }
        D_80058680++;
        if (D_80058680 == (unsigned int)-1) {
            D_80058680 = 0;
        }
    }
    stream = &D_80056230[selected];
    stream->token = D_80058680;
    D_80058680++;
    if (D_80058680 == (unsigned int)-1) {
        D_80058680 = 0;
    }
    return stream->token;
}

/* PROMOTED 2026-10-04 — func_80025264
 * Source:   cloud/work/boot_tail_promotion/sources/func_80025264.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80025264.c:func_80025264 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern StreamState_80025264 D_80056230[2];
int func_80025264(unsigned int token)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (D_80056230[i].busy && D_80056230[i].token == token) {
            return i;
        }
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800252AC.s")
/* PROMOTED 2026-10-05 — func_800254D4
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800254D4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800254D4.c:func_800254D4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_800586A0;
extern void (*D_8003801C)(void *);
extern int func_800250F0(void);
extern void func_80025120(int);
extern void func_80026348(StreamState_80025264 *);
void func_800254D4(int selected)
{
    int token;

    switch (D_80056230[selected].busy) {
    case 1:
        D_80056230[selected].busy = 0;
        if (!(D_800586A0 & 1)) {
            D_8003801C(D_80056230[selected].buffer);
        }
        break;
    case 2:
        token = func_800250F0();
        func_80026348(&D_80056230[selected]);
        D_80056230[selected].busy = 3;
        D_80056230[selected].request_state = 4;
        func_80025120(token);
        break;
    }
}

/* PROMOTED 2026-10-04 — func_80025594
 * Source:   cloud/matches/boot_tail/func_80025594.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80025594.c:func_80025594 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);
extern void func_800254D4(int);
int func_80025594(unsigned int token)
{
    int selected;

    if (token != (unsigned int)-1 && D_8002D480[0]) {
        selected = func_80025264(token);
        if (selected != -1) {
            func_800254D4(selected);
            return 1;
        }
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800255F0.s")
/* PROMOTED 2026-10-05 — func_80025670
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80025670.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_80025670.c:func_80025670 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_8001C77C(int, unsigned char, unsigned char, unsigned char, unsigned char);
int func_80025670(unsigned int token, unsigned char value1,
                  unsigned char value2, unsigned char value3,
                  unsigned char value4)
{
    int queue_token;

    if (D_8002D480[0]) {
        token = func_80025264(token);
        if (token != (unsigned int)-1) {
            queue_token = func_800250F0();
            if (D_80056230[token].handle != -1) {
                func_8001C77C(D_80056230[token].handle, value1, value2, value3, value4);
            }
            D_80056230[token].value2 = value2;
            D_80056230[token].value3 = value3;
            D_80056230[token].value1 = value1;
            func_80025120(queue_token);
            return 1;
        }
    }
    return 0;
}

/* PROMOTED 2026-10-05 — func_8002574C
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_8002574C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_8002574C.c:func_8002574C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef int (*SampleCallback_8002574C)(short *, unsigned int, short *, unsigned int, unsigned int);
typedef struct ServiceHooks_8002574C { unsigned char unknown_00[24]; void *(*allocate)(unsigned int, int); void (*release)(void *); } ServiceHooks_8002574C;
extern ServiceHooks_8002574C D_80038000;
extern void func_80025150(void);
extern void func_8002517C(void);
extern void func_80025F74(StreamState_80025264 *);
extern void func_80026328(StreamState_80025264 *);
extern int func_8001C580(unsigned char, short *, unsigned int, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, SampleCallback_8002574C, unsigned int);
extern void func_8001C7F4(int);
extern int func_80024FD4(short *, unsigned int, short *, unsigned int, unsigned int);
void func_8002574C(void)
{
    int selected;

    if (D_8002D480[0]) {
        func_80025150();
        for (selected = 0; selected != 2; selected++) {
            if (D_80056230[selected].busy) {
                func_80025F74(&D_80056230[selected]);
                switch (D_80056230[selected].busy) {
                case 1:
                    if (D_80056230[selected].state == 2) {
                        D_80056230[selected].field_1220 = (float)(unsigned int)D_80056230[selected].read_count * 160.0f / D_80056230[selected].rate;
                        D_80056230[selected].processed = 0;
                        D_80056230[selected].busy = 2;
                        func_80026328(&D_80056230[selected]);
                        D_80056230[selected].handle = func_8001C580(D_80056230[selected].option, D_80056230[selected].buffer, D_80056230[selected].buffer_count, D_80056230[selected].rate, D_80056230[selected].value1, D_80056230[selected].value2, D_80056230[selected].value3, D_80056230[selected].mode, func_80024FD4, selected);
                        if (D_80056230[selected].handle == -1) {
                            func_80026348(&D_80056230[selected]);
                            if (!(D_800586A0 & 1)) D_80038000.release(D_80056230[selected].buffer);
                            D_80056230[selected].busy = 0;
                        }
                    }
                    break;
                case 2:
                    if (D_80056230[selected].state == 4) {
                        D_80056230[selected].busy = 3;
                        D_80056230[selected].request_state = 4;
                    }
                    break;
                case 3:
                    if (D_80056230[selected].request_state == 0) {
                        if (!(D_800586A0 & 1)) D_80038000.release(D_80056230[selected].buffer);
                        D_80056230[selected].busy = 0;
                    } else {
                        if (D_80056230[selected].request_state == 2) {
                            func_8001C7F4(D_80056230[selected].handle);
                            D_80056230[selected].handle = -1;
                        }
                        D_80056230[selected].request_state--;
                    }
                    break;
                }
            }
        }
        func_8002517C();
    }
}

/* PROMOTED 2026-10-05 — func_800259A8
 * Source:   cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800259A8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/macro_stream_contracts/sources/func_800259A8.c:func_800259A8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave3.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
float func_800259A8(unsigned int token, float *value)
{
    int queue_token;
    float ratio;

    if (token != (unsigned int)-1 && D_8002D480[0]) {
        token = func_80025264(token);
        if (token != (unsigned int)-1) {
            queue_token = func_800250F0();
            if (D_80056230[token].busy == 2) {
                ratio = (float)D_80056230[token].processed / (float)D_80056230[token].rate;
            } else {
                ratio = 0.0f;
            }
            *value = D_80056230[token].field_1220;
            func_80025120(queue_token);
            return ratio;
        }
    }
    return 0.0f;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025AB4.s")
/* PROMOTED 2026-10-05 — func_80025C68
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80025C68.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80025C68.c:func_80025C68 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_hoisted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *D_8005868C;
extern void *D_80058698[2];
extern void (*D_80038024)(void);
extern void func_80025EB0(StreamState_80025264 *, void *, void *, unsigned int, void (*)(void *, void *, unsigned int, OSMesgQueue *));
extern void func_80024FB0(StreamState *);
extern void func_800250AC(void);
extern unsigned int func_8001C770(unsigned int);
extern void osInvalDCache(void *, int);
extern void func_8002574C(void);
void func_80025C68(unsigned int flags)
{
    int i;
    unsigned int size;

    D_800586A0 = flags;
    D_8005868C = 0;
    for (i = 0; i < 2; i++) {
        func_80025EB0(&D_80056230[i], 0, 0, 0, 0);
        func_80024FB0((StreamState *)&D_80056230[i]);
        D_80056230[i].busy = 0;
    }
    func_800250AC();
    if (D_800586A0 & 1) {
        size = func_8001C770(4320);
        for (i = 0; i < 2; i++) {
            D_80058698[i] = D_80038000.allocate(size, 0);
            osInvalDCache(D_80058698[i], size);
        }
    }
    D_80038024 = func_8002574C;
    D_8002D480[0] = 1;
    D_80058680 = 0;
}

/* PROMOTED 2026-10-05 — func_80025D84
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80025D84.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80025D84.c:func_80025D84 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80025D84(void)
{
    int i;

    do {
        for (i = 0; i < 2; i++) {
            if (D_80056230[i].busy != 0) {
                break;
            }
        }
    } while (i != 2);
}

/* PROMOTED 2026-10-05 — func_80025DC0
 * Source:   cloud/work/boot_tail_promotion/wave4_sources/func_80025DC0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/wave4_sources/func_80025DC0.c:func_80025DC0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context_wave4_adapted.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002D484;
extern void (*D_80038024)(void);
extern void *D_8005868C;
extern void *D_80058698[2];
extern void func_80025D84(void);
extern void func_80014594(void);
extern void func_8001061C(void);
extern void func_800145DC(void);
void func_80025DC0(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        func_800254D4(i);
    }
    func_80025D84();
    D_8002D480[0] = 0;
    func_80014594();
    D_80038024 = 0;
    func_8001061C();
    func_800145DC();
    if (D_8005868C && D_8002D484) {
        D_80038000.release(D_8005868C);
    }
    if (D_800586A0 & 1) {
        for (i = 0; i < 2; i++) {
            D_80038000.release(D_80058698[i]);
        }
    }
}

