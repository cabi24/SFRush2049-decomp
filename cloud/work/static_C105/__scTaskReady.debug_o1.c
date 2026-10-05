/* flags: -g1 -O1 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef struct Scheduler {u8 state[0x27C];u32 retrace;} Scheduler;
typedef struct Task Task;
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetFramebuffer(void);
extern s32 __osScPendingSwap;
extern u32 __osScSwapCount;
Task *__scTaskReady(Scheduler *scheduler, Task *task) {
    void *current;
    void *next;
    if (task != 0) {
        if ((current = osViGetCurrentFramebuffer()) !=
            (next = osViGetFramebuffer())) return 0;
        if (__osScPendingSwap != 0 && scheduler->retrace - __osScSwapCount < 2U)
            return 0;
        return task;
    } else {
        return 0;
    }
}
