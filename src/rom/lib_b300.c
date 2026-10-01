/* GENERATED ROM-aligned TU — segment 0xb300 (rom/lib_b300)
 * layout map ba708ac440131f8683f396b6acec82f7ebfe01d83e6cd7f7aa731471ae7c46e9; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_b300/osPfsRename.s")
/* PROMOTED 2026-10-01 — osPfsFindFile
 * Source:   cloud/work/static_C8/osPfsFindFile.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/osPfsFindFile.c:osPfsFindFile (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsFindFile(OSPfs* pfs, __OSInode* inode, u8 start_page, u8 bank, __OSInodeUnit* last_page) {
    __OSInodeUnit next_page;
    __OSInodeUnit old_page;
    s32 ret = 0;

    next_page.ipage = (bank << 8) + start_page;

    do {
        old_page = next_page;
        next_page = inode->inode_page[next_page.inode_t.page];
        inode->inode_page[old_page.inode_t.page].ipage = PFS_PAGE_NOT_USED;
    } while (next_page.ipage >= pfs->inode_start_page && next_page.inode_t.bank == bank);

    *last_page = next_page;

    return ret;
}

