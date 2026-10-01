/* GENERATED ROM-aligned TU — segment 0xe7c0 (rom/lib_e7c0)
 * layout map 0a6559b9e67a367eae3e2798ff28a2cda0fa0eac96f868fb59552d670bc56f6c; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e7c0/osPiInit.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e7c0/osPiGetAccess.s")
/* PROMOTED 2026-10-01 — osPiReleaseAccess
 * Source:   cloud/work/static_C/osPiReleaseAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osPiReleaseAccess.c:osPiReleaseAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osPiReleaseAccess(void) {
    osJamMesg(&__osPiMesgQueue, NULL, 0);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e7c0/osPiReadWord.s")
