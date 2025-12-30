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
extern void *__osViContext;  /* Current VI context pointer */

/* SP Status register bits */
#define SP_STATUS_HALT          0x0001  /* RSP is halted */
#define SP_STATUS_BROKE         0x0002  /* RSP hit break instruction */
#define SP_STATUS_DMA_BUSY      0x0004  /* DMA in progress */
#define SP_STATUS_DMA_FULL      0x0008  /* DMA queue is full */
#define SP_STATUS_IO_FULL       0x0010  /* I/O queue is full */
#define SP_STATUS_SSTEP         0x0020  /* Single-step mode */
#define SP_STATUS_INTR_BREAK    0x0040  /* Interrupt on break */
#define SP_STATUS_SIG0          0x0080  /* Signal 0 - yield request */
#define SP_STATUS_SIG1          0x0100  /* Signal 1 - task yielded */
#define SP_STATUS_SIG2          0x0200  /* Signal 2 */
#define SP_STATUS_SIG3          0x0400  /* Signal 3 */
#define SP_STATUS_SIG4          0x0800  /* Signal 4 */
#define SP_STATUS_SIG5          0x1000  /* Signal 5 */
#define SP_STATUS_SIG6          0x2000  /* Signal 6 */
#define SP_STATUS_SIG7          0x4000  /* Signal 7 */

/* Aliases for task yield signals */
#define SP_STATUS_YIELD         SP_STATUS_SIG0
#define SP_STATUS_YIELDED       SP_STATUS_SIG1

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

    ctx = __osViContext;
    framebuffer = *(void **)((u8 *)ctx + 4);

    __osRestoreInt(savedMask);

    return framebuffer;
}

/* Additional external functions for SP task loading */
extern void __osSiRawWriteDram(void *dramAddr, u32 size);
extern s32 __osSpRawReadWord(u32 spAddr);
extern s32 __osSpRawWriteWord(u32 spAddr, u32 value);
extern s32 __osSpRawStartDma(s32 direction, u32 spAddr, void *dramAddr, u32 size);
extern s32 __osSpDeviceBusy(void);
extern void __osSpSetStatus(u32 status);
extern s32 osVirtualToPhysical(void *vaddr);

/* SP task context buffer - mirrors OSTask structure for DMA */
extern u8 __osSpTaskContext[64];

/* Physical address pointers for task resources (converted from virtual) */
extern s32 __osSpTaskUcode;         /* Microcode DRAM address */
extern s32 __osSpTaskUcodeData;     /* Microcode data DRAM address */
extern s32 __osSpTaskDramStack;     /* RSP stack in DRAM */
extern s32 __osSpTaskOutputBuff;    /* Output buffer address */
extern s32 __osSpTaskOutputSize;    /* Output buffer size */
extern s32 __osSpTaskYieldData;     /* Yield data buffer */
extern s32 __osSpTaskData;          /* Task data pointer */

/**
 * Get SP task context
 * (func_80007840 - __osSpGetContext)
 *
 * Prepares the SP task context by converting virtual addresses
 * to physical addresses for DMA transfers.
 *
 * @return Pointer to task context buffer
 */
void *__osSpGetContext(void) {
    __osSiRawWriteDram(__osSpTaskContext, 0x40);

    /* Convert virtual addresses to physical for DMA */
    if (__osSpTaskUcode != 0) {
        __osSpTaskUcode = osVirtualToPhysical((void *)__osSpTaskUcode);
    }
    if (__osSpTaskUcodeData != 0) {
        __osSpTaskUcodeData = osVirtualToPhysical((void *)__osSpTaskUcodeData);
    }
    if (__osSpTaskDramStack != 0) {
        __osSpTaskDramStack = osVirtualToPhysical((void *)__osSpTaskDramStack);
    }
    if (__osSpTaskOutputBuff != 0) {
        __osSpTaskOutputBuff = osVirtualToPhysical((void *)__osSpTaskOutputBuff);
    }
    if (__osSpTaskOutputSize != 0) {
        __osSpTaskOutputSize = osVirtualToPhysical((void *)__osSpTaskOutputSize);
    }
    if (__osSpTaskYieldData != 0) {
        __osSpTaskYieldData = osVirtualToPhysical((void *)__osSpTaskYieldData);
    }
    if (__osSpTaskData != 0) {
        __osSpTaskData = osVirtualToPhysical((void *)__osSpTaskData);
    }

    return __osSpTaskContext;
}

/**
 * Load and start an RSP task
 * (func_8000794C - osSpTaskLoad)
 *
 * Loads the microcode and data into SP memory and starts execution.
 * Handles both fresh loads and yields by checking task flags.
 *
 * @param task Pointer to OSTask structure
 */
void osSpTaskLoad(void *task) {
    u8 *ctx;
    u8 *tp;  /* task pointer as u8* for arithmetic */
    u32 flags;

    ctx = (u8 *)__osSpGetContext();
    tp = (u8 *)task;

    /* Check if task was previously yielded */
    flags = *(u32 *)(ctx + 4);
    if (flags & 1) {
        /* Task was yielded - restore from yield buffer */
        *(u32 *)(ctx + 0x18) = *(u32 *)(ctx + 0x38);
        *(u32 *)(ctx + 0x1C) = *(u32 *)(ctx + 0x3C);
        *(u32 *)(tp + 4) = *(u32 *)(tp + 4) & ~1;

        /* Check if flag 4 is set - copy yield data */
        if (flags & 4) {
            *(u32 *)(ctx + 0x10) = *(u32 *)((*(u32 *)(tp + 0x38) + 0xBFC) | 0xA0000000);
        }
    }

    /* Write task context to IMEM */
    __osSiRawWriteDram(ctx, 0x40);

    /* Set SP status - clear halt and other bits */
    __osSpSetStatus(0x2B00);

    /* Wait for SP to be ready */
    while (__osSpRawReadWord(0x04001000) == -1) {
        /* spin */
    }

    /* DMA task context to SP DMEM at 0xFC0 */
    while (__osSpRawStartDma(1, 0x04000FC0, ctx, 0x40) == -1) {
        /* spin */
    }

    /* Wait for DMA to complete */
    while (__osSpDeviceBusy()) {
        /* spin */
    }

    /* DMA microcode to SP IMEM at 0x1000 */
    while (__osSpRawStartDma(1, 0x04001000, *(void **)(ctx + 8), *(u32 *)(ctx + 0xC)) == -1) {
        /* spin */
    }
}

/**
 * Start RSP execution
 * (func_80007AB4 - osSpTaskStartGo)
 *
 * Clears the halt bit to start RSP execution after task loading.
 *
 * @param task Pointer to OSTask (unused)
 */
void osSpTaskStartGo(s32 task) {
    /* Wait for SP to be ready */
    while (__osSpDeviceBusy()) {
        /* spin */
    }

    /* Clear halt bit (0x125 = clear halt + set broke + clear interrupt) */
    __osSpSetStatus(0x125);
}
