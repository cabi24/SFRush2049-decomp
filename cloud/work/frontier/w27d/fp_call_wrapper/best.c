/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * fp_call_wrapper (0x800AC6F4, 104 B) and brake_force_apply (0x800AC75C, 196 B) -- DRAFT bodies,
 * blocked on w27e's func_80096CA8 / func_80097164 (IPA register parameters: slot in $fp/s8, first in
 * $t0). Both are the resource-loader thread pair from cloud/work/ipa-groups/audio_frame_sync (bodies
 * as written there). They can only match in a unit where the callees are real C with register
 * parameters (retail saves s0-s8 here because the callees clobber them).
 */
typedef signed int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSThread OSThread;
typedef void *OSMesg;

extern s32 D_80156BB0[];
extern OSMesg D_80156BD4[];
extern s32 D_80034690[];
extern s32 D_80035440[];
extern s32 D_801569AC;

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
void osSetThreadPri(OSThread *t, s32 pri);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void func_80096CA8(s32 slot, s32 first);
void func_80097164(s32 slot);

void brake_force_apply(s32 arg0) {
    OSMesg msg;

    osCreateMesgQueue((OSMesgQueue *)D_80156BB0, D_80156BD4, 1);
    osSetThreadPri((OSThread *)D_80034690, 3);
    for (;;) {
        osRecvMesg((OSMesgQueue *)D_80156BB0, &msg, 1);
        func_80097164(D_801569AC);
        osSendMesg((OSMesgQueue *)D_80035440, 0, 1);
    }
}

void fp_call_wrapper(s32 slot) {
    func_80096CA8(slot, 0);
}
