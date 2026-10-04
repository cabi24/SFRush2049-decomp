/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF;
    OSMesgQueue request_queue;
    OSMesg request_messages[2];
    volatile unsigned char busy;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char option;
    unsigned char unknown_1205[3];
    unsigned int rate;
    int handle;
    void *buffer;
    unsigned int buffer_count;
    unsigned char request_state;
    unsigned char mode;
    unsigned char unknown_121A[2];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState;
typedef struct ServiceHooks {
    unsigned char unknown_00[20];
    void *(*translate)(void *);
    void *(*allocate)(unsigned int, int);
    void (*release)(void *);
} ServiceHooks;
extern ServiceHooks D_80038000;
extern StreamState D_80056230[2];
extern volatile unsigned char D_8002D480[];
extern void *D_80058688;
extern unsigned int *D_8005868C;
extern unsigned int D_80058690;
extern unsigned int D_800586A0;
extern void *D_80058698[2];
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern unsigned int func_8001C770(unsigned int);
extern void osInvalDCache(void *, int);
extern int func_800250F0(void);
extern void func_80025120(int);
extern unsigned int func_800251A8(int);
extern void func_80024FB0(StreamState *);
extern void func_8002506C(void *, void *, unsigned int, OSMesgQueue *);
extern void func_80025EB0(StreamState *, void *, void *, unsigned int,
                         void (*)(void *, void *, unsigned int, OSMesgQueue *));

unsigned int func_800252AC(unsigned int index, unsigned int rate,
                           unsigned char option, unsigned char value1,
                           unsigned char value2, unsigned char value3,
                           unsigned char mode)
{
    StreamState *stream;
    int selected;
    unsigned int size;
    int queue_token;

    if (!D_8002D480[0] || index >= D_80058690) {
        return (unsigned int)-1;
    }
    stream = D_80056230;
    for (selected = 0; selected < 2; selected++, stream++) {
        if (!stream->busy) {
            break;
        }
    }
    if (selected == 2) {
        return (unsigned int)-1;
    }
    osCreateMesgQueue(&stream->request_queue, stream->request_messages, 2);
    if (!rate) {
        rate = ((D_8005868C[index] >> 24) & 255) ? 16000 : 8000;
    }
    stream->buffer_count = ((rate * 8 / 30 + 159) / 160) * 160;
    stream->option = option;
    stream->rate = rate;
    stream->value1 = value1;
    stream->value2 = value2;
    stream->value3 = value3;
    stream->mode = mode;
    if (D_800586A0 & 1) {
        stream->buffer = D_80058698[selected];
    } else {
        size = func_8001C770(stream->buffer_count);
        stream->buffer = D_80038000.allocate(size, 0);
        osInvalDCache(stream->buffer, size);
    }
    if (!stream->buffer) {
        return (unsigned int)-1;
    }
    queue_token = func_800250F0();
    func_80025EB0(stream, (unsigned char *)D_80058688 + (D_8005868C[index] & 0xFFFFFF),
                   stream->buffer, stream->buffer_count, func_8002506C);
    func_80024FB0(stream);
    stream->handle = -1;
    stream->busy = 1;
    func_80025120(queue_token);
    return func_800251A8(selected);
}
