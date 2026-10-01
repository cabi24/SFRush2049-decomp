/* GENERATED ROM-aligned TU — segment 0xaf50 (rom/lib_af50)
 * layout map fb77086abb1e4022382ef2683a23c39351fef65afbeda67a71503b22a0a331fd; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsAllocate
 * Source:   cloud/work/static_C10/osPfsAllocate.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/osPfsAllocate.c:osPfsAllocate (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsAllocate(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no) {
    s32 j;
    int i;
    __OSDir dir;
    s32 ret = 0;
    int fail;

    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }
    ERRCK(osPfsReadWriteFile_pages(pfs));

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*)&dir));
        ERRCK(osContStartReadData(pfs->queue, pfs->channel));

        if ((dir.company_code == company_code) && dir.game_code == game_code) {
            fail = FALSE;

            if (game_name != NULL) {
                for (i = 0; i < ARRLEN(dir.game_name); i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = TRUE;
                        break;
                    }
                }
            }

            if (ext_name != NULL && !fail) {
                for (i = 0; i < ARRLEN(dir.ext_name); i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = TRUE;
                        break;
                    }
                }
            }

            if (!fail) {
                *file_no = j;
                return ret;
            }
        }
    }

    *file_no = -1;
    return PFS_ERR_INVALID;
}

