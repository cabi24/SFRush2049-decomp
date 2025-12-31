/**
 * @file os_pi.c
 * @brief Peripheral Interface (PI) / ROM access functions
 *
 * Decompiled from asm/us/E7C0.s, E8D0.s, E9A0.s
 * Contains functions for PI DMA and direct ROM access.
 *
 * The PI handles all cartridge ROM access on the N64.
 */

#include "types.h"
#include "PR/os_pi.h"

/* Hardware register addresses */
#define PI_DRAM_ADDR_REG    (*(vu32 *)0xA4600000)
#define PI_CART_ADDR_REG    (*(vu32 *)0xA4600004)
#define PI_RD_LEN_REG       (*(vu32 *)0xA4600008)
#define PI_WR_LEN_REG       (*(vu32 *)0xA460000C)
#define PI_STATUS_REG       (*(vu32 *)0xA4600010)

/* PI Bus timing registers - Domain 1 (cartridge ROM) */
#define PI_BSD_DOM1_LAT_REG (*(vu32 *)0xA4600014)
#define PI_BSD_DOM1_PWD_REG (*(vu32 *)0xA4600018)
#define PI_BSD_DOM1_PGS_REG (*(vu32 *)0xA460001C)
#define PI_BSD_DOM1_RLS_REG (*(vu32 *)0xA4600020)

/* PI Bus timing registers - Domain 2 (SRAM, flash) */
#define PI_BSD_DOM2_LAT_REG (*(vu32 *)0xA4600024)
#define PI_BSD_DOM2_LWD_REG (*(vu32 *)0xA4600028)
#define PI_BSD_DOM2_PGS_REG (*(vu32 *)0xA460002C)
#define PI_BSD_DOM2_RLS_REG (*(vu32 *)0xA4600030)

/* PI Status bits */
#define PI_STATUS_DMA_BUSY  0x01
#define PI_STATUS_IO_BUSY   0x02
#define PI_STATUS_ERROR     0x04

/* External data (standard libultra names) */
extern u32 osRomBase;               /* ROM base address: 0x80000308 */
extern s32 __osPiInitialized;       /* PI initialized flag: 0x8002C4A0 */
extern OSMesgQueue __osPiMesgQueue; /* PI message queue: 0x80037C78 */
extern OSMesg __osPiMesg;           /* PI message buffer: 0x80037C70 */

/* External functions */
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
extern void osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
extern u32 osVirtualToPhysical(void *addr);
extern s32 __osPiDeviceBusy(void);

/**
 * Initialize PI for DMA operations
 * (0x8000DBC0 - osPiInit / __osPiInit)
 *
 * Sets up the PI message queue for async operations.
 */
void osPiInit(void) {
    __osPiInitialized = 1;

    /* Create message queue with 1 slot */
    osCreateMesgQueue(&__osPiMesgQueue, &__osPiMesg, 1);

    /* Send initial message to indicate ready */
    osJamMesg(&__osPiMesgQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Get PI access (wait for previous operation)
 * (0x8000DC10 - osPiGetAccess)
 *
 * Initializes PI if needed, then waits for it to be free.
 */
void osPiGetAccess(void) {
    OSMesg msg;

    if (__osPiInitialized == 0) {
        osPiInit();
    }

    /* Wait for message indicating PI is free */
    osRecvMesg(&__osPiMesgQueue, &msg, OS_MESG_BLOCK);
}

/**
 * Release PI access
 * (0x8000DC54 - osPiReleaseAccess)
 *
 * Signals that PI is now available for other operations.
 */
void osPiReleaseAccess(void) {
    osJamMesg(&__osPiMesgQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Read 32-bit word from ROM
 * (0x8000DC80 - osPiReadWord)
 *
 * Performs a direct (non-DMA) read from cartridge ROM.
 * Waits for PI to be idle before reading.
 *
 * @param offset Offset from ROM base
 * @param dest Pointer to store result
 * @return 0 on success
 */
s32 osPiReadWord(u32 offset, u32 *dest) {
    u32 status;
    u32 addr;

    /* Wait for PI to be idle */
    status = PI_STATUS_REG;
    if (status & 0x03) {
        do {
            status = PI_STATUS_REG;
        } while (status & 0x03);
    }

    /* Calculate physical ROM address */
    addr = osRomBase | offset;
    addr |= 0xA0000000;  /* Uncached KSEG1 */

    /* Read word */
    *dest = *(vu32 *)addr;

    return 0;
}

/**
 * Write 32-bit word to ROM/cartridge
 * (0x8000D7F0 - osPiWriteWord)
 *
 * Performs a direct (non-DMA) write to cartridge space.
 * Waits for PI to be idle before writing.
 *
 * @param offset Offset from ROM base
 * @param value Value to write
 * @return 0 on success, -1 if PI is busy
 */
s32 osPiWriteWord(u32 offset, u32 value) {
    s32 status;
    u32 addr;

    /* Check and wait for PI to be idle */
    status = __osPiDeviceBusy();
    if (status != 0) {
        return -1;
    }

    /* Calculate physical ROM address in uncached KSEG1 */
    addr = offset | 0xA0000000;

    /* Write word */
    *(vu32 *)addr = value;

    return 0;
}

/**
 * Read 32-bit word from physical address (I/O space)
 * (0x8000D7A0 - osPiReadIo)
 *
 * Performs a direct read from any physical address in KSEG1 (uncached).
 * Waits for PI to be idle before reading.
 *
 * @param physAddr Physical address to read from
 * @param dest Pointer to store result
 * @return 0 on success, -1 if PI is busy
 */
s32 osPiReadIo(u32 physAddr, u32 *dest) {
    s32 status;
    u32 addr;

    /* Check and wait for PI to be idle */
    status = __osPiDeviceBusy();
    if (status != 0) {
        return -1;
    }

    /* Access address via uncached KSEG1 */
    addr = physAddr | 0xA0000000;

    /* Read word and store to destination */
    *dest = *(vu32 *)addr;

    return 0;
}

/**
 * Synchronized read 32-bit word from ROM
 * (0x800081D0 - osPiRawReadWord)
 *
 * Performs a synchronized ROM read with proper PI access control.
 * Gets PI access, reads word, then releases access.
 *
 * @param offset Offset from ROM base
 * @param dest Pointer to store result
 * @return Result from osPiReadWord (0 on success)
 */
s32 osPiRawReadWord(u32 offset, u32 *dest) {
    s32 result;

    osPiGetAccess();
    result = osPiReadWord(offset, dest);
    osPiReleaseAccess();

    return result;
}

/**
 * Start PI DMA transfer (low-level)
 * (0x8000DCD0 - __osPiRawStartDma)
 *
 * Initiates a DMA transfer between DRAM and cartridge ROM.
 * This is the low-level version without message queue handling.
 *
 * @param direction OS_READ (ROM to DRAM) or OS_WRITE (DRAM to ROM)
 * @param romOffset Offset from ROM base
 * @param dramAddr DRAM address (virtual)
 * @param size Transfer size in bytes
 * @return 0 on success, -1 on invalid direction
 */
s32 __osPiRawStartDma(s32 direction, u32 romOffset, void *dramAddr, u32 size) {
    u32 status;
    u32 physDram;
    u32 cartAddr;

    /* Wait for PI to be idle */
    status = PI_STATUS_REG;
    if (status & 0x03) {
        do {
            status = PI_STATUS_REG;
        } while (status & 0x03);
    }

    /* Convert DRAM address to physical */
    physDram = osVirtualToPhysical(dramAddr);

    /* Set DRAM address register */
    PI_DRAM_ADDR_REG = physDram;

    /* Calculate cartridge address (mask to 29 bits for physical) */
    cartAddr = (osRomBase | romOffset) & 0x1FFFFFFF;

    /* Set cart address register */
    PI_CART_ADDR_REG = cartAddr;

    /* Start DMA based on direction */
    if (direction == OS_READ) {
        /* ROM to DRAM (write to DRAM) */
        PI_WR_LEN_REG = size - 1;
        return 0;
    } else if (direction == OS_WRITE) {
        /* DRAM to ROM (read from DRAM) */
        PI_RD_LEN_REG = size - 1;
        return 0;
    }

    return -1;
}

/**
 * Set PI device timing parameters
 * (0x8000DDA0 - osPiSetDeviceTiming / __osPiDevConfig)
 *
 * Configures the PI bus timing registers for a device.
 * Only updates registers that have changed from current config.
 *
 * @param handle PI device handle with timing info
 * @return 0 on success
 */
s32 osPiSetDeviceTiming(OSPiHandle *handle) {
    /* Wait for PI to be idle */
    while (PI_STATUS_REG & 0x03) {
        /* Spin */
    }

    /* Update timing registers based on domain */
    if (handle->domain == PI_DOMAIN1) {
        /* Domain 1 - Cartridge ROM */
        PI_BSD_DOM1_LAT_REG = handle->latency;
        PI_BSD_DOM1_PGS_REG = handle->pageSize;
        PI_BSD_DOM1_RLS_REG = handle->relDuration;
        PI_BSD_DOM1_PWD_REG = handle->pulse;
    } else {
        /* Domain 2 - SRAM/Flash */
        PI_BSD_DOM2_LAT_REG = handle->latency;
        PI_BSD_DOM2_PGS_REG = handle->pageSize;
        PI_BSD_DOM2_RLS_REG = handle->relDuration;
        PI_BSD_DOM2_LWD_REG = handle->pulse;
    }

    return 0;
}
