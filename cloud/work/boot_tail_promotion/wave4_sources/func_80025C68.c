/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80025C68.c to the production
 * src/rom/lib_25bb0.c views: StreamState_80025264 for the records,
 * ServiceHooks_8002574C (allocate named in wave 4), and the accepted
 * func_80024FB0(StreamState *) reset view via an explicit cast; code unchanged. */
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; int validCount; int first; int msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct StreamState { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState;
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern StreamState_80025264 D_80056230[2];
extern volatile unsigned char D_8002D480[];
typedef struct ServiceHooks_8002574C { unsigned char unknown_00[24]; void *(*allocate)(unsigned int, int); void (*release)(void *); } ServiceHooks_8002574C;
extern ServiceHooks_8002574C D_80038000;
extern unsigned int D_800586A0;
extern unsigned int D_80058680;
extern void *D_8005868C;
extern void *D_80058698[2];
extern void (*D_80038024)(void);
extern void func_80025EB0(StreamState_80025264 *, void *, void *, unsigned int,
                         void (*)(void *, void *, unsigned int, OSMesgQueue *));
extern void func_80024FB0(StreamState *);
extern void func_800250AC(void);
extern unsigned int func_8001C770(unsigned int);
extern void osInvalDCache(void *, int);
extern void func_8002574C(void);

void func_80025C68(unsigned int flags)
{
    int i;
    unsigned int size;

    D_800586A0 = flags;
    D_8005868C = 0;
    for (i = 0; i < 2; i++) {
        func_80025EB0(&D_80056230[i], 0, 0, 0, 0);
        func_80024FB0((StreamState *)&D_80056230[i]);
        D_80056230[i].busy = 0;
    }
    func_800250AC();
    if (D_800586A0 & 1) {
        size = func_8001C770(4320);
        for (i = 0; i < 2; i++) {
            D_80058698[i] = D_80038000.allocate(size, 0);
            osInvalDCache(D_80058698[i], size);
        }
    }
    D_80038024 = func_8002574C;
    D_8002D480[0] = 1;
    D_80058680 = 0;
}
