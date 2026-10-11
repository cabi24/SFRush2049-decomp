/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800255F0 - lib_25bb0 (IDO 7.1). Is the stream owning TOKEN busy? With the stream system
 * up (D_8002D480[0]), map TOKEN to its stream index (func_80025264, locked, same TU; -1 = none)
 * and return whether D_80056230[index].busy (+0x1200) is nonzero; else 0.
 * Natural form: return D_80056230[token].busy != 0; (true=10 reloc_blind=0: relocation NAME
 * only). Relocation-label view (disclosed shaping device): the reloc-aware target names
 * D_80056230+0x1200 by its own label D_80057430, so the busy byte is read as
 * D_80057430[token][0] with the 0x1228-byte stream stride (sizeof(StreamState_80025264), the
 * TU's hoisted typedef, verbatim). Same practice as docs/reloc_reconciliation.md.
 * Prior: D10 BT07-followon (5.3, 2 words; 7.1 true=10 reloc_blind=0). */
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; int validCount; int first; int msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF; OSMesgQueue request_queue; OSMesg request_messages[2]; volatile unsigned char busy; unsigned char value1; unsigned char value2; unsigned char value3; unsigned char option; unsigned char unknown_1205[3]; unsigned int rate; int handle; short *buffer; unsigned int buffer_count; unsigned char request_state; unsigned char mode; unsigned char unknown_121A[2]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern volatile unsigned char D_80057430[][sizeof(StreamState_80025264)];
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);

unsigned char func_800255F0(unsigned int token)
{
    if (D_8002D480[0]) {
        token = func_80025264(token);
        if (token == -1) {
            return 0;
        }
        return D_80057430[token][0] != 0;
    }
    return 0;
}
