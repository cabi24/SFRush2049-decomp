/* GENERATED ROM-aligned TU — segment 0x10cc0 (rom/lib_10cc0)
 * layout map de94582e73efc8ed1a1a72eb83814fc99e592ac67fa47da94b9ebbad4fe324c2; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osSetGlobalIntMask
 * Source:   cloud/work/frontier/w16g/sources/__osSetGlobalIntMask.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osSetGlobalIntMask.c:__osSetGlobalIntMask (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O1.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef u32 OSHWIntr;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern u32 __osGlobalIntMask;
void __osSetGlobalIntMask(OSHWIntr mask) {
    register u32 saveMask = __osDisableInt();

    __osGlobalIntMask |= mask;
    __osRestoreInt(saveMask);
}

