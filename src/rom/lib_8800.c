/* GENERATED ROM-aligned TU — segment 0x8800 (rom/lib_8800)
 * layout map 50222c5fd248f9c10dc181392f4eb6f96365093256559b4f270ae9ee18a2a3d3; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/strchr.s")
/* PROMOTED 2026-07-11 — strlen
 * Source:   src/libc/string.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libc/string.c:strlen (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u32 strlen(const char *str) {
    const char *p;

    p = str;
    if (*(u8 *)p != 0) {
        do {
            p++;
        } while (*(u8 *)p != 0);
    }
    return (u32)(p - str);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/memcpy.s")
