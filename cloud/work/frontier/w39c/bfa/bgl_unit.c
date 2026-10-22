/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* unit-test copy of the brake_force_apply body in g3/group.c (same text), with only the declarations it needs */
typedef signed int s32;
typedef void *OSMesg;
typedef struct OSMesgQueue { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct OSThread_s { char opaque[0x1B0]; } OSThread;
#define NULL ((void *)0)
extern char D_80156BB0[], D_80156BD4[], D_80034690[], D_80035440[];
extern s32 D_801569AC;
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
void osSetThreadPri(OSThread *t, s32 pri);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void func_80097164(s32 slot);

void brake_force_apply(void *arg) {
    OSMesg msg;

    osCreateMesgQueue((OSMesgQueue *) &D_80156BB0, (void **) &D_80156BD4, 1);
    osSetThreadPri((OSThread *) &D_80034690, 3);
    for (;;) {
        osRecvMesg((OSMesgQueue *) &D_80156BB0, &msg, 1);
        func_80097164(D_801569AC);
        osSendMesg((OSMesgQueue *) &D_80035440, NULL, 1);
    }
}
