/**
 * @file os_pi_dma.c
 * @brief PI (Peripheral Interface) DMA start function
 *
 * Decompiled from asm/us/9230.s
 *
 * The Peripheral Interface handles DMA transfers between
 * cartridge ROM and RDRAM.
 */

#include "types.h"
#include "PR/os_pi.h"

/* External data (standard libultra name) */
extern s32 __osPiInitialized;   /* PI initialized flag: 0x8002C380 */

/* External functions */
extern OSPiHandle *__osPiGetHandle(void);                              /* func_8000E390 */
extern s32 __osPiQueueRequest(OSPiHandle *handle, OSIoMesg *mb, s32 flags);  /* func_80007440 */
extern s32 __osPiQueueRequestAlt(OSPiHandle *handle, OSIoMesg *mb, s32 flags); /* func_800075E0 */

/**
 * Start PI DMA transfer
 * (func_80008630 - osPiStartDma)
 *
 * Initiates a DMA transfer between cartridge ROM and RDRAM.
 * The transfer is queued and executed asynchronously.
 *
 * @param mb PI I/O message structure to fill
 * @param priority Priority (OS_MESG_PRI_HIGH = 1, OS_MESG_PRI_NORMAL = 0)
 * @param direction OS_READ (ROM -> RDRAM) or OS_WRITE (RDRAM -> ROM)
 * @param devAddr Device (ROM) address
 * @param dramAddr RDRAM buffer address
 * @param size Transfer size in bytes
 * @param mq Message queue for completion notification
 * @return 0 on success, -1 if PI not initialized
 */
s32 osPiStartDma(OSIoMesg *mb, s32 priority, s32 direction,
                 u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq) {
    OSPiHandle *handle;
    s32 result;

    /* Check if PI is initialized */
    if (__osPiInitialized == 0) {
        return -1;
    }

    /* Set request type based on direction */
    if (direction == OS_READ) {
        mb->hdr.type = OS_MESG_TYPE_DMAREAD;
    } else {
        mb->hdr.type = OS_MESG_TYPE_DMAWRITE;
    }

    /* Fill message structure */
    mb->hdr.pri = (u8)priority;
    mb->hdr.status = 0;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = devAddr;
    mb->size = size;
    mb->piHandle = NULL;

    /* Get PI handle and queue request */
    handle = __osPiGetHandle();
    if (priority == OS_MESG_PRI_HIGH) {
        result = __osPiQueueRequest(handle, mb, 0);
    } else {
        result = __osPiQueueRequestAlt(handle, mb, 0);
    }

    return result;
}
