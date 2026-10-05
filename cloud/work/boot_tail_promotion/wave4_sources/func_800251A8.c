/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800251A8.c to the production
 * src/rom/lib_25bb0.c stream record (StreamState_80025264, hoisted in wave 4);
 * code unchanged. */
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; int validCount; int first; int msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern StreamState_80025264 D_80056230[2];

extern unsigned int D_80058680;

unsigned int func_800251A8(int selected)
{
    StreamState_80025264 *stream;
    int i;

    for (;;) {
        for (i = 0; i < 2; i++) {
            stream = &D_80056230[i];
            if (stream->busy && i != selected && stream->token == D_80058680) {
                break;
            }
        }
        if (i == 2) {
            break;
        }
        D_80058680++;
        if (D_80058680 == (unsigned int)-1) {
            D_80058680 = 0;
        }
    }
    stream = &D_80056230[selected];
    stream->token = D_80058680;
    D_80058680++;
    if (D_80058680 == (unsigned int)-1) {
        D_80058680 = 0;
    }
    return stream->token;
}
