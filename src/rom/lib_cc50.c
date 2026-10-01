/* GENERATED ROM-aligned TU — segment 0xcc50 (rom/lib_cc50)
 * layout map 715f335d20fc1e63c66378d63adb2b67c62151b54d74e8f4d8c12250c15e7d99; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_remove.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_init.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_update.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_cc50/dll_reschedule.s")
/* PROMOTED 2026-10-01 — dll_insert
 * Source:   cloud/work/static_C11/dll_insert.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C11/dll_insert.c:dll_insert (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSTime dll_insert(OSTimer* t) {
    OSTimer* timep;
    OSTime tim;
    u32 savedMask = __osDisableInt();

    timep = __osTimerList->next;
    tim = t->value;
    for (; timep != __osTimerList && tim > timep->value; timep = timep->next) {
        tim -= timep->value;
    }

    t->value = tim;

    if (timep != __osTimerList) {
        timep->value -= tim;
    }

    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}

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

