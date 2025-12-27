/**
 * @file os_pfs_check.c
 * @brief Controller Pak filesystem check functions
 *
 * Decompiled from asm/us/BFE0.s
 */

#include "types.h"

/**
 * Check filesystem consistency
 * (func_8000B3E0 - osPfsChecker)
 */
s32 osPfsChecker(void *pfs) {
    /* PFS check - stub */
    (void)pfs;
    return 0;
}

/**
 * Repair filesystem
 * (func_8000B918 - osPfsRepairId)
 */
s32 osPfsRepairId(void *pfs) {
    /* PFS repair - stub */
    (void)pfs;
    return 0;
}

/**
 * Get filesystem label
 * (func_8000BBF0 - osPfsGetLabel)
 */
s32 osPfsGetLabel(void *pfs, u8 *label) {
    /* PFS get label - stub */
    (void)pfs;
    (void)label;
    return 0;
}
