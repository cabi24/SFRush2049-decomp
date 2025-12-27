/**
 * @file os_pi_dma.c
 * @brief PI (Peripheral Interface) DMA start function
 *
 * Decompiled from asm/us/9230.s
 */

#include "types.h"

/* External data */
extern s32 D_8002C380;  /* PI initialized flag */

/* External functions */
extern void *func_8000E390(void);  /* Get PI handle */
extern s32 func_80007440(void *handle, void *request, s32 flags);  /* Queue PI request */
extern s32 func_800075E0(void *handle, void *request, s32 flags);  /* Alternate PI request */

/**
 * Start PI DMA transfer
 * (func_80008630 - osPiStartDma)
 *
 * Initiates a DMA transfer between cartridge ROM and RDRAM.
 *
 * @param request PI request structure to fill
 * @param priority Priority (1 = high, other = normal)
 * @param direction 0 = read (ROM -> RDRAM), non-zero = write
 * @param devAddr Device (ROM) address
 * @param dramAddr RDRAM address
 * @param size Transfer size
 * @param queue Message queue for completion
 * @return 0 on success, -1 if PI not initialized
 */
s32 osPiStartDma(void *request, s32 priority, s32 direction,
                 u32 devAddr, void *dramAddr, u32 size, void *queue) {
    void *handle;
    s32 result;
    u8 *req = (u8 *)request;

    /* Check if PI is initialized */
    if (D_8002C380 == 0) {
        return -1;
    }

    /* Set request type based on direction */
    if (direction == 0) {
        *(s16 *)(req + 0) = 0x0B;  /* Read */
    } else {
        *(s16 *)(req + 0) = 0x0C;  /* Write */
    }

    /* Fill request structure */
    *(u8 *)(req + 2) = (u8)priority;
    *(u32 *)(req + 4) = devAddr;
    *(u32 *)(req + 8) = (u32)dramAddr;
    *(u32 *)(req + 0xC) = size;
    *(u32 *)(req + 0x10) = (u32)queue;
    *(u32 *)(req + 0x14) = 0;

    /* Get PI handle and queue request */
    handle = func_8000E390();
    if (priority == 1) {
        result = func_80007440(handle, request, 0);
    } else {
        result = func_800075E0(handle, request, 0);
    }

    return result;
}
