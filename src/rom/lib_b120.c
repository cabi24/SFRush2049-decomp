/* GENERATED ROM-aligned TU — segment 0xb120 (rom/lib_b120)
 * layout map 6108b36c777a807b89267449fa9681e02450bf89909a71a9a3439493f7d44fa3; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsDeleteFile
 * Source:   cloud/work/static_C10/osPfsDeleteFile.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/osPfsDeleteFile.c:osPfsDeleteFile (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsDeleteFile(OSPfs* pfs, s32 file_no, OSPfsState* state) {
    s32 ret;
    int pages;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit next_page;
    u8 bank;

    if (file_no >= pfs->dir_size || file_no < 0) {
        return PFS_ERR_INVALID;
    }

    PFS_CHECK_STATUS();
    ERRCK(osPfsReadWriteFile_pages(pfs));
    SET_ACTIVEBANK_TO_ZERO();

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8*)&dir));

    if (dir.company_code == 0 || dir.game_code == 0) {
        return PFS_ERR_INVALID;
    }

    pages = 0;
    next_page = dir.start_page;
    bank = 0xFF;

    while (TRUE) {
        if (next_page.ipage < pfs->inode_start_page) {
            break;
        } else if (next_page.inode_t.bank != bank) {
            bank = next_page.inode_t.bank;
            ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
        }

        pages++;
        next_page = inode.inode_page[next_page.inode_t.page];
    }

    if (next_page.ipage != PFS_EOF) {
        return PFS_ERR_INCONSISTENT;
    }

    state->file_size = pages * (PFS_ONE_PAGE * BLOCKSIZE);
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;
    bcopy(&dir.game_name, state->game_name, PFS_FILE_NAME_LEN);
    bcopy(&dir.ext_name, state->ext_name, PFS_FILE_EXT_LEN);

    ret = osContStartReadData(pfs->queue, pfs->channel);
    return ret;
}

