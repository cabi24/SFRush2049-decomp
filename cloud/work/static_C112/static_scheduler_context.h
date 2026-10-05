#ifndef RUSH_STATIC_SCHEDULER_CONTEXT_H
#define RUSH_STATIC_SCHEDULER_CONTEXT_H
/* Include after the unchanged ROM SDK context. Declarations only;
 * native scheduler offsets come from the existing OSSched definition. */
extern u32 osSetGlobalIntMask(u32);
extern void *osViGetCurrentFramebuffer(void);
extern void *osViGetFramebuffer(void);
extern s32 __osScPendingSwap;
extern u32 __osScSwapCount;
#endif
