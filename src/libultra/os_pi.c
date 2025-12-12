/**
 * @file os_pi.c
 * @brief Peripheral Interface (PI) / ROM access functions
 *
 * Decompiled from asm/us/E7C0.s, E8D0.s, E9A0.s
 * Contains functions for PI DMA and direct ROM access
 */

#include "types.h"

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

/* External data */
extern u32 osRomBase;           /* 0x80000308 - ROM base address */
extern s32 D_8002C4A0;          /* PI initialized flag */
extern void *D_80037C70;        /* PI message queue */
extern void *D_80037C78;        /* PI event queue */

/* External functions */
extern void osCreateMesgQueue(void *mq, void *msgBuf, s32 count);
extern void osJamMesg(void *mq, s32 msg, s32 arg);  /* Insert message at front */
extern s32 osRecvMesg(void *mq, s32 *msg, s32 flags);  /* Receive message */
extern u32 osVirtualToPhysical(void *addr);  /* Convert virtual to physical */
extern s32 __osPiDeviceBusy(void);  /* Wait for PI idle */

/**
 * Initialize PI for DMA operations
 * (func_8000DBC0 - osPiInit / __osPiInit)
 *
 * Sets up the PI message queue for async operations.
 */
void osPiInit(void) {
    D_8002C4A0 = 1;

    /* Create message queue with 1 slot */
    osCreateMesgQueue(&D_80037C78, &D_80037C70, 1);

    /* Send initial message to indicate ready */
    osJamMesg(&D_80037C78, 0, 0);
}

/**
 * Get PI access (wait for previous operation)
 * (func_8000DC10 - osPiGetAccess)
 *
 * Initializes PI if needed, then waits for it to be free.
 */
void osPiGetAccess(void) {
    s32 msg;

    if (D_8002C4A0 == 0) {
        osPiInit();
    }

    /* Wait for message indicating PI is free */
    osRecvMesg(&D_80037C78, &msg, 1);
}

/**
 * Release PI access
 * (func_8000DC54 - osPiReleaseAccess)
 *
 * Signals that PI is now available for other operations.
 */
void osPiReleaseAccess(void) {
    osJamMesg(&D_80037C78, 0, 0);
}

/**
 * Read 32-bit word from ROM
 * (func_8000DC80 - osPiReadWord)
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
 * (func_8000D7F0 - osPiWriteWord)
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
 * (func_8000D7A0 - osPiReadIo)
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
 * (func_800081D0 - osPiRawReadWord)
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
 * Start PI DMA transfer
 * (func_8000DCD0 - osPiStartDma)
 *
 * Initiates a DMA transfer between DRAM and cartridge ROM.
 *
 * @param direction 0 = ROM to DRAM (write), 1 = DRAM to ROM (read)
 * @param romOffset Offset from ROM base
 * @param dramAddr DRAM address (virtual)
 * @param size Transfer size in bytes
 * @return 0 on success, -1 on invalid direction
 */
s32 osPiStartDma(s32 direction, u32 romOffset, void *dramAddr, u32 size) {
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
    if (direction == 0) {
        /* ROM to DRAM (write to DRAM) */
        PI_WR_LEN_REG = size - 1;
        return 0;
    } else if (direction == 1) {
        /* DRAM to ROM (read from DRAM) */
        PI_RD_LEN_REG = size - 1;
        return 0;
    }

    return -1;
}

/**
 * PI Device info structure
 * Contains timing parameters for PI bus access
 */
typedef struct {
    u8 domain;      /* 0x04: Domain (0 = DOM1, 1 = DOM2) */
    u8 latency;     /* 0x05: Latency */
    u8 pageSize;    /* 0x06: Page size */
    u8 release;     /* 0x07: Release duration */
    u8 pulse;       /* 0x08: Pulse width */
    u8 flags;       /* 0x09: Device flags */
} OSPiDeviceInfo;

/* Array of current device configs indexed by domain */
extern OSPiDeviceInfo *D_8002C3A0[2];

/**
 * Set PI device timing parameters
 * (func_8000DDA0 - osPiSetDeviceTiming / __osPiDevConfig)
 *
 * Configures the PI bus timing registers for a device.
 * Only updates registers that have changed from current config.
 *
 * @param devInfo Device timing info structure
 * @param startAddr DMA start address (unused in this impl)
 * @param callback DMA completion callback (unused in this impl)
 * @return 0 on success
 */
s32 osPiSetDeviceTiming(OSPiDeviceInfo *devInfo, u32 startAddr, void *callback) {
    u8 domain;
    OSPiDeviceInfo *currDev;

    /* Wait for PI to be idle */
    while (PI_STATUS_REG & 0x03) {
        /* Spin */
    }

    /* Get domain from device info */
    domain = devInfo->flags;
    currDev = D_8002C3A0[domain];

    /* Check if device config has changed */
    if (devInfo->domain == currDev->domain) {
        /* Same device, skip update */
        /* But still call callback if provided */
        if (callback != NULL) {
            /* Pass through to callback handler */
        }
        return 0;
    }

    /* Update timing registers based on domain */
    if (domain == 0) {
        /* Domain 1 - Cartridge ROM */
        if (devInfo->latency != currDev->latency) {
            PI_BSD_DOM1_LAT_REG = devInfo->latency;
        }
        if (devInfo->pageSize != currDev->pageSize) {
            PI_BSD_DOM1_PGS_REG = devInfo->pageSize;
        }
        if (devInfo->release != currDev->release) {
            PI_BSD_DOM1_RLS_REG = devInfo->release;
        }
        if (devInfo->pulse != currDev->pulse) {
            PI_BSD_DOM1_PWD_REG = devInfo->pulse;
        }
    } else {
        /* Domain 2 - SRAM/Flash */
        if (devInfo->latency != currDev->latency) {
            PI_BSD_DOM2_LAT_REG = devInfo->latency;
        }
        if (devInfo->pageSize != currDev->pageSize) {
            PI_BSD_DOM2_PGS_REG = devInfo->pageSize;
        }
        if (devInfo->release != currDev->release) {
            PI_BSD_DOM2_RLS_REG = devInfo->release;
        }
        if (devInfo->pulse != currDev->pulse) {
            PI_BSD_DOM2_LWD_REG = devInfo->pulse;
        }
    }

    /* Copy new config to current device state */
    currDev->domain = devInfo->domain;
    currDev->latency = devInfo->latency;
    currDev->pageSize = devInfo->pageSize;
    currDev->release = devInfo->release;
    currDev->pulse = devInfo->pulse;

    return 0;
}
