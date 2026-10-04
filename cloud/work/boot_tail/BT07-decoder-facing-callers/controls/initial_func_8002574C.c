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
typedef int (*SampleCallback)(short *, unsigned int, short *, unsigned int,
                              unsigned int);
typedef struct ServiceHooks {
    unsigned char unknown_00[28];
    void (*release)(void *);
} ServiceHooks;
extern volatile unsigned char D_8002D480[];
extern ServiceHooks D_80038000;
extern StreamState D_80056230[2];
extern unsigned int D_800586A0;
extern void func_80025150(void);
extern void func_8002517C(void);
extern void func_80025F74(StreamState *);
extern void func_80026328(StreamState *);
extern void func_80026348(StreamState *);
extern int func_8001C580(unsigned char, short *, unsigned int, unsigned int,
                       unsigned char, unsigned char, unsigned char, unsigned char,
                       SampleCallback, unsigned int);
extern void func_8001C7F4(int);
extern int func_80024FD4(short *, unsigned int, short *, unsigned int, unsigned int);

void func_8002574C(void)
{
    StreamState *stream;
    int selected;

    if (D_8002D480[0]) {
        func_80025150();
        stream = D_80056230;
        for (selected = 0; selected != 2; selected++) {
            if (stream->busy) {
                func_80025F74(stream);
                switch (stream->busy) {
                case 1:
                    if (stream->state == 2) {
                        stream->duration = (float)(unsigned int)stream->remaining_blocks * 160.0f / stream->rate;
                        stream->processed = 0;
                        stream->busy = 2;
                        func_80026328(stream);
                        stream->handle = func_8001C580(stream->option, stream->buffer,
                            stream->buffer_count, stream->rate, stream->volume,
                            stream->pan, stream->span, stream->mode, func_80024FD4, selected);
                        if (stream->handle == -1) {
                            func_80026348(stream);
                            if (!(D_800586A0 & 1)) D_80038000.release(stream->buffer);
                            stream->busy = 0;
                        }
                    }
                    break;
                case 2:
                    if (stream->state == 4) {
                        stream->busy = 3;
                        stream->request_state = 4;
                    }
                    break;
                case 3:
                    if (stream->request_state == 0) {
                        if (!(D_800586A0 & 1)) D_80038000.release(stream->buffer);
                        stream->busy = 0;
                    } else {
                        if (stream->request_state == 2) {
                            func_8001C7F4(stream->handle);
                            stream->handle = -1;
                        }
                        stream->request_state--;
                    }
                    break;
                }
            }
            stream++;
        }
        func_8002517C();
    }
}
