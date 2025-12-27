/**
 * @file os_pfs_free.c
 * @brief Controller Pak free blocks calculation
 *
 * Decompiled from asm/us/BE40.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);                         /* func_8000F2D0 */
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode,     /* func_8000F3A4 */
                          u8 flag, u8 bank);

/**
 * Get number of free blocks on Controller Pak
 * (func_8000B240 - osPfsFreeBlocks)
 *
 * Counts the number of free (unused) blocks in the filesystem.
 * Free pages are marked with ipage value of 3.
 *
 * @param pfs PFS handle structure
 * @param freeBytes Output: number of free bytes
 * @return 0 on success, PFS_ERR_INVALID if not initialized
 */
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes) {
    __OSInode inode;
    s32 freeCount = 0;
    u8 bank;
    s32 ret;
    s32 j;
    s32 offset;

    /* Check if initialized */
    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    /* Check PFS status / ID */
    ret = __osCheckId(pfs);
    if (ret != 0) {
        return ret;
    }

    /* Iterate through each bank */
    for (bank = 0; bank < pfs->banks; bank++) {
        /* Read inode table for this bank */
        ret = __osPfsRWInode(pfs, &inode, PFS_READ, bank);
        if (ret != 0) {
            return ret;
        }

        /* Get starting page for this bank */
        offset = (bank > 0) ? 1 : pfs->inode_start_page;

        /* Count free pages (value == 3 means free) */
        for (j = offset; j < 128; j++) {
            if (inode.inode_page[j].ipage == 3) {
                freeCount++;
            }
        }
    }

    /* Convert pages to bytes: pages * 8 blocks * 32 bytes = pages * 256 */
    *freeBytes = freeCount * PFS_ONE_PAGE * PFS_BLOCKSIZE;

    return 0;
}
