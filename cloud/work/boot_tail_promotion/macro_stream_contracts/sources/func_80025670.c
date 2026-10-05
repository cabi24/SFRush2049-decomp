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
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);
extern int func_800250F0(void);
extern void func_80025120(int);
extern void func_8001C77C(int, unsigned char, unsigned char, unsigned char, unsigned char);

int func_80025670(unsigned int token, unsigned char value1,
                  unsigned char value2, unsigned char value3,
                  unsigned char value4)
{
    int queue_token;

    if (D_8002D480[0]) {
        token = func_80025264(token);
        if (token != (unsigned int)-1) {
            queue_token = func_800250F0();
            if (D_80056230[token].handle != -1) {
                func_8001C77C(D_80056230[token].handle, value1, value2, value3, value4);
            }
            D_80056230[token].value2 = value2;
            D_80056230[token].value3 = value3;
            D_80056230[token].value1 = value1;
            func_80025120(queue_token);
            return 1;
        }
    }
    return 0;
}
