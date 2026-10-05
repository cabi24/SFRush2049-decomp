/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Source-contract repair: use the shared 0x1228 stream record already
 * represented by func_80025264, expanded only with observed queue/control
 * fields. The SDK queue tag is OSMesgQueue_s, as in PR #82.
 * Original locked source is preserved. No promotion or coverage is claimed. */
typedef void *OSMesg;
typedef struct OSThread_s OSThread;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern StreamState_80025264 D_80056230[2];
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
