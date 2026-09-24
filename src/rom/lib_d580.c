/* GENERATED ROM-aligned TU — segment 0xd580 (rom/lib_d580)
 * layout map 081ae3e6e21903a9bc82a02992b8e498ed5022ab82d857a82c475108286dcc6e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osException.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osExceptionPreamble.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osSendInterrupt.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osCleanupThread.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osEnqueueThread.s")
/* PROMOTED 2026-09-24 — __osPopThread
 * Source:   work/auto/__osPopThread/matched.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/auto/__osPopThread/matched.c:__osPopThread (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSThread *__osPopThread(OSThread **queue)
{
  OSThread *temp_v0;
  short new_var;
  new_var = !queue;
  if ((new_var && (!queue)) && (!queue))
  {
  }
  temp_v0 = *queue;
  *queue = temp_v0->next;
  return temp_v0;
  if (new_var)
  {
  }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osDispatchThread.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_d580/__osExceptionPanic.s")
/* PROMOTED 2026-07-15 — bzero_alt
 * Source:   work/nearmiss/bzero_alt/source.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/nearmiss/bzero_alt/source.c:bzero_alt (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 bzero_alt(void)
{
    return SP_STATUS_REG;
}

