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
