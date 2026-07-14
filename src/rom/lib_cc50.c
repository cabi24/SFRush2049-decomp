/* GENERATED ROM-aligned TU — segment 0xcc50 (rom/lib_cc50)
 * layout map 715f335d20fc1e63c66378d63adb2b67c62151b54d74e8f4d8c12250c15e7d99; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_remove.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_init.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_update.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_reschedule.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_insert.s")
/* PROMOTED 2026-07-15 — dll_get_priority
 * Source:   work/nearmiss/dll_get_priority/source.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/nearmiss/dll_get_priority/source.c:dll_get_priority (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 dll_get_priority(void *thread)
{
    if (thread == ((void *) 0))
    {
        thread = __osRunningThread;
    }
    return *(s32 *) ((u8 *) thread + 4);
}

