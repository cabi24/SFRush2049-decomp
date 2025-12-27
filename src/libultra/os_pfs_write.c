/**
 * @file os_pfs_write.c
 * @brief Controller Pak page write function
 *
 * Decompiled from asm/us/F450.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data, u8 flag);

/**
 * Write pages to Controller Pak
 * (func_8000E850 - __osPfsWrite)
 *
 * Writes a fill pattern to Controller Pak, typically used for
 * clearing or initializing memory pages.
 *
 * @param pfs PFS handle structure
 * @param fillByte Byte value to fill buffer with
 * @return 0 on success, error code on failure
 */
s32 __osPfsWrite(OSPfs *pfs, u8 fillByte) {
    u8 buffer[PFS_PAGE_SIZE];  /* 32-byte write buffer */
    s32 result;
    s32 i;

    /* Fill buffer with fill byte */
    for (i = 0; i < PFS_PAGE_SIZE; i++) {
        buffer[i] = fillByte;
    }

    /* Write to controller pak (bank select address) */
    result = __osContRamWrite(
        pfs->queue,
        pfs->channel,
        0x8000 >> 5,           /* Bank select register address */
        buffer,
        0                      /* flags */
    );

    if (result == 0) {
        /* Store fill byte as active bank marker */
        pfs->activebank = fillByte;
    }

    return result;
}
