/* GENERATED ROM-aligned TU — segment 0x8dd0 (rom/lib_8dd0)
 * layout map d2020668a662ddee5e34191b43b9789885b7b9456059ca06cd690bbea6d4ba4d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPiRawReadWord
 * Source:   cloud/work/static_C/osPiRawReadWord.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osPiRawReadWord.c:osPiRawReadWord (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiRawReadWord(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 sp1C;

    osPiGetAccess();
    temp_v0 = osPiReadWord(arg0, arg1);
    sp1C = temp_v0;
    osPiReleaseAccess();
    return temp_v0;
}

