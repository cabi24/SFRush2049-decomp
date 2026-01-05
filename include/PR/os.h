/**
 * @file os.h
 * @brief N64 Operating System - Main include
 *
 * Central header for libultra OS functions.
 */

#ifndef _OS_H_
#define _OS_H_

#include "types.h"
#include "PR/os_message.h"
#include "PR/os_thread.h"
#include "PR/os_pi.h"
#include "PR/os_ai.h"
#include "PR/os_pfs.h"
#include "PR/os_cont.h"
#include "PR/os_time.h"

/* Cache operations */
void osInvalDCache(void *vaddr, s32 nbytes);
void osInvalICache(void *vaddr, s32 nbytes);
void osWritebackDCache(void *vaddr, s32 nbytes);
void osWritebackDCacheAll(void);

/* Memory operations */
void bzero(void *s, s32 n);
void bcopy(const void *src, void *dst, s32 len);
u32 osVirtualToPhysical(void *vaddr);
void *osPhysicalToVirtual(u32 paddr);

/* Interrupt control */
u32 __osDisableInt(void);
void __osRestoreInt(u32 flags);

/* Timer functions (see os_time.h for osSetTimer/osStopTimer) */
void osSetTime(u64 time);
u64 osGetTime(void);

/* Misc functions */
s32 osDpIsBusy(void);
void osDpSetNextBuffer(void *dramAddr, u32 size);

#endif /* _OS_H_ */
