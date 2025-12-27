/**
 * @file os_pfs_create.c
 * @brief Controller Pak file creation functions
 *
 * Decompiled from asm/us/B570.s
 */

#include "types.h"

/**
 * Create directory entry
 * (func_8000A970)
 */
s32 func_8000A970(void *pfs, void *entry) {
    /* Create entry - stub */
    (void)pfs;
    (void)entry;
    return 0;
}

/**
 * Allocate file
 * (func_8000ACA4 - osPfsAllocateFile)
 */
s32 osPfsAllocateFile(void *pfs, u16 companyCode, u32 gameCode,
                      u8 *gameName, u8 *extName, s32 size, s32 *fileNo) {
    /* PFS allocate file - stub */
    (void)pfs;
    (void)companyCode;
    (void)gameCode;
    (void)gameName;
    (void)extName;
    (void)size;
    (void)fileNo;
    return 0;
}
