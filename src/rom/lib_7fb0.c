/* GENERATED ROM-aligned TU — segment 0x7fb0 (rom/lib_7fb0)
 * layout map 1a786bb831297a8815b902edffbc3acc97075ef7084ae9a45901e970b0cfa809; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

extern OSTime gViTimeAccumHi;
extern u32 gViLastCount;

/* PROMOTED 2026-10-01 — osGetTime
 * Source:   cloud/work/static_C15/osGetTime.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C15/osGetTime.c:osGetTime (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSTime osGetTime(void) {
    u32 tmptime;
    u32 elapseCount;
    OSTime currentCount;
    register u32 saveMask;


    saveMask = __osDisableInt();
    tmptime = osGetCount();
    elapseCount = tmptime - gViLastCount;
    currentCount = gViTimeAccumHi;
    __osRestoreInt(saveMask);
    return currentCount + elapseCount;
}

