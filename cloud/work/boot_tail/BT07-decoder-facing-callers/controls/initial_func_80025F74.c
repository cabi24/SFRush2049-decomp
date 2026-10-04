/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef void *OSMesg;
typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct StreamState {
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
} StreamState;
extern int osRecvMesg(OSMesgQueue *, OSMesg *, int);
extern int func_800268D0(StreamState *, void *);
extern void bzero(void *, int);

void func_80025F74(StreamState *stream)
{
    int budget;
    int size;

    budget = stream->budget;
    if (stream->pending) {
        if (osRecvMesg(&stream->queue, 0, 0) >= 0) {
            if (stream->received == 0) {
                stream->remaining_blocks = stream->input_ring[1] >> 16;
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
            stream->callback(stream->input + stream->received,
                (unsigned char *)stream->input_ring + ((stream->received % 4096) / 8) * 8,
                size, &stream->queue);
        }
    }
    while (stream->available - stream->consumed < stream->capacity - 160
        && (stream->state == 1 || stream->remaining_blocks > 0)
        && stream->received - stream->consumed_input >= 37 && budget > 0) {
        func_800268D0(stream, (double *)stream->output + ((stream->available % stream->capacity) / 4));
        stream->consumed_input = (stream->bit_position >> 3) & ~3U;
        stream->available += 160;
        if (--stream->remaining_blocks == 0) stream->remaining = stream->capacity;
        budget--;
    }
    if ((stream->state == 3 || stream->state == 4) && stream->remaining_blocks == 0) {
        while (stream->available - stream->consumed < stream->capacity - 160 && budget > 0) {
            bzero((double *)stream->output + ((stream->available % stream->capacity) / 4), 320);
            stream->available += 160;
            budget--;
        }
    }
    if (stream->state == 1 && stream->available - stream->consumed >= stream->capacity - 320)
        stream->state = 2;
}
