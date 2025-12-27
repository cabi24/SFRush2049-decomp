/**
 * @file os_pfs_find.c
 * @brief Controller Pak file find function
 *
 * Decompiled from asm/us/AF50.s
 */

#include "types.h"

/* External functions */
extern s32 func_8000F2D0(void *pfs);  /* Check PFS status */
extern s32 func_8000E8D0(s32 channel, s32 bank, u16 page, void *buffer);
extern s32 func_8000E620(s32 channel, s32 bank);

/**
 * Find a file on Controller Pak
 * (func_8000A350 - osPfsFindFile)
 *
 * Searches the Controller Pak for a file matching the given criteria.
 *
 * @param pfs PFS handle
 * @param companyCode Company code to match
 * @param gameCode Game code to match
 * @param gameName Game name to match (can be NULL)
 * @param extName Extension name to match (can be NULL)
 * @param fileNo Output: file number if found
 * @return 0 on success, 5 if not found/invalid
 */
s32 osPfsFindFile(void *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo) {
    u8 *p = (u8 *)pfs;
    u8 buffer[0x28];  /* Directory entry buffer */
    s32 i;
    s32 numFiles;
    s32 result;
    s32 match;
    s32 j;

    /* Check if initialized */
    if ((*(u32 *)p & 1) == 0) {
        return 5;
    }

    /* Check PFS status */
    result = func_8000F2D0(pfs);
    if (result != 0) {
        return result;
    }

    /* Get number of files */
    numFiles = *(s32 *)(p + 0x50);

    /* Search through all file entries */
    for (i = 0; i < numFiles; i++) {
        /* Read directory entry */
        result = func_8000E8D0(
            *(s32 *)(p + 4),
            *(s32 *)(p + 8),
            *(s32 *)(p + 0x5C) + i,
            buffer
        );
        if (result != 0) {
            return result;
        }

        /* Release access */
        result = func_8000E620(*(s32 *)(p + 4), *(s32 *)(p + 8));
        if (result != 0) {
            return result;
        }

        /* Check company code and game code */
        if (companyCode != *(u16 *)(buffer + 4)) continue;
        if (gameCode != *(u32 *)buffer) continue;

        /* Check game name if provided */
        match = 0;
        if (gameName != NULL) {
            for (j = 0; j < 16; j++) {
                if (buffer[0x10 + j] != gameName[j]) {
                    match = 1;
                    break;
                }
            }
        }

        /* Check extension name if provided */
        if (extName != NULL && match == 0) {
            for (j = 0; j < 4; j++) {
                if (buffer[0x0C + j] != extName[j]) {
                    match = 1;
                    break;
                }
            }
        }

        /* Found match */
        if (match == 0) {
            *fileNo = i;
            return 0;
        }
    }

    /* Not found */
    *fileNo = -1;
    return 5;
}
