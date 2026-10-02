#ifndef CAMPAIGN_SCHEDULER_DECLARATIONS_H
#define CAMPAIGN_SCHEDULER_DECLARATIONS_H
/* Include after unchanged current rom_tu.h; declarations only. */
extern s16 __osScTaskCount;
extern OSViMode gViModeTableBase[];
extern void osSetEventMesg(OSPri);
extern void osSetTimerIntr(OSMesgQueue *,OSMesg,s32);
extern u32 osSetGlobalIntMask(u32);
extern void __scMain(void *);
extern s32 osSpTaskYielded(void *);
extern s32 __scExecTask(OSSched *,OSScTask *);
extern s32 __scScheduleCore(OSSched *,OSScTask **,OSScTask **,s32);
extern void __scHandlePreNMI(OSSched *);
extern void *osViGetCurrentFramebuffer(void),*osViGetFramebuffer(void);
extern s32 __osScPendingSwap;
extern u32 __osScSwapCount;
extern void osDpWait(void);
#endif
