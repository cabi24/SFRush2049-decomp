/* GENERATED ROM-aligned TU — segment 0xa800 (rom/lib_a800)
 * layout map bed467a20849a8080183c8a256a9f57116f3f145e9bf55a7b1a15d69f0cfc8bd; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-11 — osPhysicalToVirtual
 * Source:   src/libultra/os_phys.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_phys.c:osPhysicalToVirtual (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *osPhysicalToVirtual(u32 physAddr) {
    return (void *)(physAddr | 0x80000000);
}

