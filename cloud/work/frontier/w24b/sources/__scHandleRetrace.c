/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* __scHandleRetrace (static, boot 0x80000880, 208 B, segment 0x1050 -> src/rom/lib_1050_sdk.c, IDO 5.3 -g1 -O1).
 * Rush-modified SDK sched.c __scHandleRetrace (ultralib src/sched/sched.c 215): frame count++, apply a pending
 * framebuffer swap at least two retraces after the last one (Rush), notify every client (osSendMesg, as the SDK). The SDK
 * task drain/schedule block moved to __scSchedule (locked sibling).
 * Base: module_campaign_20261002 static_scheduler/__scHandleRetrace.c. w24b:
 *  - the SDK declaration list is kept: rspTask, i, state are UNUSED (SDK declarations whose uses Rush deleted);
 *    disclosed frame arithmetic: retail frame 48 = 0x18 + 6 local slots; without the three unused it is 40
 *    (0x18 + 3 slots, 8-aligned): they are exactly the frame residual. sp/dp keep the SDK `= 0` initialisers
 *    (retail's two `sw zero` stores at 0x20/0x1C); Rush moved `state` after dp (slot order 0x2C rspTask,
 *    0x28 client, 0x24 i, 0x20 sp, 0x1C dp, 0x18 state).
 *  - SHAPING DEVICE (disclosed): an empty `else {}` after the swap block (retail's `b` to the next
 *    instruction; the SDK has an else arm here whose body Rush removed).
 * The swap call is osViSwapBuffer and the notify call osSendMesg (w22g names at 0x80007590 / 0x800075E0;
 * the inventory target object still carries the pre-w22g names osViSetMode / osJamMesg for them). Retrace message = (OSMesg)scheduler (SDK &sc->retraceMsg, offset 0). */
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
extern s32 __osScPendingSwap;
extern u32 __osScSwapCount;
extern void osViSwapBuffer(void *mode);
extern void display_mode_tick(void);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

void __scHandleRetrace(OSSched *scheduler) {
    OSScTask *rspTask;
    OSScClient *client;
    s32 i;
    OSScTask *sp = 0;
    OSScTask *dp = 0;
    s32 state;

    scheduler->retraceCount++;
    if (__osScPendingSwap && scheduler->retraceCount - __osScSwapCount >= 2U) {
        osViSwapBuffer((void *)(u32)__osScPendingSwap);
        display_mode_tick();
        __osScSwapCount = scheduler->retraceCount;
        __osScPendingSwap = 0;
    } else {
    }
    for (client = scheduler->clientList; client != 0; client = client->next) {
        osSendMesg(client->msgQueue, (OSMesg)scheduler, 0);
    }
}
