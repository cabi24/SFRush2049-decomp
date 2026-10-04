/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80025264.c: file-local type names StreamState suffixed _80025264 so several bodies share one ROM TU; no other change. */
typedef void *OSMesg;
typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    int validCount;
    int first;
    int msgCount;
    OSMesg *msg;
} OSMesgQueue;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
typedef struct StreamState_80025264 {
    unsigned char unknown_0000[358];
    unsigned short block_count;
    unsigned char unknown_0168[40];
    void (*callback)(void *, void *, unsigned int, OSMesgQueue *);
    void *data;
    void *argument2;
    unsigned int argument3;
    unsigned char unknown_01A0[4096];
    unsigned short read_count;
    unsigned short write_count;
    unsigned int buffered;
    int remaining;
    OSMesgQueue queue;
    OSMesg message;
    unsigned int field_11C8;
    unsigned int field_11CC;
    unsigned int field_11D0;
    unsigned int consumed;
    unsigned int available;
    signed char field_11DC;
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[27];
    unsigned int processed;
    float field_1220;
    unsigned int token;
} StreamState_80025264;
extern StreamState_80025264 D_80056230[2];

int func_80025264(unsigned int token)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (D_80056230[i].busy && D_80056230[i].token == token) {
            return i;
        }
    }
    return -1;
}
