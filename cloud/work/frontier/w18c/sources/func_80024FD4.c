/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80024FD4 - lib_25bb0 (IDO 7.1). Stream DMA-completion callback (5-argument callback
 * ABI: two buffer pieces with their sizes, then the stream index SELECTED on the stack):
 * when the stream system is up (D_8002D480[0]), report first_count + second_count bytes to
 * func_800262BC(stream, n) and add them to D_80056230[selected].processed. Returns 0.
 * w18c: no `stream` local - the element address is a compiler temporary spilled at 0x1C,
 * as retail (a named local is placed at 0x18, see Prior). first/second are unused
 * parameters of the callback ABI (homed by IDO). Signature as the locked caller's prototype
 * (func_8002574C, SampleCallback_8002574C). No shaping devices.
 * StreamState_80025264 is the TU's hoisted typedef (verbatim); OSMesgQueue/OSMesg from rom_tu.h
 * (private copies here, dropped by context_check as rom_tu.h redeclarations).
 * Prior: D10 BT07-medium (named stream local; 7.1 true=0 reloc_blind=2, stack slot only). */
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; int validCount; int first; int msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern StreamState_80025264 D_80056230[2];
extern volatile unsigned char D_8002D480[];
extern unsigned int func_800262BC(StreamState_80025264 *, unsigned int);

int func_80024FD4(short *first, unsigned int first_count,
                  short *second, unsigned int second_count, unsigned int selected)
{
    if (D_8002D480[0]) {
        func_800262BC(&D_80056230[selected], first_count + second_count);
        D_80056230[selected].processed += first_count + second_count;
    }
    return 0;
}
