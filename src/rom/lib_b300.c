/* GENERATED ROM-aligned TU — segment 0xb300 (rom/lib_b300)
 * layout map ba708ac440131f8683f396b6acec82f7ebfe01d83e6cd7f7aa731471ae7c46e9; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsRename
 * Source:   cloud/work/static_C10/osPfsRename.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/osPfsRename.c:osPfsRename (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsRename(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name) {
    s32 file_no;
    s32 ret;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit last_page;
    u8 startpage;
    u8 bank;

    if (company_code == 0 || game_code == 0) {
        return PFS_ERR_INVALID;
    }

    ERRCK(osPfsAllocate(pfs, company_code, game_code, game_name, ext_name, &file_no));
    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8*)&dir));

    startpage = dir.start_page.inode_t.page;

    for (bank = dir.start_page.inode_t.bank; bank < pfs->banks;) {
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
        ERRCK(osPfsFindFile(pfs, &inode, startpage, bank, &last_page));
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_WRITE, bank));

        if (last_page.ipage == PFS_EOF) {
            break;
        }

        bank = last_page.inode_t.bank;
        startpage = last_page.inode_t.page;
    }

    if (bank >= pfs->banks) {
        return PFS_ERR_INCONSISTENT;
    }

    bzero(&dir, sizeof(__OSDir));

    ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8*)&dir, FALSE);

    return ret;
}

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

