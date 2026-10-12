/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* w24b research probe (NOT a deliverable): module_campaign_20261002 __scMain.c + timestamps volatile and DEFINED here (see RESULTS.md) */
/* Actual SDK data carriers used by the native scheduler. No storage defined. */
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;
typedef long long s64;typedef unsigned long long u64;
typedef u64 OSTime;typedef s32 OSPri;typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSScTask {struct OSScTask *next;s32 state,flags;void *framebuffer;s32 type;u8 pad14[36];void *unk38;s32 *unk3C;u8 pad40[16];OSMesgQueue *msgQueue;OSMesg msg;} OSScTask;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *msgQueue;} OSScClient;
typedef struct OSSched {s16 state;u8 pad02[30];s16 priority;u8 pad22[30];OSMesgQueue cmdQueue;OSMesg cmdMsgs[8];OSMesgQueue retQueue;OSMesg retMsgs[8];u8 padB0[432];OSScClient *clientList;OSScTask *rspTaskHead,*rspTaskTail,*rdpTaskHead,*rdpTaskTail,*curRSPTask,*curRDPTask;s32 retraceCount,audioListPending;} OSSched;
typedef struct OSViMode {u8 native[80];} OSViMode;
extern OSViMode gViModeTableBase[];
extern s16 __osScTaskCount;
extern s32 __osScPendingSwap,__osScFrameTimeResult;
extern u32 __osScSwapCount;
/* Each native timestamp is a contiguous high/low 64-bit object. The existing
 * physical Hi symbol names are retained, with their real complete width. */
volatile OSTime __osScFrameTimeHi,__osScRetraceTimeHi,__osScAudioStartHi;
extern OSScTask *__osScCurAudioTask;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern s32 osSendMesg(OSMesgQueue *,OSMesg,s32),osJamMesg(OSMesgQueue *,OSMesg,s32);
extern void osCreateThread(OSThread *,s32,void (*)(void *),void *,void *,OSPri);
extern void osStartThread(OSThread *);
extern void osSetEventMesg(OSPri);
extern void osSetEventMesgAlt(s32,OSMesgQueue *,OSMesg);
extern void osSetThreadPri(void *),osSetIntMask(s32);
extern u32 osSetGlobalIntMask(u32);
extern void osSetTimerIntr(OSMesgQueue *,OSMesg,s32);
extern OSTime osGetTime(void);
extern void *osViGetCurrentFramebuffer(void),*osViGetFramebuffer(void);
extern void osViSetMode(void *),display_mode_tick(void);
extern s32 osSpTaskYielded(void *);
extern void osInvalICache(void),osViModeNtscLan1(void *),osViModeNtscLpn1(void *),osDpWait(void);
extern s32 osDpSetNextBuffer(void *,u64);
void osCreateScheduler(OSSched *,void *,OSPri,u8,u8);
void osScAddClient(OSSched *,OSScClient *,OSMesgQueue *);
void __scMain(void *);
void __scSchedule(OSSched *);
void __scHandleRetrace(OSSched *),__scHandleRSP(OSSched *),__scHandleRDP(OSSched *);
OSScTask *__scTaskReady(OSSched *,OSScTask *);
s32 __scExecTask(OSSched *,OSScTask *);
void __scAppendList(OSSched *,OSScTask *),__scExec(OSSched *,OSScTask *,OSScTask *),__scHandlePreNMI(OSSched *);
s32 __scScheduleCore(OSSched *,OSScTask **,OSScTask **,s32);
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
/* Complete SDK scheduler event dispatcher; N64 adds frame timestamps and
 * task-queue scheduling, with actual five message IDs 666..670. */
void __scMain(void *arg) {
 OSMesg message;
 OSSched *scheduler=(OSSched *)arg;
 OSScClient *client;
 while(1) {
  osRecvMesg(&scheduler->cmdQueue,&message,OS_MESG_BLOCK);
  switch((s32)message) {
   case 666:
    __osScFrameTimeHi=osGetTime();
    __scHandleRetrace(scheduler);
    __scSchedule(scheduler);
    break;
   case 667:
    __scHandleRSP(scheduler);
    break;
   case 668:
    __osScFrameTimeResult=(s32)(osGetTime()-__osScRetraceTimeHi);
    __scHandleRDP(scheduler);
    break;
   case 669:
    __scSchedule(scheduler);
    break;
   case 670:
    for(client=scheduler->clientList;client!=0;client=client->next)
     osSendMesg(client->msgQueue,(OSMesg)&scheduler->priority,OS_MESG_BLOCK);
    break;
  }
 }
}
