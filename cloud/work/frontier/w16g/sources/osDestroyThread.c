/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
/* osDestroyThread (static, boot 0x8000FBA0, 272 B, segment 0x107a0 -> src/rom/lib_107a0.c, unconverted)
 * Conveyor inventory target_id: __osEnqueueAndYield (stale pre-S12 label; same address and object).
 * Origin: reference/repos/ultralib (decompals e24c836) src/os/destroythread.c, BUILD_VERSION >= VERSION_J
 *         loop (priority != -1 sentinel), -O1 like libultra os/.
 * Project names: ultralib __osDequeueThread = project dll_remove (0x8000C050, [MISATTR] in symbol_addrs);
 *   ultralib __osActiveQueue = project __osRunQueue (0x8002C3DC; names swapped, see RESULTS.md). */
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

#define NULL ((void *)0)
#define OS_STATE_STOPPED 1

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern void dll_remove(OSThread **queue, OSThread *thread);
extern void __osDispatchThread(void);
extern OSThread *__osRunningThread;
extern OSThread *__osRunQueue;

void osDestroyThread(OSThread* t) {
    register u32 saveMask;
    register OSThread* pred;
    register OSThread* succ;

    saveMask = __osDisableInt();

    if (t == NULL) {
        t = __osRunningThread;
    } else if (t->state != OS_STATE_STOPPED) {
        dll_remove(t->queue, t);
    }

    if (__osRunQueue == t) {
        __osRunQueue = __osRunQueue->tlnext;
    } else {
        pred = __osRunQueue;
        while (pred->priority != -1) {
            succ = pred->tlnext;
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
        }
    }

    if (t == __osRunningThread) {
        __osDispatchThread();
    }

    __osRestoreInt(saveMask);
}
