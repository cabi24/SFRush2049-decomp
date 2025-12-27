/**
 * @file os_pfs_alloc.c
 * @brief Controller Pak allocation functions
 *
 * Decompiled from asm/us/B9F0.s
 *
 * TODO: These are complex functions that need full decompilation.
 * They handle page allocation and inode chain management.
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External functions */
extern s32 __osCheckId(OSPfs *pfs);                         /* func_8000F2D0 */
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);          /* func_8000E850 */
extern s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode,     /* func_8000F3A4 */
                          u8 flag, u8 bank);

/**
 * Get next page in allocation chain
 * (func_8000ADF0 - internal helper)
 *
 * Follows the inode chain to get the next page, loading bank if needed.
 *
 * @param pfs PFS handle
 * @param currentBank Pointer to current bank (updated if bank changes)
 * @param inode Pointer to inode table
 * @param page Pointer to current page info (updated to next page)
 * @return 0 on success, error code on failure
 *
 * TODO: Full implementation needed for matching
 */
s32 __osPfsGetNextPage(OSPfs *pfs, u8 *currentBank,
                       __OSInode *inode, __OSInodeUnit *page) {
    s32 ret;

    /* Load new bank's inode if page is in different bank */
    if (page->inode_t.bank != *currentBank) {
        *currentBank = page->inode_t.bank;
        ret = __osPfsRWInode(pfs, inode, PFS_READ, *currentBank);
        if (ret != 0) {
            return ret;
        }
    }

    /* Get next page from inode table */
    *page = inode->inode_page[page->inode_t.page];

    /* Validate page */
    if (page->ipage < pfs->inode_start_page) {
        if (page->ipage == 1) {
            return PFS_ERR_INVALID;  /* End of chain */
        }
        return PFS_ERR_INCONSISTENT;
    }

    if (page->inode_t.bank >= pfs->banks) {
        return PFS_ERR_INCONSISTENT;
    }

    if (page->inode_t.page <= 0 || page->inode_t.page >= 128) {
        return PFS_ERR_INCONSISTENT;
    }

    return 0;
}

/**
 * Resize file on Controller Pak
 * (func_8000AEC4 - osPfsReAllocate / osPfsReSizeFile)
 *
 * Changes the size of an existing file by allocating or freeing pages.
 *
 * @param pfs PFS handle
 * @param fileNo File number to resize
 * @param flag Operation flag
 * @param offset Offset in file
 * @param newSize New size in bytes
 * @param data Data buffer (for some operations)
 * @return 0 on success, error code on failure
 *
 * TODO: Full implementation needed for matching. This is a complex
 * function (~880 bytes) that handles:
 * - Validating parameters
 * - Following inode chains
 * - Allocating new pages
 * - Freeing excess pages
 * - Updating directory entries
 */
s32 osPfsReSizeFile(OSPfs *pfs, s32 fileNo, u8 flag,
                    s32 offset, s32 newSize, u8 *data) {
    /* TODO: Full implementation */
    (void)pfs;
    (void)fileNo;
    (void)flag;
    (void)offset;
    (void)newSize;
    (void)data;
    return PFS_ERR_INVALID;
}
