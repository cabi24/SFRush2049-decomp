/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
/* Complete original SDK VI storage ownership, not a counter-only binding. */
typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue, *evtQueue, *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;
extern OSDevMgr gViMgrState;
static OSThread gViMgrThreadArg;
static u64 viThreadStack[4096 / sizeof(u64)];
static OSMesgQueue gViMgrMesgQueue;
static OSMesg gViMgrMesgBuffer[5];
static OSIoMesg gViMgrEventDP;
static OSIoMesg gViMgrEventSP;
extern u64 gViTimeAccumHi;
extern u32 gViLastCount, gViRetraceCount;
extern void dll_init(void), dll_update(void), osViInit(void);
extern s32 dll_get_priority(void *);
extern s32 osGetActiveQueue(void); /* Preserve accepted pointer-valued s32 getter. */
void vi_manager_main(void *);
#define OS_EVENT_VI 7
#define OS_EVENT_COUNTER 3
void osSetEventMesg(OSPri pri) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;


    if (gViMgrState.active) {
        return;
    }
    dll_init();
    gViMgrEventCount = 0;
    osCreateMesgQueue(&gViMgrMesgQueue, gViMgrMesgBuffer, ARRLEN(gViMgrMesgBuffer));
    gViMgrEventDP.hdr.type = OS_MESG_TYPE_VRETRACE;
    gViMgrEventDP.hdr.pri = OS_MESG_PRI_NORMAL;
    gViMgrEventDP.hdr.retQueue = NULL;
    gViMgrEventSP.hdr.type = OS_MESG_TYPE_COUNTER;
    gViMgrEventSP.hdr.pri = OS_MESG_PRI_NORMAL;
    gViMgrEventSP.hdr.retQueue = NULL;
    osSetEventMesgAlt(OS_EVENT_VI, &gViMgrMesgQueue, &gViMgrEventDP);
    osSetEventMesgAlt(OS_EVENT_COUNTER, &gViMgrMesgQueue, &gViMgrEventSP);
    oldPri = -1;
    myPri = dll_get_priority(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        osCreateViManager(NULL, pri);
    }

    savedMask = __osDisableInt();
    gViMgrState.active = TRUE;
    gViMgrState.thread = &gViMgrThreadArg;
    gViMgrState.cmdQueue = &gViMgrMesgQueue;
    gViMgrState.evtQueue = &gViMgrMesgQueue;
    gViMgrState.acsQueue = NULL;
    gViMgrState.dma = NULL;
    gViMgrState.edma = NULL;
    osCreateThread(&gViMgrThreadArg, 0, vi_manager_main, &gViMgrState, (void *)(viThreadStack + 4096/sizeof(u64)), pri);
    osViInit();
    osStartThread(&gViMgrThreadArg);
    __osRestoreInt(savedMask);

    if (oldPri != -1) {
        osCreateViManager(NULL, oldPri);
    }
}

void vi_manager_main(void* arg) {
    __OSViContext* vc;
    OSDevMgr* dm;
    OSIoMesg* mb;
    static u16 gViMgrRetraceCounter;
    
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = (__OSViContext *)osGetActiveQueue();
    gViMgrRetraceCounter = vc->retraceCount;
    if (gViMgrRetraceCounter == 0) {
        gViMgrRetraceCounter = 1;
    }
    dm = (OSDevMgr*)arg;

    while (TRUE) {
        osRecvMesg(dm->evtQueue, (OSMesg)&mb, OS_MESG_BLOCK);
        switch (mb->hdr.type) {
            case OS_MESG_TYPE_VRETRACE:
                __osViSwapContext();
                gViMgrRetraceCounter--;

                if (gViMgrRetraceCounter == 0) {
                    vc = (__OSViContext *)osGetActiveQueue();
                    if (vc->msgq != NULL) {
                        osJamMesg(vc->msgq, vc->msg, OS_MESG_NOBLOCK);
                    }
                    gViMgrRetraceCounter = vc->retraceCount;
                }

                gViRetraceCount++;

                if (first) {
                    count = osGetCount();
                    gViTimeAccumHi = count;
                    first = 0;
                }

                count = gViLastCount;
                gViLastCount = osGetCount();
                count = gViLastCount - count;
                gViTimeAccumHi = gViTimeAccumHi + count;
                break;
            case OS_MESG_TYPE_COUNTER:
                dll_update();
                break;
            default:
                break;
        }
    }
}
