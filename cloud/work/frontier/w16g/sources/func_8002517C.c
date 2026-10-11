/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002517C (static, boot 0x8002517C, 44 B, segment 0x25bb0 -> src/rom/lib_25bb0.c, converted)
 * Shape of ultralib __osSiRelAccess (src/io/siacs.c: release the access queue), but the callee at 0x800075E0 is
 * labelled osJamMesg in this project (ultralib uses osSendMesg) and the queue is the Rush-owned D_800586A8,
 * created/acquired by already-promoted siblings in lib_25bb0.c. Identity with the SDK function is unproven;
 * the body is exact either way. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

#define NULL ((void *)0)
#define OS_MESG_NOBLOCK 0

extern s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
extern OSMesgQueue D_800586A8;

void func_8002517C(void) {
    osJamMesg(&D_800586A8, NULL, OS_MESG_NOBLOCK);
}
