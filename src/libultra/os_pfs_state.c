/**
 * @file os_pfs_state.c
 * @brief Controller Pak file state functions
 *
 * Decompiled from asm/us/B120.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);                         /* 0x8000F2D0 */
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);          /* 0x8000E850 */
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel,    /* 0x8000E8D0 */
                           u16 addr, u8 *data);
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode,     /* 0x8000F3A4 */
                          u8 flag, u8 bank);
extern s32 __osPfsGetStatus(OSMesgQueue *mq, s32 channel);  /* 0x8000E620 */
extern void bcopy(void *src, void *dst, s32 len);           /* 0x8000D2B0 */

/**
 * Get file state/info from Controller Pak
 * (0x8000A520 - osPfsFileState)
 *
 * Retrieves information about a file on the Controller Pak including
 * file size, company code, game code, and file names.
 *
 * @param pfs PFS handle
 * @param fileNo File number (0 to dir_size-1)
 * @param state Output: file state structure
 * @return 0 on success, error code on failure
 */
s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state) {
    s32 ret;
    s32 pages;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit page;
    u8 bank;

    /* Validate file number */
    if (fileNo >= pfs->dir_size || fileNo < 0) {
        return PFS_ERR_INVALID;
    }

    /* Check if initialized */
    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    /* Check PFS status / ID */
    ret = __osCheckId(pfs);
    if (ret != 0) {
        return ret;
    }

    /* Select bank 0 if needed */
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0) {
            return ret;
        }
    }

    /* Read file directory entry */
    ret = __osContRamRead(pfs->queue, pfs->channel,
                          (u16)(pfs->dir_table + fileNo), (u8 *)&dir);
    if (ret != 0) {
        return ret;
    }

    /* Check if valid entry (company and game code must be non-zero) */
    if (dir.company_code == 0 || dir.game_code == 0) {
        return PFS_ERR_INVALID;
    }

    /* Count pages used by file by following inode chain */
    page = dir.start_page;
    pages = 0;
    bank = 0xFF;

    while (1) {
        /* Check if we've reached end of valid pages */
        if (page.ipage < pfs->inode_start_page) {
            break;
        }

        /* Load inode table for this bank if different */
        if (page.inode_t.bank != bank) {
            bank = page.inode_t.bank;
            ret = __osPfsRWInode(pfs, &inode, PFS_READ, bank);
            if (ret != 0) {
                return ret;
            }
        }

        pages++;
        page = inode.inode_page[page.inode_t.page];
    }

    /* Verify chain ended properly (ipage == 1 means EOF) */
    if (page.ipage != 1) {
        return PFS_ERR_INCONSISTENT;
    }

    /* Fill output structure */
    state->file_size = pages * PFS_ONE_PAGE * PFS_BLOCKSIZE;
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;

    bcopy(dir.game_name, state->game_name, PFS_FILE_NAME_LEN);
    bcopy(dir.ext_name, state->ext_name, PFS_FILE_EXT_LEN);

    return __osPfsGetStatus(pfs->queue, pfs->channel);
}
