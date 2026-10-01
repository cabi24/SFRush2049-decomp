/* GENERATED ROM-aligned TU — segment 0xb570 (rom/lib_b570)
 * layout map 554627b1e11be1c141f933c26dfabeb3225f5a4972b5b61c4a6610c3dd66c5ae; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsReadWriteFile
 * Source:   cloud/work/static_C12/osPfsReadWriteFile.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C12/osPfsReadWriteFile.c:osPfsReadWriteFile (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsReadWriteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, int file_size_in_bytes,
                      s32* file_no) {
    int start_page;
    int decleared;
    int last_page;
    int old_last_page = 0;
    s32 ret = 0;
    int file_size_in_pages;
    __OSInode inode;
    __OSInode backup_inode;
    __OSDir dir;
    u8 bank;
    u8 old_bank = 0;
    int firsttime = 0;
    s32 bytes;
    __OSInodeUnit fpage;

    if (company_code == 0 || game_code == 0) {
        return PFS_ERR_INVALID;
    }

    file_size_in_pages = ROUND_UP_DIVIDE(file_size_in_bytes, BLOCKSIZE * PFS_ONE_PAGE);

    if (((ret = osPfsAllocate(pfs, company_code, game_code, game_name, ext_name, file_no)) != 0) &&
        ret != PFS_ERR_INVALID) {
        return ret;
    }

    if (*file_no != -1) {
        return PFS_ERR_EXIST;
    }

    ret = osPfsFreeBlocks(pfs, &bytes);

    if (file_size_in_bytes > bytes) {
        return PFS_DATA_FULL;
    }

    if (file_size_in_pages == 0) {
        return PFS_ERR_INVALID;
    }

    if (((ret = osPfsAllocate(pfs, 0, 0, NULL, NULL, file_no)) != 0) && ret != PFS_ERR_INVALID) {
        return ret;
    }

    if (*file_no == -1) {
        return PFS_DIR_FULL;
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
        ERRCK(__osPfsDeclearPage(pfs, &inode, file_size_in_pages, &start_page, bank, &decleared, &last_page));

        if (start_page != -1) {
            if (firsttime == 0) {
                fpage.inode_t.page = start_page;
                fpage.inode_t.bank = bank;
            } else {
                backup_inode.inode_page[old_last_page].inode_t.bank = bank;
                backup_inode.inode_page[old_last_page].inode_t.page = start_page;
                ERRCK(__osPfsRWInode(pfs, &backup_inode, PFS_WRITE, old_bank));
            }

            if (file_size_in_pages > decleared) {
                bcopy(&inode, &backup_inode, sizeof(__OSInode));
                old_last_page = last_page;
                old_bank = bank;
                file_size_in_pages -= decleared;
                firsttime++;
            } else {
                file_size_in_pages = 0;
                ERRCK(__osPfsRWInode(pfs, &inode, PFS_WRITE, bank));
                break;
            }
        }
    }

    if (file_size_in_pages > 0 || start_page == -1) {
        return PFS_ERR_INCONSISTENT;
    }

    dir.start_page = fpage;
    dir.company_code = company_code;
    dir.game_code = game_code;
    dir.data_sum = 0;

    bcopy(game_name, dir.game_name, PFS_FILE_NAME_LEN);
    bcopy(ext_name, dir.ext_name, PFS_FILE_EXT_LEN);

    ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + *file_no, (u8*)&dir, FALSE);
    return ret;
}

/* PROMOTED 2026-10-01 — __osPfsDeclearPage
 * Source:   cloud/work/static_C9/__osPfsDeclearPage.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/__osPfsDeclearPage.c:__osPfsDeclearPage (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osPfsDeclearPage(OSPfs* pfs, __OSInode* inode, int file_size_in_pages, int* first_page, u8 bank, int* decleared,
                       int* last_page) {
    int j;
    int spage;
    int old_page;
    s32 ret = 0;
    int offset = bank > 0 ? 1 : pfs->inode_start_page;

    for (j = offset; j < ARRLEN(inode->inode_page); j++) {
        if (inode->inode_page[j].ipage == 3) {
            break;
        }
    }

    if (j == ARRLEN(inode->inode_page)) {
        *first_page = -1;
        return ret;
    }

    spage = j;
    *decleared = 1;
    old_page = j;
    j++;

    while (file_size_in_pages > *decleared && j < ARRLEN(inode->inode_page)) {
        if (inode->inode_page[j].ipage == 3) {
            inode->inode_page[old_page].inode_t.bank = bank;
            inode->inode_page[old_page].inode_t.page = j;
            old_page = j;
            (*decleared)++;
        }
        j++;
    }

    *first_page = spage;

    if (j == ARRLEN(inode->inode_page) && file_size_in_pages > *decleared) {
        *last_page = old_page;
    } else {
        inode->inode_page[old_page].ipage = 1;
        *last_page = 0;
    }

    return ret;
}

