/* GENERATED ROM-aligned TU — segment 0x8440 (rom/lib_8440)
 * layout map 613434100350849012f376728964041bd15fd845765c6bfb3b8ea8a529605a8d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8440/osViModeTableGet.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8440/osViModeNtscLan1.s")
/* PROMOTED 2026-10-01 — osViModeNtscLpn1
 * Source:   cloud/work/static_C2/osViModeNtscLpn1.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/osViModeNtscLpn1.c:osViModeNtscLpn1 (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osViModeNtscLpn1(s32 arg0) {
    if (__osSpDeviceBusy() != 0) {
        do {

        } while (__osSpDeviceBusy() != 0);
    }
    __osSpSetStatus(0x125U);
}

