/**
 * @file os_pfs_rw.c
 * @brief Controller Pak page read/write functions
 *
 * Decompiled from asm/us/F4D0.s
 *
 * Low-level functions for reading/writing pages to Controller Pak
 * through the Serial Interface (SI) and PIF.
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External data (standard libultra names) */
extern u8 __osContLastChannel;  /* Last accessed channel: 0x8002C4C0 */
extern u8 __osPfsRequestType;   /* Controller read in progress flag: 0x80037AE0 */
extern u8 __osPfsBuffer[64];    /* PIF command buffer: 0x80037F60 */

/* External functions */
extern void __osSiGetAccess(void);                           /* 0x8000E5B0 */
extern void __osSiRelAccess(void);                           /* 0x8000E5F4 */
extern s32 __osPfsGetStatus(OSMesgQueue *mq, s32 channel);   /* 0x8000E620 */
extern u8 __osPfsAddressChecksum(u16 addr);                  /* 0x8000F9F0 */
extern s32 __osSiRawStartDma(s32 direction, u8 *buffer);     /* 0x8000E4B0 */
extern s32 __osSiSync(s32 channel, s32 pad, s32 flag);       /* 0x80007270 */
extern u8 __osPfsDataChecksum(u8 *data);                     /* 0x8000FAC0 */
extern void bcopy(void *src, void *dst, s32 len);            /* 0x8000D2B0 */

/**
 * Read a page from Controller Pak
 * (0x8000E8D0 - __osContRamRead)
 *
 * Low-level function to read a single page (32 bytes) from Controller Pak.
 * This communicates directly with the PIF to send the read command and
 * receive the response.
 *
 * @param mq Message queue for async notification
 * @param channel Controller channel (0-3)
 * @param addr Page address to read
 * @param data Output buffer (32 bytes)
 * @return 0 on success, error code on failure
 */
s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data) {
    u8 *cmdBuf;
    s32 addrHi, addrLo;
    s32 result;
    s32 retries;
    u8 status;
    s32 i;

    __osSiGetAccess();  /* Acquire SI access */

    addrHi = addr >> 3;
    addrLo = (addr & 0x07) << 5;

    retries = 2;

retry:
    /* Check if we need to reinitialize for this channel */
    if (__osPfsRequestType != PFS_CMD_READ || __osContLastChannel != channel) {
        __osPfsRequestType = PFS_CMD_READ;
        __osContLastChannel = (u8)channel;

        /* Clear command buffer up to channel offset */
        cmdBuf = __osPfsBuffer;
        for (i = 0; i < channel; i++) {
            cmdBuf[i] = 0;
        }
        cmdBuf += channel;

        /* Set up read command packet */
        *(s32 *)(cmdBuf + 0x3C) = 1;  /* Enable PIF */
        cmdBuf[0] = 0xFF;              /* Skip channel marker */
        cmdBuf[1] = 3;                 /* TX bytes (addr high, addr low, data CRC) */
        cmdBuf[2] = 0x21;              /* RX bytes (32 data bytes + 1 status) */
        cmdBuf[3] = PFS_CMD_READ;      /* Command: Read page */
        cmdBuf[0x26] = 0xFF;           /* Response data area */
        cmdBuf[0x27] = 0xFE;           /* End marker */
    } else {
        cmdBuf = __osPfsBuffer + channel;
    }

    /* Set page address with checksum */
    cmdBuf[4] = (u8)addrHi;
    cmdBuf[5] = __osPfsAddressChecksum(addr) | (u8)addrLo;

    /* Send command to PIF */
    __osSiRawStartDma(1, __osPfsBuffer);

    /* Wait for write to complete */
    __osSiSync(channel, 0, 1);

    /* Read response from PIF */
    __osSiRawStartDma(0, __osPfsBuffer);
    __osSiSync(channel, 0, 1);

    /* Check status byte */
    status = cmdBuf[2] & 0xC0;
    result = status >> 4;

    if (result == 0) {
        /* Verify data CRC */
        if (__osPfsDataChecksum(cmdBuf + 6) != cmdBuf[0x26]) {
            /* CRC error - retry */
            __osPfsGetStatus(mq, channel);
            if (result == 0) {
                result = PFS_ERR_CONTRFAIL;  /* Controller failure */
            }
        } else {
            /* Copy data to output buffer */
            bcopy(cmdBuf + 6, data, PFS_PAGE_SIZE);
        }
    }

    if (result == PFS_ERR_CONTRFAIL && retries >= 0) {
        retries--;
        goto retry;
    }

    __osSiRelAccess();  /* Release SI access */
    return result;
}

/**
 * Write a page to Controller Pak
 * (0x8000F680 - __osContRamWrite)
 *
 * Low-level function to write a single page (32 bytes) to Controller Pak.
 *
 * @param mq Message queue for async notification
 * @param channel Controller channel (0-3)
 * @param addr Page address to write
 * @param data Input buffer (32 bytes)
 * @param flag PFS flag (usually 0)
 * @return 0 on success, error code on failure
 */
s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data, u8 flag) {
    u8 *cmdBuf;
    s32 addrHi, addrLo;
    s32 result;
    s32 retries;
    u8 status;
    s32 i;

    (void)flag;  /* Flag parameter unused in this implementation */

    __osSiGetAccess();  /* Acquire SI access */

    addrHi = addr >> 3;
    addrLo = (addr & 0x07) << 5;

    retries = 2;

retry:
    /* Check if we need to reinitialize for this channel */
    if (__osPfsRequestType != PFS_CMD_WRITE || __osContLastChannel != channel) {
        __osPfsRequestType = PFS_CMD_WRITE;
        __osContLastChannel = (u8)channel;

        /* Clear command buffer up to channel offset */
        cmdBuf = __osPfsBuffer;
        for (i = 0; i < channel; i++) {
            cmdBuf[i] = 0;
        }
        cmdBuf += channel;

        /* Set up write command packet */
        *(s32 *)(cmdBuf + 0x3C) = 1;  /* Enable PIF */
        cmdBuf[0] = 0xFF;              /* Skip channel marker */
        cmdBuf[1] = 0x23;              /* TX bytes (addr + 32 data bytes) */
        cmdBuf[2] = 1;                 /* RX bytes (status only) */
        cmdBuf[3] = PFS_CMD_WRITE;     /* Command: Write page */
        cmdBuf[0x26] = 0xFF;           /* Response area */
        cmdBuf[0x27] = 0xFE;           /* End marker */
    } else {
        cmdBuf = __osPfsBuffer + channel;
    }

    /* Set page address with checksum */
    cmdBuf[4] = (u8)addrHi;
    cmdBuf[5] = __osPfsAddressChecksum(addr) | (u8)addrLo;

    /* Copy data to command buffer */
    bcopy(data, cmdBuf + 6, PFS_PAGE_SIZE);

    /* Compute and store data CRC */
    cmdBuf[0x26] = __osPfsDataChecksum(cmdBuf + 6);

    /* Send command to PIF */
    __osSiRawStartDma(1, __osPfsBuffer);

    /* Wait for write to complete */
    __osSiSync(channel, 0, 1);

    /* Read response from PIF */
    __osSiRawStartDma(0, __osPfsBuffer);
    __osSiSync(channel, 0, 1);

    /* Check status byte */
    status = cmdBuf[2] & 0xC0;
    result = status >> 4;

    if (result == PFS_ERR_CONTRFAIL && retries >= 0) {
        retries--;
        goto retry;
    }

    __osSiRelAccess();  /* Release SI access */
    return result;
}
