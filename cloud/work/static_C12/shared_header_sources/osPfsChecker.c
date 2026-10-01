/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
s32 osPfsChecker(OSPfs* pfs) {
    int j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    int fixed = 0;
    u8 bank;
    u8 oldbank = 254;
    s32 cc;
    s32 cl;
    int offset;

    ret = osPfsReadWriteFile_pages(pfs);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osGetId(pfs);
    }

    if (ret != 0) {
        return ret;
    }

    ERRCK(__osPfsCheckPages(pfs, &cache));

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*)&tmp_dir));

        if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
            if (tmp_dir.company_code == 0 || tmp_dir.game_code == 0) {
                cc = -1;
            } else {
                next_page = tmp_dir.start_page;
                cl = cc = 0;
                bank = 255;

                while (CHECK_IPAGE(next_page)) {
                    if (bank != next_page.inode_t.bank) {
                        bank = next_page.inode_t.bank;

                        if (oldbank != bank) {
                            ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);
                            oldbank = bank;
                        }

                        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                            return ret;
                        }
                    }

                    if ((cc = __osPfsPageCheck(pfs, next_page, &cache) - cl) != 0) {
                        break;
                    }

                    cl = 1;
                    next_page = tmp_inode.inode_page[next_page.inode_t.page];
                }
            }

            if (cc != 0 || next_page.ipage != PFS_EOF) {
                bzero(&tmp_dir, sizeof(__OSDir));

                SET_ACTIVEBANK_TO_ZERO();
                ERRCK(__osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*)&tmp_dir, FALSE));
                fixed++;
            }
        }
    }
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*)&tmp_dir));

        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0 &&
            tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);

        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }

        offset = (bank > 0) ? 1 : pfs->inode_start_page;

        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }

        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = PFS_PAGE_NOT_USED;
        }

        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank && file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        ERRCK(__osPfsRWInode(pfs, &checked_inode, PFS_WRITE, bank));
    }

    if (fixed) {
        pfs->status |= PFS_CORRUPTED;
    } else {
        pfs->status &= ~PFS_CORRUPTED;
    }

    return 0;
}
