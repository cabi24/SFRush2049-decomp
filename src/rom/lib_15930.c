/* GENERATED ROM-aligned TU — segment 0x15930 (rom/lib_15930)
 * layout map de003be105741e6994da52fab58833577c09b8f2e9958c6fed60aca373036350; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80014D30
 * Source:   cloud/matches/boot_tail/func_80014D30.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014D30.c:func_80014D30 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int D_8004F800;
unsigned int func_80014D30(unsigned int rate)
{
    return ((float)rate * 4096.0f) / D_8004F800;
}

/* PROMOTED 2026-10-04 — func_80014E00
 * Source:   cloud/matches/boot_tail/func_80014E00.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80014E00.c:func_80014E00 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C604[];
void *func_80014E00(void)
{
    return D_8002C604;
}

