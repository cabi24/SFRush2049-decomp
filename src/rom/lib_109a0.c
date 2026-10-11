/* GENERATED ROM-aligned TU — segment 0x109a0 (rom/lib_109a0)
 * layout map 72f0136027861a4c61c346998f8ed21ac161d55b4e0a67e23d62ec6a8d963cf6; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osResetGlobalIntMask
 * Source:   cloud/work/frontier/w16g/sources/__osResetGlobalIntMask.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osResetGlobalIntMask.c:__osResetGlobalIntMask (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O1.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef u32 OSHWIntr;
#define OS_IM_RCP 0x00000401
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern u32 __osGlobalIntMask;
void __osResetGlobalIntMask(OSHWIntr mask) {
    register u32 saveMask = __osDisableInt();

    __osGlobalIntMask &= ~(mask & ~OS_IM_RCP);
    __osRestoreInt(saveMask);
}

