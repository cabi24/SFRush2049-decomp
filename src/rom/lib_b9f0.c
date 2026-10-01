/* GENERATED ROM-aligned TU — segment 0xb9f0 (rom/lib_b9f0)
 * layout map bb949529a461cb0d86ad90e99394bbb7c63d781016b2821bbc04d597c813e590; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsGetFileStat
 * Source:   cloud/work/static_C8/osPfsGetFileStat.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/osPfsGetFileStat.c:osPfsGetFileStat (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsGetFileStat(OSPfs* pfs, u8* bank, __OSInode* inode, __OSInodeUnit* page) {
    s32 ret;

    if (page->inode_t.bank != *bank) {
        *bank = page->inode_t.bank;
        ERRCK(__osPfsRWInode(pfs, inode, PFS_READ, *bank));
    }

    *page = inode->inode_page[page->inode_t.page];

    if (!CHECK_IPAGE(*page)) {
        if (page->ipage == PFS_EOF) {
            return PFS_ERR_INVALID;
        }

        return PFS_ERR_INCONSISTENT;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_b9f0/osPfsGetFileSize.s")
