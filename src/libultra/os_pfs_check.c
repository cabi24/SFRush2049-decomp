/**
 * @file os_pfs_check.c
 * @brief Controller Pak filesystem check functions
 *
 * Decompiled from asm/us/BFE0.s
 *
 * These functions verify and repair Controller Pak filesystem integrity.
 * osPfsChecker is particularly complex (~1.3KB) and handles:
 * - Verifying inode chain consistency
 * - Detecting corrupted directory entries
 * - Rebuilding inode tables from valid chains
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);
extern s32 __osGetId(OSPfs *pfs);
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data);
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data, u8 flag);
extern void bzero(void *ptr, s32 size);

/**
 * Check filesystem consistency
 * (0x8000B3E0 - osPfsChecker)
 *
 * Verifies the Controller Pak filesystem by:
 * 1. Checking pack ID
 * 2. Validating directory entries
 * 3. Following inode chains for each file
 * 4. Detecting and clearing corrupted entries
 * 5. Rebuilding inode tables with valid chains only
 *
 * @param pfs PFS handle
 * @return 0 on success (may have fixed errors), error code on failure
 *
 * TODO: Full implementation needed (~1.3KB of assembly)
 */
s32 osPfsChecker(OSPfs *pfs) {
    s32 ret;

    /* Verify pack ID */
    ret = __osCheckId(pfs);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osGetId(pfs);
    }
    if (ret != 0) {
        return ret;
    }

    /* TODO: Full filesystem check implementation
     * - Initialize corruption cache
     * - Iterate directory entries
     * - Follow inode chains
     * - Clear corrupted entries
     * - Rebuild inode tables
     */

    return 0;
}

/**
 * Repair filesystem ID
 * (0x8000B918 - osPfsRepairId)
 *
 * Attempts to repair a damaged pack ID by rewriting it.
 *
 * @param pfs PFS handle
 * @return 0 on success, error code on failure
 *
 * TODO: Full implementation needed
 */
s32 osPfsRepairId(OSPfs *pfs) {
    s32 ret;

    /* Check if initialized */
    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    /* TODO: Implement ID repair logic */
    (void)pfs;
    return 0;
}

/**
 * Get filesystem label
 * (0x8000BBF0 - osPfsGetLabel)
 *
 * Retrieves the user-defined label from the Controller Pak.
 *
 * @param pfs PFS handle
 * @param label Output buffer for label (32 bytes)
 * @param length Output: actual label length
 * @return 0 on success, error code on failure
 */
s32 osPfsGetLabel(OSPfs *pfs, u8 *label, s32 *length) {
    s32 i;

    /* Check if initialized */
    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    /* Copy label from PFS structure */
    for (i = 0; i < 32; i++) {
        label[i] = pfs->label[i];
    }

    /* Find actual length (null-terminated or max 32) */
    if (length != NULL) {
        for (i = 0; i < 32 && label[i] != 0; i++) {
            /* count */
        }
        *length = i;
    }

    return 0;
}
