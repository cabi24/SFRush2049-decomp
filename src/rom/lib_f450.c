/* GENERATED ROM-aligned TU — segment 0xf450 (rom/lib_f450)
 * layout map a063291fc46457344bc2ca917d0c100ed0b3aca05acacc54db321af71254b124; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osPfsSelectBank
 * Source:   cloud/work/static_C8/__osPfsSelectBank.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/__osPfsSelectBank.c:__osPfsSelectBank (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osPfsSelectBank(OSPfs* pfs, u8 bank) {
    u8 temp[BLOCKSIZE];
    int i;
    s32 ret = 0;

    for (i = 0; i < BLOCKSIZE; i++) {
        temp[i] = bank;
    }

    ret = __osContRamWrite(pfs->queue, pfs->channel, CONT_BLOCK_DETECT, temp, FALSE);

    if (ret == 0) {
        pfs->activebank = bank;
    }

    return ret;
}

