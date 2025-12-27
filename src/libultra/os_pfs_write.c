/**
 * @file os_pfs_write.c
 * @brief Controller Pak page write function
 *
 * Decompiled from asm/us/F450.s
 */

#include "types.h"

/* External functions */
extern s32 func_8000F680(s32 channel, s32 bank, u32 size, u8 *buffer, s32 flags);

/**
 * Write pages to Controller Pak
 * (func_8000E850 - __osPfsWrite)
 *
 * Writes data to Controller Pak, filling unused space with a fill byte.
 *
 * @param pfs PFS handle structure
 * @param fillByte Byte value to fill buffer with
 * @return 0 on success, error code on failure
 */
s32 __osPfsWrite(void *pfs, u8 fillByte) {
    u8 buffer[0x20];  /* 32-byte write buffer */
    u8 *p = (u8 *)pfs;
    s32 result;
    s32 i;

    /* Fill buffer with fill byte */
    for (i = 0; i < 0x20; i++) {
        buffer[i] = fillByte;
    }

    /* Write to controller pak */
    result = func_8000F680(
        *(s32 *)(p + 4),   /* channel */
        *(s32 *)(p + 8),   /* bank */
        0x400,             /* size (1KB) */
        buffer,
        0                  /* flags */
    );

    if (result == 0) {
        /* Store fill byte in PFS structure */
        *(u8 *)(p + 0x65) = fillByte;
    }

    return result;
}
