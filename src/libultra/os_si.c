/**
 * @file os_si.c
 * @brief Serial Interface (SI) / Controller functions
 *
 * Decompiled from asm/us/F0B0.s, F160.s
 * Contains functions for SI DMA and controller communication via PIF
 *
 * The SI is used for controller input and EEPROM access on the N64.
 * It communicates through the PIF (Peripheral Interface) which handles
 * the actual controller protocol.
 */

#include "types.h"
#include "PR/os_message.h"

/* Hardware register addresses */
#define SI_DRAM_ADDR_REG        (*(vu32 *)0xA4800000)
#define SI_PIF_ADDR_RD64B_REG   (*(vu32 *)0xA4800004)
#define SI_PIF_ADDR_WR64B_REG   (*(vu32 *)0xA4800010)
#define SI_STATUS_REG           (*(vu32 *)0xA4800018)

/* PIF RAM address (where controller data is exchanged) */
#define PIF_RAM_START           0x1FC007C0

/* SI Status bits */
#define SI_STATUS_DMA_BUSY      0x01
#define SI_STATUS_IO_BUSY       0x02

/* DMA direction constants */
#define OS_READ     0   /* Read from PIF to DRAM */
#define OS_WRITE    1   /* Write from DRAM to PIF */

/* External data */
extern s32 __osSiAccessInited;          /* SI initialized flag */
extern OSMesg __osSiAccessMesg;         /* SI access message */
extern OSMesgQueue __osSiAccessQueue;   /* SI access message queue */
extern u8 __osContPifRam[64];           /* PIF buffer (64 bytes) */
extern u8 __osContLastCmd;              /* Last controller command type */
extern u8 __osPifRamCmd;                /* PIF RAM command byte */

/* External functions */
extern void osWritebackDCache(void *addr, s32 size);  /* Writeback D-cache */
extern void osInvalDCache(void *addr, s32 size);  /* Invalidate D-cache */

/* Forward declarations */
void __osContBuildRequest(s32 ctrlNum, s32 cmd);
void __osContParseResponse(s32 ctrlNum, u8 *out);

/**
 * Start SI DMA transfer
 * (0x8000E4B0 - __osSiRawStartDma)
 *
 * Initiates a DMA transfer between RDRAM and PIF RAM.
 *
 * @param direction 0 = read from PIF, 1 = write to PIF
 * @param dramAddr DRAM buffer address
 * @return 0 on success, -1 if SI is busy
 */
s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    u32 status;
    u32 physAddr;

    /* Check if SI is busy */
    status = SI_STATUS_REG;
    if (status & 0x03) {
        return -1;
    }

    /* If writing to PIF, flush cache first */
    if (direction == 1) {
        osWritebackDCache(dramAddr, 64);
    }

    /* Convert to physical address and set DRAM register */
    physAddr = osVirtualToPhysical(dramAddr);
    SI_DRAM_ADDR_REG = physAddr;

    /* Start transfer */
    if (direction == 0) {
        /* Read from PIF to DRAM */
        SI_PIF_ADDR_RD64B_REG = PIF_RAM_START;
    } else {
        /* Write from DRAM to PIF */
        SI_PIF_ADDR_WR64B_REG = PIF_RAM_START;
    }

    /* If reading, invalidate cache to get fresh data */
    if (direction == 0) {
        osInvalDCache(dramAddr, 64);
    }

    return 0;
}

/**
 * Initialize SI subsystem
 * (0x8000E560 - __osSiInit / osSiInit)
 *
 * Sets up the SI message queue for async operations.
 */
void __osSiCreateAccessQueue(void) {
    __osSiAccessInited = 1;

    /* Create message queue with 1 slot */
    osCreateMesgQueue(&__osSiAccessQueue, &__osSiAccessMesg, 1);

    /* Send initial message to indicate ready */
    osJamMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Get SI access (wait for SI to be free)
 * (0x8000E5B0 - __osSiGetAccess)
 *
 * Initializes SI if needed, then waits for it to be free.
 */
void __osSiGetAccess(void) {
    OSMesg msg;

    if (__osSiAccessInited == 0) {
        __osSiCreateAccessQueue();
    }

    /* Wait for message indicating SI is free */
    osRecvMesg(&__osSiAccessQueue, &msg, OS_MESG_BLOCK);
}

/**
 * Release SI access
 * (0x8000E5F4 - __osSiRelAccess)
 *
 * Signals that SI is now available for other operations.
 */
void __osSiRelAccess(void) {
    osJamMesg(&__osSiAccessQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Read controller status
 * (0x8000E620 - osContStartReadData / osContGetReadData)
 *
 * Polls controller for button state and stick position.
 *
 * @param mq Message queue for async notification
 * @param ctrlNum Controller number (0-3)
 * @return Status code
 */
s32 osContStartReadData(OSMesgQueue *mq, s32 ctrlNum) {
    OSMesg msg;
    s32 ret;
    u8 response[4];

    /* Set controller request command */
    __osPifRamCmd = 0xFA;  /* Read controller command */

    /* Build request packet */
    __osContBuildRequest(ctrlNum, 0);

    /* Write request to PIF */
    __osSiRawStartDma(OS_WRITE, __osContPifRam);

    /* Wait for write to complete */
    osRecvMesg(mq, &msg, OS_MESG_BLOCK);

    /* Read response from PIF */
    __osSiRawStartDma(OS_READ, __osContPifRam);
    ret = (s32)(uintptr_t)msg;  /* Save status */

    /* Wait for read to complete */
    osRecvMesg(mq, &msg, OS_MESG_BLOCK);

    /* Parse response */
    __osContParseResponse(ctrlNum, response);

    /* Check response status */
    if ((response[2] & 0x01) && (response[2] & 0x02)) {
        return 2;  /* Controller not connected */
    }
    if (response[3] != 0 || (response[2] & 0x01)) {
        return 1;  /* Error */
    }
    if (response[2] & 0x04) {
        return 4;  /* Pak inserted */
    }

    return ret;
}

/**
 * Build controller request packet
 * (0x8000E6F0 - __osContBuildRequest / __osPackRequestData)
 *
 * Fills the PIF buffer with a controller request command.
 *
 * @param ctrlNum Controller number
 * @param cmd Command to send (0 = status, 1 = read buttons)
 */
void __osContBuildRequest(s32 ctrlNum, s32 cmd) {
    u8 *buf = __osContPifRam;
    u8 packet[6];
    s32 i;

    __osContLastCmd = 0xFE;  /* Terminator */
    buf[0x3C] = 1;           /* Enable */

    /* Build packet: tx_len, rx_len, cmd, padding... */
    packet[0] = 1;      /* TX length */
    packet[1] = 3;      /* RX length */
    packet[2] = cmd;    /* Command */
    packet[3] = 0xFF;   /* Padding */
    packet[4] = 0xFF;
    packet[5] = 0xFF;

    /* Clear buffer */
    i = 0;
    if (ctrlNum > 0) {
        s32 rem = ctrlNum & 3;
        if (rem != 0) {
            for (i = 0; i < rem; i++) {
                *buf++ = 0;
            }
            if (i == ctrlNum) goto copy_packet;
        }
        while (i != ctrlNum) {
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            buf[3] = 0;
            buf += 4;
            i += 4;
        }
    }

copy_packet:
    /* Copy packet to buffer */
    *(u32 *)buf = *(u32 *)packet;
    buf[4] = packet[4];
    buf[5] = packet[5];
    buf[6] = 0xFE;  /* End marker */
}

/**
 * Parse controller response
 * (0x8000E7B4 - __osContParseResponse / __osContGetInitData)
 *
 * Extracts button and stick data from PIF response.
 *
 * @param ctrlNum Controller number
 * @param out Output buffer for response data
 */
void __osContParseResponse(s32 ctrlNum, u8 *out) {
    u8 *buf = __osContPifRam;
    u8 data[6];
    s32 i;

    /* Skip to controller's response slot */
    i = 0;
    if (ctrlNum > 0) {
        s32 rem = ctrlNum & 3;
        if (rem != 0) {
            for (i = 0; i < rem; i++) {
                buf++;
            }
            if (i == ctrlNum) goto parse;
        }
        while (i != ctrlNum) {
            buf += 4;
            i += 4;
        }
    }

parse:
    /* Copy response data */
    *(u32 *)data = *(u32 *)buf;
    data[4] = buf[4];
    data[5] = buf[5];

    /* Extract status */
    out[3] = (data[1] & 0xC0) >> 4;

    /* If no error, extract button/stick data */
    if (out[3] == 0) {
        /* Buttons: data[4] << 8 | data[3] */
        out[0] = data[4];
        out[1] = data[3];
        /* Stick: data[5] */
        out[2] = data[5];
    }
}
