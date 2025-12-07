/**
 * @file os_sp_task.c
 * @brief RSP task management functions
 *
 * Decompiled from asm/us/8330.s
 * Contains functions for checking RSP task completion
 */

#include "types.h"

/* External functions */
extern u32 __osSpGetStatus(void);
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);

/* External data */
extern void *D_8002C460;  /* VI/RSP context pointer */

/* SP Status bits */
#define SP_STATUS_YIELD         0x0080  /* Signal 0 - task yielded */
#define SP_STATUS_YIELDED       0x0100  /* Signal 1 - task has been yielded */

/**
 * Check if RSP task is done or yielded
 * (func_80007730 - osSpTaskYielded)
 *
 * Checks if the RSP task has completed or yielded.
 * If yielded, updates the task flags.
 *
 * @param task Pointer to OSTask structure
 * @return 1 if task yielded with signal 1, 0 otherwise
 */
s32 osSpTaskYielded(void *task) {
    u32 status;
    s32 result;

    status = __osSpGetStatus();

    /* Check if signal 1 (YIELDED) is set */
    if (status & SP_STATUS_YIELDED) {
        result = 1;
    } else {
        result = 0;
    }

    /* If signal 0 (YIELD) is set, update task flags */
    if (status & SP_STATUS_YIELD) {
        u32 *flags = (u32 *)((u8 *)task + 4);
        *flags = (*flags | result) & ~2;  /* Set yielded bit, clear running bit */
    }

    return result;
}

/**
 * Get current framebuffer with interrupt protection
 * (func_80007790 - osViGetCurrentFramebuffer variant)
 *
 * Returns the current framebuffer pointer from the VI context.
 * Disables interrupts during access.
 *
 * @return Framebuffer pointer
 */
void *osViGetFramebuffer(void) {
    s32 savedMask;
    void *framebuffer;
    void *ctx;

    savedMask = __osDisableInt();

    ctx = D_8002C460;
    framebuffer = *(void **)((u8 *)ctx + 4);

    __osRestoreInt(savedMask);

    return framebuffer;
}
