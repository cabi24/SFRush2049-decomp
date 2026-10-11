/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800252AC - lib_25bb0 (IDO 7.1). Start a stream on a free voice of the two-entry stream table
 * D_80056230 (0x1228-byte StreamState_80025264, the TU's own type): when streaming is enabled
 * (D_8002D480[0]) and the entry index is in range (< D_80058690), take the first entry that is
 * not busy, create its request queue, pick the rate (argument, else 8000/16000 Hz from the top
 * byte of the entry word D_8005868C[index]), store the options, size the buffer
 * (((rate * 8 / 30 + 159) / 160) * 160 samples), take the buffer from the preallocated pair
 * D_80058698 (D_800586A0 bit 0) or allocate and invalidate it (func_8001C770 size, allocator
 * D_80038018), then under func_800250F0/func_80025120 bracket start the transfer
 * (func_80025EB0 with the entry's data at D_80058688 + low 24 bits, callback func_8002506C),
 * reset the stream (func_80024FB0), mark it busy and return its handle (func_800251A8);
 * -1 when nothing starts.
 * Levers (w21a): index the global table (`stream = &D_80056230[selected]` after a loop over
 * D_80056230[selected].busy, as the TU's func_800251A8): with a running pointer
 * uopt emits no `.noalias` for s0, so as1 cannot move the u8 argument reloads (lbu sp) above
 * the s0 stores (true=960); the buffer size is written straight into stream->buffer_count (no
 * buffer_count local: five named locals give retail's frame, the spill temp at sp+44).
 * No shaping devices, no unused locals, no pad arrays, no false prototypes. */
#include "types.h"
#include "PR/os_message.h"
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
typedef struct StreamState { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState;
extern StreamState_80025264 D_80056230[2];
extern volatile unsigned char D_8002D480[];
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void *D_80058688;
extern void *D_8005868C;
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
extern void func_80025EB0(StreamState_80025264 *, void *, void *, unsigned int, void (*)(void *, void *, unsigned int, OSMesgQueue *));

unsigned int func_800252AC(unsigned int index, unsigned int rate,
                           unsigned char option, unsigned char value1,
                           unsigned char value2, unsigned char value3,
                           unsigned char mode)
{
    StreamState_80025264 *stream;
    int selected;
    int queue_token;
    unsigned int size;
    unsigned int frequency;

    if (D_8002D480[0] && index < D_80058690) {
        for (selected = 0; selected < 2; selected++) {
            if (D_80056230[selected].busy == 0) {
                break;
            }
        }
        if (selected != 2) {
            stream = &D_80056230[selected];
            osCreateMesgQueue(&stream->request_queue, stream->request_messages, 2);
            frequency = rate;
            if (!frequency) {
                frequency = ((((unsigned int *)D_8005868C)[index] >> 24) & 255) == 0 ? 8000 : 16000;
            }
            stream->option = option;
            stream->rate = frequency;
            stream->value1 = value1;
            stream->value2 = value2;
            stream->value3 = value3;
            stream->mode = mode;
            stream->buffer_count = ((frequency * 8 / 30 + 159) / 160) * 160;
            if (D_800586A0 & 1) {
                stream->buffer = D_80058698[selected];
            } else {
                size = func_8001C770(stream->buffer_count);
                stream->buffer = D_80038018(size, 0);
                osInvalDCache(stream->buffer, size);
            }
            if (stream->buffer) {
                queue_token = func_800250F0();
                func_80025EB0(stream, (unsigned char *)D_80058688 + (((unsigned int *)D_8005868C)[index] & 0xFFFFFF),
                stream->buffer, stream->buffer_count, func_8002506C);
                func_80024FB0((StreamState *)stream);
                stream->handle = -1;
                stream->busy = 1;
                func_80025120(queue_token);
                return func_800251A8(selected);
            }
        }
    }
    return (unsigned int)-1;
}
