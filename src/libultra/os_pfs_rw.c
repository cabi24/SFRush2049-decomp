/**
 * @file os_pfs_rw.c
 * @brief Controller Pak page read/write function
 *
 * Decompiled from asm/us/F4D0.s
 */

#include "types.h"

/* External data */
extern u8 D_8002C4C0;     /* Last accessed channel */
extern u8 D_80037AE0;     /* Controller read in progress flag */
extern u8 D_80037F60[];   /* PIF command buffer */

/* External functions */
extern void func_8000E5B0(void);  /* Acquire SI access */
extern void func_8000E5F4(void);  /* Release SI access */
extern s32 func_8000E620(s32 channel, s32 bank);
extern s32 func_8000F9F0(u16 page);
extern s32 func_8000E4B0(s32 write, u8 *buffer);
extern s32 func_80007270(s32 channel, s32 pad, s32 flag);
extern s32 func_8000FAC0(u8 *buffer);
extern void func_8000D2B0(void *src, void *dst, s32 len);

/**
 * Read or write a page to Controller Pak
 * (func_8000E8D0 - __osPfsRWPage)
 *
 * Low-level function to read or write a single page.
 *
 * @param channel Controller channel
 * @param bank Memory bank
 * @param page Page number
 * @param buffer Data buffer
 * @return 0 on success, error code on failure
 */
s32 __osPfsRWPage(s32 channel, s32 bank, u16 page, void *buffer) {
    u8 *cmdBuf;
    s32 pageHi, pageLo;
    s32 result;
    s32 retries;
    u8 status;

    func_8000E5B0();  /* Acquire access */

    pageHi = page >> 3;
    pageLo = page << 5;

    retries = 2;

retry:
    /* Check if we need to reinitialize for this channel */
    if (D_80037AE0 != 2 || D_8002C4C0 != channel) {
        D_80037AE0 = 2;
        D_8002C4C0 = channel;

        /* Clear command buffer up to channel offset */
        cmdBuf = D_80037F60;
        for (s32 i = 0; i < channel; i++) {
            cmdBuf[i] = 0;
        }
        cmdBuf += channel;

        /* Set up read command */
        *(s32 *)(cmdBuf + 0x3C) = 1;
        cmdBuf[0] = 0xFF;
        cmdBuf[1] = 3;
        cmdBuf[2] = 0x21;
        cmdBuf[3] = 2;
        cmdBuf[0x26] = 0xFF;
        cmdBuf[0x27] = 0xFE;
    } else {
        cmdBuf = D_80037F60 + channel;
    }

    /* Set page address */
    cmdBuf[4] = pageHi;
    cmdBuf[5] = func_8000F9F0(page) | pageLo;

    /* Send command */
    func_8000E4B0(1, D_80037F60);

    /* Wait for response */
    func_80007270(channel, 0, 1);

    /* Read response */
    func_8000E4B0(0, D_80037F60);
    func_80007270(channel, 0, 1);

    /* Check status */
    status = cmdBuf[2] & 0xC0;
    result = status >> 4;

    if (result == 0) {
        /* Verify CRC */
        if (func_8000FAC0(cmdBuf + 6) != cmdBuf[0x26]) {
            /* CRC error - retry */
            func_8000E620(channel, bank);
            if (result == 0) {
                result = 4;  /* Controller failure */
            }
        } else {
            /* Copy data to output buffer */
            func_8000D2B0(cmdBuf + 6, buffer, 0x20);
        }
    }

    if (result == 4 && retries >= 0) {
        retries--;
        goto retry;
    }

    func_8000E5F4();  /* Release access */
    return result;
}
