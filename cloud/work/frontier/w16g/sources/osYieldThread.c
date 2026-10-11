/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
/* osYieldThread (static, boot 0x80010110, 80 B, segment 0x10d10 -> src/rom/lib_10d10.c, unconverted)
 * Origin: reference/repos/ultralib (decompals e24c836) src/os/yieldthread.c (all versions), -O1 like libultra os/.
 * Project names (both historical mislabels, see RESULTS.md "labels"):
 *   ultralib __osEnqueueAndYield = project __osCleanupThread (0x8000CFC4, exceptasm.s)
 *   ultralib __osRunQueue        = project __osActiveQueue   (0x8002C3D8; the project's
 *                                  __osRunQueue at 0x8002C3DC is ultralib __osActiveQueue) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
} OSThread;

#define OS_STATE_RUNNABLE 2

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern void __osCleanupThread(OSThread **queue);
extern OSThread *__osRunningThread;
extern OSThread *__osActiveQueue;

void osYieldThread(void) {
    register u32 saveMask = __osDisableInt();

    __osRunningThread->state = OS_STATE_RUNNABLE;
    __osCleanupThread(&__osActiveQueue);
    __osRestoreInt(saveMask);
}
