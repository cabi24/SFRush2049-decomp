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

/* PROMOTED (w17g dry run) — func_80025F74
 * Source:   cloud/work/frontier/w17g/sources/func_80025F74.c
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared (IDO 7.1)
 */
typedef struct StreamState_80025F74 { unsigned char opaque_decoder[400]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); unsigned char *input; void *output; unsigned int capacity; unsigned int input_ring[1024]; unsigned short remaining_blocks; unsigned short header_low; unsigned int remaining_input; int remaining; OSMesgQueue queue; OSMesg message; unsigned int bit_position; unsigned int consumed_input; unsigned int received; unsigned int consumed; unsigned int available; signed char pending; signed char state; signed char budget; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char volume; unsigned char pan; unsigned char span; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float duration; unsigned int token; } StreamState_80025F74;
extern int osRecvMesg(OSMesgQueue *, OSMesg *, int);
extern int func_800268D0(StreamState_80025F74 *, void *);
void func_80025F74(StreamState_80025F74 *stream)
{
    int budget;
    int size;

    budget = stream->budget;
    if (stream->pending) {
        if (osRecvMesg(&stream->queue, 0, 0) >= 0) {
            if (stream->received == 0) {
                stream->remaining_blocks = (stream->input_ring[1] >> 16) & 65535;
                stream->header_low = stream->input_ring[1] & 65535;
                stream->remaining_input = stream->input_ring[2] & 0xFFFFFF;
                stream->bit_position = 96;
            }
            stream->received += 1024;
            if (stream->remaining_input > 256) stream->remaining_input -= 256;
            else stream->remaining_input = 0;
            stream->pending = 0;
        }
    }
    if (!stream->pending && (stream->state == 1 || stream->state == 2 || stream->state == 3)
        && stream->received - stream->consumed_input <= 3072) {
        if (stream->received == 0) size = 1024;
        else size = stream->remaining_input > 256 ? 1024 : stream->remaining_input * 4;
        if (size > 0) {
            stream->pending = 1;
            stream->callback(stream->input + stream->received, (unsigned char *)stream->input_ring + ((stream->received % 4096) / 8) * 8, size, &stream->queue);
        }
    }
    while (stream->available - stream->consumed < stream->capacity - 160
        && (stream->state == 1 || stream->remaining_blocks > 0)
        && stream->received - stream->consumed_input >= 37 && budget > 0) {
        func_800268D0(stream, (unsigned char *)stream->output + ((stream->available % stream->capacity) / 4) * 8);
        stream->consumed_input = (stream->bit_position >> 3) & ~3U;
        stream->available += 160;
        if (--stream->remaining_blocks == 0) stream->remaining = stream->capacity;
        budget--;
    }
    if ((stream->state == 3 || stream->state == 4) && stream->remaining_blocks <= 0) {
        while (stream->available - stream->consumed < stream->capacity - 160 && budget > 0) {
            bzero((unsigned char *)stream->output + ((stream->available % stream->capacity) / 4) * 8, 320);
            stream->available += 160;
            budget--;
        }
    }
    if (stream->state == 1 && stream->available - stream->consumed >= stream->capacity - 320)
        stream->state = 2;
}

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

