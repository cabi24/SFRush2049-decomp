/**
 * @file os_thread.h
 * @brief N64 OS thread definitions
 */

#ifndef _OS_THREAD_H_
#define _OS_THREAD_H_

#include "types.h"

/* Thread priority type */
typedef s32 OSPri;

/* Thread priority values */
#define OS_PRIORITY_IDLE        0
#define OS_PRIORITY_APPMAX      127
#define OS_PRIORITY_SIMGR       140
#define OS_PRIORITY_PIMGR       150
#define OS_PRIORITY_RMONSPIN    200
#define OS_PRIORITY_RMON        250
#define OS_PRIORITY_VIMGR       254
#define OS_PRIORITY_MAX         255

/* Thread states */
#define OS_STATE_STOPPED        1
#define OS_STATE_RUNNABLE       2
#define OS_STATE_RUNNING        4
#define OS_STATE_WAITING        8

/* Thread flags */
#define OS_FLAG_CPU_BREAK       1
#define OS_FLAG_FAULT           2

/* Thread context (register save area) */
typedef struct __OSThreadContext {
    u64     at, v0, v1, a0, a1, a2, a3;
    u64     t0, t1, t2, t3, t4, t5, t6, t7;
    u64     s0, s1, s2, s3, s4, s5, s6, s7;
    u64     t8, t9, gp, sp, s8, ra;
    u64     lo, hi;
    u32     sr, pc, cause, badvaddr, rcp;
    u32     fpcsr;
    f32     fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    f32     fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;

/* Thread structure */
typedef struct OSThread_s {
    struct OSThread_s   *next;      /* Next thread in queue */
    s32                 priority;   /* Thread priority */
    struct OSThread_s   **queue;    /* Queue this thread is on */
    struct OSThread_s   *tlnext;    /* Thread list next */
    u16                 state;      /* Thread state */
    u16                 flags;      /* Thread flags */
    s32                 id;         /* Thread ID */
    s32                 fp;         /* FP used flag */
    __OSThreadContext   context;    /* Thread context (registers) */
} OSThread;

/* Function prototypes */
void osCreateThread(OSThread *thread, s32 id, void (*entry)(void *),
                    void *arg, void *sp, s32 priority);
void osStartThread(OSThread *thread);
void osStopThread(OSThread *thread);
s32 osSetThreadPri(OSThread *thread, s32 priority);
OSPri osGetThreadPri(OSThread *thread);
s32 osGetThreadId(OSThread *thread);
OSThread *__osPopThread(OSThread **queue);
void __osEnqueueThread(OSThread **queue, OSThread *thread);

/* External thread pointers */
extern OSThread *__osRunningThread;
extern OSThread *__osActiveQueue;

#endif /* _OS_THREAD_H_ */
