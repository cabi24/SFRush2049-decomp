/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct OSThread_s { char pad[0x1B0]; } OSThread;
extern OSMesgQueue D_80156BB0;      /* background loader: request queue */
extern OSMesg D_80156BD4[];         /* its message buffer */
extern OSMesgQueue D_80035440;      /* background loader: done queue */
extern OSThread D_80034690;         /* background loader thread */
extern s32 D_801569AC;              /* wave instance to read */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
void osSetThreadPri(OSThread *t, s32 pri);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void func_80097164(s32 idx);
void brake_force_apply(void *arg) {
    OSMesg msg;

    osCreateMesgQueue(&D_80156BB0, D_80156BD4, 1);
    osSetThreadPri(&D_80034690, 3);
loop:
    osRecvMesg(&D_80156BB0, &msg, 1);
    func_80097164(D_801569AC);
    osSendMesg(&D_80035440, ((void *)0), 1);
    goto loop;
}
