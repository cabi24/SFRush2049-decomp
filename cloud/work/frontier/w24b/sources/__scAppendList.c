/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* __scAppendList (static, boot 0x80000D6C, 180 B, segment 0x1050 -> src/rom/lib_1050_sdk.c, IDO 5.3 -g1 -O1).
 * Rush-modified SDK sched.c __scAppendList (ultralib src/sched/sched.c 408): audio tasks go on the audio list
 * (Rush fields rdpTaskHead/rspTaskHead = SDK audioListTail/audioListHead), graphics on the gfx list; Rush adds
 * a compiled-out check of the pending flag before setting it. Types = include/m2c_types.h's (rom_tu.h).
 * Base: module_campaign_20261002 static_scheduler/__scAppendList.c (1 insn off). w24b levers:
 *  - the assert is the `if (!(EX)) {}` form (beq/beq to the same label; `if (EX) {}` gives beq/bne);
 *  - SHAPING DEVICE (disclosed): an empty statement after `audioListPending = 1;` (a compiled-out logging
 *    call, cf. SDK `#ifdef SC_LOGGING osLogEvent(...)`): at -g1 it ends the store's line, and as1 then keeps
 *    the store out of the following `b`'s delay slot, as retail.
 * No unused locals (`type` is SDK's own local). */
typedef signed short s16;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSThread_s OSThread;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSScTask_s {
    struct OSScTask_s *next;   /* 0x00 */
    s32 state;                  /* 0x04 */
    s32 flags;                  /* 0x08 */
    void *framebuffer;          /* 0x0C */
    s32 type;                   /* 0x10 */
    u8 pad14[0x38 - 0x14];
    void *unk38;                 /* 0x38 */
    s32 *unk3C;                  /* 0x3C */
    u8 pad40[0x50 - 0x40];
    OSMesgQueue *msgQueue;        /* 0x50 */
    OSMesg msg;                    /* 0x54 */
} OSScTask;
typedef struct OSScClient_s {
    struct OSScClient_s *next;  /* 0x00 */
    OSMesgQueue *msgQueue;       /* 0x04 */
} OSScClient;
typedef struct {
    s16 state;                        /* 0x00 */
    u8 pad02[0x20 - 0x02];
    s16 priority;                     /* 0x20 */
    u8 pad22[0x40 - 0x22];
    OSMesgQueue cmdQueue;              /* 0x40 */
    OSMesg cmdMsgs[8];                 /* 0x58 */
    OSMesgQueue retQueue;              /* 0x78 */
    OSMesg retMsgs[8];                 /* 0x90 */
    u8 padB0[0x260 - 0xB0];            /* thread lives here */
    OSScClient *clientList;            /* 0x260 */
    OSScTask *rspTaskHead;             /* 0x264 */
    OSScTask *rspTaskTail;             /* 0x268 */
    OSScTask *rdpTaskHead;             /* 0x26C */
    OSScTask *rdpTaskTail;             /* 0x270 */
    OSScTask *curRSPTask;              /* 0x274 */
    OSScTask *curRDPTask;              /* 0x278 */
    s32 retraceCount;                  /* 0x27C */
    s32 audioListPending;              /* 0x280 */
} OSSched;

void __scAppendList(OSSched *scheduler, OSScTask *task)
{
    long type = task->type;

    if (!((type == 2) || (type == 1))) { }

    if (type == 2) {
        if (scheduler->rdpTaskHead)
            scheduler->rdpTaskHead->next = task;
        else
            scheduler->rspTaskHead = task;
        scheduler->rdpTaskHead = task;
        if (scheduler->audioListPending) { }
        scheduler->audioListPending = 1;
        ;
    } else {
        if (scheduler->rdpTaskTail)
            scheduler->rdpTaskTail->next = task;
        else
            scheduler->rspTaskTail = task;
        scheduler->rdpTaskTail = task;
    }
    task->next = 0;
    task->state = task->flags & 3;
}
