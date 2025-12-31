/**
 * @file os_motor.c
 * @brief N64 Rumble Pak (Motor) functions
 *
 * Decompiled from asm/us/AB20.s
 * Contains functions for controlling the N64 Rumble Pak motor.
 *
 * The Rumble Pak is accessed through the SI (Serial Interface) using
 * PIF commands at address 0xC000 in the controller pak space.
 * It uses the same OSPfs handle as the Controller Pak.
 */

#include "types.h"
#include "PR/os_pfs.h"

/* Motor command addresses and values */
#define MOTOR_ADDR              0xC000
#define MOTOR_ON_VALUE          0x01
#define MOTOR_OFF_VALUE         0x00
#define MOTOR_PROBE_VALUE       0xFE

/* External functions */
extern s32 osRecvMesg(OSMesgQueue *queue, OSMesg *msg, s32 flags);
extern void __osSiRawStartDma(s32 direction, void *dramAddr, u32 size, s32 channel);
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data);
extern s32 __osContAddressCrc(u16 addr);
extern s32 __osCheckId(OSPfs *pfs);

/* PIF buffer for motor commands (64 bytes per controller, 4 controllers) */
extern u8 __osMotorPifBuf[256];  /* 0x80037B30 */

/* Global motor probe byte */
extern s8 __osMotorProbe;  /* 0x80037AE0 */

/**
 * Initialize rumble pak motor
 * (0x80009F20 - osMotorInit)
 *
 * Probes the controller pak to detect if a Rumble Pak is connected.
 * Sends probe commands via SI and checks the response.
 *
 * @param pfs PFS handle (must be initialized for the controller)
 * @param value Motor initialization value (0 for off, non-zero for on check)
 * @return 0 on success, PFS_ERR_CONTRFAIL on error, PFS_ERR_INVALID if not initialized
 */
s32 osMotorInit(OSPfs *pfs, s32 value) {
    u8 *pifBuf;
    s32 channel;
    s32 status;
    s32 i;

    /* Check if motor pak flag is set */
    if (!(pfs->status & PFS_MOTOR_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    channel = pfs->channel;
    pifBuf = &__osMotorPifBuf[channel << 6];

    /* Acquire SI access */
    __osSiGetAccess();

    /* Set motor flag in PIF buffer */
    pifBuf[0x3C] = 1;

    /* Fill data bytes with the probe value */
    for (i = 0; i < 32; i += 4) {
        pifBuf[channel + 7 + i] = (s8)value;
        pifBuf[channel + 8 + i] = (s8)value;
        pifBuf[channel + 9 + i] = (s8)value;
        pifBuf[channel + 10 + i] = (s8)value;
    }

    /* Set probe byte */
    __osMotorProbe = MOTOR_PROBE_VALUE;

    /* Write command to PIF */
    __osSiRawStartDma(1, &__osMotorPifBuf[channel << 6], 0x40, channel);
    osRecvMesg(pfs->queue, NULL, 1);

    /* Read response from PIF */
    __osSiRawStartDma(0, &__osMotorPifBuf[channel << 6], 0x40, channel);
    osRecvMesg(pfs->queue, NULL, 1);

    /* Check status byte for errors (0xC0 mask indicates error) */
    status = pifBuf[channel + 2] & 0xC0;

    if (status == 0) {
        /* No error - check response value */
        if (value == 0) {
            /* For off command, response should be 0 */
            if (pifBuf[channel + 0x26] != 0) {
                status = PFS_ERR_CONTRFAIL;
            }
        } else {
            /* For on command, response should be 0xEB */
            if (pifBuf[channel + 0x26] != 0xEB) {
                status = PFS_ERR_CONTRFAIL;
            }
        }
    }

    /* Release SI access */
    __osSiRelAccess();

    return status;
}

/**
 * Build rumble pak PIF command
 * (0x8000A088 - __osMotorAccess)
 *
 * Builds a PIF command buffer for rumble pak communication.
 * The command writes to controller pak address 0xC000 with CRC.
 *
 * @param channel Controller channel (0-3)
 * @param buffer Output buffer for PIF command
 * @return Length of command buffer
 */
s32 __osMotorAccess(s32 channel, u8 *buffer) {
    u8 cmdBuf[0x30];
    s32 len;
    s32 i;
    u16 addrCrc;

    /* Build command header */
    cmdBuf[0] = 0xFF;       /* Padding/skip */
    cmdBuf[1] = 0x23;       /* Command: write controller pak */
    cmdBuf[2] = 0x01;       /* One byte to write */
    cmdBuf[3] = 0x03;       /* Expected response bytes */
    cmdBuf[4] = 0xC0;       /* High byte of address (0xC000) */

    /* Calculate address CRC for 0xC000 */
    addrCrc = __osContAddressCrc(0x600);
    cmdBuf[5] = (u8)(addrCrc | 0xC0);  /* Address low with CRC */

    /* Zero-fill the buffer up to channel offset */
    len = 0;
    if (channel != 0) {
        if (channel > 0) {
            if (channel & 3) {
                do {
                    len++;
                    *buffer++ = 0;
                } while ((channel & 3) != len);
                if (len != channel) {
                    goto fill_loop;
                }
                len += 4;
            } else {
                do {
                fill_loop:
                    len += 4;
                    buffer[1] = 0;
                    buffer[2] = 0;
                    buffer[3] = 0;
                    buffer += 4;
                    buffer[-4] = 0;
                } while (len != channel);
            }
        }
    }

    /* Copy command buffer to output */
    for (i = 0; i < 0x24; i += 0xC) {
        *(u32 *)(buffer + i) = *(u32 *)(cmdBuf + i);
        *(u32 *)(buffer + i + 4) = *(u32 *)(cmdBuf + i + 4);
        *(u32 *)(buffer + i + 8) = *(u32 *)(cmdBuf + i + 8);
    }
    buffer[0x24] = cmdBuf[0x24];
    buffer[0x25] = cmdBuf[0x25];
    buffer[0x26] = cmdBuf[0x26];

    /* Set terminator */
    buffer[0x27] = 0xFE;

    return len;
}

/**
 * Start rumble pak motor
 * (0x8000A194 - osMotorStart)
 *
 * Initializes the motor pak and turns on the motor.
 * First probes to ensure a Rumble Pak is connected.
 *
 * @param mq Message queue for SI operations
 * @param pfs PFS handle to initialize
 * @param channel Controller channel (0-3)
 * @return 0 on success, error code on failure
 */
s32 osMotorStart(OSMesgQueue *mq, OSPfs *pfs, s32 channel) {
    u8 data[PFS_PAGE_SIZE];
    s32 ret;

    pfs->queue = mq;
    pfs->status = 0;
    pfs->channel = channel;

    /* Probe with 0xFE (detect mode) */
    ret = __osPfsSelectBank(pfs, MOTOR_PROBE_VALUE);
    if (ret == PFS_ERR_NEW_PACK) {
        /* Retry with 0x80 */
        ret = __osPfsSelectBank(pfs, 0x80);
    }

    if (ret != 0) {
        return ret;
    }

    /* Read from controller pak address 0x400 to verify pak type */
    ret = __osContRamRead(mq, channel, 0x400, data);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    /* Check for Rumble Pak signature (0xFE at offset 0x1F means NOT rumble pak) */
    if (data[0x1F] == 0xFE) {
        return PFS_ERR_DEVICE;  /* Not a Rumble Pak (likely memory pak) */
    }

    /* Select bank 0x80 (motor control bank) */
    ret = __osPfsSelectBank(pfs, 0x80);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    /* Read again to verify motor control */
    ret = __osContRamRead(mq, channel, 0x400, data);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    /* Check for motor pak response (0x80 at offset 0x1F) */
    if (data[0x1F] != 0x80) {
        return PFS_ERR_DEVICE;  /* Motor pak not responding correctly */
    }

    /* Initialize PIF buffer if not already done */
    if (!(pfs->status & PFS_MOTOR_INITIALIZED)) {
        __osMotorAccess(channel, &__osMotorPifBuf[channel << 6]);
    }

    pfs->status = PFS_MOTOR_INITIALIZED;
    return 0;
}

/**
 * Stop rumble pak motor
 * (0x8000A2F0 - osMotorStop)
 *
 * Turns off the Rumble Pak motor.
 *
 * @param pfs PFS handle
 * @return 0 on success, PFS_ERR_INVALID if motor not initialized
 */
s32 osMotorStop(OSPfs *pfs) {
    s32 ret = PFS_ERR_INVALID;

    if (pfs->status & PFS_MOTOR_INITIALIZED) {
        ret = __osCheckId(pfs);
        if (ret == 0) {
            /* Motor stopped successfully */
            pfs->status &= ~PFS_MOTOR_INITIALIZED;
        }
    }

    return ret;
}
