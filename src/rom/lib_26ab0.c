/* GENERATED ROM-aligned TU — segment 0x26ab0 (rom/lib_26ab0)
 * layout map 6aab71ce56f5b9bb93e0474f813bb2c47f546fff1a84ecc2e02e9a912df26ae7; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80025EB0
 * Source:   cloud/matches/boot_tail/func_80025EB0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80025EB0.c:func_80025EB0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
typedef struct StreamState { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[27]; unsigned int processed; float field_1220; unsigned int token; } StreamState;
extern void bzero(void *, int);
void func_80025EB0(StreamState *stream, void *data, void *argument2,
                   unsigned int argument3,
                   void (*callback)(void *, void *, unsigned int, OSMesgQueue *))
{
    bzero(stream, 400);
    stream->block_count = 40;
    stream->callback = callback;
    stream->data = data;
    stream->argument2 = argument2;
    stream->argument3 = argument3;
    stream->remaining = 0;
    osCreateMesgQueue(&stream->queue, &stream->message, 1);
    if (data == 0 || callback == 0) {
        stream->state = 0;
    } else {
        stream->state = 1;
    }
    stream->field_11DC = 0;
    stream->read_count = stream->buffered = stream->write_count = 0;
    stream->field_11C8 = 0;
    stream->field_11D0 = 0;
    stream->field_11CC = 0;
    stream->available = 0;
    stream->consumed = 0;
    stream->scale = 2;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_26ab0/func_80025F74.s")
/* PROMOTED 2026-10-04 — func_800262BC
 * Source:   cloud/matches/boot_tail/func_800262BC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800262BC.c:func_800262BC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned int func_800262BC(StreamState *stream, unsigned int count)
{
    unsigned int amount;

    if (stream->state == 3) {
        if (stream->remaining > 0) {
            stream->remaining -= count;
            if (stream->remaining <= 0) {
                stream->state = 4;
                stream->remaining = 0;
            }
        }
        amount = count < stream->available - stream->consumed ? count : stream->available - stream->consumed;
        stream->consumed += amount;
        return amount;
    }
    return 0;
}

/* PROMOTED 2026-10-04 — func_80026328
 * Source:   cloud/work/boot_tail_promotion/sources/func_80026328.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80026328.c:func_80026328 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StreamState_80026328 { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState_80026328;
void func_80026328(StreamState_80026328 *stream)
{
    if (stream->state == 2) {
        stream->state = 3;
    }
}

/* PROMOTED 2026-10-04 — func_80026348
 * Source:   cloud/work/boot_tail_promotion/sources/func_80026348.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80026348.c:func_80026348 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StreamState_80026348 { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState_80026348;
void func_80026348(StreamState_80026348 *stream)
{
    stream->state = 4;
}

