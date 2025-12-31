/**
 * @file os_pfs_find.c
 * @brief Controller Pak file find function
 *
 * Decompiled from asm/us/AF50.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);                         /* 0x8000F2D0 */
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel,    /* 0x8000E8D0 */
                           u16 addr, u8 *data);
extern s32 __osPfsGetStatus(OSMesgQueue *mq, s32 channel);  /* 0x8000E620 */

/**
 * Find a file on Controller Pak
 * (0x8000A350 - osPfsFindFile)
 *
 * Searches the Controller Pak for a file matching the given criteria.
 *
 * @param pfs PFS handle
 * @param companyCode Company code to match (0 for any)
 * @param gameCode Game code to match (0 for any)
 * @param gameName Game name to match (NULL for any)
 * @param extName Extension name to match (NULL for any)
 * @param fileNo Output: file number if found, -1 if not found
 * @return 0 on success, PFS_ERR_INVALID if not found/invalid
 */
s32 osPfsFindFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo) {
    __OSDir dir;
    s32 j;
    s32 i;
    s32 ret;
    s32 err;

    /* Check if initialized */
    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    /* Check PFS status / ID */
    ret = __osCheckId(pfs);
    if (ret != 0) {
        return ret;
    }

    /* Search through all directory entries */
    for (j = 0; j < pfs->dir_size; j++) {
        /* Read directory entry */
        ret = __osContRamRead(pfs->queue, pfs->channel,
                              (u16)(pfs->dir_table + j), (u8 *)&dir);
        if (ret != 0) {
            return ret;
        }

        /* Get status / release access */
        ret = __osPfsGetStatus(pfs->queue, pfs->channel);
        if (ret != 0) {
            return ret;
        }

        /* Check company code and game code */
        if (dir.company_code != companyCode) continue;
        if (dir.game_code != gameCode) continue;

        /* Check game name if provided */
        err = 0;
        if (gameName != NULL) {
            for (i = 0; i < PFS_FILE_NAME_LEN; i++) {
                if (dir.game_name[i] != gameName[i]) {
                    err = 1;
                    break;
                }
            }
        }

        /* Check extension name if provided */
        if (extName != NULL && err == 0) {
            for (i = 0; i < PFS_FILE_EXT_LEN; i++) {
                if (dir.ext_name[i] != extName[i]) {
                    err = 1;
                    break;
                }
            }
        }

        /* Found match */
        if (err == 0) {
            *fileNo = j;
            return 0;
        }
    }

    /* Not found */
    *fileNo = -1;
    return PFS_ERR_INVALID;
}
