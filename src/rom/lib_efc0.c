/* GENERATED ROM-aligned TU — segment 0xefc0 (rom/lib_efc0)
 * layout map 4f3f427d418a827f79392ed5cd2e534299f40208223718dc18b06a16e0b36ecb; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* IDO intrinsic required: the same-name call denotes hardware square root. */
float sqrtf(float);
#pragma intrinsic(sqrtf)

/* PROMOTED 2026-10-02 — sqrtf
 * Source:   cloud/work/static_acceptance/sqrtf/sqrtf.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_acceptance/sqrtf/sqrtf.c:sqrtf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
float sqrtf(float value)
{
    return sqrtf(value);
}

