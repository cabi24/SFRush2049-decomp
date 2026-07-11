/* GENERATED ROM-aligned TU — segment 0xf700 (rom/lib_f700)
 * layout map 4f66863fb5ebae8c13b1a88ee16b3303b8bf374da66df1a42ffd8d8012d9048c; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osSumcalc.s")
/* PROMOTED 2026-07-11 — __osIdCheckSum
 * Source:   src/util/checksum.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/util/checksum.c:__osIdCheckSum (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;

    *csum = *icsum = 0;

    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u32)ptr + j);
        *csum += data;
        *icsum += ~data;
    }

    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osRepairId.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osCheckId.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osGetId.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/osPfsReadWriteFile_pages.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osPfsRWInode.s")
