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
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
typedef struct StreamState {
    unsigned char unknown_0000[358];
    unsigned short block_count;
    unsigned char unknown_0168[40];
    void (*callback)(void *, void *, unsigned int, OSMesgQueue *);
    void *data;
    void *argument2;
    unsigned int argument3;
    unsigned char unknown_01A0[4096];
    unsigned short read_count;
    unsigned short write_count;
    unsigned int buffered;
    int remaining;
    OSMesgQueue queue;
    OSMesg message;
    unsigned int field_11C8;
    unsigned int field_11CC;
    unsigned int field_11D0;
    unsigned int consumed;
    unsigned int available;
    signed char field_11DC;
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[27];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState;

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
