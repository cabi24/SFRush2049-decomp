/* GENERATED ROM-aligned TU — segment 0xbfe0 (rom/lib_bfe0)
 * layout map 9074d20edd30236ef863847f66076c326909df6fbb7df5255e71b3dcfa69059b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_bfe0/osPfsChecker.s")
/* PROMOTED 2026-10-01 — __osPfsCheckPages
 * Source:   cloud/work/static_C11/__osPfsCheckPages.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C11/__osPfsCheckPages.c:__osPfsCheckPages (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osPfsCheckPages(OSPfs* pfs, __OSInodeCache* cache) {
    int i;
    int n;
    int offset;
    u8 bank;
    __OSInodeUnit tpage;
    __OSInode tmp_inode;
    s32 ret;

    for (i = 0; i < PFS_INODE_DIST_MAP; i++) {
        cache->map[i] = 0;
    }

    cache->bank = -1;
    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;

        ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);

        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }

        for (i = offset; i < ARRLEN(tmp_inode.inode_page); i++) {
            tpage = tmp_inode.inode_page[i];

            if (tpage.ipage >= pfs->inode_start_page && tpage.inode_t.bank != bank) {
                n = ((tpage.inode_t.page & 0x7F) / PFS_SECTOR_SIZE) +
                    ((tpage.inode_t.bank % PFS_BANK_LAPPED_BY) * BLOCKSIZE);
                cache->map[n] |= 1 << (bank % PFS_BANK_LAPPED_BY);
            }
        }
    }
    return 0;
}

/* PROMOTED 2026-10-01 — __osPfsPageCheck
 * Source:   cloud/work/static_C11/__osPfsPageCheck.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C11/__osPfsPageCheck.c:__osPfsPageCheck (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osPfsPageCheck(OSPfs* pfs, __OSInodeUnit fpage, __OSInodeCache* cache) {
    int j;
    int n;
    int hit;
    u8 bank;
    int offset;
    s32 ret;

    hit = 0;
    ret = 0;
    n = (fpage.inode_t.page / PFS_SECTOR_SIZE) + (fpage.inode_t.bank % PFS_BANK_LAPPED_BY) * BLOCKSIZE;

    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;

        if (bank == fpage.inode_t.bank || cache->map[n] & (1 << (bank % PFS_BANK_LAPPED_BY))) {
            if (bank != cache->bank) {
                ret = __osPfsRWInode(pfs, &cache->inode, PFS_READ, bank);

                if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                    return ret;
                }

                cache->bank = bank;
            }

            for (j = offset; hit < 2 && (j < ARRLEN(cache->inode.inode_page)); j++) {
                if (cache->inode.inode_page[j].ipage == fpage.ipage) {
                    hit++;
                }
            }

            if (hit >= 2) {
                return PFS_ERR_NEW_PACK;
            }
        }
    }
    return hit;
}

