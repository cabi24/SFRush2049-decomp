/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_80091AF0 @ 0x80091AF0 (locked caller-less 2-word stub, given its body; w17h): slot-queue lock: osRecvMesg(&D_80142728, NULL, OS_MESG_BLOCK).
 * Internal and umerge-inlined into every caller (func_800C8918 here); only the retail jr-ra stub remains,
 * like the heap lock/unlock stubs func_80095CF4/func_80095CFC in wheel_params_set.c. */
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
#define NULL ((void *)0)
extern s32 D_80142728;
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

void func_80091AF0(void) {
    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
}

