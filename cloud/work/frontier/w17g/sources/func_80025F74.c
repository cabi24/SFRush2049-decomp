/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80025F74 (static, boot 0x80025F74, 840 B, segment 0x26ab0 -> src/rom/lib_26ab0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 2 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): buffered_stream_service_80025F74.
 * Body + declarations verbatim from cloud/work/boot_tail/BT07-decoder-facing-callers/func_80025F74.c (Astra / BT07-decoder-facing-callers).
 * w17g TU-context edit (codegen-neutral, re-proved): private struct view renamed StreamState_80025F74 (lib_26ab0.c
 * already defines a different StreamState view of the same stream object).
 * Shaping devices: `volatile` on a global (as in the D10 source; busy/handshake flag). */
typedef void *OSMesg;
typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct StreamState_80025F74 {
    unsigned char opaque_decoder[400];
    void (*callback)(void *, void *, unsigned int, OSMesgQueue *);
    unsigned char *input;
    void *output;
    unsigned int capacity;
    unsigned int input_ring[1024];
    unsigned short remaining_blocks;
    unsigned short header_low;
    unsigned int remaining_input;
    int remaining;
    OSMesgQueue queue;
    OSMesg message;
    unsigned int bit_position;
    unsigned int consumed_input;
    unsigned int received;
    unsigned int consumed;
    unsigned int available;
    signed char pending;
    signed char state;
    signed char budget;
    unsigned char unknown_11DF;
    OSMesgQueue request_queue;
    OSMesg request_messages[2];
    volatile unsigned char busy;
    unsigned char volume;
    unsigned char pan;
    unsigned char span;
    unsigned char option;
    unsigned char unknown_1205[3];
    unsigned int rate;
    int handle;
    short *buffer;
    unsigned int buffer_count;
    unsigned char request_state;
    unsigned char mode;
    unsigned char unknown_121A[2];
    unsigned int processed;
    float duration;
    unsigned int token;
} StreamState_80025F74;
extern int osRecvMesg(OSMesgQueue *, OSMesg *, int);
extern int func_800268D0(StreamState_80025F74 *, void *);
extern void bzero(void *, int);

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
