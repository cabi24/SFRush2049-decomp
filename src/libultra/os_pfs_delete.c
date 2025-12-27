/**
 * @file os_pfs_delete.c
 * @brief Controller Pak file delete functions
 *
 * Decompiled from asm/us/B300.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);           /* func_8000E850 */
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel,     /* func_8000E8D0 */
                           u16 addr, u8 *data);
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode,      /* func_8000F3A4 */
                          u8 flag, u8 bank);
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel,    /* func_8000F680 */
                            u16 addr, u8 *data, u8 pfs_flag);
extern void bzero(void *ptr, s32 size);                      /* func_80008590 */

/* Forward declaration */
static s32 __osPfsReleasePages(OSPfs *pfs, __OSInode *inode, u8 startPage,
                               u8 bank, __OSInodeUnit *nextPage);

/**
 * Delete a file from Controller Pak
 * (func_8000A700 - osPfsDeleteFile)
 *
 * Deletes a file from the Controller Pak filesystem by:
 * 1. Finding the file in the directory
 * 2. Following the inode chain and marking pages as free
 * 3. Clearing the directory entry
 *
 * @param pfs PFS handle
 * @param companyCode Company code of file
 * @param gameCode Game code of file
 * @param gameName Game name (can be NULL)
 * @param extName Extension name (can be NULL)
 * @return 0 on success, error code on failure
 */
s32 osPfsDeleteFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                    u8 *gameName, u8 *extName) {
    s32 fileNo;
    s32 ret;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit lastPage;
    u8 startPage;
    u8 bank;

    /* Validate parameters */
    if (companyCode == 0 || gameCode == 0) {
        return PFS_ERR_INVALID;
    }

    /* Find the file */
    ret = osPfsFindFile(pfs, companyCode, gameCode, gameName, extName, &fileNo);
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

    /* Get starting page from directory entry */
    startPage = dir.start_page.inode_t.page;
    bank = dir.start_page.inode_t.bank;

    /* Free all pages in the file chain */
    while (bank < pfs->banks) {
        /* Read inode table for this bank */
        ret = __osPfsRWInode(pfs, &inode, PFS_READ, bank);
        if (ret != 0) {
            return ret;
        }

        /* Release pages and get next chain location */
        ret = __osPfsReleasePages(pfs, &inode, startPage, bank, &lastPage);
        if (ret != 0) {
            return ret;
        }

        /* Write updated inode table */
        ret = __osPfsRWInode(pfs, &inode, PFS_WRITE, bank);
        if (ret != 0) {
            return ret;
        }

        /* Check if end of chain (ipage == 1 means EOF) */
        if (lastPage.ipage == 1) {
            break;
        }

        /* Get next page location */
        bank = lastPage.inode_t.bank;
        startPage = lastPage.inode_t.page;
    }

    /* Check for incomplete chain */
    if (bank >= pfs->banks) {
        return PFS_ERR_INCONSISTENT;
    }

    /* Clear directory entry */
    bzero(&dir, sizeof(__OSDir));

    /* Write cleared entry back */
    return __osContRamWrite(pfs->queue, pfs->channel,
                            (u16)(pfs->dir_table + fileNo), (u8 *)&dir, 0);
}

/**
 * Release pages in the inode chain
 * (__osPfsReleasePages / func_8000A8D8)
 *
 * Marks pages as free (value 3) and returns the next page in the chain.
 *
 * @param pfs PFS handle
 * @param inode Inode table to modify
 * @param startPage Starting page number
 * @param bank Current bank
 * @param nextPage Output: next page in chain (or EOF marker)
 * @return 0 on success
 */
static s32 __osPfsReleasePages(OSPfs *pfs, __OSInode *inode, u8 startPage,
                               u8 bank, __OSInodeUnit *nextPage) {
    __OSInodeUnit next;
    __OSInodeUnit prev;

    /* Build initial page index */
    next.ipage = (u16)((bank << 8) + startPage);

    /* Follow chain and mark pages as free */
    do {
        prev = next;
        next = inode->inode_page[next.inode_t.page];
        inode->inode_page[prev.inode_t.page].ipage = 3;  /* Mark as free */
    } while (next.ipage >= pfs->inode_start_page && next.inode_t.bank == bank);

    *nextPage = next;
    return 0;
}
