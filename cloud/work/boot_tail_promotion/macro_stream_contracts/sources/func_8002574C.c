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
typedef int (*SampleCallback_8002574C)(short *, unsigned int, short *, unsigned int,
                              unsigned int);
typedef struct ServiceHooks_8002574C {
    unsigned char unknown_00[28];
    void (*release)(void *);
} ServiceHooks_8002574C;
extern volatile unsigned char D_8002D480[];
extern ServiceHooks_8002574C D_80038000;
extern StreamState_80025264 D_80056230[2];
extern unsigned int D_800586A0;
extern void func_80025150(void);
extern void func_8002517C(void);
extern void func_80025F74(StreamState_80025264 *);
extern void func_80026328(StreamState_80025264 *);
extern void func_80026348(StreamState_80025264 *);
extern int func_8001C580(unsigned char, short *, unsigned int, unsigned int,
                       unsigned char, unsigned char, unsigned char, unsigned char,
                       SampleCallback_8002574C, unsigned int);
extern void func_8001C7F4(int);
extern int func_80024FD4(short *, unsigned int, short *, unsigned int, unsigned int);

void func_8002574C(void)
{
    int selected;

    if (D_8002D480[0]) {
        func_80025150();
        for (selected = 0; selected != 2; selected++) {
            if (D_80056230[selected].busy) {
                func_80025F74(&D_80056230[selected]);
                switch (D_80056230[selected].busy) {
                case 1:
                    if (D_80056230[selected].state == 2) {
                        D_80056230[selected].field_1220 = (float)(unsigned int)D_80056230[selected].read_count * 160.0f / D_80056230[selected].rate;
                        D_80056230[selected].processed = 0;
                        D_80056230[selected].busy = 2;
                        func_80026328(&D_80056230[selected]);
                        D_80056230[selected].handle = func_8001C580(D_80056230[selected].option, D_80056230[selected].buffer, D_80056230[selected].buffer_count, D_80056230[selected].rate, D_80056230[selected].value1, D_80056230[selected].value2, D_80056230[selected].value3, D_80056230[selected].mode, func_80024FD4, selected);
                        if (D_80056230[selected].handle == -1) {
                            func_80026348(&D_80056230[selected]);
                            if (!(D_800586A0 & 1)) D_80038000.release(D_80056230[selected].buffer);
                            D_80056230[selected].busy = 0;
                        }
                    }
                    break;
                case 2:
                    if (D_80056230[selected].state == 4) {
                        D_80056230[selected].busy = 3;
                        D_80056230[selected].request_state = 4;
                    }
                    break;
                case 3:
                    if (D_80056230[selected].request_state == 0) {
                        if (!(D_800586A0 & 1)) D_80038000.release(D_80056230[selected].buffer);
                        D_80056230[selected].busy = 0;
                    } else {
                        if (D_80056230[selected].request_state == 2) {
                            func_8001C7F4(D_80056230[selected].handle);
                            D_80056230[selected].handle = -1;
                        }
                        D_80056230[selected].request_state--;
                    }
                    break;
                }
            }
        }
        func_8002517C();
    }
}
