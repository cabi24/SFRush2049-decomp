/* GENERATED ROM-aligned TU — segment 0x1f50 (rom/lib_1050)
 * layout map c8f0857aebf4ea1ed90dde16bb4db378a8281234d10bd7a742394c75635cc457; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"


#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viTickStart.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viEnableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viDisableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viUpdateTime.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viScheduleTick.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viAddTicks.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viGetTimeToDeadline.s")

/* PROMOTED 2026-10-01 — viDeadlinePassed
 * Source:   cloud/work/static_C/viDeadlinePassed.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/viDeadlinePassed.c:viDeadlinePassed (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 viDeadlinePassed(void) {
    return (gViAccumTime - gViTickCounter) < 1;
}


/* PROMOTED 2026-09-24 — viStub
 * Source:   work/auto/viStub/matched.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/auto/viStub/matched.c:viStub (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void viStub(void)
{
  int new_var2;
  unsigned short new_var;
 new_var2 = 1; new_var2 = 0; new_var = new_var2; if (new_var & (0xFFFF ^ new_var2)) { } if (new_var) { } if (new_var) { } if (new_var) { }
}
