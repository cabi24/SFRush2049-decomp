/**
 * @file os_pfs_free.c
 * @brief Controller Pak free blocks calculation
 *
 * Decompiled from asm/us/BE40.s
 */

#include "types.h"

/* External functions */
extern s32 func_8000F2D0(void *pfs);  /* Check PFS status */
extern s32 func_8000F3A4(void *pfs, u16 *buffer, s32 start, u8 bank);

/**
 * Get number of free blocks on Controller Pak
 * (func_8000B240 - osPfsFreeBlocks)
 *
 * Counts the number of free (unused) blocks in the filesystem.
 *
 * @param pfs PFS handle structure
 * @param freeBlocks Output: number of free blocks (multiplied by 256)
 * @return 0 on success, 5 if not initialized, error code on failure
 */
s32 osPfsFreeBlocks(void *pfs, s32 *freeBlocks) {
    u8 *p = (u8 *)pfs;
    u16 buffer[0x80];  /* 256-byte page buffer */
    s32 freeCount = 0;
    u8 numBanks;
    u8 bank;
    s32 result;
    s32 i;
    s32 start;

    /* Check if initialized */
    if ((*(u32 *)p & 1) == 0) {
        return 5;  /* PFS_ERR_INVALID */
    }

    /* Check PFS status */
    result = func_8000F2D0(pfs);
    if (result != 0) {
        return result;
    }

    /* Get number of banks */
    numBanks = *(u8 *)(p + 0x64);

    /* Iterate through each bank */
    for (bank = 0; bank < numBanks; bank++) {
        /* Read page table for this bank */
        result = func_8000F3A4(pfs, buffer, 0, bank);
        if (result != 0) {
            return result;
        }

        /* Get starting page for this bank */
        if (bank > 0) {
            start = 1;
        } else {
            start = *(s32 *)(p + 0x60);  /* freePages offset */
        }

        /* Count free pages (value == 3 means free) */
        for (i = start; i < 0x80; i++) {
            if (buffer[i] == 3) {
                freeCount++;
            }
        }
    }

    /* Return free block count (shifted left by 8) */
    *freeBlocks = freeCount << 8;

    return 0;
}
