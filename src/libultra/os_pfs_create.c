/**
 * @file os_pfs_create.c
 * @brief Controller Pak file creation functions
 *
 * Decompiled from asm/us/B570.s
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);                         /* func_8000F2D0 */
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);          /* func_8000E850 */
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel,    /* func_8000E8D0 */
                           u16 addr, u8 *data);
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel,   /* func_8000F680 */
                            u16 addr, u8 *data, u8 flag);
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode,     /* func_8000F3A4 */
                          u8 flag, u8 bank);
extern s32 __osPfsGetStatus(OSMesgQueue *mq, s32 channel);  /* func_8000E620 */
extern void bcopy(void *src, void *dst, s32 len);           /* func_8000D2B0 */
extern void bzero(void *ptr, s32 size);                     /* func_80008590 */

/**
 * Create directory entry for file
 * (func_8000A970 - __osPfsCreateDirEntry)
 *
 * Creates a new directory entry with the specified parameters.
 *
 * @param pfs PFS handle
 * @param entry Directory entry to create
 * @return 0 on success, error code on failure
 */
s32 __osPfsCreateDirEntry(OSPfs *pfs, __OSDir *entry) {
    s32 i;
    s32 ret;
    __OSDir dir;

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

    /* Find empty directory slot */
    for (i = 0; i < pfs->dir_size; i++) {
        ret = __osContRamRead(pfs->queue, pfs->channel,
                              (u16)(pfs->dir_table + i), (u8 *)&dir);
        if (ret != 0) {
            return ret;
        }

        /* Empty slot found (company and game code are 0) */
        if (dir.company_code == 0 && dir.game_code == 0) {
            /* Write the new entry */
            return __osContRamWrite(pfs->queue, pfs->channel,
                                    (u16)(pfs->dir_table + i), (u8 *)entry, 0);
        }
    }

    /* No empty slots */
    return PFS_ERR_DIR_FULL;
}

/**
 * Allocate file on Controller Pak
 * (func_8000ACA4 - osPfsAllocateFile)
 *
 * Allocates a new file with the specified size. This creates a directory
 * entry and allocates pages in the inode table.
 *
 * @param pfs PFS handle
 * @param companyCode Company code for file
 * @param gameCode Game code for file
 * @param gameName Game name (16 bytes)
 * @param extName Extension name (4 bytes)
 * @param size Size in bytes (must be multiple of 256)
 * @param fileNo Output: file number of created file
 * @return 0 on success, error code on failure
 */
s32 osPfsAllocateFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                      u8 *gameName, u8 *extName, s32 size, s32 *fileNo) {
    s32 ret;
    s32 i;
    s32 pages;
    s32 freeBytes;
    __OSDir dir;
    __OSInode inode;
    __OSInodeUnit firstPage;
    __OSInodeUnit prevPage;
    __OSInodeUnit page;
    u8 bank;

    /* Validate parameters */
    if (companyCode == 0 || gameCode == 0) {
        return PFS_ERR_INVALID;
    }

    /* Size must be positive and multiple of page size */
    if (size <= 0 || (size % (PFS_ONE_PAGE * PFS_BLOCKSIZE)) != 0) {
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

    /* Check if file already exists */
    ret = osPfsFindFile(pfs, companyCode, gameCode, gameName, extName, fileNo);
    if (ret == 0) {
        return PFS_ERR_EXIST;  /* File already exists */
    }

    /* Check free space */
    ret = osPfsFreeBlocks(pfs, &freeBytes);
    if (ret != 0) {
        return ret;
    }

    if (freeBytes < size) {
        return PFS_ERR_FULL;
    }

    /* Calculate pages needed */
    pages = size / (PFS_ONE_PAGE * PFS_BLOCKSIZE);

    /* Initialize directory entry */
    bzero(&dir, sizeof(__OSDir));
    dir.company_code = companyCode;
    dir.game_code = gameCode;

    if (gameName != NULL) {
        bcopy(gameName, dir.game_name, PFS_FILE_NAME_LEN);
    }
    if (extName != NULL) {
        bcopy(extName, dir.ext_name, PFS_FILE_EXT_LEN);
    }

    /* Allocate pages from inode tables */
    firstPage.ipage = 0;
    prevPage.ipage = 0;

    for (bank = 0; bank < pfs->banks && pages > 0; bank++) {
        /* Read inode table for this bank */
        ret = __osPfsRWInode(pfs, &inode, PFS_READ, bank);
        if (ret != 0) {
            return ret;
        }

        /* Find free pages in this bank */
        for (i = pfs->inode_start_page; i < 128 && pages > 0; i++) {
            if (inode.inode_page[i].ipage == PFS_PAGE_FREE) {
                /* Build page reference */
                page.inode_t.bank = bank;
                page.inode_t.page = (u8)i;

                if (firstPage.ipage == 0) {
                    /* First page becomes start page */
                    firstPage = page;
                } else {
                    /* Link previous page to this one */
                    inode.inode_page[prevPage.inode_t.page] = page;
                }

                /* Mark as end of chain for now */
                inode.inode_page[i].ipage = PFS_EOF;

                prevPage = page;
                pages--;
            }
        }

        /* Write updated inode table */
        ret = __osPfsRWInode(pfs, &inode, PFS_WRITE, bank);
        if (ret != 0) {
            return ret;
        }
    }

    /* Check if we allocated enough pages */
    if (pages > 0) {
        /* This shouldn't happen if free space check passed */
        return PFS_ERR_FULL;
    }

    /* Set start page in directory entry */
    dir.start_page = firstPage;

    /* Create directory entry */
    ret = __osPfsCreateDirEntry(pfs, &dir);
    if (ret != 0) {
        /* TODO: Should free allocated pages on failure */
        return ret;
    }

    /* Find the file number we just created */
    return osPfsFindFile(pfs, companyCode, gameCode, gameName, extName, fileNo);
}
