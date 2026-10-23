/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_80091AF8 @ 0x80091AF8 (locked caller-less 2-word stub, given its body; w17h): slot-queue unlock: osJamMesg(&D_80142728, NULL, OS_MESG_NOBLOCK).
 * Internal and umerge-inlined into every caller (func_800C8918 here); only the retail jr-ra stub remains,
 * like the heap lock/unlock stubs func_80095CF4/func_80095CFC in wheel_params_set.c. */
typedef int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
#define NULL ((void *)0)
extern s32 D_80142728;
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

void func_80091AF8(void) {
    osSendMesg((OSMesgQueue *) &D_80142728, NULL, 0);
}


