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

/* Additional external functions for SP task loading */
extern void __osSiRawWriteDram(void *dramAddr, u32 size);
extern s32 __osSpRawReadWord(u32 spAddr);
extern s32 __osSpRawWriteWord(u32 spAddr, u32 value);
extern s32 __osSpRawStartDma(s32 direction, u32 spAddr, void *dramAddr, u32 size);
extern s32 __osSpDeviceBusy(void);
extern void __osSpSetStatus(u32 status);
extern s32 osVirtualToPhysical(void *vaddr);

/* SP task context globals */
extern u8 D_80036790[64];    /* SP task context buffer */
extern s32 D_800367A0;       /* Task ucode pointer */
extern s32 D_800367A8;       /* Task ucode_data pointer */
extern s32 D_800367B0;       /* Task dram_stack pointer */
extern s32 D_800367B8;       /* Task output_buff pointer */
extern s32 D_800367BC;       /* Task output_buff_size */
extern s32 D_800367C0;       /* Task yield_data pointer */
extern s32 D_800367C8;       /* Task data pointer */

/**
 * Get SP task context
 * (func_80007840 - __osSpGetContext)
 *
 * Prepares the SP task context by converting virtual addresses
 * to physical addresses for DMA transfers.
 *
 * @return Pointer to task context
 */
void *__osSpGetContext(void) {
    __osSiRawWriteDram(D_80036790, 0x40);

    if (D_800367A0 != 0) {
        D_800367A0 = osVirtualToPhysical((void *)D_800367A0);
    }
    if (D_800367A8 != 0) {
        D_800367A8 = osVirtualToPhysical((void *)D_800367A8);
    }
    if (D_800367B0 != 0) {
        D_800367B0 = osVirtualToPhysical((void *)D_800367B0);
    }
    if (D_800367B8 != 0) {
        D_800367B8 = osVirtualToPhysical((void *)D_800367B8);
    }
    if (D_800367BC != 0) {
        D_800367BC = osVirtualToPhysical((void *)D_800367BC);
    }
    if (D_800367C0 != 0) {
        D_800367C0 = osVirtualToPhysical((void *)D_800367C0);
    }
    if (D_800367C8 != 0) {
        D_800367C8 = osVirtualToPhysical((void *)D_800367C8);
    }

    return D_80036790;
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
    u32 flags;

    ctx = (u8 *)__osSpGetContext();

    /* Check if task was previously yielded */
    flags = *(u32 *)(ctx + 4);
    if (flags & 1) {
        /* Task was yielded - restore from yield buffer */
        *(u32 *)(ctx + 0x18) = *(u32 *)(ctx + 0x38);
        *(u32 *)(ctx + 0x1C) = *(u32 *)(ctx + 0x3C);
        *(u32 *)(task + 4) = *(u32 *)(task + 4) & ~1;

        /* Check if flag 4 is set - copy yield data */
        if (flags & 4) {
            *(u32 *)(ctx + 0x10) = *(u32 *)((*(u32 *)(task + 0x38) + 0xBFC) | 0xA0000000);
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
