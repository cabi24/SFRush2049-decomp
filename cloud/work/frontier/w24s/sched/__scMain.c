/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* __scMain (static, lib_1050_sdk, IDO 5.3 -g1 -O1): w24b's code-identical probe (cloud/work/frontier/w24b/sched/__scMain_probe.c,
 * itself module_campaign_20261002's SDK-derived body) with the callee names passed through
 * static_rename.renamed_source (w22g/w23g tables) and the scheduler TU's .bss defined below (w24s).
 * Devices (from the probe, disclosed there): compiled-out assertion blocks `if (x) { }`; the three
 * OSTime timestamps volatile. The .bss block defines two unreferenced placeholders (D_8002EB6C,
 * D_8002EB88) to place the referenced objects; see the block comment. */
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;
typedef long long s64;typedef unsigned long long u64;typedef float f32;
typedef u64 OSTime;typedef s32 OSPri;typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSScTask {struct OSScTask *next;s32 state,flags;void *framebuffer;s32 type;u8 pad14[36];void *unk38;s32 *unk3C;u8 pad40[16];OSMesgQueue *msgQueue;OSMesg msg;} OSScTask;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *msgQueue;} OSScClient;
typedef struct OSSched {s16 state;u8 pad02[30];s16 priority;u8 pad22[30];OSMesgQueue cmdQueue;OSMesg cmdMsgs[8];OSMesgQueue retQueue;OSMesg retMsgs[8];u8 padB0[432];OSScClient *clientList;OSScTask *rspTaskHead,*rspTaskTail,*rdpTaskHead,*rdpTaskTail,*curRSPTask,*curRDPTask;s32 retraceCount,audioListPending;} OSSched;
typedef struct OSViMode {u8 native[80];} OSViMode;
/* lib_1050_sdk's whole .bss = one owned .bss slot (0x8002E8E0, 0x2C8 B of objects, 0x2D0 B section),
 * in definition order (IDO 5.3 emits .bss in definition order). The three scheduler timestamps are one
 * OSTime each (single lui for both halves = defined in this TU, w24b) and volatile (hi/lo load order).
 * gScheduler is the OSSched (0x284 B) at 0x8002E8E8: the vi code's gViTickCounter (0x8002EB64) is its
 * retraceCount (+0x27C) and 0x8002EB68 its audioListPending, so neither is a separate object.
 * D_8002EB6C (4 B) and D_8002EB88 (8 B) are unreferenced anywhere in the ROM (static, game, overlay
 * asm); identity unknown, defined only so the referenced objects land at their retail addresses. */
volatile OSTime __osScRetraceTime;
OSSched gScheduler;
s32 D_8002EB6C;
s16 __osScTaskCount;
volatile OSTime __osScAudioStart;
volatile OSTime __osScFrameTime;
u64 D_8002EB88;
f32 __osScElapsedTime;
f32 __osScDeltaTime;
s32 gViFrameDelta;
s32 gViAccumTime;
s32 gViAccumTotal;
volatile s32 gViLastTick;
extern OSViMode gViModeTableBase[];
extern s32 __osScPendingSwap,__osScFrameTimeResult;
extern u32 __osScSwapCount;
extern OSScTask *__osScCurAudioTask;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern s32 osJamMesg(OSMesgQueue *,OSMesg,s32),osSendMesg(OSMesgQueue *,OSMesg,s32);
extern void osCreateThread(OSThread *,s32,void (*)(void *),void *,void *,OSPri);
extern void osStartThread(OSThread *);
extern void osCreateViManager(OSPri);
extern void osSetEventMesg(s32,OSMesgQueue *,OSMesg);
extern void osViSetMode(void *),osViBlack(s32);
extern u32 osSetIntMask(u32);
extern void osViSetEvent(OSMesgQueue *,OSMesg,s32);
extern OSTime osGetTime(void);
extern void *osViGetCurrentFramebuffer(void),*osViGetNextFramebuffer(void);
extern void osViSwapBuffer(void *),display_mode_tick(void);
extern s32 osSpTaskYielded(void *);
extern void osWritebackDCacheAll(void),osSpTaskLoad(void *),osSpTaskStartGo(void *),osSpTaskYield(void);
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
    __osScFrameTime=osGetTime();
    __scHandleRetrace(scheduler);
    __scSchedule(scheduler);
    break;
   case 667:
    __scHandleRSP(scheduler);
    break;
   case 668:
    __osScFrameTimeResult=(s32)(osGetTime()-__osScRetraceTime);
    __scHandleRDP(scheduler);
    break;
   case 669:
    __scSchedule(scheduler);
    break;
   case 670:
    for(client=scheduler->clientList;client!=0;client=client->next)
     osJamMesg(client->msgQueue,(OSMesg)&scheduler->priority,OS_MESG_BLOCK);
    break;
  }
 }
}
