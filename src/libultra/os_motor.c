/**
 * @file os_motor.c
 * @brief N64 Rumble Pak (Motor) functions
 *
 * Decompiled from asm/us/AB20.s
 * Contains functions for controlling the N64 Rumble Pak motor.
 *
 * The Rumble Pak is accessed through the SI (Serial Interface) using
 * PIF commands at address 0xC000 in the controller pak space.
 */

#include "types.h"

/* Motor pak state flags */
#define MOTOR_PAK_INITIALIZED   0x08
#define MOTOR_PAK_RUNNING       0x04
#define MOTOR_PAK_FLAG          0x05

/* Motor command addresses */
#define MOTOR_ADDR              0xC000
#define MOTOR_ON_VALUE          0x01
#define MOTOR_OFF_VALUE         0x00
#define MOTOR_PROBE_VALUE       0xFE

/* External functions */
extern s32 osRecvMesg(void *queue, void **msg, s32 flags);
extern void __osSiRawStartDma(s32 direction, void *dramAddr, u32 size, s32 channel);
extern void __osSiGetAccess(s32 channel);
extern void __osSiRelAccess(void);
extern s32 __osPfsSelectBank(void *pfs, u8 bank);
extern s32 __osContRamRead(s32 mq, s32 channel, u16 addr, u8 *data);
extern s32 __osContAddressCrc(u16 addr, s32 channel);
extern s32 __osGetId(void);

/* PIF buffer for motor commands (64 bytes per controller, 4 controllers) */
extern u8 D_80037B30[256];

/* Global motor probe byte */
extern s8 D_80037AE0;

/**
 * Motor pak PIF structure (inside the 64-byte buffer per controller)
 */
typedef struct {
    u8 flags;           /* 0x00 */
    u8 pad1;
    u8 status;          /* 0x02 */
    u8 pad2[4];
    u8 data[32];        /* 0x07-0x26 */
    u8 response;        /* 0x26 */
    u8 pad3[21];
    u8 motor_flag;      /* 0x3C */
} OSMotorPifBuf;

/**
 * Motor pak handle structure
 */
typedef struct {
    s32 flags;          /* 0x00 - state flags (MOTOR_PAK_INITIALIZED, etc.) */
    void *mq;           /* 0x04 - message queue for SI operations */
    s32 channel;        /* 0x08 - controller channel (0-3) */
} OSMotorHandle;

/**
 * Initialize rumble pak motor
 * (func_80009F20 - osMotorInit)
 *
 * Probes the controller pak to detect if a Rumble Pak is connected.
 * Sends probe commands via SI and checks the response.
 *
 * @param handle Pointer to motor handle structure
 * @param value Motor initialization value (0 for off, non-zero for on check)
 * @return 0 on success, 4 on communication error, 5 if not initialized
 */
s32 osMotorInit(OSMotorHandle *handle, s32 value) {
    u8 *pifBuf;
    s32 channel;
    s32 status;
    s32 i;

    /* Check if motor pak flag is set */
    if (!(handle->flags & MOTOR_PAK_INITIALIZED)) {
        return 5;  /* Not initialized */
    }

    channel = handle->channel;
    pifBuf = &D_80037B30[channel << 6];

    /* Acquire SI access */
    __osSiGetAccess(channel);

    /* Set motor flag in PIF buffer */
    ((OSMotorPifBuf *)&D_80037B30[channel << 6])->motor_flag = 1;

    /* Fill data bytes with the probe value */
    for (i = 0; i < 32; i += 4) {
        pifBuf[channel + 7 + i] = (s8)value;
        pifBuf[channel + 8 + i] = (s8)value;
        pifBuf[channel + 9 + i] = (s8)value;
        pifBuf[channel + 10 + i] = (s8)value;
    }

    /* Set probe byte */
    D_80037AE0 = MOTOR_PROBE_VALUE;

    /* Write command to PIF */
    __osSiRawStartDma(1, &D_80037B30[channel << 6], 0x40, channel);
    osRecvMesg(handle->mq, NULL, 1);

    /* Read response from PIF */
    __osSiRawStartDma(0, &D_80037B30[channel << 6], 0x40, channel);
    osRecvMesg(handle->mq, NULL, 1);

    /* Check status byte for errors (0xC0 mask indicates error) */
    status = pifBuf[channel + 2] & 0xC0;

    if (status == 0) {
        /* No error - check response value */
        if (value == 0) {
            /* For off command, response should be 0 */
            if (pifBuf[channel + 0x26] != 0) {
                status = 4;  /* Error */
            }
        } else {
            /* For on command, response should be 0xEB */
            if (pifBuf[channel + 0x26] != 0xEB) {
                status = 4;  /* Error */
            }
        }
    }

    /* Release SI access */
    __osSiRelAccess();

    return status;
}

/**
 * Build rumble pak PIF command
 * (func_8000A088 - __osMotorAccess)
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
    addrCrc = __osContAddressCrc(0x600, channel);
    cmdBuf[5] = (u8)(addrCrc | 0xC000);  /* Address with CRC */

    /* Zero-fill the buffer up to channel offset */
    if (channel != 0) {
        len = 0;
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
 * (func_8000A194 - osMotorStart)
 *
 * Turns on the Rumble Pak motor by writing 0x01 to address 0xC000.
 * First probes to ensure a Rumble Pak is connected.
 *
 * @param mq Message queue for SI operations
 * @param handle Motor handle to initialize
 * @param channel Controller channel (0-3)
 * @return 0 on success, error code on failure
 */
s32 osMotorStart(s32 mq, OSMotorHandle *handle, s32 channel) {
    u8 data[0x20];
    s32 ret;

    handle->mq = (void *)mq;
    handle->flags = 0;
    handle->channel = channel;

    /* Probe with 0xFE (detect mode) */
    ret = __osPfsSelectBank(handle, MOTOR_PROBE_VALUE);
    if (ret == 2) {
        /* Retry with 0x80 */
        ret = __osPfsSelectBank(handle, 0x80);
    }

    if (ret != 0) {
        return ret;
    }

    /* Read from controller pak address 0x400 to verify pak type */
    ret = __osContRamRead(mq, channel, 0x400, data);
    if (ret == 2) {
        ret = 4;  /* Communication error */
    }

    if (ret != 0) {
        return ret;
    }

    /* Check for Rumble Pak signature (0xFE at offset 0x1F) */
    if (data[0x1F] == 0xFE) {
        return 0x0B;  /* Not a Rumble Pak (likely memory pak) */
    }

    /* Select bank 0x80 (motor control bank) */
    ret = __osPfsSelectBank(handle, 0x80);
    if (ret == 2) {
        ret = 4;
    }

    if (ret != 0) {
        return ret;
    }

    /* Read again to verify motor control */
    ret = __osContRamRead(mq, channel, 0x400, data);
    if (ret == 2) {
        ret = 4;
    }

    if (ret != 0) {
        return ret;
    }

    /* Check for motor pak response (0x80 at offset 0x1F) */
    if (data[0x1F] != 0x80) {
        return 0x0B;  /* Motor pak not responding correctly */
    }

    /* Initialize PIF buffer if not already done */
    if (!(handle->flags & MOTOR_PAK_INITIALIZED)) {
        __osMotorAccess(channel, &D_80037B30[channel << 6]);
    }

    handle->flags = MOTOR_PAK_INITIALIZED;
    return 0;
}

/**
 * Stop rumble pak motor
 * (func_8000A2F0 - osMotorStop)
 *
 * Turns off the Rumble Pak motor.
 * Clears the running flag in the handle.
 *
 * @param handle Motor handle
 * @return 0 on success, 5 if not running
 */
s32 osMotorStop(OSMotorHandle *handle) {
    s32 ret = 5;  /* Default: not running */

    if (handle->flags & MOTOR_PAK_FLAG) {
        ret = __osGetId();
        if (ret == 0) {
            /* Clear the running flag */
            handle->flags &= ~MOTOR_PAK_RUNNING;
        }
    }

    return ret;
}
